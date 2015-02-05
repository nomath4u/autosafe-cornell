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
        UIData(double acc[3],
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

        ~UIData();

    private:
        double strict_str2double(char* str);
        void copy_string(char *target, char *source);
        void parse_data();
        void dump_data();

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
        double _Speed;

        char * _OBDIICommand;
        int _OBDIIValue;

//        QList<ODBIICommand> _OBDIIData;

//        QList<QString> _Alerts;
//        QList<QString> _Conditions;
};


#endif // UIDATA_H
