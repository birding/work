
python-2.7.14.amd64.msi
windows环境变量：
系统变量和用户变量的Path中加 ;C:\Python27\;C:\Python27\Scripts;

C:\Users\Shanping>python --version
Python 2.7.14

C:\Users\Shanping>pip install virtualenv

cd f:\python\merger_xls

f:\python\merger_xls>virtualenv env

env\Scripts\activate

pip install xlrd
pip install xlsxwriter
(env) f:\python\merger_xls>pip freeze >requirements.txt
##pip install -r requirements.txt




C:\Users\Shanping>f:\python\merger_xls\env\Scripts\activate

(env) C:\Users\Shanping>python f:\python\merger_xls\main.py
remove previous file
hit any key to exit









http://xlrd.readthedocs.io/en/latest/index.html
https://xlsxwriter.readthedocs.io/
https://github.com/jmcnamara/XlsxWriter/raw/master/docs/XlsxWriter.pdf



 