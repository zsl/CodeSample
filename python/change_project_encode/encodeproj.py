#coding:gbk
ICU = 'uconv.exe'
CMD_TEM = ICU + ' -f %s -t %s -o %s --remove-signature %s'

import sys
import os
import argparse
import shutil
from xml.dom import minidom

def parse_arg():
    parser = argparse.ArgumentParser('parse a jcide project encoding to another.')
    parser.add_argument('-f', '--from-code', dest='from_code', help='source coding')
    parser.add_argument('-t', '--to-code', dest='to_code', help='dest coding')
    parser.add_argument('-o', '--output', dest='d_proj', help='output dir')
    parser.add_argument(dest='proj', help='proj dir')
    #args = parser.print_help()
    args = parser.parse_args()
    return args

def process_proj_file(srcpath, dstpath, coding):
    # 修改工程的encoding 属性
    import re
    if re.match(r'utf(\w+)', coding, re.IGNORECASE):
        coding = 'utf-%s' % coding[3:]

    dom = minidom.parse(srcpath)
    proj_node = dom.getElementsByTagName('JavaCardProject')[0]
    if proj_node.hasAttribute('encoding'):
        old_coding = proj_node.getAttribute('encoding')
        assert old_coding.lower() != coding.lower()
    else:
        proj_node.setAttribute('encoding', coding)

    with open(dstpath, 'w') as outf:
        dom.writexml(outf)
        print 'convert to %s' % dstpath

    dom.unlink()

    
def process_java_file(srcpath, dstpath, from_code, to_code):
    # 修改java文件的编码
    cmd = CMD_TEM % (from_code, to_code, dstpath, srcpath)
    print cmd
    os.system(cmd)

def process_other_file(srcpath, dstpath):
    print 'copy to %s' % dstpath
    shutil.copyfile(srcpath, dstpath)

def walk_proj(args, srcdir, dstdir):
    if not os.path.exists(dstdir):
        os.mkdir(dstdir)

    for name in os.listdir(srcdir):
        srcpath = os.path.join(srcdir, name)
        dstpath = os.path.join(dstdir, name)

        print 'process %s ...'% srcpath
        if os.path.isdir(srcpath):
            walk_proj(args, srcpath, dstpath)
        else:
            # second, convert file
            ext = os.path.splitext(srcpath)[-1]
            if '.jcproj' == ext:
                process_proj_file(srcpath, dstpath, args.to_code)
            elif '.java' == ext:
                process_java_file(srcpath, dstpath, args.from_code, args.to_code)
            else:
                process_other_file(srcpath, dstpath)

        print 'end process %s'% srcpath

def main():
    args = parse_arg()
    print 'srcproj: %s, encoding: %s' % (args.proj, args.from_code)
    print 'dstproj: %s, encoding: %s' % (args.d_proj, args.to_code)
    print
    walk_proj(args, args.proj, args.d_proj)

if __name__ == '__main__':
    main()
