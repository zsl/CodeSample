#coding:gbk

from ctypes import *
import ctypes.wintypes as wintype

import sys

kernel32 = windll.kernel32
psapi = windll.psapi

g_inqname = 'inq70.exe'
g_filename = ''
g_start_filename = ''
g_end_filename = ''
g_functype = 'Sub'


class CReadMemoryError(IOError):
    pass

def GetProcessHandleByBaseName(strBaseName):
    PROCESS_QUERY_INFORMATION          = 0x0400
    PROCESS_VM_READ                    = 0x0010
#    PROCESS_QUERY_LIMITED_INFORMATION  = 0x1000
      
    # 1. ö�ٽ���ID
    arrProcessId = (c_ulong * 1024)()
    nNeeded = c_ulong()
    if not psapi.EnumProcesses( arrProcessId, sizeof(arrProcessId), byref(nNeeded) ):
        return 0
    
    nCountOfProcess = nNeeded.value / sizeof(c_ulong)
    arrProcessId = [i for i in arrProcessId][:nCountOfProcess]
    
    #print "pid:", arrProcessId
    
    # 2. ��������ID���ҵ�basename == str �Ľ���
    strProcessName = create_string_buffer(260)
    for pid in arrProcessId :
        if not pid :
            continue
        
        hProcess = kernel32.OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, False, pid)

        if hProcess :
            hMod = wintype.HMODULE()
            nNeeded = c_ulong()

            ret = psapi.EnumProcessModules( hProcess, byref(hMod), sizeof(hMod), byref(nNeeded) )
            if not ret :
                errCode = GetLastError()
                print "ErrorCode: %d, %s" % (errCode, FormatError(errCode))
            
            psapi.GetModuleBaseNameA( hProcess, hMod, strProcessName, sizeof(strProcessName) );
                      
            if strBaseName.upper() == strProcessName.value.upper() :
                print 'find :', strBaseName
                return hProcess
            
    return 0


import binascii
def CaculateMemoryCrc32(hProcess, addrBase, size) :
    arrData = (c_byte * size)()
    addr    = c_ulong(addrBase)
    nSize   = c_ulong(size)
    nReaded = c_ulong()
    
    ret = kernel32.ReadProcessMemory(hProcess, addr, arrData, nSize, byref(nReaded))
    if not ret :
        errCode = GetLastError()
        raise CReadMemoryError, "ErrorCode:%d, %s" % (errCode, FormatError(errCode))
    
   # for i in arrData : print (i),
    
    listChar = [chr( i & 0xff ) for i in arrData]
    strData = ''.join(listChar)
    accum = binascii.crc32(strData) & 0xffffffff
    
    return accum


def GetAddrAndSizeList() :
    import pefile
    
    listAddrSize = []
    
    nBaseOfImage = 0x400000
    # 1.1 INQ �����
    nRvaOfInqCode    = 0x1000
    nOffsetOfInqCode = 0x4CC

    peinq70 = pefile.PE("inq70.exe")
    
    for section in peinq70.sections :
        if section.Name.startswith('.text') :
            nSizeOfInqCode = section.Misc_VirtualSize
            break
    
    pareInqAddrSize = nBaseOfImage + nRvaOfInqCode + nOffsetOfInqCode, nSizeOfInqCode - nOffsetOfInqCode
    listAddrSize.append(pareInqAddrSize)
    
    # 1.2 Shell
    # ȡ�λ���ַ nBaseAddrOfLoaderSeg
    for section in peinq70.sections :
        if section.Name.startswith('.KAV8') :
            nBaseAddrOfLoaderSeg = section.VirtualAddress
            break
    print 'nBaseAddrOfLoaderSeg:%X' % nBaseAddrOfLoaderSeg
    nOffsetOfLoaderCode  = 0x1000
    nBeginOfLoaderCode   = 0        # �ӵ������ж�ȡ
    nEndOfLoaderCode     = 0
    
    strShellPath = "RyXLoader.dll"
    peShell = pefile.PE(strShellPath)
    
    for export in peShell.DIRECTORY_ENTRY_EXPORT.symbols :
        if export.name == '__g_p_code_seg_start' :
            nBeginOfLoaderCode = export.address
        elif export.name == '__g_p_code_seg_end' :
            nEndOfLoaderCode = export.address
    
    pareShellAddrSize = nBaseOfImage + nBaseAddrOfLoaderSeg + nBeginOfLoaderCode - nOffsetOfLoaderCode, nEndOfLoaderCode - nBeginOfLoaderCode
    listAddrSize.append(pareShellAddrSize)
    
    # 1.3 sx32w.dll
    nBaseAddrOfSx32wCode = 0x10001000  # ����sw32�ǵ�һ�������صĶ�̬�⣬���������ַ�̶�
    nSizeOfsx32wCode     = 0
    
    strSx32wPath = 'sx32w.dll'
    peSx32w = pefile.PE(strSx32wPath)
    #nSizeOfsx32wCode = pefile.OPTIONAL_HEADER.SizeOfCode
    
    for section in peSx32w.sections :
        if section.Name.startswith('.text') :
            nSizeOfsx32wCode = section.Misc_VirtualSize
    
    pareSx32wAddrSize = nBaseAddrOfSx32wCode, nSizeOfsx32wCode
    listAddrSize.append(pareSx32wAddrSize);
    
    return listAddrSize


