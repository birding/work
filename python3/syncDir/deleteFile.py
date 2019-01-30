# -*- coding: utf-8 -*-

import os
import sys
from glob import glob
from os.path import join, dirname
import shutil

test_files = [{'type': 'static', 'disk':'g:', 'dir': 'NAS'},
              {'type': 'temp', 'disk':'g:', 'dir': 'NAS2'},
              {'type': 'temp', 'disk':'g:', 'dir': 'NAS3'},
              ]
Backup_dir = [{'type': 'static', 'disk':'f:', 'dir': 'Backup'},
            {'type': 'temp', 'disk':'H:','dir': 'Backup'},
          ]
OS_ISO_dir = [{'type': 'static', 'disk':'f:', 'dir': 'OS_ISO'},
            {'type': 'temp', 'disk':'H:', 'dir': 'OS_ISO'},
          ]          
SDK_dir = [{'type': 'static', 'disk':'f:', 'dir': 'SDK'},
            {'type': 'temp', 'disk':'h:', 'dir': 'SDK'},
          ]
Supports_dir = [{'type': 'static', 'disk':'f:', 'dir': 'Supports'},
            {'type': 'temp', 'disk':'h:', 'dir': 'Supports'},
          ]                    
Tech_dir = [{'type': 'static', 'disk':'f:', 'dir': 'Tech'},
            {'type': 'temp', 'disk':'h:', 'dir': 'Tech'},
          ]
work_dir = [{'type': 'static', 'disk':'f:', 'dir': 'work'},
            {'type': 'temp', 'disk':'H:', 'dir': 'work'},
          ]
          

sync_jobs = [
#         test_files,
         Backup_dir,
         OS_ISO_dir,
         SDK_dir,
         Supports_dir,
         Tech_dir,
         work_dir,
        ]
        
deprecated_files = [{'type': 'local', 'dir': 'ALL', 'file': '*.bak'},
#                   {'type': 'local', 'dir': 'NAS3', 'file': '*.txt'},
               ]


def exit_code():
    input("press ENTER key to exit")
    exit(0)


def delete_files(files, diskPath):
    print("file count: %d" % len(files))
    # print file_name

    for i in range(0, len(dirs)):
        file_path = join(dirs[i]["dir"], file_name)
        print("deleting %s" % file_path)
        if os.path.exists(file_path):
            os.remove(file_path)
            print(" - done")
        else:
            print(" - not exist")

Delete_enable = 0
searched_list = []
def get_files_of_current(searchP):
    ret=glob(searchP)
    for fInfo in ret:
        if Delete_enable == 1:
            print("Deleting\t\t %s" %fInfo)
            os.remove(fInfo)
        else:
            print("found\t\t %s" %fInfo)
    
    
def search_files(fileName, filePath):
    if not os.path.exists(filePath):
        return
    searchPath = filePath + os.sep + fileName
    if searchPath in searched_list:
        return
    print("searching:\t" + searchPath)
    searched_list.append(searchPath)
    get_files_of_current(searchPath)
    
    for (root, dirs, files) in os.walk(filePath):
        for d in dirs:
            d = os.path.join(root, d)
            # print(d)
            if os.path.exists(d):
                searchPath = d + os.sep + fileName
                get_files_of_current(searchPath)
            

def walking_dir(curdir):
    for fileInfo in deprecated_files:
        if fileInfo['dir'] != 'ALL':
            workPath =  os.path.join(curdir['disk'], os.sep, fileInfo['dir'])
        else:
            workPath =  os.path.join(curdir['disk'], os.sep, curdir['dir'])
        search_files(fileInfo['file'], workPath)


def walking_dirs():
    # searched_list.clear()
    for job in sync_jobs:
        for i in range(0, len(job)):
            if os.path.exists(job[i]['disk']):
                print("dir[%d]:%s" % (i, job[i]['dir']))
                walking_dir(job[i])



if __name__ == '__main__':
    mode = input("input D/d for delete file, others for search>>")
    Delete_enable = 0
    if mode == 'd' or mode == 'D' :
        Delete_enable =1
    walking_dirs()
    exit_code()
