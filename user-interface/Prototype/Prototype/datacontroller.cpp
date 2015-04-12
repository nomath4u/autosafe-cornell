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
    char *token = (char *)malloc(100*sizeof(char));
    token = strtok(buffer, ",");
    for (int i=0; token != NULL; i++) {
        switch (i) {
            case 0:
                _AccelerometerX = strToDouble(token);
                break;
            case 1:
                _AccelerometerY = strToDouble(token);
                break;
            case 2:
                _AccelerometerZ = strToDouble(token);
                break;
           case 3:
                _GyroX = strToDouble(token);
                break;
            case 4:
                _GyroY = strToDouble(token);
                break;
            case 5:
                _GyroZ = strToDouble(token);
                break;
            case 6:
                _MagX = strToDouble(token);
                break;
            case 7:
                _MagY = strToDouble(token);
                break;
            case 8:
                _MagZ = strToDouble(token);
                break;
            case 9:
                _PositionFix = atoi(token);
                break;
            case 10:
                _Satellites = atoi(token);
                break;
            case 11:
                _Hour = (time_t)atoi(token);
                break;
            case 12:
                _Minute = (time_t)atoi(token);
                break;
            case 13:
                _Second = (time_t)atoi(token);
                break;
            case 14:
                _Month = (time_t)atoi(token);
                break;
            case 15:
                _Day = (time_t)atoi(token);
                break;
            case 16:
                _Year = (time_t)atoi(token);
                break;
            case 17:
                _CoordinatesLat = strToDouble(token);
                break;
            case 18:
                _CoordinatesLong = strToDouble(token);
                break;
            case 19:
                _Speed = atoi(token);
                break;
            case 20:
                _OBDIICommand = token;
                break;
            case 21:
                _OBDIIValue = atoi(token);
                break;
            case 22:
                _KnobTurn = atoi(token);
                break;
            case 23:
                _KnobPress = atoi(token);
                break;
        }

        token = strtok(NULL, ",");

    }

    //handle data
    emit dataAvailable();
    handleData();
}

void DataController::dumpData(){
    qDebug() << "Accelerometer X: "  << _AccelerometerX;
    qDebug() << "Accelerometer Y: "  << _AccelerometerY;
    qDebug() << "Accelerometer Z: "  << _AccelerometerZ;
    qDebug() << "Gyro X: "           << _GyroX;
    qDebug() << "Gyro Y: "           << _GyroY;
    qDebug() << "Gyro Z: "           << _GyroZ;
    qDebug() << "Mag X: "            << _MagX;
    qDebug() << "Mag Y: "            << _MagY;
    qDebug() << "Mag Z: "            << _MagZ;
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
    qDebug() << "KnobTurn: "          << _KnobTurn;
    qDebug() << "KnobPress: "          << _KnobPress;
    qDebug() << "-------------------------------------------------";
}

//do stuff with the new data
void DataController::handleData(){

    dumpData();

    //control UI if necessary 0 = nothing 1 = left 2 = right
    if (_KnobTurn == 1) {
        emit tabLeft();
    }

    else if (_KnobTurn == 2) {
        emit tabRight();
    }

    //update diagnostic info
    emit updateDiagnosticInfo();

    //send new data to crash detection
    SensorData data = getDataPacket();
    emit sendToCrashDetection(data);
}

SensorData DataController::getDataPacket(){
    SensorData data;

    data.accelerometerX = _AccelerometerX;
    data.accelerometerY = _AccelerometerY;
    data.accelerometerZ = _AccelerometerZ;
    data.gyroX = _GyroX;
    data.gyroY = _GyroY;
    data.gyroZ = _GyroZ;
    data.magX = _MagX;
    data.magY = _MagY;
    data.magZ = _MagZ;
    data.hour = _Hour;
    data.minute = _Minute;
    data.second = _Second;
    data.day = _Day;
    data.month = _Month;
    data.year = _Year;
    data.speed = _Speed;
    data.knobTurn = _KnobTurn;
    data.knobPress = _KnobPress;

    return data;
}

//List displayed on UI
QStringList DataController::getList()
{
    //qDebug() << "UIDataController: Updating data list.";
    _DiagnosticDataList.clear();
    _DiagnosticDataList.append("Accelerometer X: " + QString::number(_AccelerometerX));
    _DiagnosticDataList.append("Accelerometer Y: " + QString::number(_AccelerometerY));
    _DiagnosticDataList.append("Accelerometer Z: " + QString::number(_AccelerometerZ));
    _DiagnosticDataList.append("Gyro X: " + QString::number(_GyroX));
    _DiagnosticDataList.append("Gyro Y: " + QString::number(_GyroY));
    _DiagnosticDataList.append("Gyro Z: " + QString::number(_GyroZ));
    _DiagnosticDataList.append("Mag X: " + QString::number(_MagX));
    _DiagnosticDataList.append("Mag Y: " + QString::number(_MagY));
    _DiagnosticDataList.append("Mag Z: " + QString::number(_MagZ));

    return _DiagnosticDataList;
}

//List displayed on UI
QStringList DataController::getMessageList(){
    return _MessageList;
}

void DataController::getMessage(const QString &msg){
    _MessageList.append(msg);
    qDebug() << "Made it to getMessage" << _MessageList;
    emit updateMessages();
}

void DataController::handleLocalIncident(const QString &msg){
    _MessageList.append(msg);
    emit updateMessages();

    //confirm with driver that situation happened

    //send out over network
    emit sendMessageOverNetwork("!" + msg);
}

void DataController::handleMessageFromNetwork(const QString &msg){

}

/**** UI TEST FUNCTIONS - disregard when connected to sensors/network ****/
void DataController::handleTestCrashFromQML(){
    //qDebug() << "Test crash from QML detected!";
    emit confirmLocalIncident();
}

void DataController::handleTestNetworkMessageFromQML(){
    //qDebug() << "Test network message from QML detected!";
    emit alertDriverToIncidentAhead();
}

void DataController::handleTabLeftFromQML(){
    //qDebug() << "Tab left detected!";
    emit tabLeft();
}

void DataController::handleTabRightFromQML(){
    //qDebug() << "Tab right detected!";
    emit tabRight();
}


/************************ THREAD SETUP **************************************/
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
    connect(crashDetectionThread, SIGNAL(situationDetected(QString)), this, SLOT(handleLocalIncident(QString)));
    crashDetectionThread->start();
}

void DataController::runNetworkThread()
{
    qDebug() << "Running Network thread.";
    NetworkThread *networkThread = new NetworkThread(this);
    connect(networkThread, SIGNAL(messageReceived(QString)), this, SLOT(getMessage(QString)));
    connect(this, SIGNAL(sendMessageOverNetwork(QString)), networkThread, SLOT(sendMessage(QString)));
    networkThread->start();
}
