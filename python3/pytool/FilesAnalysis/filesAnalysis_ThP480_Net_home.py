# -*- coding: utf-8 -*-

import os
import sys, time
from os.path import join, dirname
import filesAnalysis_lib as fAlib


test_files = [{'type': 'static', 'disk':'c:', 'dir': 'NAS'},
              {'type': 'temp', 'disk':'c:', 'dir': 'NAS2'},
              {'type': 'temp', 'disk':'c:', 'dir': 'NAS3'},
              ]

b64vedio_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'b64vedio'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk','dir': 'b64vedio'},
          ]
Backup_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'Backup'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk','dir': 'Backup'},
          ]          
Desktop_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'Desktop'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk','dir': 'Desktop'},
          ]   
MARL2019_dir =  [{'type': 'static', 'disk':'c:', 'dir': 'MARL2019'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk', 'dir': 'MARL2019'},
          ]
OS_ISO_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'OS_ISO'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk', 'dir': 'OS_ISO'},
          ]
pictures_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'pictures'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk', 'dir': 'pictures'},
          ]        
pytool_dir = [{'type': 'temp', 'disk':'c:', 'dir': 'pytool'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk','dir': 'pytool'},
          ]
SDK_dir = [{'type': 'static', 'disk':'c:', 'dir': 'SDK'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk', 'dir': 'SDK'},
          ]
Supports_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'Supports'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk', 'dir': 'Supports'},
          ]                    
Tech_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'Tech'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk', 'dir': 'Tech'},
          ]
tools_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'tools'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk', 'dir': 'tools'},
          ]          
tools_free_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'tools_free'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk', 'dir': 'tools_free'},
          ]          
work_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'work'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk', 'dir': 'work'},
          ]
 
sync_jobs = {
#         "test" : test_files,
#         "b64vedio" : b64vedio_dir,
         "Backup" : Backup_dir,
#         "Desktop" : Desktop_dir,
         "MARL2019" : MARL2019_dir,
#         "OS_ISO" : OS_ISO_dir,
#         "pictures" : pictures_dir,
         "pytool" : pytool_dir,
         "SDK" : SDK_dir,
#         "Supports" : Supports_dir,
#         "Tech" : Tech_dir,
#         "tools_free" : tools_free_dir,
#         "tools" : tools_dir,
#         "work" : work_dir,
        }


def exit_code():
    input("press ENTER key to exit")
    exit(0)
        
        
if __name__ == '__main__':
    fAlib.searchFiles_lib(os.path.realpath(__file__), sync_jobs)
    exit_code()

