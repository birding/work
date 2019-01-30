#! python2
# -*- coding: utf-8 -*-

import os
import sys
from glob import glob
from os.path import join, dirname
import shutil


sdk620_files = [{'type': 'local', 'os': 'windows', 'ip': 'localhost', 'dir': 'f:\\SDK\\SDK620'},
                {'type': 'remote', 'os': 'windows', 'ip': '10.142.5.30', 'dir': '\\\\10.142.5.30\\root\\home\\SDK\\SDK620'},
                {'type': 'remote', 'os': 'windows', 'ip': '10.142.1.46', 'dir': '\\\\10.142.1.46\\root\\home\\SDK\\SDK620'},
                {'type': 'remote', 'os': 'windows', 'ip': '10.142.1.36', 'dir': '\\\\10.142.1.36\\root\\home\\SDK\\SDK620'}
               ]


def exit_code():
    raw_input("press ENTER key to exit")
    exit(0)


def delete_file(dirs):
    print "dir count: %d" % len(dirs)
    file_name = raw_input("please input filename>> ")
    file_name = os.path.basename(file_name)
    # print file_name
    print "Will delete \'%s\' from the below dir:" % file_name
    for f_dir in dirs:
        print "\t%s" % f_dir["dir"]
    confirm = raw_input("Please confirm (Y/N) >> ")
    if confirm != 'Y' and confirm != 'y':
        return

    for i in range(0, len(dirs)):
        file_path = join(dirs[i]["dir"], file_name)
        print "deleting %s" % file_path,
        if os.path.exists(file_path):
            os.remove(file_path)
            print " - done"
        else:
            print " - not exist"


if __name__ == '__main__':
    delete_file(sdk620_files)
    exit_code()
