1.0.0

pip freeze > requirements.txt

pip install -r requirement.txt



##encode##
C:\Users\Shanping>python f:\python\Base64Coding\coding.py F:\python\Base64Coding\testfile.txt 1 1 1
for help: python f:\python\Base64Coding\coding.py -h

F:\python\Base64Coding\testfile.txt
F:\python\Base64Coding\dGVzdGZpbGUudHh0.b64


##decode 1##
C:\Users\Shanping>python f:\python\Base64Coding\coding.py F:\python\Base64Coding\dGVzdGZpbGUudHh0.b64 2 1 1
for help: python f:\python\Base64Coding\coding.py -h

F:\python\Base64Coding\dGVzdGZpbGUudHh0.b64
F:\python\Base64Coding\testfile.txt.new

##decode 2##
C:\Users\Shanping>python f:\python\Base64Coding\coding.py F:\python\Base64Coding\dGVzdGZpbGUudHh0.b64 2 1 0
for help: python f:\python\Base64Coding\coding.py -h

F:\python\Base64Coding\dGVzdGZpbGUudHh0.b64
remove previous file
F:\python\Base64Coding\testfile.txt

