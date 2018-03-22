'''
Pictures demo
=============

This is a basic picture viewer, using the scatter widget.
'''

import kivy
kivy.require('1.0.6')
from kivy.uix.widget import Widget
from kivy.uix.label import Label

from glob import glob
from random import randint
from os.path import join, dirname
from kivy.app import App
from kivy.logger import Logger
from kivy.uix.scatter import Scatter
from kivy.properties import StringProperty
# FIXME this shouldn't be necessary
from kivy.core.window import Window
import platform
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button
from functools import partial
from kivy.uix.image import AsyncImage
import threading, time
from glob import glob

import os
import linecache

class PicReload():
    def loadpics(self):
        pass

class UrlParser():
    def __init__(self):
        self.configFile = []
        self.picCount = 0
        self.currentIndex = 1;
        
    def RegisterConfigFile(self, name):
        if os.path.exists(name):
            cache_data = linecache.getlines(name)
            cache_len = len(cache_data)
            self.picCount += cache_len
            self.configFile.append((name,cache_len))
            linecache.clearcache()
    
    def resetParser(self):
        self.configFile = []
        self.picCount = 0
        self.currentIndex = 1;
           
    def ReadUri(self, line):
        if (0 == line):
            line = self.currentIndex
        cFlen = len(self.configFile)
        if (0 == cFlen):
            return 0, "pic_end.jpg"
            
        #print self.configFile[0][0], self.configFile[0][1]
        if (line <= self.configFile[0][1]):
            cache_data = linecache.getline(self.configFile[0][0], line)
            linecache.clearcache()
            self.currentIndex += 1
            uri =  cache_data.strip('\n')
            if os.path.exists(uri):                
                return line, uri               
            else:
                return 0, "pic_oops.jpg"
        else:
            self.currentIndex = 1
            return 0, "pic_end.jpg"

