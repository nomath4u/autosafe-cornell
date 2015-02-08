#ifndef SENSORTHREAD
#define SENSORTHREAD

#include <QObject>
#include <QString>
#include <QDebug>
#include <QThread>

class SensorThread : public QThread
{
    Q_OBJECT

    void run() Q_DECL_OVERRIDE {
        QString result;
        qDebug() << "Sensor thread is running!";
        result = "done!";
        emit resultReady(result);
    }

public:
    SensorThread(QObject *parent = 0);
    ~SensorThread();

signals:
    void resultReady(const QString &s);
};

#endif // SENSORTHREAD

