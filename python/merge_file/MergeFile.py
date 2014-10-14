# coding:utf-8

import sys, os

def showInfo():
    print """
    �����ʽ��mergefile -i srcfile1 srcfile2 ... -o dstfile
    ˵����-i ѡ������ָ������ļ���һ���ļ��У�����ᰴ��ָ����˳�����е��ļ����ļ����е��ļ����кϲ�.
          -o ѡ������ָ���ϲ����Ŀ���ļ�."""
    
    
def mergeFiles(files, dstfilename):
    bufCount = 1024 * 1024
    dstfile = open(dstfilename, 'wb')
    
    for file in files:
        srcfile = open(file, 'rb')
        
        # �����ļ�
        data = srcfile.read(bufCount)
        while '' != data:
            dstfile.write(data)
            data = srcfile.read(bufCount)
        
        srcfile.close()
        
    dstfile.close()
    

if '__main__' == __name__:
    
    if sys.argv.count('-i') == 0 :
        print "��������ȱ�ٲ���'-i'"
        showInfo()
        exit(1)
    elif sys.argv.count('-o') == 0:
        print "��������ȱ�ٲ���'-o'"
        showInfo()
        exit(1)
    cmds = ['-i', '-o']
    filenames = []
    dstfile = ''
    
    i = 1
    while i < len(sys.argv):
        cmd = sys.argv[i]
        i += 1
        
        if 0 == cmds.count(cmd):
            print cmd
            print "��������"
            showInfo()
            exit(1)
        else:
            if cmd == '-i':
                j = i
                while j < len(sys.argv):
                    arg = sys.argv[j]
                    j += 1
                    
                    if 0 != cmds.count(arg):
                        i = j - 1
                        break
                    
                    if not os.path.exists(arg):
                        print '�ļ���', arg, '"������'
                        showInfo()
                        exit(2)
                    else:
                        if os.path.isfile(arg):
                            filenames.append(arg)
                        else:
                            dirfiles = os.listdir(arg)
                            for dfn in dirfiles:
                                fn = os.path.join(arg, dfn)
                                if os.path.isfile(fn):
                                    filenames.append(fn)
            elif cmd == '-o':
                dstfile = sys.argv[i]
                i += 1
                if os.path.exists(cmd):
                    if os.path.isfile(cmd):
                        os.remove(cmd)
                    else:
                        print "��������-o ����Ӧ���ļ���"
                        showInfo()
                        exit(1)
    
    print "srcfiles:"
    for fn in filenames:
        print '  ', fn
    
    print 'dstfile:\n  ', dstfile
    
    mergeFiles(filenames, dstfile)
    print '�ϲ��ɹ�'