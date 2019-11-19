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


def generate_filedb(dom, cur_node, cur_uri):
    if not os.path.exists(cur_uri):
        print("%s not exist" % cur_uri)
        return 0

    tol_count = 0
    contents=os.listdir(cur_uri)
    contents.sort()
    for filename in contents:
        if filename.startswith('.'):
            continue
        tmpURI = cur_uri + os.sep + filename
        if os.path.isdir(tmpURI):
            tmpNode=dom.createElement('dir')
            cur_node.appendChild(tmpNode)
            tmpNode.setAttribute('name', filename)
            print(".", end=" ", file=sys.stdout, flush=True)
            tol_count += generate_filedb(dom, tmpNode, tmpURI)
        else:
            tol_count += 1
            tmpNode=dom.createElement('file')
            cur_node.appendChild(tmpNode)
            tmpNode.setAttribute('name', filename)
            
            filestat = os.stat(tmpURI)
            tmpNode.setAttribute('size', str(filestat.st_size))
            timelocal = time.localtime(filestat.st_mtime)
            formtime = time.strftime("%Y%m%d-%H:%M:%S",timelocal)
            tmpNode.setAttribute('date', formtime)

    return tol_count


def fix_spaceline(infile, outfile):
    infopen = open(infile, 'r',encoding="utf-8")
    outfopen = open(outfile, 'w',encoding="utf-8")
    lines = infopen.readlines()
    for line in lines:
        a = line.strip()
        if len(a)!=0:
            outfopen.writelines(line)
    infopen.close()
    outfopen.close()


def collect_Files_xml():
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
                if not os.path.exists(destdir):
                    print("dir is not exist %s" % destdir)
                    continue
                print("collecting %s " % node.getAttribute("name"), end=" ",file=sys.stdout, flush=True)
                filesTotal = generate_filedb(domdst, node, destdir)
                print("%d files " % filesTotal)

    filename = 'tmpDisk_' + rootdst.getAttribute("name") +'.xml'
    try:
        with open(filename, 'w',encoding='UTF-8') as fh:
            domdst.writexml(fh,indent='',addindent='\t',newl='\n',encoding='UTF-8')
            print('write %s OK!' % filename)
    except Exception as err:
        print('error {0}'.format(err))            
    tname = 'Disk_' + rootdst.getAttribute("name") +'.xml'
    fix_spaceline(filename, tname)
    os.remove(filename)


def args_prompt():
    global uldstXML
    
    files = sorted(glob.glob("CFG*.xml"))
    print("Select the disk:")
    for i, node in enumerate(files):
        print("\t%d:%s" % (i,node))
    ulSelected_func= get_int_input("(default 0) >> ", 0, len(files))
    uldstXML = files[ulSelected_func]
    print("the disk IS:  %s " % uldstXML)
    print(" ")


if __name__ == '__main__':
    #print(os.path.realpath(__file__))
    args_prompt()
    collect_Files_xml()
    exit_code()

