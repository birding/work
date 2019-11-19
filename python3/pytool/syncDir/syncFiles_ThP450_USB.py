# -*- coding: utf-8 -*-

import os
import sys, time
from os.path import join, dirname
import syncFiles_lib3 as sFlib


test_files = [{'type': 'static', 'disk':'c:', 'dir': 'NAS'},
              {'type': 'temp', 'disk':'c:', 'dir': 'NAS2'},
              {'type': 'temp', 'disk':'c:', 'dir': 'NAS3'},
              ]

b64vedio_dir = [{'type': 'temp', 'disk':'F:', 'dir': 'b64vedio'},
            {'type': 'temp', 'disk':'J:','dir': 'b64vedio'},
          ]
Backup_dir = [{'type': 'temp', 'disk':'f:', 'dir': 'Backup'},
            {'type': 'temp', 'disk':'J:','dir': 'Backup'},
          ]          
Desktop_dir = [{'type': 'temp', 'disk':'f:', 'dir': 'Desktop'},
            {'type': 'temp', 'disk':'J:','dir': 'Desktop'},
          ]   
MARL2019_dir =  [{'type': 'temp', 'disk':'f:', 'dir': 'MARL2019'},
            {'type': 'temp', 'disk':'J:', 'dir': 'MARL2019'},
          ]
OS_ISO_dir = [{'type': 'temp', 'disk':'f:', 'dir': 'OS_ISO'},
            {'type': 'temp', 'disk':'J:', 'dir': 'OS_ISO'},
          ]
pictures_dir = [{'type': 'temp', 'disk':'f:', 'dir': 'pictures'},
            {'type': 'temp', 'disk':'J:', 'dir': 'pictures'},
          ]        
pytool_dir = [{'type': 'temp', 'disk':'f:', 'dir': 'pytool'},
            {'type': 'temp', 'disk':'J:','dir': 'pytool'},
          ]
SDK_dir = [{'type': 'temp', 'disk':'f:', 'dir': 'SDK'},
            {'type': 'temp', 'disk':'J:', 'dir': 'SDK'},
          ]
Supports_dir = [{'type': 'temp', 'disk':'f:', 'dir': 'Supports'},
            {'type': 'temp', 'disk':'J:', 'dir': 'Supports'},
          ]                    
Tech_dir = [{'type': 'temp', 'disk':'f:', 'dir': 'Tech'},
            {'type': 'temp', 'disk':'J:', 'dir': 'Tech'},
          ]
tools_dir = [{'type': 'temp', 'disk':'f:', 'dir': 'tools'},
            {'type': 'temp', 'disk':'J:', 'dir': 'tools'},
          ]          
tools_free_dir = [{'type': 'temp', 'disk':'f:', 'dir': 'tools_free'},
            {'type': 'temp', 'disk':'J:', 'dir': 'tools_free'},
          ]          
work_dir = [{'type': 'temp', 'disk':'f:', 'dir': 'work'},
            {'type': 'temp', 'disk':'J:', 'dir': 'work'},
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
    input("press ENTER key to exit")
    exit(0)
        
        
if __name__ == '__main__':
    sFlib.syncFiles_lib(os.path.realpath(__file__), sync_jobs)
    exit_code()

