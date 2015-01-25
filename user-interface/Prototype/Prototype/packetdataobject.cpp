#include "packetdataobject.h"

PacketDataObject::PacketDataObject(QObject *parent) : QObject(parent)
{

}

PacketDataObject::PacketDataObject(double accelerometer[3], double gyroscope[3], double magnetometer, int satellites, time_t time_h, time_t time_m, time_t time_s, time_t date_d, time_t date_m, time_t date_y, double coordinates[], int pos_fix, double speed, char obd_com[10], int obd_val, QObject *parent)
{

    mAccelerometerX = accelerometer[0];
    mAccelerometerY = accelerometer[1];
    mAccelerometerZ = accelerometer[2];

    mGyroX = gyroscope[0];
    mGyroY = gyroscope[1];
    mGyroZ = gyroscope[2];

    mMagnetometer = magnetometer;
    mSatellites = satellites;
    mHour = time_h;
    mMinute = time_m;
    mSecond = time_s;
    mDay = date_d;
    mMonth = date_m;
    mYear = date_y;

    mCoordinatesLat = coordinates[0];
    mCoordinatesLong = coordinates[1];

    mPositionFix = pos_fix;
    mSpeed = speed; /* convert from knots to to mph here? */

    //list of ODBII data in command & value pair */
    mODBIIData.append(new ODBIICommand("command", "value")); //need to talk this through more...

}

PacketDataObject::~PacketDataObject()
{

}

