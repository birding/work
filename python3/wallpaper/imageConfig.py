#! python2
# -*- coding: utf-8 -*-

# known issue 1#: unable to support draw chinese on the picture

import os
from PIL import Image, ImageDraw, ImageFont
import configparser

# https://docs.python.org/2.7/library/configparser.html
class img_config:
    def __init__(self, filename):
        self.cf_done = 0
        if not os.path.exists(filename):
            return "error input"
        self.cf = ConfigParser.ConfigParser()        
        self.cf.read(filename)
        self.cf_done = 1
    
    def get_str(self, opt_name, default_val):
        if self.cf_done == 0:
            return default_val
        secs = self.cf.sections()
        for sec in secs:
            if self.cf.has_option(sec, opt_name):
                return self.cf.get(sec, opt_name)
        return default_val

    def get_int(self, opt_name, default_val):
        if self.cf_done == 0:
            return default_val
        secs = self.cf.sections()
        for sec in secs:
            if self.cf.has_option(sec, opt_name):
                return self.cf.getint(sec, opt_name)
        return default_val


def generate_oops_img(err_info):
    oops_img = Image.new("RGBA",(150,20),(0,0,0,0))
    draw = ImageDraw.Draw(oops_img)
    font = ImageFont.truetype("consola.ttf", 20, encoding="unic")
    draw.text((0, 0), err_info, 'fuchsia',font)
    del draw
    return oops_img
    
# https://pillow.readthedocs.io/en/latest/reference/Image.html
def modify_img_by_cfg(orgFile):
    if not os.path.exists(orgFile):
        print("error input")
        return generate_oops_img("Nofile")
        
    (shotname,extension) = os.path.splitext(orgFile);
    cf_name = '%s.conf' % (shotname)
    print(cf_name)
    
    tmp_img = Image.open(orgFile)  
    if not os.path.exists(cf_name):
        print("no cfg file")
        return tmp_img
        
    config = img_config(cf_name)
    
    if config.get_int("Show_disable", 0):
        print("Show disabled")
        return generate_oops_img("disabled")
    
    print('2 image width %d, height %d'%(tmp_img.width,tmp_img.height))
    if config.get_int("Rotation", 0):
        #tmp_img = tmp_img.rotate(config.get_int("Rotation", 0))
        if config.get_int("Rotation", 0) == 90:
            tmp_img = tmp_img.transpose(Image.ROTATE_90)
        elif config.get_int("Rotation", 0) == 180:
            tmp_img = tmp_img.transpose(Image.ROTATE_180)
        elif config.get_int("Rotation", 0) == 270:
            tmp_img = tmp_img.transpose(Image.ROTATE_270)
                
    left_position = tmp_img.width * config.get_int("Left_strip", 0)/100
    top_position = tmp_img.height * config.get_int("Top_strip", 0)/100
    right_position = tmp_img.width * (100 - config.get_int("Right_strip", 0))/100
    bottom_position = tmp_img.height * (100 - config.get_int("Bottom_strip", 0))/100
    tmp_img = tmp_img.crop((left_position,top_position,right_position,bottom_position))
    
    if config.get_int("Text_enable", 0):
        draw = ImageDraw.Draw(tmp_img)
        font = ImageFont.truetype("consola.ttf", config.get_int("Font_size", 0), encoding="unic")
        draw.text((config.get_int("Text_left_margin", 0), config.get_int("Text_top_margin", 0)), 
                   config.get_str("Text_String", ""), 'fuchsia',font)
        del draw
    
    print('3 image width %d, height %d'%(tmp_img.width,tmp_img.height))
    return tmp_img
    
    
def exit_code():
    raw_input("press ENTER key to exit")
    exit(0)


if __name__ == '__main__':
    # imgFile = "IMG_20181004_202109.jpg"
    imgFile = "F:\\python\\imageConfig\\IMG_20181004_202109111.jpg"
    newimg = modify_img_by_cfg(imgFile)
    if newimg != None:
        newimg.show()    
    