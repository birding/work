#include "diskinfo.h"
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QDateTime>

DiskInfo::DiskInfo()
{
    DBname = "FileDB";
}

int DiskInfo::DirCheck(QString dir)
{
    bool ret;
    QFileInfo fi(dir);

    ret = fi.isDir();
    return ret;
}

int DiskInfo::CreateDB(void)
{
    bool ret=0;
    QDir mDir;
    qDebug() << mDir.currentPath();

    QString mPath = mDir.currentPath()+"/" + DBname;
    qDebug() << mPath;

    if(mDir.exists(mPath))
    {
        qDebug()<<"Already exists";
    }else{
        mDir.mkpath(mPath);
        qDebug()<<"Created";
    }
    return ret;
}

QString DiskInfo::CreateDisk(QString dName)
{
    QDir mDir;

    Diskname = dName;
    qDebug() << mDir.currentPath();
    QString mPath = mDir.currentPath() +"/" +DBname+"/"+ Diskname;
    qDebug() << mPath;

    if(mDir.exists(mPath))
    {
        qDebug()<<"warning Already exists";
        bool ret = mDir.rmdir(mPath);
        if(false == ret)
        {
            qDebug()<<"rmdir failed!";
        }
    }

    mDir.mkpath(mPath);
    qDebug()<< dName + " Created";

    DiskDir = mPath;
    return DiskDir;
}

int DiskInfo::BuildDisk(QString srcBaseDir, QString destDiskDir)
{
    QDir sdir(srcBaseDir);
    QDir dDir(destDiskDir);

    qDebug() << "srcBase:" + srcBaseDir +" destBase:" +destDiskDir;
    qDebug() << "dDir.currentPath:" << dDir.currentPath();
    qDebug() << "dDir.absolutePath:" << dDir.absolutePath();
    qDebug() << "sdir.currentPath:" << sdir.currentPath();
    qDebug() << "sdir.absolutePath:"<< sdir.absolutePath();

    BuildDisk2(sdir, dDir);
    return 0;
}

int DiskInfo::BuildDisk2(QDir sdir, QDir dDir)
{
    foreach(QFileInfo mfi ,sdir.entryInfoList())
    {
        if(mfi.isFile())
        {
            qDebug()<< "File :" << mfi.absoluteFilePath() << "size " << mfi.size()
                    << " name:" << mfi.fileName();

            QString filepath = dDir.absolutePath()+ "/"+ mfi.fileName();// + ".txt";
            //qDebug()<< "Filepath:" << filepath;
            QFile file( filepath );
            file.open( QIODevice::ReadWrite | QIODevice::Text );
            QTextStream fout(&file);
            fout << "FilePath:" << mfi.absoluteFilePath() << " size:" << mfi.size()
                 << " name:" << mfi.fileName() << " Time:" << mfi.created().toString(Qt::ISODate) ;
            fout.flush();
            file.close();
        }else
        {
            if(mfi.fileName()=="." || mfi.fileName() == "..") continue;
            qDebug() << "Entry Dir" << mfi.absoluteFilePath();

            QString dirpath = dDir.absolutePath()+ "/" + mfi.fileName();
            dDir.mkdir(dirpath);

            dDir.cd(mfi.fileName());
            qDebug() << "old sdir:" << sdir.absolutePath() << " " << mfi.fileName();
            sdir.cd(mfi.absoluteFilePath());
            qDebug() << "new sdir:" << sdir.absolutePath();
            qDebug() << "new dest dir:" << dDir.absolutePath();
            BuildDisk2(sdir, dDir);
            dDir.cdUp();
            sdir.cdUp();
        }
    }
    return 0;
}



