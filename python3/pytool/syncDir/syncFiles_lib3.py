# -*- coding: utf-8 -*-

import os
import sys, time
from glob import glob
from os.path import join, dirname
import shutil
import logging

logger = logging.getLogger()


def init_logger(filename):
    logger.setLevel(logging.INFO)
    file_formatter = logging.Formatter(
        '%(asctime)s - %(name)s - %(levelname)s: - %(message)s',
        datefmt='%Y-%m-%d %H:%M:%S')
    #print(time.strftime("%Y_%m_%d"))
    #print(os.path.basename(os.path.realpath(__file__)))
    fileH = logging.FileHandler('log_'+os.path.basename(filename)+time.strftime("%Y_%m")+'.txt')
    fileH.setLevel(logging.INFO)
    fileH.setFormatter(file_formatter)
    logger.addHandler(fileH)
    stream_formatter = logging.Formatter('%(message)s')
    streamH = logging.StreamHandler()
    streamH.setLevel(logging.INFO)
    streamH.setFormatter(stream_formatter)
    logger.addHandler(streamH)
    logger.info('\nlogger is starting')
    #logger.info('this is info message %d %s ' % (1, time.asctime()))
    #logger.warn('this is warn message')
    #exit(0)


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
SDK_dir = [{'type': 'static', 'disk':'c:', 'dir': 'SDK'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk', 'dir': 'SDK'},
          ]
Supports_dir = [{'type': 'temp', 'disk':'e:', 'dir': 'Supports'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk', 'dir': 'Supports'},
          ]                    
syncDir_dir = [{'type': 'temp', 'disk':'c:', 'dir': 'syncDir'},
            {'type': 'temp', 'disk':'\\\\172.16.1.150\\usbdisk','dir': 'syncDir'},
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
         "SDK" : SDK_dir,
#         "Supports" : Supports_dir,
         "syncDir" : syncDir_dir,
#         "Tech" : Tech_dir,
#         "tools_free" : tools_free_dir,
#         "tools" : tools_dir,
#         "work" : work_dir,
        }

def exit_code():
    input("press ENTER key to exit")
    exit(0)


def get_KMGB(number):
    tmp_str = 'B'
    if number > 1024:
        tmp_str = 'KB'
        number = number/1024
    if number > 1024:
        tmp_str = 'MB'
        number = number/1024
    if number > 1024:
        tmp_str = 'GB'
        number = number/1024
    return number, tmp_str
    

ulTotolFileSize = 0
ulUpdate_enable = 0
def show_speed(fsize,sec):
    global ulTotolFileSize
    ulTotolFileSize += fsize
    # print("ulTotolFileSize:%d" % ulTotolFileSize)
    
    if sec == 0:
        sp = 0
    else:
        sp = fsize/sec
    sp, sp_str = get_KMGB(sp)
    fsize, fsize_str = get_KMGB(fsize)
    logger.info("[%ds][%.1f%sps]" % (sec, sp, sp_str))


def mycopy(src_file, dst_file):
    dst_dl = dst_file+'.dl'
    try:
        with open(src_file, "rb") as fr,open(dst_dl, 'wb') as fw:  # fr = file read
            fromstat = os.stat(src_file)
            copysize = 0
            while True:
                data = fr.read(4096)
                if not data:
                    break
                fw.write(data)
                copysize += len(data) 
                percent=float(copysize)*100/float(fromstat.st_size)
                print("\r %.2f %% " % percent, end=" ")
    except OSError as e:
        print("open failed:", e)
        exit_code()
    except:
        print("copy is interrupted")
        exit_code()
    print("\r 100%           ", end=" ")
    shutil.copystat(src_file,dst_dl)
    if os.path.exists(dst_file) :
        os.remove(dst_file)
    os.rename(dst_dl, dst_file)
    return True


def __copyFile(fromfile, tofile):
    fromstat = os.stat(fromfile)
    fsize, fsize_str = get_KMGB(fromstat.st_size)
    # copy_ops 0: files are same; 1:copy;
    copy_ops = 0
    if not os.path.exists(tofile) :
        if ulUpdate_enable == 0:
            logger.info("WARNING: create new file \t=> %s" % tofile)
        else:
            copy_ops = 1
            logger.info("new: %s ==> %s [%.1f%s]" % (fromfile,tofile,fsize,fsize_str))
    else:
        tostat = os.stat(tofile)
        timelocal = time.localtime(fromstat.st_mtime)
        fromtime = time.strftime("%Y%m%d %H:%M:%S",timelocal)
        timelocal = time.localtime(tostat.st_mtime)
        totime = time.strftime("%Y%m%d %H:%M:%S",timelocal)
        if (fromstat.st_mtime > (tostat.st_mtime+5)):
            if ulUpdate_enable == 0:
                logger.info("WARNING: file doesn't match \t=> %s ([%s][%dB]->[%s][%dB])" % \
                (tofile,fromtime,fromstat.st_size, totime,tostat.st_size,))
            else:
                copy_ops = 1
                logger.info("update: %s[%.1f%s] ==> %s ([%s][%dB]->[%s][%dB])" % \
                (fromfile, fsize,fsize_str, tofile,fromtime,fromstat.st_size, totime,tostat.st_size,))
        elif (fromstat.st_mtime == tostat.st_mtime):
            if ((fromstat.st_size != tostat.st_size) and (ulUpdate_enable == 0)):
                logger.info("WARNING in size: file doesn't match \t=> %s ([%s][%dB]->[%s][%dB])" % \
                (tofile,fromtime,fromstat.st_size, totime,tostat.st_size,))

    if copy_ops == 1:        
        start_time = time.time()
        #shutil.copy2(fromfile,tofile)
        mycopy(fromfile, tofile)
        logger.info("-- done      ")
        show_speed(fromstat.st_size, (time.time() - start_time))
        return 1
    else:
        return 0


def __copyDir(fromdir,todir):
    todir = todir.strip()
    todir = todir.rstrip(os.sep)
    if not os.path.exists(todir):
        try:
            os.makedirs(todir)
        except OSError as e:
            print(e)
            return
    tol_count = 0
    for filename in os.listdir(fromdir):
        if filename.startswith('.'):
            continue
        fromfile = fromdir + os.sep + filename
        tofile = todir + os.sep + filename
        if os.path.isdir(fromfile):
            if (ulUpdate_enable == 0) and (not os.path.exists(tofile)):
                logger.info("WARNING: create new directory \t=> %s" % tofile)
            else:
                tol_count += __copyDir(fromfile,tofile)
        else:
            tol_count += __copyFile(fromfile,tofile)
    return tol_count


def sync_dir(dir_info, src_idx, dst_idx):
    if src_idx == dst_idx:
        return
    src_uri = dir_info[src_idx]["disk"] + os.sep + dir_info[src_idx]["dir"]
    dst_uri = dir_info[dst_idx]["disk"] + os.sep + dir_info[dst_idx]["dir"]
    if ulUpdate_enable == 0:
        logger.info("\n#comparing [%s]%s  with  [%s]%s" % (dir_info[src_idx]["type"], src_uri, dir_info[dst_idx]["type"], dst_uri))
    else:
        logger.info("\n#sync up [%s]%s  with  [%s]%s" % (dir_info[src_idx]["type"], src_uri, dir_info[dst_idx]["type"], dst_uri))
    if not os.path.exists(src_uri):
        logger.info("@@src dir not exist: "+ src_uri)
        return
    if dir_info[dst_idx]["type"] == 'static':
        if (ulUpdate_enable == 1):
            logger.info("----skip static directory:%s" % dst_uri)
            return
    if not os.path.exists(dir_info[dst_idx]["disk"]):
        logger.info("@@dest disk not exist: "+ dir_info[dst_idx]["disk"])
        return    
    if not os.path.exists(os.path.dirname(dst_uri)):
        logger.info("@@base of dest dir not exist: " + os.path.dirname(dst_uri))
        return
    if not os.path.exists(dst_uri):
        logger.info("@@dest dir not exist: " + dst_uri)
        return
    count = __copyDir(src_uri, dst_uri)
    return count



def sync_dirs(dirs):
    for i in range(0, len(dirs)):
        for j in range(0, len(dirs)):
            sync_dir(dirs, i, j)


ulSelected_job = ''
def args_prompt(jobs):
    global ulUpdate_enable
    global ulSelected_job
    print("#Total jobs %d" % len(jobs))
    
    print("Please select job for sync up:")
    y=list(jobs.keys())
    y.append("ALL")
    for i in range(0, len(y)):  
        print("\t%d : %s" % (i, y[i]))
    sel = input("(default 'ALL') >> ")
    if sel == '':
        idx = len(y)-1
    else:
        idx = int(sel)
    if idx > len(y) :
        ulSelected_job = 'ALL'
    else :
        ulSelected_job = y[idx]
    
    ulUpdate_enable = 0  
    print("Please Select function:\n\t0:search the difference\n\t1:sync up the files")
    sel_func = input("(default 0) >> ")
    if sel_func == '1':
        print("\nWARNING:sync up is selected. Suggest to search and check the diff first!\n")
        mode = input("input Y/y to confirm (default n) >> ")
        if mode == 'y' or mode == 'Y' :
            ulUpdate_enable =1
        

def syncFiles_lib(fname, jobs_db):
    init_logger(fname)
    args_prompt(jobs_db)
    s_time =  time.time()
    if ulSelected_job == 'ALL':
        for key,value in jobs_db.items():
            logger.info("\n##start sync [%s] with %d directories " % (key, len(value)))
            sync_dirs(value)
    else:
        if ulSelected_job in jobs_db.keys():
            sync_dirs(jobs_db[ulSelected_job])
        else :
            logger.info("#job lost...")
    show_speed(ulTotolFileSize, (time.time() - s_time))
    logger.info("...")


if __name__ == '__main__':
    syncFiles_lib(os.path.realpath(__file__), sync_jobs)
    exit_code()

