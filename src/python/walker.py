#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import os
import stat
import time

from os import path

fns = None

def inode(s):
    return u'i-node: %u' % s.st_ino

def size(s):
    return u'size: %u' % s.st_size

def perms(s):
    p = [u'-'] * 9
    signs = u'rwx'
    for i, flag in enumerate([stat.S_IRUSR,
                              stat.S_IWUSR,
                              stat.S_IXUSR,
                              stat.S_IRGRP,
                              stat.S_IWGRP,
                              stat.S_IXGRP,
                              stat.S_IROTH,
                              stat.S_IWOTH,
                              stat.S_IXOTH]):
        if s.st_mode & flag:
            p[i] = signs[i % 3]

    return u'permissions: %s' % u''.join(p)

def ids(s):
    return u'user id: %u, group id: %u' % (s.st_uid, s.st_gid)

def mtime(s):
    return u'modification time: %s' % time.ctime(s.st_mtime)

def walk(dir, indent=1):
    indent_string = u'  ' * indent

    def print_info(p):
        s = os.stat(p)
        if fns:
            for fn in fns:
                print u'%s- %s' % (indent_string, fn(s))
            print

    dir_content = os.listdir(dir)
    subdirs = [i for i in dir_content
               if path.isdir(path.join(dir, i))]
    subdirs.sort()

    files = [i for i in dir_content
             if path.isfile(path.join(dir, i))]
    files.sort()

    for d in subdirs:
        print u'%s%s/' % (indent_string, d)
        print_info(path.join(dir, d))
        walk(path.join(dir, d), indent + 1)

    for f in files:
        print u'%s%s' % (indent_string, f)
        print_info(path.join(dir, f))

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=u'Walk directories.')

    parser.add_argument(u'dirs', metavar=u'd', type=unicode, nargs=u'*',
                        default=u'.', help=u'directories to walk in and over')

    for option, fn, h in [(u'-i', inode, u'print i-node'),
                          (u'-s', size,  u'print size'),
                          (u'-p', perms, u'print permissions'),
                          (u'-o', ids,   u'print owners'),
                          (u'-m', mtime, u'print modification time')]:
        parser.add_argument(option, dest=u'fns', action=u'append_const',
                            const=fn, help=h)

    args = parser.parse_args()
    dirs = args.dirs
    fns = args.fns

    dirs.sort()
    for d in args.dirs:
        print u'%s/' % d.rstrip(u'/')
        walk(d)
