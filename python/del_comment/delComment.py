#coding:utf-8

import os, sys

def help():
    print """�����и�ʽ: DelComment srcpath [dstpath]
    srcpathΪԴĿ¼���ļ���·����
    detpath�������Ѿ����ڵģ����Ǵ��ת�����Ŀ¼��·����Ĭ��Ϊ��ǰ����Ŀ¼��
    """

def stripSlash(str):
    return str.rstrip('\\').rstrip('/')

def copyRemoveAllComment(srcpath, dstpath):
    #����ɨ���״̬
    STATE_SCAN = 0
    STATE_IS_COMMENT_START = 1
    STATE_IN_BLOCK_COMMENT = 2
    STATE_IN_LINE_COMMENT = 3
    STATE_IS_COMMENT_END = 4
    
    #��dstpath�д��ļ�
    srcfile = open(srcpath, 'rb')
    dstfile = open(dstpath, 'wb')
    
    #����ע��
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
    
    #д�ļ�
    dstfile.writelines(text)
    dstfile.flush()
    #�ر��ļ�
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
    #����һ������
    if argvlen > 2+1 or argvlen <= 2-1:
        print '��������:-(\n'
        help()
        sys.exit(1)
    elif not os.path.isfile(sys.argv[1]) and not os.path.isdir(sys.argv[1]) or argvlen == 1 + 2 and os.path.isfile(sys.argv[1]):
        print '��������:-(\n'
        help()
        sys.exit(2)
    #��ȡԴ·��
    srcpath = stripSlash(sys.argv[1])
    print 'srcpath: ' + srcpath
    
    #��ȡĿ��·��, 
    dstpath = ""
    if argvlen == 1 + 2:
        dstpath = stripSlash(sys.argv[2])
    else:
        dstpath = os.getcwd() + '\\delcomment'
        if not os.path.exists(dstpath):
            os.mkdir(dstpath)
    
    #���Դ·�����ļ�����ֱ�ӷ���Ŀ��·����	
    if os.path.isdir(srcpath):
        dstpath = dstpath  + '\\' + os.path.basename(srcpath) + 'delcomment'
    if not os.path.exists(dstpath):
        os.mkdir(dstpath)
    print 'dstpath: ' + dstpath
    
    #��ʼת��
    convert(srcpath, dstpath)