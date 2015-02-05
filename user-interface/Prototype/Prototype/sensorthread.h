#ifndef SENSORTHREAD_H
#define SENSORTHREAD_H

#include <QThread>
#include "uidata.h"

class SensorThread : public QThread
{
    Q_OBJECT

public:
    SensorThread(QObject * parent = 0);
    ~SensorThread();

protected:
    void run();

    public slots:


    signals:

};

#endif // SENSORTHREAD_H
