@echo off

echo 当前盘符：%~d0
echo 当前路径：%cd%
echo 当前执行命令行：%0
echo 当前bat文件路径：%~dp0
echo 当前bat文件短路径：%~sdp0

cd /D %~dp0
echo 当前路径：%cd%

set "PATH=C:\Python36;C:\Python36\Scripts;%PATH%"
rem echo %PATH%

Rem cmd /k "python --version && pip -V"

Rem cmd /k "env\Scripts\activate & python main.py"

env\Scripts\activate & python main.py

REM python main.py

env\Scripts\deactivate.bat

pause
