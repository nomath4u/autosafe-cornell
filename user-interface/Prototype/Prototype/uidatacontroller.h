#ifndef UIDATACONTROLLER_H
#define UIDATACONTROLLER_H

#include <QObject>
#include <QString>
#include <QQmlApplicationEngine>
#include <QQmlListProperty>

#define BAUDRATE B9600

class UIDataController : public QObject
{
    Q_OBJECT

public:
    UIDataController(QObject * parent = 0);
    ~UIDataController();

    double strToDouble(char* str);
    int openPort();
    void parseData();
    void dumpData();
    void updateData();
    QStringList getList();
    void runSensorThread();

public slots:

signals:
    void sendToQml(QString &string);

private:
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
