windows:
pics_config\
pic_end.jpg
pic_loading.jpg
pic_oops.jpg
pics.dir_config_win.txt

release:
build-PicsWidget-Desktop_Qt_5_9_2_MinGW_32bit-Release\release\PicsWidget.exe
C:\Qt\Qt5.9.2\5.9.2\mingw53_32\bin\
Qt5Widgets.dll, Qt5Gui.dll, libgcc_s_dw2-1.dll, Qt5Core.dll, libstdc++-6.dll, libwinpthread-1.dll


Android:

http://www.qter.org/forum.php?mod=viewthread&tid=203&extra=page%3D1
ysp123ysp:6310xxx

adb push
pics_config\
pic_end.jpg
pic_loading.jpg
pic_oops.jpg
pics.dir_config_android.txt

adb shell
cd /storage/13EF-3B08/pics_config
echo "/storage/13EF-3B08/pics/2013" > pics.dir_config_android.txt
echo "/storage/13EF-3B08/pics/2014" >> pics.dir_config_android.txt