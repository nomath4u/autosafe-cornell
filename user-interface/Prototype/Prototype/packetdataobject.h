#ifndef PACKETDATAOBJECT_H
#define PACKETDATAOBJECT_H

#include <QObject>

struct ODBIICommand{
    QString command;
    Qstring value;
};

class PacketDataObject : public QObject
{
    Q_OBJECT

    public:
        PacketDataObject(QObject *parent = 0);

        /* constructor to take in Sam's data packet? */
        PacketDataObject(
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
                int obd_val,
                QObject *parent = 0);

        ~PacketDataObject();

    signals:
        //data changed

    public slots:

    private:
        double mAccelerometerX;
        double mAccelerometerY;
        double mAccelerometerZ;

        double mGyroX;
        double mGyroY;
        double mGyroZ;

        double mMagnetometer;
        int    mSatellites;
        time_t mHour;
        time_t mMinute;
        time_t mSecond;
        time_t mDay;
        time_t mMonth;
        time_t mYear;

        double mCoordinatesLat;
        double mCoordinatesLong;

        int mPositionFix;
        double mSpeed; /* is this really in knots? */

        //list of ODBII data in command & value pairs
        QList<ODBIICommand> mODBIIData;
};

#endif // PACKETDATAOBJECT_H
