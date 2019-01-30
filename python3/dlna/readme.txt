
install python-3.6.7-amd64.exe to C:\Python36
run F:\python3\python3.bat
Python 3.6.7
pip 10.0.1 from c:\python36\lib\site-packages\pip (python 3.6)

pip install virtualenv

cd dlna

virtualenv env

env\Scripts\activate

# https://pypi.org/project/nanodlna/#files
# download nanodlna-0.1.6-py3-none-any.whl
# pip install f:\python3\dlna\nanodlna-0.1.6-py3-none-any.whl
# ---failed

https://github.com/cherezov/dlnap
download dlnap-master.zip

(env) F:\python3\dlna>python dlnap.py
Discovered devices:
 [a] µçÊÓ¹û(319) @ 192.168.18.15

dlnap.py --ip 192.168.18.15 --play http://somewhere.com/music.mp3
python dlnap.py --ip 192.168.18.15 --play F:\pictures\2018_2\IMG_20181001_184932_HHT_mh1538393487235.jpg --Í¼Æ¬¼ÓÔØ´íÎó

python dlnap.py --ip 192.168.18.15 --play F:\H3C_Projects_draft_v0.png --Í¼Æ¬¼ÓÔØ´íÎó
python dlnap.py --ip 192.168.18.15 --play F:\BaiduYunDownload\newconcept_child\001.U01_L01_A.mp3 --ÄÚÈÝ»ñÈ¡Ê§°Ü
python dlnap.py --ip 192.168.18.15 --play http://192.168.18.8:8000/BaiduYunDownload/Dead_Pool2.mp4 -- download slow

cmd 
F:\>python -m SimpleHTTPServer
http://192.168.25.1:8000/

python dlnap.py --ip 192.168.18.15 --play http://192.168.18.8:8000/pictures/2018_2/IMG_20180720_072117.jpg -- ok
python dlnap.py --ip 192.168.18.15 --play http://192.168.18.8:8000/pictures/2018_2/IMG_20181001_183320_HDR.jpg -- ok
python dlnap.py --ip 192.168.18.15 --stop
#############################


env\Scripts\deactivate.bat



