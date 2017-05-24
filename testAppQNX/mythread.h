#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include "PCI1753.h"

class MyThread : public QThread
{
Q_OBJECT
public:
    explicit MyThread();

    void run();
signals:
    void send(double, double);
private:
    PCI1753 PCI1753_0;

};

#endif // MYTHREAD_H
