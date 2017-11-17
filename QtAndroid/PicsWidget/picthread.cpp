#include "picthread.h"
#include <QThread>
#include <QDebug>

picthread::picthread()
{
    interval = 5000;
    state = 1; //runing
}

void picthread::run()
{
    while (1) {
        msleep(interval*1000);
        if(state)
        {
            //qDebug("thread running");
            emitUpdateSignal();
        }
    }
}

void picthread::emitUpdateSignal()
{
    //qDebug("thread updateSignal>>");
    updateSignal();
}
