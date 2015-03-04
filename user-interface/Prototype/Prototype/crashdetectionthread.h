#ifndef CRASHDETECTIONTHREAD_H
#define CRASHDETECTIONTHREAD_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QThread>
#include <datacontroller.h>

class CrashDetectionThread : public QThread
{
    Q_OBJECT
    void run();

public:
    CrashDetectionThread(QObject *parent = 0);
    ~CrashDetectionThread();

public slots:
    void analyzeData(const SensorData &data);

signals:

};

#endif // CRASHDETECTIONTHREAD_H
