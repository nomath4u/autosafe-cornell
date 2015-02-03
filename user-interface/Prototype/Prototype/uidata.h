#ifndef UIDATA_H
#define UIDATA_H

#include <QObject>
#include <QString>

struct ODBIICommand{
    QString command;
    QString value;
};

class UIData : public QObject
{
    Q_OBJECT

    public:
        UIData(QObject * parent = 0);
        UIData(
                double accelerometer[3],
                double gyroscope[3],
                double magnetometer,
                int satellites,
                time_t time_h,
                time_t time_m,
                time_t time_s,
                time_t date_d,
                time_t date_m,
                time_t date_y,
                double coordinates[2],
                int pos_fix,
                double speed,
                char obd_com[10],
                int obd_val
        );
        ~UIData();

    void updateData();

    signals:

    public slots:

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
        double _Speed; /* is this really in knots? */

        //list of ODBII data in command & value pairs
        QList<ODBIICommand> _ODBIIData;

        QList<QString> _Alerts;
        QList<QString> _Conditions;
};


#endif // UIDATA_H
