
install python-3.6.7-amd64.exe to C:\Python36
run F:\python3\python3.bat
Python 3.6.7
pip 10.0.1 from c:\python36\lib\site-packages\pip (python 3.6)

pip install virtualenv

cd opencv\face

virtualenv env

env\Scripts\activate


pip install --upgrade setuptools
pip install numpy Matplotlib
pip install opencv-python

goto https://pypi.org/project/dlib/19.7.0/#files and 
download dlib-19.8.1-cp36-cp36m-win_amd64.whl 

pip install F:\python3\opencv\dlib-19.8.1-cp36-cp36m-win_amd64.whl
pip install face_recognition

(env) F:\python3\opencv\face>python main.py
press ENTER key to exit

#############################
(env) F:\python3\opencv\face>env\Scripts\face_detection.exe shanping.jpg
shanping.jpg,209,496,424,281

(env) F:\python3\opencv\face>env\Scripts\face_recognition.exe ./knownfaces/ liguanghui.jpg
liguanghui.jpg,known_lele01
liguanghui.jpg,known_liguanghui01
liguanghui.jpg,known_qiqi01

(env) F:\python3\opencv\face>env\Scripts\face_recognition.exe --tolerance 0.5 ./knownfaces/ liguanghui.jpg
liguanghui.jpg,known_liguanghui01

(env) F:\python3\opencv\face>env\Scripts\face_recognition.exe ./knownfaces/ shanping_2.jpg
shanping_2.jpg,known_lele01
shanping_2.jpg,known_lina01
shanping_2.jpg,known_shanping01
shanping_2.jpg,known_shanping02

(env) F:\python3\opencv\face>env\Scripts\face_recognition.exe --tolerance 0.5 ./knownfaces/ shanping_2.jpg
shanping_2.jpg,known_shanping01
shanping_2.jpg,known_shanping02

(env) F:\python3\opencv\face>env\Scripts\face_recognition.exe ./knownfaces/ liguanghui.jpg
liguanghui.jpg,known_lele01
liguanghui.jpg,known_liguanghui01
liguanghui.jpg,known_qiqi01

(env) F:\python3\opencv\face>env\Scripts\face_recognition.exe --tolerance 0.5 ./knownfaces/ liguanghui.jpg
liguanghui.jpg,known_liguanghui01

(env) F:\python3\opencv\face>env\Scripts\face_recognition.exe ./knownfaces/ shanping_2.jpg
shanping_2.jpg,known_lele01
shanping_2.jpg,known_lina01
shanping_2.jpg,known_shanping01
shanping_2.jpg,known_shanping02

(env) F:\python3\opencv\face>env\Scripts\face_recognition.exe --tolerance 0.5 ./knownfaces/ shanping_2.jpg
shanping_2.jpg,known_shanping01
shanping_2.jpg,known_shanping02

env\Scripts\deactivate.bat

########################
cd F:\python3\opencv\face
env\Scripts\activate
pip freeze > requirements.txt
##pip install -r requirements.txt



######doc########
https://pypi.org/project/face_recognition/

https://face-recognition.readthedocs.io/en/latest/readme.html

Adjusting Tolerance / Sensitivity
If you are getting multiple matches for the same person, it might be that
the people in your photos look very similar and a lower tolerance value
is needed to make face comparisons more strict.
You can do that with the --tolerance parameter. The default tolerance
value is 0.6 and lower numbers make face comparisons more strict:

