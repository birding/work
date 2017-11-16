#ifndef PICTHREAD_H
#define PICTHREAD_H
#include <QThread>

class picthread : public QThread
{
    Q_OBJECT
public:
    picthread();
    void run();

    int interval;
    int state;
    void emitUpdateSignal();

signals:
    void updateSignal();

};

#endif // PICTHREAD_H
