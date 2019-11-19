#!/usr/bin/python
# -*- coding: utf-8 -*-
from __future__ import print_function 

import os
import sys, time
from os.path import join, dirname
import filesAnalysis_lib as fAlib


test_files = [{'type': 'static', 'disk':'/home/yisp', 'dir': 'NAS'},
              {'type': 'temp', 'disk':'/home/yisp', 'dir': 'NAS2'},
              {'type': 'temp', 'disk':'/home/yisp', 'dir': 'NAS3'},
              ]

b64vedio_dir = [{'type': 'temp', 'disk':'/media/USBdisk', 'dir': 'b64vedio'},
            {'type': 'temp', 'disk':'/home/yisp','dir': 'b64vedio'},
          ]
Backup_dir = [{'type': 'temp', 'disk':'/media/USBdisk', 'dir': 'Backup'},
            {'type': 'temp', 'disk':'/home/yisp','dir': 'Backup'},
          ]          
Desktop_dir = [{'type': 'temp', 'disk':'/media/USBdisk', 'dir': 'Desktop'},
            {'type': 'temp', 'disk':'/home/yisp','dir': 'Desktop'},
          ]   
MARL2019_dir =  [{'type': 'temp', 'disk':'/media/USBdisk', 'dir': 'MARL2019'},
            {'type': 'temp', 'disk':'/home/yisp', 'dir': 'MARL2019'},
          ]
OS_ISO_dir = [{'type': 'temp', 'disk':'/media/USBdisk', 'dir': 'OS_ISO'},
            {'type': 'temp', 'disk':'/home/yisp', 'dir': 'OS_ISO'},
          ]
pictures_dir = [{'type': 'temp', 'disk':'/media/USBdisk', 'dir': 'pictures'},
            {'type': 'temp', 'disk':'/home/yisp', 'dir': 'pictures'},
          ]        
pytool_dir = [{'type': 'temp', 'disk':'/media/USBdisk', 'dir': 'pytool'},
            {'type': 'temp', 'disk':'/home/yisp','dir': 'pytool'},
          ]
SDK_dir = [{'type': 'temp', 'disk':'/media/USBdisk', 'dir': 'SDK'},
            {'type': 'temp', 'disk':'/home/yisp', 'dir': 'SDK'},
          ]
Supports_dir = [{'type': 'temp', 'disk':'/media/USBdisk', 'dir': 'Supports'},
            {'type': 'temp', 'disk':'/home/yisp', 'dir': 'Supports'},
          ]                    
Tech_dir = [{'type': 'temp', 'disk':'/media/USBdisk', 'dir': 'Tech'},
            {'type': 'temp', 'disk':'/home/yisp', 'dir': 'Tech'},
          ]
tools_dir = [{'type': 'temp', 'disk':'/media/USBdisk', 'dir': 'tools'},
            {'type': 'temp', 'disk':'/home/yisp', 'dir': 'tools'},
          ]          
tools_free_dir = [{'type': 'temp', 'disk':'/media/USBdisk', 'dir': 'tools_free'},
            {'type': 'temp', 'disk':'/home/yisp', 'dir': 'tools_free'},
          ]          
work_dir = [{'type': 'temp', 'disk':'/media/USBdisk', 'dir': 'work'},
            {'type': 'temp', 'disk':'/home/yisp', 'dir': 'work'},
          ]

sync_jobs = {
#         "test" : test_files,
         "b64vedio" : b64vedio_dir,
         "Backup" : Backup_dir,
         "Desktop" : Desktop_dir,
         "MARL2019" : MARL2019_dir,
         "OS_ISO" : OS_ISO_dir,
         "pictures" : pictures_dir,
         "pytool" : pytool_dir,
         "SDK" : SDK_dir,
         "Supports" : Supports_dir,
         "Tech" : Tech_dir,
         "tools_free" : tools_free_dir,
         "tools" : tools_dir,
         "work" : work_dir,
        }


def exit_code():
    raw_input("press ENTER key to exit")
    exit(0)
        
        
if __name__ == '__main__':
    fAlib.searchFiles_lib(os.path.realpath(__file__), sync_jobs)
    exit_code()

