#include <datacontroller.h>
#include <sensorthread.h>
#include <crashdetectionthread.h>
#include "networkthread.h"

#include <QDebug>
#include <QStringListModel>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <math.h>

DataController::DataController(QObject *)
{

}

DataController::~DataController()
{

}

double DataController::strToDouble(char* str)
{
    char* endptr;
    double value = strtod(str, &endptr);
    if (*endptr) return 0;
    return value;
}

void DataController::parseData(char *buffer){

    //qDebug() << "UIDataController: Parsing data.";

    char *token = (char *)malloc(10*sizeof(char));
    token = strtok(buffer, ",");
    for (int i=0; token != NULL; i++) {
        if (i == 0) {
            _AccelerometerX = strToDouble(token);
            getRoll();
        }
        else if (i == 1) {
            _AccelerometerY = strToDouble(token);
            getPitch();
        }
        else if (i == 2) {
            _AccelerometerZ = strToDouble(token);
            getYaw();
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
            strcpy(_OBDIICommand, token);
        }
        else if (i == 18) {
            _OBDIIValue = atoi(token);
        }
        token = strtok(NULL, ",");
    }

    emit updateDiagnosticInfo();

    SensorData data = getDataPacket();
    emit sendToCrashDetection(data);

    dumpData();
}

void DataController::getRoll(){
    _Roll = (atan(_AccelerometerX/(sqrt((_AccelerometerY*_AccelerometerY) + (_AccelerometerZ*_AccelerometerZ)))) * 180) / M_PI;
}

void DataController::getPitch(){
   _Pitch = (atan(_AccelerometerY/(sqrt((_AccelerometerX*_AccelerometerX) + (_AccelerometerZ*_AccelerometerZ)))) * 180) / M_PI;
}

void DataController::getYaw(){
    _Yaw = (atan(sqrt((_AccelerometerX*_AccelerometerX) + (_AccelerometerY*_AccelerometerY))/ _AccelerometerZ) * 180) / M_PI;
}

void DataController::dumpData(){

    qDebug() << "Accelerometer X: "  << _AccelerometerX;
    qDebug() << "Roll: "             << _Roll;
    qDebug() << "Accelerometer Y: "  << _AccelerometerY;
    qDebug() << "Pitch: "            << _Pitch;
    qDebug() << "Accelerometer Z: "  << _AccelerometerZ;
    qDebug() << "Yaw: "              << _Yaw;
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

QStringList DataController::getList()
{
    //qDebug() << "UIDataController: Updating data list.";
    _DiagnosticDataList.clear();
    _DiagnosticDataList.append("Accelerometer X: " + QString::number(_AccelerometerX));
    _DiagnosticDataList.append("Accelerometer Y: " + QString::number(_AccelerometerY));
    _DiagnosticDataList.append("Accelerometer Z: " + QString::number(_AccelerometerZ));
    return _DiagnosticDataList;
}

QStringList DataController::getMessageList(){
    return _MessageList;
}

SensorData DataController::getDataPacket(){
    SensorData data;

    data.accelerometerX = _AccelerometerX;
    data.roll = _Roll;
    data.accelerometerY = _AccelerometerY;
    data.pitch = _Pitch;
    data.accelerometerZ = _AccelerometerZ;
    data.yaw = _Yaw;
    data.gyroX = _GyroX;
    data.gyroY = _GyroY;
    data.gyroZ = _GyroZ;
    data.hour = _Hour;
    data.minute = _Minute;
    data.second = _Second;
    data.day = _Day;
    data.month = _Month;
    data.year = _Year;
    data.speed = _Speed;

    return data;
}

void DataController::getMessage(const QString &msg){
    _MessageList.append(msg);
    qDebug() << "Made it to getMessage" << _MessageList;
    emit updateMessages();
}

void DataController::runSensorThread()
{
    qDebug() << "UIDataController: Running sensor thread.";
    SensorThread *sensorThread = new SensorThread(this);
    connect(sensorThread, SIGNAL(bufferReady(char*)), this, SLOT(parseData(char*)));
    sensorThread->start();
}

void DataController::runCrashDetectionThread()
{
    qDebug() << "DataController: Running crash detection thread.";
    CrashDetectionThread *crashDetectionThread = new CrashDetectionThread(this);
    connect(this, SIGNAL(sendToCrashDetection(SensorData)), crashDetectionThread, SLOT(analyzeData(SensorData)));
    crashDetectionThread->start();
}

void DataController::runNetworkThread()
{
    qDebug() << "Running Network thread.";
    NetworkThread *networkThread = new NetworkThread(this);
    connect(networkThread, SIGNAL(messageReceived(QString)), this, SLOT(getMessage(QString)));
    networkThread->start();
}
