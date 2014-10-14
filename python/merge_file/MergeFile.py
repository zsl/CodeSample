# coding:utf-8

import sys, os

def showInfo():
    print """
    命令格式：mergefile -i srcfile1 srcfile2 ... -o dstfile
    说明：-i 选项用于指定多个文件或一个文件夹，程序会按照指定的顺序所有的文件及文件夹中的文件进行合并.
          -o 选项用于指定合并后的目标文件."""
    
    
def mergeFiles(files, dstfilename):
    bufCount = 1024 * 1024
    dstfile = open(dstfilename, 'wb')
    
    for file in files:
        srcfile = open(file, 'rb')
        
        # 复制文件
        data = srcfile.read(bufCount)
        while '' != data:
            dstfile.write(data)
            data = srcfile.read(bufCount)
        
        srcfile.close()
        
    dstfile.close()
    

if '__main__' == __name__:
    
    if sys.argv.count('-i') == 0 :
        print "参数错误，缺少参数'-i'"
        showInfo()
        exit(1)
    elif sys.argv.count('-o') == 0:
        print "参数错误，缺少参数'-o'"
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
            print "参数错误啊"
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
                        print '文件“', arg, '"不存在'
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
                        print "参数错误：-o 后面应跟文件名"
                        showInfo()
                        exit(1)
    
    print "srcfiles:"
    for fn in filenames:
        print '  ', fn
    
    print 'dstfile:\n  ', dstfile
    
    mergeFiles(filenames, dstfile)
    print '合并成功'