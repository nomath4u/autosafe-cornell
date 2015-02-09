#ifndef SENSORTHREAD
#define SENSORTHREAD

#include <QObject>
#include <QString>
#include <QDebug>
#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

class SensorThread : public QThread
{
    Q_OBJECT
    void run();

public:
    SensorThread(QObject *parent = 0);
    double StringToDouble(char* str);
    int readPort();
    int parseData();
    ~SensorThread();

signals:
    void resultReady(const QString &s);
};

#endif // SENSORTHREAD

