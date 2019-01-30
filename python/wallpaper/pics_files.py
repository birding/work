#! python2
# -*- coding: utf-8 -*-

import os
import linecache
from glob import glob
from os.path import join, dirname 
    
def exit_code():
    raw_input("press ENTER key to exit")
    exit(0)


def gen_pics_config_file():
    DIR_FILE = 'dirs_config_win.txt'
    PICS_FILE = 'pics_files.txt'
    if os.path.exists(DIR_FILE):
        cache_data = linecache.getlines(DIR_FILE)
        #linecache.clearcache()

        pics_config = open(PICS_FILE, 'w')
        total_pics = 0
        for dirname in cache_data:
            dirname = dirname.strip('\n')
            if not os.path.exists(dirname):
                continue

            print("dirName : %s" % dirname)
            print("dirName2 : %s" % join(dirname, "*.jpg"))
            pics = glob(join(dirname, "*.jpg"))
            # print "pics : %s" % pics

            total_pics += len(pics)
            print("pic count : %d %d" % (len(pics), total_pics))

            sample_list = [line + '\n' for line in pics]
            # print "sample_list : %s" % sample_list

            pics_config.writelines(sample_list)
        pics_config.close()


def clear_cache(cache_patch):
    ls = os.listdir(cache_patch)
    for i in ls:
        c_path = os.path.join(cache_patch, i)
        if os.path.isdir(c_path):
            del_file(c_path)
        else:
            os.remove(c_path)
            
            
if __name__ == '__main__':
    print "clear cached files"
    cache_dir = "d:\\wallpaper_temp\\"
    clear_cache(cache_dir)
    print "generate pics_files.txt"
    gen_pics_config_file()
    exit_code() 
    
    