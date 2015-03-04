#ifndef UIDATACONTROLLER_H
#define UIDATACONTROLLER_H

#include <QObject>
#include <QString>
#include <QQmlApplicationEngine>
#include <QQmlListProperty>

struct SensorData {
    double accelerometerX;
    double accelerometerY;
    double accelerometerZ;
    double gyroX;
    double gyroY;
    double gyroZ;
    time_t hour;
    time_t minute;
    time_t second;
    time_t day;
    time_t month;
    time_t year;
    double speed;
};

class UIDataController : public QObject
{
    Q_OBJECT

public:
    UIDataController(QObject * parent = 0);
    ~UIDataController();

    void runSensorThread();
    void runCrashDetectionThread();

    double strToDouble(char* str);
    int openPort();
    void dumpData();

    SensorData getDataPacket();

    Q_INVOKABLE QStringList getList();

public slots:
    void parseData(char* buffer);

signals:
    void sendToQml();
    void sendToCrashDetection(const SensorData &data);

protected:
    QQmlApplicationEngine _Engine;
    QList<QString> _List;
    double _AccelerometerX;
    double _AccelerometerY;
    double _AccelerometerZ;
    double _GyroX;
    double _GyroY;
    double _GyroZ;
    int    _Satellites;
    time_t _Hour;
    time_t _Minute;
    time_t _Second;
    time_t _Day;
    time_t _Month;
    time_t _Year;
    double _CoordinatesLat;
    double _CoordinatesLong;
    int _PositionFix;
    double _Speed;
    char *_OBDIICommand;
    int _OBDIIValue;
};

#endif // UIDATA_H