def GenerateScript(listAddrSize, listCrc) :
    fileDst = open(g_filename, 'w')
    
    # 3.1 ��ͷ����
    fileStart = open(g_start_filename, 'r')
    listLines = fileStart.readlines()
    fileStart.close()
    
    fileDst.writelines(listLines)
    
    # 3.2 ����Ĵ���
    strCodeDeclare   = '    Dim addr, memSize, accum'
    strCodeFormat = """
        addr = &H%X
        memSize = &H%X&
        
        accum = CalculateMemoryCrc(addr, memSize)
        
        If accum <> &H%X Then
            Exit %s
        End If
    """
    
    listFileContent = []
    listFileContent.append(strCodeDeclare)
    for i in range(3) :
        listFileContent.append( strCodeFormat % (listAddrSize[i][0], listAddrSize[i][1], listCrc[i], g_functype) )
    
    fileDst.writelines(listFileContent)
    
    # 3.3 ��β����
    fileEnd = open(g_end_filename, 'r')
    listLines = fileEnd.readlines()
    fileEnd.close()
    
    fileDst.writelines(listLines)
    
    # �ر��ļ�
    fileDst.close()

def main():
    
    # step 1: ȡ����������(addr, size), 1,INQ����Σ�2�� Shell��3. sx32w.dll�����  ==>(addr, size)
    listAddrSize = GetAddrAndSizeList()
   
    print "addr and size : ", 
    for i in listAddrSize : print '(%X, %X)' % (i[0], i[1]),
    print
    
    # step 2: ʹ�ô�3�����ݷֱ��������ǵ�CRC32  ==>   CRC32
    listCrc = []
    hProcessInq = GetProcessHandleByBaseName(g_inqname)
    
    for addrSize in listAddrSize :
        accum = CaculateMemoryCrc32(hProcessInq, addrSize[0], addrSize[1])
        listCrc.append(accum)
    
    kernel32.CloseHandle(hProcessInq)
    
    print 'crc : 0x%X, 0x%X, 0x%X' % (listCrc[0], listCrc[1], listCrc[2])
    
    # step 3: ���ɽű��ļ�
    GenerateScript(listAddrSize, listCrc)

usage = '''usage : ScriptGenerator file_basename functype [inq_exe_name]
                   file_basename ָ���� ��Ҫ���ɵĽű������֣��� Utilities.ibs
                   functype      ָ���� �ű�������Ҫ�޸ĵĺ��������ͣ����� Function, Sub
                   inq_exe_name  ָ����inq�Ľ���������inq70.exe'''

if __name__ == '__main__':
    if len(sys.argv) < 3 :
        print usage
        exit(0)
    import os

    g_filename = sys.argv[1]
    basename, ext = os.path.splitext(sys.argv[1])
    if ext == '': ext = '.ibs'
    g_filename = basename + ext
    g_start_filename = basename + '_start' + ext
    g_end_filename = basename + '_end' + ext

    if not ( os.path.exists(g_start_filename) and os.path.exists(g_end_filename) ) :
        print 'file not exists : %s, %s' % (g_start_filename, g_end_filename)
        print usage
        exit(0)

    g_functype = sys.argv[2]
    if not (g_functype == 'Sub' or g_functype == 'Function'):
        print 'invalid functype :%s' % g_functype
        print usage
        exit(0)

    if len(sys.argv) > 3 :
        g_inqname = sys.argv[3]

    main()
    
