
install python-3.6.7-amd64.exe to C:\Python36
run F:\python3\python3.bat
Python 3.6.7
pip 10.0.1 from c:\python36\lib\site-packages\pip (python 3.6)

pip install virtualenv

cd opencv\camera

virtualenv env

env\Scripts\activate


pip install --upgrade setuptools
pip install numpy Matplotlib
pip install opencv-python

(env) F:\python3\opencv\camera>python main.py
press ENTER key to exit

env\Scripts\deactivate.bat

########################
cd F:\python3\opencv\camera
env\Scripts\activate
pip freeze > requirements.txt
##pip install -r requirements.txt



 