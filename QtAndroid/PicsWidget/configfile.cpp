#include "configfile.h"
#include <string.h>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QMessageBox>

configfile::configfile()
{
    /*
    productType:  "windows"
    kernelType:  "winnt"
    productType:  "10"
    **
    productType:  "android"
    kernelType:  "linux"
    productType:  "6.0"
    */
    qDebug() << "productType: " << QSysInfo::productType(); //android,windows
    qDebug() << "kernelType: " << QSysInfo::kernelType();
    qDebug() << "productType: " << QSysInfo::productVersion();

    if(QSysInfo::productType() ==  "windows")
    {
        CfgDir = "F:/QT/QtAt1/PicsWidget/pics_config";
        dirFile = "/pics.dir_config_win.txt";
        picsCfgFile = "/pics.file_config_win.txt";
    }
    else if(QSysInfo::productType() ==  "android")
    {
        CfgDir = "/storage/13EF-3B08/pics_config";
        dirFile = "/pics.dir_config_android.txt";
        picsCfgFile = "/pics.file_config_android.txt";
    }
    else
    {
        qDebug() << "unknow: ";
        qDebug() << "productType: " << QSysInfo::productType(); //android,windows
        qDebug() << "kernelType: " << QSysInfo::kernelType();
        qDebug() << "productType: " << QSysInfo::productVersion();
    }

    reload();
}

void configfile::reload()
{
    picIndex=0;
    picCount=0;
    picsList.clear();
    QString CfgFile = CfgDir + picsCfgFile;
    qDebug("[%d]%s", __LINE__, CfgFile.toLocal8Bit().constData());
    if(false == QFile::exists(CfgFile))
    {
        QMessageBox::information(NULL,"reload config","error: no pic cfgfile!");
        return ;
    }

    QFile file(CfgFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(NULL,"reload config","open pic cfgfile failed!");
         return;
    }

     while (!file.atEnd()) {
         QString line = file.readLine();
         picsList.append(line);
     }
     picCount = picsList.count();
     qDebug("picCount %d",  picCount);
     //qDebug("%s",  qPrintable(picsList[10]));
}

QString configfile::getNextPic()
{
    if(picCount == 0)
    {
        return QString(CfgDir+"/pic_oops.jpg");
    }
    //qDebug("[%d]picIndex %d, picCount %d", __LINE__, picIndex, picCount);

    if(picIndex == picCount)
        picIndex =0;
    QString picFile = picsList[picIndex].toLocal8Bit();
    //qDebug("[%d]%s", __LINE__, picFile.toLocal8Bit().constData());

    picIndex++;
    picFile.replace("\\","\\\\");
    picFile.replace(QString("\n"), QString(""));
    if(true == QFile::exists(picFile))
    {
        return picFile;
    }
    //qDebug("[%d]%s", __LINE__, picFile.toLocal8Bit().constData());
    return QString(CfgDir+"/pic_oops.jpg");
}
