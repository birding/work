#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>
#include "diskinfo.h"

class WorkThread : public QThread
{
    Q_OBJECT
public:
    WorkThread();
    void setMessage(QString message);
    void stop();
    void setDiskInfo(DiskInfo * dInfo);

signals:
    void mySignal(QString message);

protected:
    void run();

private:
    volatile bool stopped;
    QString messageStr;
    DiskInfo * diskInfo;

};

#endif // WORKTHREAD_H
