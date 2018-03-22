##on windows10#
install PyCharm Community Edition 2017.2.3
install python2.7 on windows
	python-2.7.14.amd64.msi 选择安装组件的一步时，勾上所有的组件.特别要注意选上pip和Add python.exe to Path
	默认会安装到C:\Python27目录下，添加C:\Python27至用户环境变量和系统环境变量的path中。
	打开命令提示符窗口，敲入python --version

python -m pip install --upgrade pip wheel setuptools
python -m pip install docutils pygments pypiwin32 kivy.deps.sdl2 kivy.deps.glew
python -m pip install kivy.deps.gstreamer
python -m pip install kivy.deps.angle
python -m pip install kivy
python -m pip install kivy_examples

C:\Python27\share\kivy-examples



debug in C:\Users\cv\.kivy\logs\

###on android###
KivyLauncher-1.9.0.0.apk
kivydemo-for-android.zip
view the file property to make sure it's in /sdcard/kivy