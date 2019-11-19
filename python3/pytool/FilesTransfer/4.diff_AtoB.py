# -*- coding: utf-8 -*-

import os
import sys, time
from os.path import join, dirname
from xml.dom import minidom
import glob


def exit_code():
    input("press ENTER key to exit")
    exit(0)


#find the node's name in the children of dstnode
#return <0, None> : not found, or found a static rootdir 
#    <1, dstchild> : found a same file/dir, or the dst file is newer. 
# if the dst is older, take is as a not found file
def check_exist(childnode, dstnode):
    for node in dstnode.childNodes:
        if node.nodeType == node.ELEMENT_NODE:
            if childnode.getAttribute("name") == node.getAttribute("name"):
                if(node.nodeName == "rootdir"):
                    if (node.getAttribute("type") == "static"):
                        return 0,None
                if(node.nodeName == "file"):
                    if (node.getAttribute("date") < childnode.getAttribute("date")):
                        # found the dst file is older, we need to replace it. so report not found
                        return 0,None
                return 1,node
    #not found
    return 0,None


#compare the children's name in both node
def diff_children(src_node, dst_node):
    #print(src_node.childNodes)
    for node in src_node.childNodes:
        if node.nodeType == node.ELEMENT_NODE:
            found,targetnode = check_exist(node, dst_node)
            #print("process source:%s %s" % (node.nodeName, node.getAttribute("name")))
            if(node.nodeName == "rootdir"):
                if found == 1:
                    diff_children(node,targetnode)
                else:
                    print("not found or found a static rootdir")
                    src_node.removeChild(node)
            elif(node.nodeName == "dir"):
                if found == 1:
                    diff_children(node,targetnode)
                #else:  --- need sync up the whole dir
                #    src_node.removeChild(node)
            elif(node.nodeName == "file"):
                if found == 1:
                    src_node.removeChild(node)
            else:
                print("error NodeName: %s" % node.NodeName)


def get_file_count(Pnode):
    file_node_count=0
    for node in Pnode.childNodes:
        if node.nodeType == node.ELEMENT_NODE:
            if(node.nodeName == "file"):
                file_node_count=file_node_count+1
            elif(node.nodeName == "dir"):
                file_node_count=file_node_count+get_file_count(node)
    return file_node_count


#remove NULL dir
def fix_null_dir(Pnode):
    for node in Pnode.childNodes:
        if node.nodeType == node.ELEMENT_NODE:
            if(node.nodeName == "rootdir" or node.nodeName == "dir"):
                count=get_file_count(node)
                #print("name %s count %d" % (node.getAttribute("name"),count))
                if(count == 0):
                    Pnode.removeChild(node)
                    continue
                else:
                    fix_null_dir(node)


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


def processing_XML(filesrc, filedst):
    with open(filesrc,'r',encoding='utf8') as fh:
        domsrc=minidom.parse(fh)
    rootsrc=domsrc.documentElement
    if(rootsrc.nodeName != 'rootNode'):
        print("error format xml file %s" % filesrc)
        return

    with open(filedst,'r',encoding='utf8') as fhdst:
        domdst=minidom.parse(fhdst)
    rootdst=domdst.documentElement
    if(rootdst.nodeName != 'rootNode'):
        print("error format xml file %s" % filedst)
        return

    #debug code
    rootdirNodes_src = rootsrc.getElementsByTagName("rootdir")
    rootdirNodes_dst = rootdst.getElementsByTagName("rootdir")
    for n in rootdirNodes_src:
        print(n.getAttribute("name"))
    print(" ")
    for n in rootdirNodes_dst:
        print(n.getAttribute("name"))
    print(" ")
    
    diff_children(rootsrc, rootdst)
    rootsrc.setAttribute('from', filesrc[:-4]) #remove '.xml'
    rootsrc.setAttribute('to', filedst[:-4])
    rootsrc.setAttribute('sync_date', time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time())))
    
    # add_destinfo
    destinfo=domsrc.createElement('destinfo')
    rootsrc.appendChild(destinfo)
    for n in rootdst.childNodes: # rootNode
        for node in n.childNodes:   #rootdir
            if node.nodeType == node.ELEMENT_NODE:
                ret = n.removeChild(node)
                if(ret==None):
                    print("remove failed %s" % node.nodeName)
    destinfo.appendChild(rootdst)

    fix_null_dir(rootsrc)
    
    #filename = 'sync_AtoB_tmp.xml'    
    filename = 'diff_'+filesrc[5:-4]+'_to_'+filedst[5:-4]+'_tmp.xml'    
    try:
        with open(filename, 'w',encoding='UTF-8') as fh:
            domsrc.writexml(fh,indent='',addindent='\t',newl='\n',encoding='UTF-8')
            print('write %s OK!' % filename)
    except Exception as err:
        print('error {0}'.format(err)) 
    
    #tname = 'sync_AtoB.xml'    
    tname = 'diff_'+filesrc[5:-4]+'_to_'+filedst[5:-4]+'.xml'    
    fix_spaceline(filename, tname)
    os.remove(filename)


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


def syn_AtoB():
    #dirs = os.listdir(os.getcwd())
    rootNode = sorted(glob.glob("Disk*.xml"))
    #print(rootNode)

    print("Select the source:")
    for i, node in enumerate(rootNode):
        print("\t%d:%s" % (i,node))
    ulSelected_func= get_int_input("(default 0) >> ", 0, len(rootNode))
    print("the source IS:  %s" % rootNode[ulSelected_func])
    src=rootNode[ulSelected_func]
    rootNode.remove(rootNode[ulSelected_func])
    
    print("Select the destination:")
    for i, node in enumerate(rootNode):
        print("\t%d:%s" % (i,node))
    ulSelected_func= get_int_input("(default 0) >> ", 0, len(rootNode))
    dst=rootNode[ulSelected_func]
    print("the dest IS:  %s " % rootNode[ulSelected_func])
    print(" ")
    #processing_XML("ThP480.xml", "ThCenter.xml")
    #print(src[5:-4], dst[5:-4])
    processing_XML(src, dst)

        
if __name__ == '__main__':
    syn_AtoB()
    exit_code()

