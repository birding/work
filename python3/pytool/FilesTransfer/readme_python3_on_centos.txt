###basic use###
mkdir /home/pytool
cd /home/pytool
wget https://www.python.org/ftp/python/3.7.4/Python-3.7.4.tgz
tar xzvf Python-3.7.4.tgz
cd Python-3.7.4
./configure
make

/home/pytool/Python-3.7.4/python 1.filesCollect.py
/home/pytool/Python-3.7.4/python 4.syncToDisk.py


####use python3 virtual-env on centos###
##upgrade openssl#
cd /home/pytool
wget https://www.openssl.org/source/openssl-1.1.1d.tar.gz
tar xzvf openssl-1.1.1d.tar.gz
cd openssl-1.1.1d
./config --prefix=/usr/local/openssl
./config -t
make
make install

##
yum install python-virtualenv libffi-devel xz-devel
cd /home/pytool
cd Python-3.7.4
./configure --prefix=/home/pytool/python3 --with-openssl=/usr/local/openssl --with-ssl-default-suites=python

vi Modules/Setup.dist
	# Socket module helper for socket(2)
	_socket socketmodule.c
	
	# Socket module helper for SSL support; you must comment out the other
	# socket line above, and possibly edit the SSL variable:
	SSL=/usr/local/ssl
	_ssl _ssl.c \
	        -DUSE_SSL -I$(SSL)/include -I$(SSL)/include/openssl \
	        -L$(SSL)/lib -lssl -lcrypto

make
make install

cd /home/pytool
virtualenv -p /home/pytool/python3/bin/python3 env

source /home/pytool/env/bin/activate

(env) [root@ThC pytool]# python --version
Python 3.7.4
(env) [root@ThC pytool]# pip --version
pip 9.0.1 from /home/pytool/env/lib/python3.7/site-packages (python 3.7)

(env) [root@ThC pytool]# pip install pandas


(env) [root@ThC FilesTransfer]# python 1.filesCollect.py
(env) [root@ThC FilesTransfer]# python 6.deleteFiles.py


(env) [root@ThC Python-3.7.4]# deactivate



#######QA##########
Q:python3 make install No module named '_ctypes'
A:yum install libffi-devel

Q:pip is configured with locations that require TLS/SSL, however the ssl module in Python is not available.
A:upgrade openssl
./configure --prefix=/home/pytool/python3 --with-openssl=/usr/local/openssl --with-ssl-default-suites=python



