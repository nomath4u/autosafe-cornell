#ifndef SENSORTHREAD_H
#define SENSORTHREAD_H

#include <QThread>

class SensorThread : public QThread
{
    Q_OBJECT

public:
    SensorThread(QObject * parent = 0 );
    ~SensorThread();

protected:
    void run();

    public slots:
        void getData();

    signals:
        void dataReceived();

};

#endif // SENSORTHREAD_H
