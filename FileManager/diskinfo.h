#ifndef DISKINFO_H
#define DISKINFO_H
#include <qstring.h>
#include <QFileInfo>
#include <QDebug>
#include <QDir>

class DiskInfo
{
public:
    DiskInfo();
    int DirCheck(QString dir);
    int CreateDB(void);
    QString CreateDisk(QString dName);
    int BuildDisk(QString srcBaseDir, QString destDiskDir);
    int BuildDisk2(QDir sdir, QDir dDir);
    int CreateFileInfo(QString srcBaseDir, QFileInfo fName);
private:
    QString DBname;
    QString Diskname;
    QString DiskDir;
};

#endif // DISKINFO_H
