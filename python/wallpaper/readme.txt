
python-2.7.14.amd64.msi
windows����������
ϵͳ�������û�������Path�м� ;C:\Python27\;C:\Python27\Scripts;

C:\Users\Shanping>python --version
Python 2.7.14

C:\Users\Shanping>pip install virtualenv

cd F:\python\wallpaper

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

win10�ĵ�ǰ�����ֽ����λ��
C:\Users\Shanping\AppData\Roaming\Microsoft\Windows\Themes\CachedFiles
ϵͳ�Դ�����ͼƬ
C:\Windows\Web\Wallpaper\Theme1
ɾ��Win10����ͼƬ����ֽ����ʷ��¼
win+R regedit 
HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\Wallpapers
BackgroundHistoryPath0~4

����������
C:\ProgramData\Microsoft\Windows\Start Menu\Programs\StartUp