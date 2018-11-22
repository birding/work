
python-2.7.14.amd64.msi
windows环境变量：
系统变量和用户变量的Path中加 ;C:\Python27\;C:\Python27\Scripts;

C:\Users\Shanping>python --version
Python 2.7.14

C:\Users\Shanping>pip install virtualenv

cd F:\python\opencv\camera

virtualenv env

env\Scripts\activate


pip install --upgrade setuptools
pip install numpy Matplotlib
pip install opencv-python

(env) F:\python\opencv\image>python main.py
press ENTER key to exit

env\Scripts\deactivate.bat

########################
cd F:\python\opencv\image
env\Scripts\activate
pip freeze > requirements.txt
##pip install -r requirements.txt



 