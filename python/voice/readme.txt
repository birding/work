
python-2.7.14.amd64.msi
windows环境变量：
系统变量和用户变量的Path中加 ;C:\Python27\;C:\Python27\Scripts;

C:\Users\Shanping>python --version
Python 2.7.14

C:\Users\Shanping>pip install virtualenv

cd F:\python\voice

virtualenv env

env\Scripts\activate
pip install pyttsx
pip install pywin32

(env) F:\python\voice>python main.py
press ENTER key to exit

(env) F:\python\batForVirtualenv>env\Scripts\deactivate.bat

F:\python\batForVirtualenv>python main.py
Traceback (most recent call last):
  File "main.py", line 5, in <module>
    from PIL import Image
ImportError: No module named PIL


env\Scripts\deactivate.bat


########################
cd F:\python\batForVirtualenv
env\Scripts\activate
pip freeze >requirements.txt
##pip install -r requirements.txt


#########################
windows cmd.exe
F:\python\batForVirtualenv\start.bat  --- run successfully
F:\python\batForVirtualenv\main.py   --- report No module named PIL
 