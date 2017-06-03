#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include "PCI1753.h"
#include "PCI1713.h"
#include <QString>
#include "PulseGeneratorController.h"
#include "PortReaderWriter.h"

class MyThread : public QThread
{
Q_OBJECT
public:
    explicit MyThread();

    void run();
signals:
    void send(double, double);
    void sendMsgToConsole(QString);

    void sendf(double, double, double, double);
    void sendinfo(double, double, double);
private:
    PCI1753 PCI1753_0;
    PCI1713 PCI1713_3;
    void reload_PCI1713();
    float data[32];
    PulseGeneratorController boardController;
    PortReaderWriter prw;
    static const int activeChannels[];

};

#endif
