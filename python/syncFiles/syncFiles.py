#! python2
# -*- coding: utf-8 -*-

import os
import sys
from glob import glob
from os.path import join, dirname
import shutil


test_files = [{'type': 'local', 'os': 'windows', 'ip': 'localhost', 'dir': 'f:\\NAS'},
              {'type': 'remote', 'os': 'windows', 'ip': 'none', 'dir': 'f:\\NAS2'},
              {'type': 'remote', 'os': 'windows', 'ip': 'none', 'dir': 'f:\\NAS3'},
              ]

sdk620_files = [{'type': 'local', 'os': 'windows', 'ip': 'localhost', 'dir': 'f:\\SDK\\SDK620'},
                {'type': 'remote', 'os': 'windows', 'ip': '10.142.5.30', 'dir': '\\\\10.142.5.30\\root\\home\\SDK\\SDK620'},
                {'type': 'remote', 'os': 'windows', 'ip': '10.142.5.30', 'dir': '\\\\10.142.5.30\\root\\diskdata\\SDK\\SDK620'},
                {'type': 'remote', 'os': 'windows', 'ip': '10.142.1.46', 'dir': '\\\\10.142.1.46\\root\\home\\SDK\\SDK620'},
                {'type': 'remote', 'os': 'windows', 'ip': '10.142.1.36', 'dir': '\\\\10.142.1.36\\root\\home\\SDK\\SDK620'},
               ]

sdk312_files = [{'type': 'local', 'os': 'windows', 'ip': 'localhost', 'dir': 'f:\\SDK\\SDK312'},
                {'type': 'remote', 'os': 'windows', 'ip': '10.142.1.36', 'dir': '\\\\10.142.1.36\\root\\home\\SDK\\SDK312'},
                {'type': 'remote', 'os': 'windows', 'ip': '10.142.5.30', 'dir': '\\\\10.142.5.30\\root\\diskdata\\SDK\\SDK312'},
               ]

def get_files_list(dirs):
    for i in range(0, len(dirs)):
        if dirs[i]["os"] == 'windows':
            dir_info = dirs[i]
            uri = dir_info["dir"]
            # print uri
            # print join(uri, "*")
            ret_files = glob(join(uri, "*"))
            # print "ret_files : %s" % ret_files
            # print len(ret_files)
            for fil in ret_files:
                if not os.path.isfile(fil):
                    ret_files.remove(fil)
            print "%d files in %s " % (len(ret_files), uri)
        else:
            print dirs[i]["os"]


def sync_dir(dir_info, src_idx, dst_idx):
    if src_idx == dst_idx:
        return
    if dir_info[dst_idx]["type"] == "local":
        return
    src_uri = dir_info[src_idx]["dir"]
    dst_uri = dir_info[dst_idx]["dir"]
    print "\n sync up  %s to %s" % (src_uri, dst_uri)
    if not os.path.exists(src_uri):
        print src_uri + " not exist"
        return
    if not os.path.exists(dst_uri):
        print dst_uri + " not exist"
        return
    src_files = glob(join(src_uri, "*"))

    print " (%d) files for sync up" % (len(src_files))
    # print src_files
    idx = 0
    for s_file in src_files:
        idx = idx + 1
        if not os.path.isfile(s_file):
            print "copying[%d/%d] %s - skip directory" % (idx, len(src_files), s_file)
            continue
        filename = os.path.basename(s_file)
        print "copying[%d/%d] %s - " % (idx, len(src_files), filename),
        dst_file = join(dst_uri, filename)
        if os.path.exists(dst_file):
            f_size = os.path.getsize(s_file)
            f2_size = os.path.getsize(dst_file)
            if f_size == f2_size:
                print "dstfile is existing"
                continue
            else:
                print "delete&replace partial dstfile ",
                os.remove(dst_file)

        shutil.copyfile(s_file, dst_file)
        print " - done"


def exit_code():
    raw_input("press ENTER key to exit")
    exit(0)


def sync_dirs(dirs):
    print "dir count: %d" % len(dirs)
    # get_files_list(dirs)

    for i in range(0, len(dirs)):
        for j in range(0, len(dirs)):
            sync_dir(dirs, i, j)


if __name__ == '__main__':
    sync_dirs(sdk620_files)
    sync_dirs(sdk312_files)
    exit_code()
