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

int UIDataController::openPort()
{
    int fd;

    fd = open("/dev/ttyACM0", O_RDONLY | O_NOCTTY);
    if (fd == -1)
    {
        qDebug() << "Failed to open port.";
    }

    return (fd);
}

void UIDataController::parseData(){

    qDebug() << "Made it to parseData()";

    int fd, i, n, e;

    char buffer[128];
    char *token = (char *)malloc(10*sizeof(char));

    fd = openPort();

    forever{
        n = read(fd, buffer, sizeof(buffer));
        if (n == -1)
        {
            e = errno;
            qDebug() << "Read error: " << e;
        }

        token = strtok(buffer, ",");
        for (i=0; token != NULL; i++) {
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
                //strncpy(_OBDIICommand, token, 10);
            }
            else if (i == 18) {
                //_OBDIIValue = atoi(token);
            }
            token = strtok(NULL, ",");
        }

        dumpData();
    }
}

void UIDataController::dumpData(){

    qDebug() << "Accelerometer X: "  << _AccelerometerX;
    qDebug() << "Accelerometer Y: "  << _AccelerometerY;
    qDebug() << "Accelerometer Z: "  << _AccelerometerZ;
    qDebug() << "Gyro X: "           << _GyroX;
    qDebug() << "Gyro Y: "           << _GyroY;
    qDebug() << "Gyro Z: "           << _GyroZ;
    //qDebug() << "Magentometer: "     << _Magnetometer;
    qDebug() << "Position Fix: "     << _PositionFix;
    qDebug() << "Satellites: "       << _Satellites;
    qDebug() << "Time Hour: "        << _Hour;
    qDebug() << "Time Minutes: "     << _Minute;
    qDebug() << "Time Seconds: "     << _Second;
    qDebug() << "Date Month: "       << _Month;
    qDebug() << "Date Day: "         << _Day;
    qDebug() << "Date Year: "        << _Year;
    qDebug() << "Lattitude: "        << _CoordinatesLat;
    qDebug() << "Longitude: "        << _CoordinatesLong;
    qDebug() << "Speed: "            << _Speed;
    //qDebug() << "OBDII Command: "    << _OBDIICommand;
    //qDebug() << "OBDII Value: "      << _OBDIIValue;
}


void UIDataController::updateData()
{
    qDebug() << "Updating data!";
}

QStringList UIDataController::getList()
{
    QStringList list;
    list.append("Accelerometer X: " + QString::number(_AccelerometerX));
    list.append("Accelerometer Y: " + QString::number(_AccelerometerY));
    list.append("Accelerometer Z: " + QString::number(_AccelerometerZ));
    return list;
}

void UIDataController::runSensorThread()
{
    SensorThread *sensorThread = new SensorThread(this);
    //connect read signal to parse slot

    connect(sensorThread, &SensorThread::finished, this, &UIDataController::deleteLater);
    sensorThread->start();
}

