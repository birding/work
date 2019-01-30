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
         test_files,
#         Backup_dir,
#         OS_ISO_dir,
#         SDK_dir,
#         Supports_dir,
#         Tech_dir,
#         work_dir,
        ]
          
def __copyFile(fromfile, tofile):
    if not os.path.exists(tofile) :
        print("new: %s ==> %s" % (fromfile,tofile), end=' ')
        shutil.copy2(fromfile,tofile)
        print(" - done")
        return 1
    fromstat = os.stat(fromfile)
    tostat = os.stat(tofile)
    if fromstat.st_mtime > (tostat.st_mtime+5):
        print("TIME %d ==> %d" % (fromstat.st_mtime,tostat.st_mtime))
        print("update %s ==> %s" % (fromfile,tofile), end=' ')
        shutil.copy2(fromfile,tofile)
        print(" - done")
        return 1
    
    return 0


def __copyDir(fromdir,todir):
    todir = todir.strip()
    todir = todir.rstrip(os.sep)
    if not os.path.exists(todir):
        try:
            os.makedirs(todir)
        except OSError as e:
            print(e)
            return
    tol_count = 0
    for filename in os.listdir(fromdir):
        if filename.startswith('.'):
            continue
        fromfile = fromdir + os.sep + filename
        tofile = todir + os.sep + filename
        if os.path.isdir(fromfile):
            tol_count += __copyDir(fromfile,tofile)
        else:
            tol_count += __copyFile(fromfile,tofile)
    return tol_count


def sync_dir(dir_info, src_idx, dst_idx):
    if src_idx == dst_idx:
        return
    src_uri = dir_info[src_idx]["disk"] + os.sep + dir_info[src_idx]["dir"]
    dst_uri = dir_info[dst_idx]["disk"] + os.sep + dir_info[dst_idx]["dir"]
    print("sync up  %s to %s" % (src_uri, dst_uri))
    if not os.path.exists(src_uri):
        print("@@src dir not exist: "+ src_uri)
        return
    if not os.path.exists(dir_info[dst_idx]["disk"]):
        print("@@dest disk not exist: "+ src_uri)
        return    
    if dir_info[dst_idx]["type"] == 'static':
        print("----skip static directory:%s" % dst_uri)
        return
    if not os.path.exists(os.path.dirname(dst_uri)):
        print("@@base of dest dir not exist: " + os.path.dirname(dst_uri))
        return
    count = __copyDir(src_uri, dst_uri)
    return count


def exit_code():
    input("press ENTER key to exit")
    exit(0)


def sync_dirs(dirs):
    for i in range(0, len(dirs)):
        for j in range(0, len(dirs)):
            sync_dir(dirs, i, j)


if __name__ == '__main__':
    print("#Total jobs %d" % len(sync_jobs))
    for job in sync_jobs:
        print("#start sync job %d's %d directories " % (sync_jobs.index(job), len(job)))
        sync_dirs(job)
    # sync_dirs(test_files)
    exit_code()
