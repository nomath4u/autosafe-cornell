#ifndef CRASHDETECTIONTHREAD_H
#define CRASHDETECTIONTHREAD_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QThread>
#include <QList>

#include <datacontroller.h>

#define ROLLOVER_MAX 7
#define SPIN_MAX 35
#define STOP_FORCE -3
#define TBONE_FORCE 3

#define RCDEMO

class CrashDetectionThread : public QThread
{
    Q_OBJECT
    void run();
    SensorData parseData(QString line);
    void dumpData(SensorData data);


private:
    void complementaryFilter(SensorData data);


public:
    CrashDetectionThread(QObject *parent = 0);
    ~CrashDetectionThread();

public slots:
    void analyzeData(const SensorData &data);

signals:
    void crashDetected(const QString &msg);
};

#endif // CRASHDETECTIONTHREAD_H
