#include "uidata.h"

UIData::UIData(QObject * parent)
{

}

UIData::UIData(
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
            int obd_val)
{
    _AccelerometerX = accelerometer[0];
    _AccelerometerY = accelerometer[1];
    _AccelerometerZ = accelerometer[2];

    _GyroX = gyroscope[0];
    _GyroY = gyroscope[1];
    _GyroZ = gyroscope[2];

    _Magnetometer = magnetometer;
    _Satellites = satellites;
    _Hour = time_h;
    _Minute = time_m;
    _Second = time_s;
    _Day = date_d;
    _Month = date_m;
    _Year = date_y;

    _CoordinatesLat = coordinates[0];
    _CoordinatesLong = coordinates[1];

    _PositionFix = pos_fix;
    _Speed = speed; //convert from knots to to mph here?

    //list of ODBII data in command & value pair

    ODBIICommand newCommand;

    //for each command at the end of the packet:
        newCommand.command = "command";
        newCommand.value = "value";
        _ODBIIData.append(newCommand);
}

void UIData::getData()
{

}

void UIData::updateUI(UIData *newData)
{

}

void UIData::updateConditions(QString conditions)
{

}

UIData::~UIData()
{

}

