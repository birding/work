# -*- coding: utf-8 -*-

import os
import sys, time
from os.path import join, dirname
from xml.dom import minidom
import lib3copyFile as cpyf
import glob
import pandas as pd
import shutil
import platform

def exit_code():
    input("press ENTER key to exit")
    exit(0)


def get_input(str_prompt):
    if sys.version_info.major == 2:
        ret = raw_input(str_prompt)
    else:
        ret = input(str_prompt)
    return ret


def get_int_input(str_prompt, minV, maxV):
    sel_func = get_input(str_prompt)
    ulSelected_func = minV 
    try:
        ulSelected_func= int(sel_func)
    except:
        ulSelected_func = minV 
    finally:
        if ulSelected_func < minV or ulSelected_func >= maxV :
            ulSelected_func = minV
    return ulSelected_func


def get_dest_node_from_root(root_n, name):
    for node in root_n.childNodes:
        if node.nodeType != node.ELEMENT_NODE:
            continue
        if(node.nodeName == "rootdir"):
            if name == node.getAttribute("name"):
                return node
    return None


def get_dest_uri_from_root(name):
    if ulUSBDisk != None:
        return ulUSBDisk + os.sep + name

    with open(uldstXML,'r',encoding='utf8') as fh:
        domdst=minidom.parse(fh)
    rootdst=domdst.documentElement
    if(rootdst.nodeName != 'rootNode'):
        print("error format xml file %s" % uldstXML)
        return None

    destnode=get_dest_node_from_root(rootdst, name)
    if destnode == None:
        return None
    destdir = destnode.getAttribute("disk") + os.sep + destnode.getAttribute("name")
    if not os.path.exists(destdir):
        #print("dest rootdir is not exist %s" % destdir)
        return None
    return destdir


def delete_procedure():
    if not os.path.exists(ulcfg_csv):
        print("%s not exist" % ulcfg_csv)
        return 0
    print("reading %s ... " % ulcfg_csv)

    df = pd.read_csv(ulcfg_csv, encoding='utf_8_sig')
    #print(df)
    print("## %d rows for process ##" % df.shape[0])
    for index,row in df.iterrows():
        #print(index, row['rootdir'], row['uri'], row['type'])
        destdir = get_dest_uri_from_root(row['rootdir'])
        if destdir == None:
            print("[%d]no %s in this disk" % (index,row['rootdir']))
            continue
        if not os.path.exists(destdir):
            print("[%d]destdir is not exist %s" % (index,destdir))
            continue
        #print(row['rootdir'],destdir)
        fileurl=destdir +os.sep + row['fileurl']
        if ulLinuxOS == 1:
            fileurl = fileurl.replace('\\','/')

        if not os.path.exists(fileurl):
            continue
        if ulUpdate_enable == 0:
            if((row['type'] == 'dir') and (os.path.isdir(fileurl))):
                print("[%d] %s %s will be deleted" % (index,row['type'],fileurl))
            elif((row['type'] == 'file') and (os.path.isfile(fileurl))):
                print("[%d] %s %s will be deleted" % (index,row['type'],fileurl))
            else:
                print("error occur [%d]%s in type %s" % (index,fileurl,row['type']))
        else:
            print("[%d] %s %s is deleted" % (index,row['type'],fileurl))
            if((row['type'] == 'dir') and (os.path.isdir(fileurl))):
                shutil.rmtree(fileurl)
            elif((row['type'] == 'file') and (os.path.isfile(fileurl))):
                os.remove(fileurl)
            else:
                print("\terror occur [%d] %s in type %s" % (index,fileurl,row['type']))


def args_prompt():
    global uldstXML
    global ulcfg_csv
    global ulUpdate_enable
    global ulUSBDisk
    global ulLinuxOS

    files = sorted(glob.glob("delete*.csv"))
    print("Select cfg file for deleting:")
    for i, node in enumerate(files):
        print("\t%d:%s" % (i,node))
    ulSelected_func= get_int_input("(default 0) >> ", 0, len(files))
    ulcfg_csv = files[ulSelected_func]
    print("the deleting cfg file IS:  %s " % ulcfg_csv)
    print(" ")

    ulUSBDisk = None  
    print("Please Select function:\ndelete files\n\t0:in HDD Disk\n\t1:in USB Disk")
    sel_func = input("(default 0) >> ")
    if sel_func == '1':
        ulUSBDisk = "c:\\temp\\USBDisk"
        print("Please input the USB DISK URI (default %s):" % ulUSBDisk)
        dir_uri = input(" >> ")
        if not os.path.exists(dir_uri):
            print("disk %s is not exist, using %s" % (dir_uri, ulUSBDisk))
        else:
            ulUSBDisk = dir_uri
            print("using %s as target" % ulUSBDisk)
    else:
        files = sorted(glob.glob("CFG*.xml"))
        print("Select the disk:")
        for i, node in enumerate(files):
            print("\t%d:%s" % (i,node))
        ulSelected_func= get_int_input("(default 0) >> ", 0, len(files))
        uldstXML = files[ulSelected_func]
        print("the disk IS:  %s " % uldstXML)
        print(" ")

    ulUpdate_enable = 0  
    print("Please Select function:\n\t0:checking files\n\t1:delete files")
    sel_func = input("(default 0) >> ")
    if sel_func == '1':
        print("\nWARNING:deleting is selected. Suggest to search and check first!\n")
        mode = input("input Y/y to confirm (default n) >> ")
        if mode == 'y' or mode == 'Y' :
            ulUpdate_enable =1

    ulLinuxOS=0
    sysstr = platform.system()
    if sysstr == "Windows":
        print ("run on Windows")
    elif sysstr == "Linux":
        print ("run on Linux, will replace '\\' to '/'")
        ulLinuxOS=1
    else:
        print ("Other System tasks: %s" % sysstr)


if __name__ == '__main__':
    args_prompt()
    delete_procedure()
    exit_code()

