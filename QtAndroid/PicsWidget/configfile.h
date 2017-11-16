#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QString>
#include <QStringList>

class configfile
{
public:
    configfile();

    int picIndex;
    int picCount;
    QStringList picsList;
    void reload();

    QString getNextPic();
    QString dirFile;
    QString picsCfgFile;
    QString CfgDir;
};

#endif // CONFIGFILE_H
