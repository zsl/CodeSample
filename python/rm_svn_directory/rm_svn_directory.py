#coding:utf-8

import os
import shutil
import stat

def processError(func, path, exceptionInfo):
    #os.chmod(path, stat.S_IWRITE)
    os.system('attrib -r -h ' + path) # »•µÙ÷ª∂¡£¨“˛≤ÿ  Ù–‘
    if os.path.isdir(path):
        os.rmdir(path)
    else:
        os.remove(path)

def processDir(dir):
    for name in os.listdir(dir):
        abspath = os.path.join(dir, name)
        if os.path.isdir(abspath):
            if '.svn' == name.lower():
                print 'rmdir: ', abspath
                shutil.rmtree(abspath, False, processError)
            else:
                processDir(abspath)



if __name__ == '__main__':
    processDir('.')
