#include "diskinfo.h"

DiskInfo::DiskInfo()
{
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

    DBname = "FileDB";
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

QString DiskInfo::CreateDisk(QString srcDir, QString dName)
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

    baseDestDir = mPath;
    baseSrcDir = srcDir;
    return baseDestDir;
}

void DiskInfo::BuildDisk(int)
{
    QDir sdir(baseSrcDir);
    QDir dDir(baseDestDir);

    qDebug() << "srcBase:" + baseSrcDir +" destBase:" +baseSrcDir;
    qDebug() << "dDir.currentPath:" << dDir.currentPath();
    qDebug() << "dDir.absolutePath:" << dDir.absolutePath();
    qDebug() << "sdir.currentPath:" << sdir.currentPath();
    qDebug() << "sdir.absolutePath:"<< sdir.absolutePath();

    SrcFileCount = DiskFileCount(sdir);
    currentFileIndex = 0;
    //qDebug() << "SrcFileCount: " << SrcFileCount;

    BuildDisk2(sdir, dDir);

    emit sBuildProgress("done!");
    return ;
}

int DiskInfo::DiskFileCount(QDir sdir)
{
    int count = sdir.entryInfoList().count() - 2; //"." and ".."

    foreach(QFileInfo mfi ,sdir.entryInfoList())
    {
        if(mfi.isDir())
        {
            if(mfi.fileName()=="." || mfi.fileName() == "..") continue;

            sdir.cd(mfi.fileName());
            count = count + DiskFileCount(sdir);
            sdir.cdUp();
        }
    }
    return count;
}


int DiskInfo::BuildDisk2(QDir sdir, QDir dDir)
{
    foreach(QFileInfo mfi ,sdir.entryInfoList())
    {
        if(mfi.isFile())
        {
            currentFileIndex++;
            //qDebug() << "("<< currentFileIndex << "/" << SrcFileCount<< ")"
            //        << "File :" << mfi.absoluteFilePath() << "size " << mfi.size()
            //        << " name:" << mfi.fileName();

            QString filepath = dDir.absolutePath()+ "/"+ mfi.fileName();// + ".txt";
            //qDebug()<< "Filepath:" << filepath;
            QFile file( filepath );
            file.open( QIODevice::ReadWrite | QIODevice::Text );
            QTextStream fout(&file);
            fout << "FilePath:" << mfi.absoluteFilePath() << endl;
            fout << "size:" << mfi.size() << endl;
            fout << "name:" << mfi.fileName() << endl;
            fout << "Time:" << mfi.created().toString(Qt::ISODate) << endl;
            fout.flush();
            file.close();
        }else
        {
            if(mfi.fileName()=="." || mfi.fileName() == "..") continue;
            //qDebug() << "Entry Dir" << mfi.absoluteFilePath();

            QString dirpath = dDir.absolutePath()+ "/" + mfi.fileName();

            dDir.mkdir(dirpath);
            dDir.cd(mfi.fileName());
            //qDebug() << "old sdir:" << sdir.absolutePath() << " " << mfi.fileName();
            sdir.cd(mfi.absoluteFilePath());

            currentFileIndex++;

            //qDebug() << "("<< currentFileIndex << "/" << SrcFileCount<< ")"
            //         << "Dir: " << mfi.absoluteFilePath();

            //qDebug() << "new sdir:" << sdir.absolutePath();
            //qDebug() << "new dest dir:" << dDir.absolutePath();
            BuildDisk2(sdir, dDir);
            dDir.cdUp();
            sdir.cdUp();
        }

        QString progress = QString("(%1/%2)%3").arg(currentFileIndex).arg(SrcFileCount)
                .arg(mfi.absoluteFilePath());
        emit sBuildProgress(progress);
    }

    return 0;
}



