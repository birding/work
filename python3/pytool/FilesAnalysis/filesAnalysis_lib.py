# -*- coding: utf-8 -*-

import os
import sys, time
import pandas as pd

MARL2019_dir =  [{'type': 'static', 'disk':'c:', 'dir': 'MARL2019'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk', 'dir': 'MARL2019'},
          ]
SDK_dir = [{'type': 'static', 'disk':'c:', 'dir': 'SDK'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk', 'dir': 'SDK'},
          ]
work_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'work'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk', 'dir': 'work'},
          ]

sync_jobs = {
         "MARL2019" : MARL2019_dir,
         "SDK" : SDK_dir,
#         "work" : work_dir,
        }


def get_input(str_prompt):
    if sys.version_info.major == 2:
        ret = raw_input(str_prompt)
    else:
        ret = input(str_prompt)
    return ret


def exit_code():
    get_input("press ENTER key to exit")
    exit(0)

ulSelected_func = 0
ulSelected_job = ''
ulOnRemote = 0 
def args_prompt(jobs):
    global ulSelected_func
    global ulSelected_job
    global ulOnRemote
    
    ulSelected_func = 0  
    print("Please Select function:\n\t0:collect files info\n\t1:files analysis")
    sel_func = get_input("(default 0) >> ")
    if sel_func == '1':
        ulSelected_func = 1
    
    ulOnRemote = 0  
    print("Please Select local or remote:\n\t0:local\n\t1:remote\n\tothers:local")
    sel_func = get_input("(default 0) >> ")
    if sel_func == '1':
        ulOnRemote =1
    
    if ulSelected_func == 1:
        return

    print("#Total jobs %d" % len(jobs))
    print("Please select job:")
    y=list(jobs.keys())
    y.append("ALL")
    for i in range(0, len(y)):  
        print("\t%d : %s" % (i, y[i]))
    sel = get_input("(default 'ALL') >> ")
    if sel == '':
        idx = len(y)-1
    else:
        idx = int(sel)
    if idx > len(y) :
        ulSelected_job = 'ALL'
    else :
        ulSelected_job = y[idx]
    

filelist = []
def insert_filelist(disk, bd, wu, fn):
    global filelist
    filename = disk + os.sep + bd + os.sep + wu + os.sep +fn
    if not os.path.exists(filename):
        print("ERR %s not exist" % filename)
        return 0
    filestat = os.stat(filename)
    timelocal = time.localtime(filestat.st_mtime)
    formtime = time.strftime("%Y%m%d-%H:%M:%S",timelocal)
    #fileitem = [disk, bd, wu, fn, formtime, filestat.st_size]
    #filelist.append(fileitem)
    filelist['filename'].append(fn)
    filelist['filedate'].append(formtime)
    filelist['filesize'].append(filestat.st_size)
    filelist['basedir'].append(bd)
    filelist['uri'].append(wu)
    filelist['disk'].append(disk)


def generate_filedb(disk, basedir, work_uri):
    if work_uri == '' :
        dst_uri = disk + os.sep + basedir 
    else:
        dst_uri = disk + os.sep + basedir + os.sep + work_uri
    # print("1 ", disk, basedir, work_uri)
    if not os.path.exists(dst_uri):
        print("%s not exist" % dst_uri)
        return 0

    tol_count = 0
    for filename in os.listdir(dst_uri):
        if filename.startswith('.'):
            continue
        fromfile = dst_uri + os.sep + filename
        # print("2 ", disk, basedir, work_uri, filename)
        if os.path.isdir(fromfile):
            if work_uri == '' :
                tmp_uri = filename
            else :
                tmp_uri = work_uri + os.sep + filename
            tol_count += generate_filedb(disk, basedir, tmp_uri)
        else:
            tol_count += 1
            insert_filelist(disk, basedir, work_uri, filename)

    return tol_count


def collect_Files_lib(hname, jobs_db):
    global filelist 
    filelist = {'filename':[], 'filedate':[], 'filesize':[], 'disk':[], 'basedir':[], 'uri':[]}
    if ulSelected_job == 'ALL':
        for key,value in jobs_db.items():
            print("working on %s ..." % value[ulOnRemote]["dir"])
            generate_filedb(value[ulOnRemote]["disk"], value[ulOnRemote]["dir"], '')
    else:
        if ulSelected_job in jobs_db.keys():
            print("working on %s ..." % jobs_db[ulSelected_job][ulOnRemote]["dir"])
            generate_filedb(jobs_db[ulSelected_job][ulOnRemote]["disk"], jobs_db[ulSelected_job][ulOnRemote]["dir"], '')
        else :
            print("#job lost...")
    #print(filelist)
    df = pd.DataFrame(filelist)
    csvFileName = hname + '_' + str(ulOnRemote) + '.csv'
    df.to_csv(csvFileName, encoding='utf_8_sig', index=False)
    print(df)
    print("write to ", csvFileName)
    #df1 = df[df.filesize>1000]
    #print(df1)
    #df2 = df[df.filename.str.contains('SDK')]
    #print(df2)
    #df3 = df[df.filename.str.contains('SDK.*txt')]
    #print(df3)
    #df4 = df[df.filename.str.match('.*\.txt')]
    #print(df4)


def analyse_Files_lib(hname, jobs_db):
    csv_file = hname + '_' + str(ulOnRemote) + '.csv'
    if not os.path.exists(csv_file):
        print("%s not exist" % csv_file)
        return 0
    print("reading %s ... " % csv_file)
    df = pd.read_csv(csv_file, encoding='utf_8_sig')
    print("---------------------origin-----------------------")
    print(df)
#    print(df.shape[0])
#    print(df.filename.size)
    series_cnt = df.filename.value_counts()
    # print(series_cnt)
    print("\n\nchecking out the duplicated files ...")
    series_name = df.filename.unique()
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
                #df3 = df[df.filename.str.contains(name)]
                df3 = df[df.filename == name]
            except:
                print("%s matching exception" % name)
                continue
            # print("")
            # print(df3)
            df_t = pd.concat([df_t, df3])
    print("-------------------duplicated-----------------------")
    print(df_t)
    df_t.to_csv("analysis_result.csv", encoding='utf_8_sig', index=False)


def searchFiles_lib(hname, jobs_db):
    global filelist 
    args_prompt(jobs_db)
    if ulSelected_func !=1 :
        collect_Files_lib(hname, jobs_db)
    else :
        analyse_Files_lib(hname, jobs_db)


if __name__ == '__main__':
    hostname = 'ThP480s'
    searchFiles_lib(hostname, sync_jobs)
    exit_code()

