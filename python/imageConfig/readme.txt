
python-2.7.14.amd64.msi
windows环境变量：
系统变量和用户变量的Path中加 ;C:\Python27\;C:\Python27\Scripts;

C:\Users\Shanping>python --version
Python 2.7.14

C:\Users\Shanping>pip install virtualenv

cd F:\python\imageConfig

virtualenv env

env\Scripts\activate
pip install pillow


(env) F:\python\wallpaper>python main.py



(env) F:\python\wallpaper>env\Scripts\deactivate.bat

env\Scripts\deactivate.bat


########################
cd F:\python\wallpaper
env\Scripts\activate
pip freeze >requirements.txt
##pip install -r requirements.txt


#########################
windows cmd.exe
F:\python\batForVirtualenv\start.bat  --- run successfully
F:\python\batForVirtualenv\main.py   --- report No module named PIL
 