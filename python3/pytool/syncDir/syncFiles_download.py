# -*- coding: utf-8 -*-

import os
import sys, time
from os.path import join, dirname
import syncFiles_lib2 as sFlib


test_files = [{'type': 'static', 'disk':'c:', 'dir': 'NAS'},
              {'type': 'temp', 'disk':'c:', 'dir': 'NAS2'},
              {'type': 'temp', 'disk':'c:', 'dir': 'NAS3'},
              ]

download_dir = [{'type': 'temp', 'disk':'C:\\MARL2019\\FAETraining\\2019', 'dir': 'IPBU Disty FAE Training'},
            {'type': 'static', 'disk':'\\\\sj1svm1.caveonetworks.com\\FAE-from-castr1\\Training\\Cavium Training\\2019', 'dir': 'IPBU Disty FAE Training'},
          ]
 
copy_dir = [{'type': 'temp', 'disk':'/home/usbdisk', 'dir': 'USBDisk'},
            {'type': 'static', 'disk':'/mnt/usbdisk', 'dir': 'USBDisk'},
          ]
sync_jobs = {
#         "download" : download_dir,
         "copydir" : copy_dir,
        }


def exit_code():
    input("press ENTER key to exit")
    exit(0)
        
        
if __name__ == '__main__':
    sFlib.syncFiles_lib(os.path.realpath(__file__), sync_jobs)
    exit_code()

