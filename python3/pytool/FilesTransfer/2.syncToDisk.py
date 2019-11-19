# -*- coding: utf-8 -*-

import os
import sys, time
from os.path import join, dirname
from xml.dom import minidom
import lib3copyFile as cpyf
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


def args_prompt():
    global ulUpdate_enable
    global ulUSBDisk
    global uldstXML
    global ulsrcXML

    files = sorted(glob.glob("CFG*.xml"))
    print("Select the destination:\n\t0:sync diff to Disk\n\t1:sync Disk to Disk")
    sel_func = input("(default 0) >> ")
    if sel_func == '1':
        ulUSBDisk = None
        print("Select the source Disk:")
        for i, node in enumerate(files):
            print("\t%d:%s" % (i,node))
        ulSelected_func= get_int_input("(default 0) >> ", 0, len(files))
        ulsrcXML = files[ulSelected_func]
        print("the source IS:  %s " % ulsrcXML)
        files.remove(ulsrcXML)
        print(" ")
    else:
        ulUSBDisk = "c:\\temp\\USBDisk"
        print("Please input the USB DISK URI (default c:\\temp\\USBDisk):")
        dir_uri = input(" >> ")
        if not os.path.exists(dir_uri):
            print("disk %s is not exist, using %s" % (dir_uri, ulUSBDisk))
        else:
            ulUSBDisk = dir_uri
            print("using %s as target" % ulUSBDisk)

    print("Select the destination:")
    for i, node in enumerate(files):
        print("\t%d:%s" % (i,node))
    ulSelected_func= get_int_input("(default 0) >> ", 0, len(files))
    uldstXML = files[ulSelected_func]
    print("the dest IS:  %s " % uldstXML)
    print(" ")

    ulUpdate_enable = 0  
    print("Please Select function:\n\t0:search the difference\n\t1:sync up the files")
    sel_func = input("(default 0) >> ")
    if sel_func == '1':
        print("\nWARNING:sync up is selected. Suggest to search and check the diff first!\n")
        mode = input("input Y/y to confirm (default n) >> ")
        if mode == 'y' or mode == 'Y' :
            ulUpdate_enable =1


def copy_diffToDisk():
    with open(uldstXML,'r',encoding='utf8') as fh:
        domdst=minidom.parse(fh)
    rootdst=domdst.documentElement
    if(rootdst.nodeName != 'rootNode'):
        print("error format xml file %s" % uldstXML)
        return

    for node in rootdst.childNodes:
        if node.nodeType == node.ELEMENT_NODE:
            if(node.nodeName == "rootdir"):
                destdir = node.getAttribute("disk")
                destdir = destdir + os.sep + node.getAttribute("name")
                srcdir = ulUSBDisk + os.sep + node.getAttribute("name")
                if not os.path.exists(srcdir):
                    continue
                if not os.path.exists(destdir):
                    print("rootdir is not exist %s" % destdir)
                    continue
                print("\n%s: sync from diff ..." % node.getAttribute("name"))
                cpyf.__syncDir(srcdir, destdir, ulUpdate_enable) 
                #if ulUpdate_enable == 0 :
                #    print("\n%s: checking deleted files ..." % node.getAttribute("name"))
                #    cpyf.__syncDir(destdir, srcdir, ulUpdate_enable) 


def get_dest_uri_from_root(root_n, name):
    for node in root_n.childNodes:
        if node.nodeType != node.ELEMENT_NODE:
            continue
        if(node.nodeName == "rootdir"):
            if name == node.getAttribute("name"):
                return node
    return None


def copy_DiskToDisk():
    with open(ulsrcXML,'r',encoding='utf8') as fhsrc:
        domsrc=minidom.parse(fhsrc)
    rootsrc=domsrc.documentElement
    if(rootsrc.nodeName != 'rootNode'):
        print("error format xml file %s" % ulsrcXML)
        return

    with open(uldstXML,'r',encoding='utf8') as fh:
        domdst=minidom.parse(fh)
    rootdst=domdst.documentElement
    if(rootdst.nodeName != 'rootNode'):
        print("error format xml file %s" % uldstXML)
        return

    for node in rootsrc.childNodes:
        if node.nodeType == node.ELEMENT_NODE:
            if(node.nodeName == "rootdir"):
                srcdisk = node.getAttribute("disk")
                srcdir = srcdisk + os.sep + node.getAttribute("name")
                destnode = get_dest_uri_from_root(rootdst, node.getAttribute("name"))
                if destnode == None:
                    continue
                if destnode.getAttribute("type") == "static":
                    print("%s: skip dest static dir" % destnode.getAttribute("name"))
                    continue
                destdir = destnode.getAttribute("disk") + os.sep + destnode.getAttribute("name")
                if not os.path.exists(destdir):
                    print("dest dir is not exist %s" % destdir)
                    continue
                if not os.path.exists(srcdir):
                    continue
                print("\n%s: sync up ..." % node.getAttribute("name"))
                cpyf.__syncDir(srcdir, destdir, ulUpdate_enable) 
                if ulUpdate_enable == 0 :
                    if node.getAttribute("type") == "static":
                        print("%s: skip checing deleted file in src static dir " % node.getAttribute("name"))
                        continue
                    print("\n%s: checking deleted files..." % node.getAttribute("name"))
                    cpyf.__syncDir(destdir, srcdir, ulUpdate_enable) 


if __name__ == '__main__':
    args_prompt()
    cpyf.init_logger(os.path.realpath(__file__))
    if ulUSBDisk != None:
        copy_diffToDisk()
    else:
        copy_DiskToDisk()
    exit_code()

