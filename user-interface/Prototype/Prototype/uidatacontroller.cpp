#include "uidatacontroller.h"
#include "sensorthread.h"

#include <QDebug>
#include <QStringListModel>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

UIDataController::UIDataController(QObject *parent)
{

}

UIDataController::~UIDataController()
{

}

double UIDataController::strToDouble(char* str)
{
    char* endptr;
    double value = strtod(str, &endptr);
    if (*endptr) return 0;
    return value;
}

void UIDataController::parseData(char *buffer){

    qDebug() << "UIDataController: Parsing data.";

    char *token = (char *)malloc(10*sizeof(char));
    token = strtok(buffer, ",");
    for (int i=0; token != NULL; i++) {
        if (i == 0) {
            _AccelerometerX = strToDouble(token);
        }
        else if (i == 1) {
            _AccelerometerY = strToDouble(token);
        }
        else if (i == 2) {
            _AccelerometerZ = strToDouble(token);
        }
        else if (i == 3) {
            _GyroX = strToDouble(token);
        }
        else if (i == 4) {
            _GyroY = strToDouble(token);
        }
        else if (i == 5) {
            _GyroZ = strToDouble(token);
        }
        else if (i == 6) {
            _PositionFix = atoi(token);
        }
        else if (i == 7) {
            _Satellites = atoi(token);
        }
        else if (i == 8) {
            _Hour = (time_t)atoi(token);
        }
        else if (i == 9) {
            _Minute = (time_t)atoi(token);
        }
        else if (i == 10) {
            _Second = (time_t)atoi(token);
        }
        else if (i == 11) {
            _Month = (time_t)atoi(token);
        }
        else if (i == 12) {
            _Day = (time_t)atoi(token);
        }
        else if (i == 13) {
            _Year = (time_t)atoi(token);
        }
        else if (i == 14) {
            _CoordinatesLat = strToDouble(token);
        }
        else if (i == 15) {
            _CoordinatesLong = strToDouble(token);
        }
        else if (i == 16) {
            _Speed = atoi(token);
        }
        else if (i == 17) {
            _OBDIICommand = "None";
        }
        else if (i == 18) {
            _OBDIIValue = atoi(token);
        }
        token = strtok(NULL, ",");
    }

    emit sendToQml();
    dumpData();
}

void UIDataController::dumpData(){

    qDebug() << "Accelerometer X: "  << _AccelerometerX;
    qDebug() << "Accelerometer Y: "  << _AccelerometerY;
    qDebug() << "Accelerometer Z: "  << _AccelerometerZ;
    qDebug() << "Gyro X: "           << _GyroX;
    qDebug() << "Gyro Y: "           << _GyroY;
    qDebug() << "Gyro Z: "           << _GyroZ;
    qDebug() << "Position Fix: "     << _PositionFix;
    qDebug() << "Satellites: "       << _Satellites;
    qDebug() << "Time Hour: "        << (int)_Hour;
    qDebug() << "Time Minutes: "     << (int)_Minute;
    qDebug() << "Time Seconds: "     << (int)_Second;
    qDebug() << "Date Month: "       << (int)_Month;
    qDebug() << "Date Day: "         << (int)_Day;
    qDebug() << "Date Year: "        << (int)_Year;
    qDebug() << "Lattitude: "        << _CoordinatesLat;
    qDebug() << "Longitude: "        << _CoordinatesLong;
    qDebug() << "Speed: "            << _Speed;
    qDebug() << "OBDII Command: "    << _OBDIICommand;
    qDebug() << "OBDII Value: "      << _OBDIIValue;
    qDebug() << "-------------------------------------------------";
}

QStringList UIDataController::getList()
{
    qDebug() << "UIDataController: Updating data list.";
    _List.clear();
    _List.append("Accelerometer X: " + QString::number(_AccelerometerX));
    _List.append("Accelerometer Y: " + QString::number(_AccelerometerY));
    _List.append("Accelerometer Z: " + QString::number(_AccelerometerZ));
    return _List;
}

void UIDataController::runSensorThread()
{
    qDebug() << "UIDataController: Running sensor thread.";
    SensorThread *sensorThread = new SensorThread(this);
    connect(sensorThread, SIGNAL(bufferReady(char*)), this, SLOT(parseData(char*)));
    connect(sensorThread, &SensorThread::finished, this, &UIDataController::deleteLater);
    sensorThread->start();
}
