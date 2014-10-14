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
      
    # 1. 枚举进程ID
    arrProcessId = (c_ulong * 1024)()
    nNeeded = c_ulong()
    if not psapi.EnumProcesses( arrProcessId, sizeof(arrProcessId), byref(nNeeded) ):
        return 0
    
    nCountOfProcess = nNeeded.value / sizeof(c_ulong)
    arrProcessId = [i for i in arrProcessId][:nCountOfProcess]
    
    #print "pid:", arrProcessId
    
    # 2. 遍历进程ID，找到basename == str 的进程
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
    # 1.1 INQ 代码段
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
    # 取段基地址 nBaseAddrOfLoaderSeg
    for section in peinq70.sections :
        if section.Name.startswith('.KAV8') :
            nBaseAddrOfLoaderSeg = section.VirtualAddress
            break
    print 'nBaseAddrOfLoaderSeg:%X' % nBaseAddrOfLoaderSeg
    nOffsetOfLoaderCode  = 0x1000
    nBeginOfLoaderCode   = 0        # 从导出表中读取
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
    nBaseAddrOfSx32wCode = 0x10001000  # 由于sw32是第一个被加载的动态库，所以其基地址固定
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
    
    # 3.1 开头部分
    fileStart = open(g_start_filename, 'r')
    listLines = fileStart.readlines()
    fileStart.close()
    
    fileDst.writelines(listLines)
    
    # 3.2 插入的代码
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
    
    # 3.3 结尾部分
    fileEnd = open(g_end_filename, 'r')
    listLines = fileEnd.readlines()
    fileEnd.close()
    
    fileDst.writelines(listLines)
    
    # 关闭文件
    fileDst.close()

def main():
    
    # step 1: 取得三组数据(addr, size), 1,INQ代码段；2， Shell；3. sx32w.dll代码段  ==>(addr, size)
    listAddrSize = GetAddrAndSizeList()
   
    print "addr and size : ", 
    for i in listAddrSize : print '(%X, %X)' % (i[0], i[1]),
    print
    
    # step 2: 使用此3组数据分别计算出它们的CRC32  ==>   CRC32
    listCrc = []
    hProcessInq = GetProcessHandleByBaseName(g_inqname)
    
    for addrSize in listAddrSize :
        accum = CaculateMemoryCrc32(hProcessInq, addrSize[0], addrSize[1])
        listCrc.append(accum)
    
    kernel32.CloseHandle(hProcessInq)
    
    print 'crc : 0x%X, 0x%X, 0x%X' % (listCrc[0], listCrc[1], listCrc[2])
    
    # step 3: 生成脚本文件
    GenerateScript(listAddrSize, listCrc)

usage = '''usage : ScriptGenerator file_basename functype [inq_exe_name]
                   file_basename 指的是 需要生成的脚本的名字，如 Utilities.ibs
                   functype      指的是 脚本中所需要修改的函数的类型，包扩 Function, Sub
                   inq_exe_name  指的是inq的进程名，如inq70.exe'''

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
    
