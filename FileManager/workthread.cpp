#include "workthread.h"
#include <QDebug>

WorkThread::WorkThread()
{
    stopped = false;
    diskInfo = NULL;
}

void WorkThread::run()
{
    while(!stopped)
    {
        //emit mySignal(messageStr);
        //qDebug()<< "running:" << messageStr;
        if(NULL != diskInfo)
        {
            diskInfo->BuildDisk(0);
        }
        stopped = true;
        sleep(1);
    }
    stopped = false;
}

void WorkThread::stop()
{
     stopped = true;
}

void WorkThread::setMessage(QString message)
{
    messageStr = message;
}

void WorkThread::setDiskInfo(DiskInfo * dInfo)
{
    diskInfo = dInfo;
}
