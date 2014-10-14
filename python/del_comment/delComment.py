#coding:utf-8

import os, sys

def help():
    print """命令行格式: DelComment srcpath [dstpath]
    srcpath为源目录或文件的路径；
    detpath必须是已经存在的，它是存放转换后的目录的路径，默认为当前工作目录。
    """

def stripSlash(str):
    return str.rstrip('\\').rstrip('/')

def copyRemoveAllComment(srcpath, dstpath):
    #定义扫描的状态
    STATE_SCAN = 0
    STATE_IS_COMMENT_START = 1
    STATE_IN_BLOCK_COMMENT = 2
    STATE_IN_LINE_COMMENT = 3
    STATE_IS_COMMENT_END = 4
    
    #在dstpath中打开文件
    srcfile = open(srcpath, 'rb')
    dstfile = open(dstpath, 'wb')
    
    #过滤注释
    readState = STATE_SCAN
    text = []
    while True:
        ch = srcfile.read(1)

        if ch == '':
            break;
        if readState == STATE_SCAN:
            if ch == '/':
                readState = STATE_IS_COMMENT_START
            else:
                text.append(ch)
        elif readState == STATE_IS_COMMENT_START:
            if ch == '/':
                readState = STATE_IN_LINE_COMMENT
            elif ch == '*':
                readState = STATE_IN_BLOCK_COMMENT
            else:
                readState = STATE_SCAN
                text.append('/'+ch)
        elif readState == STATE_IN_LINE_COMMENT:
            if ch == '\n':
                readState = STATE_SCAN
        elif readState == STATE_IN_BLOCK_COMMENT:
            if ch == '*':
                readState = STATE_IS_COMMENT_END
        elif readState == STATE_IS_COMMENT_END:
            if ch == '/':
                readState = STATE_SCAN
                ch = srcfile.read(1)
                while ch == '\r' or ch == '\n':
                    ch = srcfile.read(1)
                srcfile.seek(-1, 1)
            else:
                readState = STATE_IN_BLOCK_COMMENT
                srcfile.seek(-1, 1)
    
    #写文件
    dstfile.writelines(text)
    dstfile.flush()
    #关闭文件
    srcfile.close()
    dstfile.close()

def convert(srcpath, dstpath):
    if os.path.isdir(srcpath):
        dirfiles = os.listdir(srcpath)
        for fn in dirfiles:
            srcfp = os.path.join(srcpath, fn)
            if os.path.isfile(srcfp):
                convert(srcfp, dstpath)
            else:
                dstfp = os.path.join(dstpath, fn)
                if not os.path.exists(dstfp):
                    os.mkdir(dstfp)
                convert(srcfp, dstfp)
    else:
        if  srcpath.endswith('.java'):
            srcfn = srcpath
            dstfn = os.path.join(dstpath, os.path.basename(srcpath))
            copyRemoveAllComment(srcfn, dstfn)

if __name__ == '__main__':
    argvlen = len(sys.argv)
    #至少一个参数
    if argvlen > 2+1 or argvlen <= 2-1:
        print '参数错误！:-(\n'
        help()
        sys.exit(1)
    elif not os.path.isfile(sys.argv[1]) and not os.path.isdir(sys.argv[1]) or argvlen == 1 + 2 and os.path.isfile(sys.argv[1]):
        print '参数错误！:-(\n'
        help()
        sys.exit(2)
    #获取源路径
    srcpath = stripSlash(sys.argv[1])
    print 'srcpath: ' + srcpath
    
    #获取目标路径, 
    dstpath = ""
    if argvlen == 1 + 2:
        dstpath = stripSlash(sys.argv[2])
    else:
        dstpath = os.getcwd() + '\\delcomment'
        if not os.path.exists(dstpath):
            os.mkdir(dstpath)
    
    #如果源路径是文件，则直接放在目标路径下	
    if os.path.isdir(srcpath):
        dstpath = dstpath  + '\\' + os.path.basename(srcpath) + 'delcomment'
    if not os.path.exists(dstpath):
        os.mkdir(dstpath)
    print 'dstpath: ' + dstpath
    
    #开始转换
    convert(srcpath, dstpath)