#coding:gbk

import os, sys, zipfile
from os import path
from datetime import datetime


src_path = '.'
dst_path = 'jcdebugger_%s.zip' % datetime.now().strftime('%y_%m_%d')
zip_root = 'jcdebugger'
valid_ext = ['.py', '.pyd']

# required file
required_file = ['jcdebugger.exe', 'dbglib.pyd', 'help.docx', 'history.txt', 'libs', 'plugin', 'script']

def check_file():
    global src_path, dst_path
    for name in required_file:
        file_path = path.join(src_path, name)
        if not path.exists(file_path) :
            print_error('cannot find path: %s' % file_path)
            sys.exit(1)
    
def make_zip():
    global src_path, dst_path, zip_root
    zf = zipfile.ZipFile(dst_path, 'w', zipfile.ZIP_DEFLATED)

    for name in required_file:
        file_path = path.join(src_path, name)
        if path.isdir(file_path):
            process_dir(zf, file_path, path.join(zip_root, name) )
        else:
            zf.write(file_path, path.join(zip_root, name))

    zf.close()

def process_dir(zf, dir_path, zf_dir):
    for name in os.listdir(dir_path) :
        file_path = path.join(dir_path, name)
        if path.isdir(file_path):
            process_dir(zf, file_path, path.join(zf_dir, name))
        else:
            if path.splitext(name)[1] in valid_ext:
                zf.write(file_path, path.join(zf_dir, name))

def print_error(str):
    print 'error: %s'

if '__main__' == __name__ :
    check_file()
    make_zip()
    print 'over.'
