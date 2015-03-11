#ifndef CRASHDETECTIONTHREAD_H
#define CRASHDETECTIONTHREAD_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QThread>
#include <datacontroller.h>

#define ROLLOVER_MAX 90
#define SPIN_MAX 100
#define STOP_FORCE -3
#define TBONE_FORCE 3


class CrashDetectionThread : public QThread
{
    Q_OBJECT
    void run();

private:
    SensorData prevData;

public:
    CrashDetectionThread(QObject *parent = 0);
    ~CrashDetectionThread();

public slots:
    void analyzeData(const SensorData &data);

signals:
    void situationDetected(const QString msg);
};

#endif // CRASHDETECTIONTHREAD_H
