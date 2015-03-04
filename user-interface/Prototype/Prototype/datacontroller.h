#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

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

class DataController : public QObject
{
    Q_OBJECT

public:
    DataController(QObject * parent = 0);
    ~DataController();

    void runSensorThread();
    void runCrashDetectionThread();
    void runNetworkThread();

    double strToDouble(char* str);
    int openPort();
    void dumpData();

    SensorData getDataPacket();

    Q_INVOKABLE QStringList getList();
    Q_INVOKABLE QStringList getMessageList();

public slots:
    void parseData(char* buffer);
    void getMessage(const QString &msg);

signals:
    void updateDiagnosticInfo();
    void updateMessages();
    void sendToCrashDetection(const SensorData &data);

protected:
    QQmlApplicationEngine _Engine;
    QList<QString> _DiagnosticDataList;
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

    QList<QString> _Messages;
};

#endif // DATA_H