class PicturesApp(App):
        
    def add_Step(self, label,  *largs):
        step = self.btn_Step.text
        if (step == "1"):
            self.btn_Step.text = "10"
            self.btn_Prev.text = "Prev 10"
            self.btn_Next.text = "Next 10"
        elif (step == "10"):
            self.btn_Step.text = "50"
            self.btn_Prev.text = "Prev 50"
            self.btn_Next.text = "Next 50"
        elif (step == "50"):
            self.btn_Step.text = "100" 
            self.btn_Prev.text = "Prev 100"
            self.btn_Next.text = "Next 100"           
        else:
            self.btn_Step.text = "1"
            self.btn_Prev.text = "Prev 1"
            self.btn_Next.text = "Next 1"
            
    def add_next(self, label,  *largs):
        state = self.btn_Pause.text
        if (state == "Pause"):
            #timer is running
            self.SWTimerStop()
            
        #LoadPic() will increase the currentIndex for 1
        step = int(self.btn_Step.text) -1
        print str(step)
        if(self.up.picCount > (step+self.up.currentIndex)):
            self.up.currentIndex += step       
        self.LoadPic()
        
        if (state == "Pause"):
            self.SWTimerStart()
            
    def add_prev(self, label,  *largs):
        state = self.btn_Pause.text
        if (state == "Pause"):
            #timer is running
            self.SWTimerStop()
        
        #self.up.currentIndex is already pointing the next pic
        step = int(self.btn_Step.text) +1
        if(self.up.currentIndex > step):
            self.up.currentIndex -= step            
        self.LoadPic()
        
        if (state == "Pause"):
            self.SWTimerStart()
               
    def add_Pause(self, label,  *largs):        
        state = self.btn_Pause.text
        if (state == "Pause"):
            self.btn_Pause.text = "Start"
            self.SWTimerStop()
        else:
            self.btn_Pause.text = "Pause"
            self.SWTimerStart()
                

    def add_hide(self, label,  *largs):
        pass
         
    def add_Reload(self, label,  *largs):
        #cancel timer
        self.SWTimerStop()
        self.label.text = str("reloading")
        
        ostype = platform.system()
        if (ostype == "Windows"):
            config_name = "dirs_config_win.txt"
        else:
            config_name = "dirs_config_android.txt"
            
        if os.path.exists(config_name):
            cache_data = linecache.getlines(config_name)
            linecache.clearcache()
            
            pics_config = open("pics.config.txt", 'w')
            for dirName in cache_data:
                dirName = dirName.strip('\n')
                print dirName
                pics = glob(join(dirName, "*.jpg"))
                sample_list = [line+'\n' for line in pics]
                pics_config.writelines(sample_list)
            pics_config.close()
        
        self.up.resetParser()
        self.up.RegisterConfigFile("pics.config.txt")
        
        self.label.text = str("reloaded")
        self.SWTimerStart()
                
    def build(self):
        
        #img = AsyncImage(source="cityCC0.png")
        self.img = AsyncImage(source="pic_loading.jpg", nocache=True)
        
        layoutUP = BoxLayout(size_hint=(1, 1))
        layoutUP.add_widget(self.img)
        
        self.label = Label(text='0', color=[.8,.9,0,1],)
        self.btn_Pause = Button(text='Pause', color=[.8,.8,0,1],
                            on_press=partial(self.add_Pause, self.label,  500))
        self.btn_Step = Button(text='1', color=[.8,.9,0,1], size_hint=(0.4, 1),
                            on_press=partial(self.add_Step, self.label,  100))
        self.btn_Prev = Button(text='Prev', color=[.8,.8,0,1],
                            on_press=partial(self.add_prev, self.label,  500))
        self.btn_Next = Button(text='Next', color=[.8,.9,0,1],
                            on_press=partial(self.add_next, self.label,  100))
        btn_Hide = Button(text='Hide', color=[.7,.7,0,1],
                            on_press=partial(self.add_hide, self.label,  500))
        btn_Reload = Button(text='Reload', color=[.6,.6,0,1], size_hint=(0.5, 1),
                            on_press=partial(self.add_Reload, self.label))
        
                            
        #layout = BoxLayout(size_hint=(1, None), height=50)
        layout = BoxLayout(size_hint=(1, 0.05))
        
        layout.add_widget(self.label)
        layout.add_widget(self.btn_Pause)
        layout.add_widget(self.btn_Step)
        layout.add_widget(self.btn_Prev)
        layout.add_widget(self.btn_Next)
        layout.add_widget(btn_Hide)
        layout.add_widget(btn_Reload)
        
        #root = BoxLayout(orientation='horizontal')
        root = BoxLayout(orientation='vertical')
        root.add_widget(layoutUP)
        root.add_widget(layout)
        
        self.up = UrlParser()
        cFile = "pics.config.txt"
        if os.path.exists(cFile):
            self.up.RegisterConfigFile(cFile)

        return root
        
    def on_start(self):
        self.SWTimerStart()
          
    def on_stop(self):
        print('app exit') 
        self.SWTimerStop()
    
    def getImageSrc(self):
        self.picline += 1
        #self.label.text = str(self.picline)
        
        print "line " + str(self.picline) + " " + str(self.picline%2)
        self.picline = self.picline%2
        if (self.picline%2 == 0):
            url = "cityCC0.png"
        else:
            url = "IMG_20170114_094015.jpg"        
        return self.picline, url
        
    def LoadPic(self):   
        #print "hello, world " + name
        #(ind, url) = self.getImageSrc()
        (ind, url) = self.up.ReadUri(0)
        #print ind,url        
        self.label.text = str(ind) + "/" + str(self.up.picCount)
        self.img.source = str(url)
          
    def SWTimerStart(self):
        self.timer1 = threading.Timer(5.0, self.SwitchPic, ["Hawk"])
        self.timer1.start()
        
    def SWTimerStop(self):
        self.timer1.cancel()
              	
    def SwitchPic(self, name):   
        self.LoadPic()        
        self.SWTimerStart()
          

if __name__ in ('__main__', '__android__'):
    PicturesApp().run()



