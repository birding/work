# -*- coding: utf-8 -*-

import os
import sys, time
from os.path import join, dirname
import syncFiles_lib3 as sFlib


test_files = [{'type': 'static', 'disk':'g:', 'dir': 'NAS'},
              {'type': 'temp', 'disk':'g:', 'dir': 'NAS2'},
              {'type': 'temp', 'disk':'g:', 'dir': 'NAS3'},
              ]

b64vedio_dir = [{'type': 'temp', 'disk':'d:', 'dir': 'b64vedio'},
            {'type': 'temp', 'disk':'H:','dir': 'b64vedio'},
          ]
Backup_dir = [{'type': 'temp', 'disk':'d:', 'dir': 'Backup'},
            {'type': 'temp', 'disk':'H:','dir': 'Backup'},
          ]          
Desktop_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'Desktop'},
            {'type': 'temp', 'disk':'H:','dir': 'Desktop'},
          ]   
MARL2019_dir =  [{'type': 'temp', 'disk':'e:', 'dir': 'MARL2019'},
            {'type': 'temp', 'disk':'H:', 'dir': 'MARL2019'},
          ]
OS_ISO_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'OS_ISO'},
            {'type': 'temp', 'disk':'h:', 'dir': 'OS_ISO'},
          ]
pictures_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'pictures'},
            {'type': 'temp', 'disk':'H:', 'dir': 'pictures'},
          ]
pytool_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'pytool'},
            {'type': 'temp', 'disk':'H:','dir': 'pytool'},
          ]
SDK_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'SDK'},
            {'type': 'temp', 'disk':'h:', 'dir': 'SDK'},
          ]
Supports_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'Supports'},
            {'type': 'temp', 'disk':'h:', 'dir': 'Supports'},
          ]                    
Tech_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'Tech'},
            {'type': 'temp', 'disk':'h:', 'dir': 'Tech'},
          ]
tools_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'tools'},
            {'type': 'temp', 'disk':'h:', 'dir': 'tools'},
          ]          
tools_free_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'tools_free'},
            {'type': 'temp', 'disk':'h:', 'dir': 'tools_free'},
          ]          
work_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'work'},
            {'type': 'temp', 'disk':'h:', 'dir': 'work'},
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

