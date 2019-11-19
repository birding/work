# -*- coding: utf-8 -*-

import os
import sys, time
from os.path import join, dirname
from xml.dom import minidom
import glob


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


def get_childnode_by_name(childname, p_node):
    #print(childname,p_node.getAttribute("name"))
    for node in p_node.childNodes:
        if node.nodeType != node.ELEMENT_NODE:
            continue
        if childname == node.getAttribute("name"):
            #print("find %s" % childname)
            return node
    return None


def checking_exist_in_disk(fileuri, filesize, filetime, diskDom):
    rootdst=diskDom.documentElement
    if(rootdst.nodeName != 'rootNode'):
        print("error format xml file %s" % uldstXML)
        return 0
    dirlist=fileuri.split(os.path.sep)
    #print(dirlist)
    cur_node = rootdst
    for i,name in enumerate(dirlist):
        cur_node = get_childnode_by_name(name, cur_node)
        if(cur_node == None):
            if i == 0 :
                print("no rootdir, skip checking")
                return 1
            else:
                return 0

    if(cur_node == None):
        return 0
    if(cur_node.nodeName != 'file'):
        print("error in search")
        return 0
    #print(filetime[:-1],filesize)
    #print(cur_node.getAttribute("date")[:-1],cur_node.getAttribute("size"))
    if(filetime[:-1] == cur_node.getAttribute("date")[:-1]) and (filesize == cur_node.getAttribute("size")):
        #print("match !")
        return 1
    return 0


def checking_exist_in_disks(diskdir, fileurl):
    if not os.path.exists(fileurl):
        return 0
    filestat = os.stat(fileurl)
    filesize_str = str(filestat.st_size)
    timelocal = time.localtime(filestat.st_mtime)
    filetime_str = time.strftime("%Y%m%d-%H:%M:%S",timelocal)
    
    fileuri=fileurl[len(diskdir)+1:]
    #print(fileuri)
    print("checking %s" % fileurl)
    
    for i, curDom in enumerate(diskDoms):
        ret = checking_exist_in_disk(fileuri,filesize_str, filetime_str, curDom)
        if (ret == 0):
            return 0
    if ulUpdate_enable == 1:
        print("... removed")
        os.remove(fileurl)
    else:
        print("will be deleted ...")

    return 1


def delete_files():
    for root, dirs, files in os.walk(ulUSBDisk):
        for name in files:
            f_url = os.path.join(root, name)
            checking_exist_in_disks(ulUSBDisk,f_url)


def args_prompt():
    global ulUSBDisk
    global diskDoms
    global ulUpdate_enable

    ulUpdate_enable = 0  
    print("Please Select function:\n\t0:check the files\n\t1:check and delete files")
    sel_func = input("(default 0) >> ")
    if sel_func == '1':
        print("\nWARNING:delete is selected. Suggest check first!\n")
        mode = input("input Y/y to confirm (default n) >> ")
        if mode == 'y' or mode == 'Y' :
            ulUpdate_enable =1

    diskDoms = list() 
    ulUSBDisk = "c:\\temp\\USBDisk"
    print("Please input the USB DISK URI (default c:\\temp\\USBDisk):")
    dir_uri = input(" >> ")
    if not os.path.exists(dir_uri):
        print("disk %s is not exist, using %s" % (dir_uri, ulUSBDisk))
    else:
        if (dir_uri[-1] == '/') or (dir_uri[-1] == '\\'):
            dir_uri = dir_uri[:-1]
        ulUSBDisk = dir_uri
        print("using %s as target" % ulUSBDisk)

    diskList = sorted(glob.glob("Disk_*.xml"))
    for i, node in enumerate(diskList):
        print("\t%s" % (node))
        with open(node,'r',encoding='utf8') as fh:
            domdst=minidom.parse(fh)
        rootdst=domdst.documentElement
        if(rootdst.nodeName != 'rootNode'):
            print("error format xml file %s" % uldstXML)
            return
        diskDoms.append(domdst)
    
    #for i, node in enumerate(diskList):
    #    print("\t%s" % (node))
    #for root, dirs, files in os.walk(ulUSBDisk):
        #print('root_dir:', root)
        #print('sub_dirs:', dirs)
        #print('files:', files)
    #    for name in files:
    #        print(os.path.join(root, name))


if __name__ == '__main__':
    args_prompt()
    delete_files()
    exit_code()

