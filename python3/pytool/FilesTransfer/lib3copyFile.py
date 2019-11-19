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
    
    #file_formatter = logging.Formatter(
    #    '%(asctime)s - %(name)s - %(levelname)s: - %(message)s',
    #    datefmt='%Y-%m-%d %H:%M:%S')
    #fileH = logging.FileHandler('log_'+os.path.basename(filename)+time.strftime("%Y_%m")+'.txt')
    #fileH.setLevel(logging.INFO)
    #fileH.setFormatter(file_formatter)
    #logger.addHandler(fileH)
    
    stream_formatter = logging.Formatter('%(message)s')
    streamH = logging.StreamHandler()
    streamH.setLevel(logging.INFO)
    streamH.setFormatter(stream_formatter)
    logger.addHandler(streamH)
    logger.info('\nlogger is starting')


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
    except Exception as e:
        print("copy is interrupted",e)
        exit_code()
    print("\r 100%           ", end=" ")
    shutil.copystat(src_file,dst_dl)
    if os.path.exists(dst_file) :
        os.remove(dst_file)
    os.rename(dst_dl, dst_file)
    return True


def __copyFile(fromfile, tofile,update_en):
    fromstat = os.stat(fromfile)
    fsize, fsize_str = get_KMGB(fromstat.st_size)
    # copy_ops 0: files are same; 1:copy;
    copy_ops = 0
    if not os.path.exists(tofile) :
        if update_en == 0:
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
            if update_en == 0:
                logger.info("WARNING: file doesn't match \t=> %s ([%s][%dB]->[%s][%dB])" % \
                (tofile,fromtime,fromstat.st_size, totime,tostat.st_size))
            else:
                copy_ops = 1
                logger.info("update: %s[%.1f%s] ==> %s ([%s][%dB]->[%s][%dB])" % \
                (fromfile, fsize,fsize_str, tofile,fromtime,fromstat.st_size, totime,tostat.st_size,))
        elif (fromstat.st_mtime == tostat.st_mtime):
            if ((fromstat.st_size != tostat.st_size)and (ulUpdate_enable == 0)):
                logger.info("WARNING in size: file doesn't match \t=> %s ([%s][%dB]->[%s][%dB])" % \
                (tofile,fromtime,fromstat.st_size, totime,tostat.st_size,))
            #else:
            #    logger.info("files are same")

    if copy_ops == 1:        
        start_time = time.time()
        #shutil.copy2(fromfile,tofile)
        mycopy(fromfile, tofile)
        logger.info("-- done      ")
        show_speed(fromstat.st_size, (time.time() - start_time))
        return 1
    else:
        return 0


def __syncDir(fromdir,todir, update):
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
            if (update == 0) and (not os.path.exists(tofile)):
                logger.info("WARNING: create new directory \t=> %s" % tofile)
            else:
                tol_count += __syncDir(fromfile,tofile,update)
        else:
            tol_count += __copyFile(fromfile,tofile,update)
    return tol_count


if __name__ == '__main__':
    init_logger(os.path.realpath(__file__))
    exit_code()

