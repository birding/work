@echo off

echo ��ǰ�̷���%~d0
echo ��ǰ·����%cd%
echo ��ǰִ�������У�%0
echo ��ǰbat�ļ�·����%~dp0
echo ��ǰbat�ļ���·����%~sdp0

cd /D %~dp0
echo ��ǰ·����%cd%

set "PATH=C:\Python36;C:\Python36\Scripts;%PATH%"
rem echo %PATH%

Rem cmd /k "python --version && pip -V"

Rem cmd /k "env\Scripts\activate & python main.py"

env\Scripts\activate & python main.py

REM python main.py

env\Scripts\deactivate.bat

pause
