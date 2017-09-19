#ifndef DISKINFO_H
#define DISKINFO_H
#include <qstring.h>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QObject>
#include <QThread>
#include <QDateTime>

class DiskInfo: public QObject
{
    Q_OBJECT
public:
    DiskInfo();
    int DirCheck(QString dir);
    int CreateDB(void);
    QString CreateDisk(QString SrcDir, QString dName);
    int BuildDisk2(QDir sdir, QDir dDir);
    int CreateFileInfo(QString srcBaseDir, QFileInfo fName);
    int DiskFileCount(QDir sdir);
    void BuildDisk(int para);

signals:
    void sBuildProgress(QString message);
    void sBuildProgressDone();

private:
    QString DBname;

    QString Diskname; //from UI
    QString baseDestDir;
    QString baseSrcDir; //from UI
    int SrcFileCount;
    int currentFileIndex;
};

#endif // DISKINFO_H
