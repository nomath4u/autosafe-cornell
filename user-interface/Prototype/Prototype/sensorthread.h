#ifndef SENSORTHREAD_H
#define SENSORTHREAD_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QThread>

#define BAUDRATE B9600

class SensorThread : public QThread
{
    Q_OBJECT
    void run();

public:
    SensorThread(QObject *parent = 0);
    ~SensorThread();

    double strToDouble(char* str);
    int openPort();
    void parseData();

signals:
    void bufferReady(char* buffer);
};

#endif // SENSORTHREAD

