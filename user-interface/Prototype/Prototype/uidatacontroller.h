#ifndef UIDATA_H
#define UIDATA_H

#include <QObject>
#include <QString>
#include <QDebug>

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "sensorthread.h"

class UIDataController : public QObject
{
    Q_OBJECT

public:
    UIDataController(QObject * parent = 0);
    void setValues(double acc[3],
            double gy[3],
            double mag,
            int sats,
            time_t hour,
            time_t min,
            time_t sec,
            time_t day,
            time_t month,
            time_t year,
            double coords[2],
            int fix,
            double spd,
            char com[10],
            int val
    );

    void updateData();
    QStringList getList();
    void runSensorThread();

    ~UIDataController();

public slots:
    void handleResults(const QString &);

signals:
    //signal UI new data is ready?

private:
    double _AccelerometerX;
    double _AccelerometerY;
    double _AccelerometerZ;
    double _GyroX;
    double _GyroY;
    double _GyroZ;
    double _Magnetometer;
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
    char * _OBDIICommand;
    int _OBDIIValue;

//    QList<QString> _Alerts;
//    QList<QString> _Conditions;

    double strict_str2double(char* str);
    void parse_data();
    void dump_data();
};

#endif // UIDATA_H
