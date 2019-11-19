# -*- coding: utf-8 -*-

import os
import sys, time
from os.path import join, dirname
from xml.dom import minidom
import lib3copyFile as cpyf
import glob
import pandas as pd

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


filedict={
            'name':['name'],
            'date':['date'],
            'size':['size'],
            'rootdir':['rootdir'],
            'fileurl':['fileurl'],
        }
def add_file(cur_node, rootdir, cur_uri):
    for node in cur_node.childNodes:
        if node.nodeType == node.ELEMENT_NODE:
            if(node.nodeName == "dir"):
                if cur_uri == None:
                    new_uri = node.getAttribute("name")
                else:
                    new_uri = cur_uri + os.sep + node.getAttribute("name")
                add_file(node, rootdir, new_uri)
            elif(node.nodeName == "file"):
                #print("uri:%s,name:%s,date:%s,size:%s" % (cur_uri,node.getAttribute("name"),node.getAttribute("date"),node.getAttribute("size")))
                filedict['date'].append(node.getAttribute('date'))
                filedict['size'].append(node.getAttribute('size'))
                filedict['rootdir'].append(rootdir)
                filedict['name'].append(node.getAttribute('name'))
                if cur_uri == None:
                    filedict['fileurl'].append(node.getAttribute('name'))
                else:
                    filedict['fileurl'].append(cur_uri+os.sep+node.getAttribute('name'))
            else:
                print("error occur in add files");


def filtering_files():
    with open(uldstXML,'r',encoding='utf8') as fhdst:
        domdst=minidom.parse(fhdst)
    rootdst=domdst.documentElement
    if(rootdst.nodeName != 'rootNode'):
        print("error format xml file %s" % filedst)
        return

    rootdirNodes_dst = rootdst.getElementsByTagName("rootdir")
    for n in rootdirNodes_dst:
        print(n.getAttribute("name"))
    print(" ")

    for node in rootdst.childNodes:
        if node.nodeType == node.ELEMENT_NODE:
            if(node.nodeName == "rootdir"):
                add_file(node, node.getAttribute("name"), None)

    #print(filedict)
    df = pd.DataFrame.from_dict(filedict)
    print("---------------------origin-----------------------")
    print(df)

    series_cnt = df.name.value_counts()
    # print(series_cnt)
    print("\n\nchecking out the duplicated files ...")
    series_name = df.name.unique()
    # print(series_name)
    df_t = None
    i = 0
    for name in series_name:
        #print(name)
        #print(series_cnt[name])
        print("\r working on %d/%d ..." % (i, series_name.size), end=" ")
        i = i+1
        if series_cnt[name] > 1 :
            # print(name, series_cnt[name])
            try:
                #df3 = df[df.name.str.contains(name)]
                df3 = df[df.name == name]
            except:
                print("%s matching exception" % name)
                continue
            # print("")
            # print(df3)
            df_t = pd.concat([df_t, df3])
    print("-------------------duplicated-----------------------")
    print(df_t)
    arfilename = uldstXML[5:-4]+"_analysis_result.csv"
    print("saving to "+arfilename)
    df_t.to_csv(arfilename, encoding='utf_8_sig', index=False)


def args_prompt():
    global uldstXML

    rootNode = sorted(glob.glob("Disk*.xml"))
    print("Select the Disk:")
    for i, node in enumerate(rootNode):
        print("\t%d:%s" % (i,node))
    ulSelected_func= get_int_input("(default 0) >> ", 0, len(rootNode))
    print("the Disk IS:  %s" % rootNode[ulSelected_func])
    uldstXML=rootNode[ulSelected_func]


if __name__ == '__main__':
    args_prompt()
    filtering_files()
    exit_code()

