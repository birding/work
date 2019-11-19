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


def get_total_size_of_diff(root_node):
    files=root_node.getElementsByTagName('file');
    #print(files, len(files))
    size=0
    for f in files:
        #print(f.getAttribute("name"),f.getAttribute("size"))
        size = size + int(f.getAttribute("size"))
    print("\n%s has %d files in %d bytes\n" % (diffxml,len(files),size))
    return size


def checking_dir(cur_node, cur_uri):
    if not os.path.exists(cur_uri):
        print("%s is not exist" % cur_uri)
        return -1

    for n in cur_node.childNodes:
        if n.nodeType == n.ELEMENT_NODE:
            url= cur_uri + os.sep + n.getAttribute("name")
            if not os.path.exists(url):
                print("%s is not exist" % url)
                continue
            if(n.nodeName == "dir"):
                new_uri = cur_uri + os.sep + n.getAttribute("name")
                checking_dir(n, new_uri)
            elif(n.nodeName == "file"):
                fdate=n.getAttribute("date")
                fsize=n.getAttribute("size")

                filestat = os.stat(url)
                timelocal = time.localtime(filestat.st_mtime)
                formtime = time.strftime("%Y%m%d-%H:%M:%S",timelocal)

                timexml = time.mktime(time.strptime(fdate, "%Y%m%d-%H:%M:%S"))
                #newtime = time.strftime("%Y%m%d-%H:%M:%S",time.localtime(timexml))
                #print(filestat.st_mtime,fdate,newtime,timexml)
                if((fsize != str(filestat.st_size)) or (abs(filestat.st_mtime - timexml)>5)):
                    print("WARNING: file doesn't match with XML file (%s[%s][%dB])<->xml([%s][%sB])" % \
                    (url,formtime,filestat.st_size, fdate,fsize))
    return 0


def checking_src_dir(rootsrc):
    print("\nchecking source:")
    for node in rootsrc.childNodes:
        if node.nodeType == node.ELEMENT_NODE:
            if(node.nodeName == "rootdir"):
                #print(node.getAttribute("name"), node.getAttribute("disk"))
                dir_t=node.getAttribute("disk")
                if not os.path.exists(dir_t):
                    print("disk %s is not exist" % dir_t)
                    continue
                dir_t=dir_t + os.sep + node.getAttribute("name")
                if not os.path.exists(dir_t):
                    print("disk %s is not exist" % dir_t)
                    continue
                print("%s ..." % dir_t)
                checking_dir(node, dir_t) 


def checking_dest_dir(rootsrc, dstDisk):
    print("\nchecking dest USBDisk:")
    if not os.path.exists(dstDisk):
        print("disk %s is not exist" % dstDisk)
        return -1
    for node in rootsrc.childNodes:
        if node.nodeType == node.ELEMENT_NODE:
            if(node.nodeName == "rootdir"):
                dir_t=dstDisk + os.sep + node.getAttribute("name")
                if not os.path.exists(dir_t):
                    print("disk %s is not exist" % dir_t)
                    continue
                print("%s ..." % dir_t)
                checking_dir(node, dir_t) 
            

def copyFiles_dir(cur_node, src_uri, dest_uri):
    if not os.path.exists(src_uri):
        print("%s is not exist" % src_uri)
        return -1
    if not os.path.exists(dest_uri):
        print("%s is not exist" % dest_uri)
        return -1

    for n in cur_node.childNodes:
        if n.nodeType == n.ELEMENT_NODE:
            src_url= src_uri + os.sep + n.getAttribute("name")
            dest_url= dest_uri + os.sep + n.getAttribute("name")
            if not os.path.exists(src_url):
                print("%s is not exist" % src_url)
                continue
            if(n.nodeName == "dir"):
                if not os.path.exists(dest_url):
                    os.makedirs(dest_url)
                copyFiles_dir(n, src_url, dest_url)
            elif(n.nodeName == "file"):
                cpyf.__copyFile(src_url, dest_url, 1)
    return 0


def copyFiles_xml(rootsrc, dstDisk):
    if not os.path.exists(dstDisk):
        print("disk %s is not exist" % dstDisk)
        return -1
    for node in rootsrc.childNodes:
        if node.nodeType == node.ELEMENT_NODE:
            if(node.nodeName == "rootdir"):
                srcdir = node.getAttribute("disk")
                srcdir = srcdir + os.sep + node.getAttribute("name")
                destdir = dstDisk + os.sep + node.getAttribute("name")
                if not os.path.exists(destdir):
                    os.makedirs(destdir)
                copyFiles_dir(node, srcdir, destdir) 
            

def args_prompt():
    global ulUpdate_enable
    global ulUSBDisk
    global diffxml

    ulUSBDisk = "c:\\temp\\USBDisk"
    print("Please input the USB DISK URI (default c:\\temp\\USBDisk):")
    dir_uri = input(" >> ")
    if not os.path.exists(dir_uri):
        print("disk %s is not exist, using %s" % (dir_uri, ulUSBDisk))
    else:
        ulUSBDisk = dir_uri
        print("using %s as target" % ulUSBDisk)

    xmlfiles = sorted(glob.glob("diff_*_to_*.xml"))
    print("Select the diff file:")
    for i, node in enumerate(xmlfiles):
        print("\t%d:%s" % (i,node))
    ulSelected_func= get_int_input("(default 0) >> ", 0, len(xmlfiles))
    print("the source IS:  %s" % xmlfiles[ulSelected_func])
    diffxml=xmlfiles[ulSelected_func]

    ulUpdate_enable = 0  
    print("Please Select function:\n\t0:search the difference\n\t1:sync up the files")
    sel_func = input("(default 0) >> ")
    if sel_func == '1':
        print("\nWARNING:sync up is selected. Suggest to search and check the diff first!\n")
        mode = input("input Y/y to confirm (default n) >> ")
        if mode == 'y' or mode == 'Y' :
            ulUpdate_enable =1
        

def copy_diffToTemp():
    #diffxml="sync_AtoB.xml"
    if not os.path.exists(diffxml):
        print("diff %s is not exist" % diffxml)
        return
    with open(diffxml,'r',encoding='utf8') as fh:
        domdiff=minidom.parse(fh)
    root_n=domdiff.documentElement
    if(root_n.nodeName != 'rootNode'):
        print("error format xml file %s" % filediff)
        return
    if ulUpdate_enable == 0:
        checking_src_dir(root_n)
        checking_dest_dir(root_n, ulUSBDisk)
        get_total_size_of_diff(root_n)
    else:
        cpyf.init_logger(os.path.realpath(__file__))
        copyFiles_xml(root_n, ulUSBDisk)
       

if __name__ == '__main__':
    args_prompt()
    copy_diffToTemp()
    exit_code()

