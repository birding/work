
安装python-3.6.7-amd64.exe

windows环境变量：
系统变量和用户变量的Path中加 ;C:\Python36\;C:\Python36\Scripts;

C:\Users\Shanping>python --version
Python 3.6.7

C:\Users\Shanping>pip install virtualenv

cd F:\Tech\python3\wallpaper

virtualenv env

env\Scripts\activate
pip install pillow
pip install pywin32

create D:\wallpaper_temp first
Need to clear the cache file in 'D:\wallpaper_temp' when picture infos are changed

(env) F:\python\wallpaper>python main.py
press ENTER key to exit




(env) F:\python\wallpaper>env\Scripts\deactivate.bat

env\Scripts\deactivate.bat

#Run pics_files.py to generate the pics_files.txt
pics_files.txt: the database of pictures' path from dirs_config_win.txt


########################
cd F:\python\wallpaper
env\Scripts\activate
pip freeze >requirements.txt
##pip install -r requirements.txt


#########################
windows cmd.exe
F:\python\batForVirtualenv\start.bat  --- run successfully
F:\python\batForVirtualenv\main.py   --- report No module named PIL

win10的当前桌面壁纸保存位置
C:\Users\Shanping\AppData\Roaming\Microsoft\Windows\Themes\CachedFiles
系统自带背景图片
C:\Windows\Web\Wallpaper\Theme1
删除Win10背景图片（壁纸）历史记录
win+R regedit 
HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\Wallpapers
BackgroundHistoryPath0~4

开机启动：
C:\ProgramData\Microsoft\Windows\Start Menu\Programs\StartUp