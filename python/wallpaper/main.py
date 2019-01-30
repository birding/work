
# -*- coding: utf-8 -*-

import os
from PIL import Image, ImageDraw
import win32gui
import win32con
import win32api
import random
import datetime
import imageConfig
import re

debug_discached = 0
debug_idx = 0

# https://blog.csdn.net/felixzh123/article/details/76085132
def set_wallpaper(img_path):
    # 打开指定注册表路径
    reg_key = win32api.RegOpenKeyEx(win32con.HKEY_CURRENT_USER, "Control Panel\\Desktop", 0, win32con.KEY_SET_VALUE)
    # 最后的参数:2拉伸,0居中,6适应,10填充,0平铺
    win32api.RegSetValueEx(reg_key, "WallpaperStyle", 0, win32con.REG_SZ, "0") 
    # 最后的参数:1表示平铺,拉伸居中等都是0
    win32api.RegSetValueEx(reg_key, "TileWallpaper", 0, win32con.REG_SZ, "0") 
    # 刷新桌面
    win32gui.SystemParametersInfo(win32con.SPI_SETDESKWALLPAPER, img_path, win32con.SPIF_SENDWININICHANGE)


def get_img_uri():
    pic_cfile = "F:\\python\\wallpaper\\pics_files.txt"
    if not os.path.exists(pic_cfile):
        return -1,"no pic"
    fp = open(pic_cfile)
    pic_files = fp.readlines()
    # print len(pic_files), pic_files[0]
    idx = random.randint(0,len(pic_files)-1)
    print('uri index %d in %d'%(idx, len(pic_files)))
    if debug_idx:
        idx = debug_idx
    cfile = pic_files[idx]
    cfile = cfile.strip('\n')
    cfile = cfile.replace('\\', '/')
    if not os.path.exists(cfile):
        print "file not exist2: %s" % cfile
        return -1,"nopic"
    return idx,cfile
    

def calc_box_size(win_w, win_h, img_w, img_h):
    box_width = win_w * 0.5
    box_height = win_h * 0.6
    if(box_width < img_w):
        img_h = (img_h*box_width)/img_w
        img_w = box_width
    if(box_height < img_h):
        img_w = (img_w*box_height)/img_h
        img_h = box_height
    print('img width %d, height %d'%(img_w, img_h))
    right_m = win_w * 0.9
    left_m = right_m - img_w
    top_m = win_h * 0.1
    bottom_m = top_m + img_h
    return (int(left_m), int(top_m), int(right_m), int(bottom_m))


def gen_img(idx, orgFile):
    base_file = "F:\\python\\wallpaper\\base.jpg"
    print orgFile
    if not os.path.exists(orgFile):
        return "error input"
        
    (filepath,tempfilename) = os.path.split(orgFile);
    (shotname,extension) = os.path.splitext(tempfilename);
    # paint_str = "[" + str(idx) + "]" + shotname
    paint_str = re.sub(r'/|:', '', filepath)
    paint_str = paint_str + shotname
    # print filepath, paint_str
    # f_pwd = os.getcwd()
    f_pwd = "d:\\wallpaper_temp\\"
    f_pwd = os.path.join(f_pwd, 'wallpaper'+paint_str+'.jpg')
    print f_pwd
    if os.path.exists(f_pwd):
        if debug_discached:
            os.remove(f_pwd)
        else:
            return f_pwd
    
    base_img = Image.open(base_file)
    base_width = win32api.GetSystemMetrics(win32con.SM_CXSCREEN)
    base_height = win32api.GetSystemMetrics(win32con.SM_CYSCREEN)
    base_img = base_img.resize((base_width,base_height))
    
    # tmp_img = Image.open(orgFile)
    tmp_img = imageConfig.modify_img_by_cfg(orgFile)
    if tmp_img == None:
        print "no picture"
        return ''
    
    #print('image width %d, height %d'%(tmp_img.size[0],tmp_img.size[1]))
    print('image width %d, height %d'%(tmp_img.width,tmp_img.height))
    print('windows width %d, height %d'%(base_width, base_height))
    box = calc_box_size(base_width, base_height, tmp_img.width, tmp_img.height)
        
    region = tmp_img
    region = region.resize((box[2] - box[0], box[3] - box[1]), Image.ANTIALIAS)
    base_img.paste(region, box)
    
    draw = ImageDraw.Draw(base_img)
    draw.text((box[0],box[3]), "[%d]%s"%(idx,paint_str))
    del draw
    
    # base_img.show()
    base_img.save(f_pwd, 'JPEG', quality = 95)
    return f_pwd
    
    
def exit_code():
    # raw_input("press ENTER key to exit")
    exit(0)


if __name__ == '__main__':
    # abs_path = "F:\\pictures\\2013\\1.jpg"
    # set_wallpaper(abs_path)
    
    ret, cfile = get_img_uri()
    newfile = gen_img(ret, cfile)
    print newfile
    if not os.path.exists(newfile):
        print "file not exist"
        exit_code() 
    set_wallpaper(newfile)
    
    