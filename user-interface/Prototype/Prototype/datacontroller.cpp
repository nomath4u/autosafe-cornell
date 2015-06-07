#include <datacontroller.h>
#include <sensorthread.h>
#include <crashdetectionthread.h>
#include "networkthread.h"

#include <QDebug>
#include <QStringListModel>
#include <iostream>
#include <fstream>
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
    _AccelerometerX                          = 0;
    _AccelerometerY                          = 0;
    _AccelerometerZ                          = 0;
    _GyroX                                   = 0;
    _GyroY                                   = 0;
    _GyroZ                                   = 0;
    _MagX                                    = 0;
    _MagY                                    = 0;
    _MagZ                                    = 0;
    _PositionFix                             = 0;
    _Satellites                              = 0;
    _Hour                                    = 0;
    _Minute                                  = 0;
    _Second                                  = 0;
    _Month                                   = 0;
    _Day                                     = 0;
    _Year                                    = 0;
    _CoordinatesLat                          = 0;
    _CoordinatesLong                         = 0;
    _Speed                                   = 0;
    _OBDIICommand                            = "";
    _OBDIIValue                              = 0;
    _KnobTurn                                = 0;
    _KnobPress                               = 0;

    _OBDIITRBL = 0;
    _OBDIIRPMS = 0;
    _OBDIISpeed = 0;
    _OBDIIFuelPressure = 0;
    _OBDIIOilTemp = 0;
    _OBDIIBattery = 0;
    _OBDIIFuelInput = 0;
    _OBDIIBoost = 0;
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
                _Speed = _Speed * 2.23694;
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

    if (_OBDIICommand == "TRBL"){
        _OBDIITRBL = _OBDIIValue;
    }
    else if (_OBDIICommand == "RPM") {
        _OBDIIRPMS = _OBDIIValue;
    }
    else if (_OBDIICommand == "Speed") {
        _OBDIISpeed = _OBDIIValue * 0.621371; //convert to miles per hour
    }
    else if (_OBDIICommand == "Fuel Pressure") {
        _OBDIIFuelPressure = _OBDIIValue;
    }
    else if(_OBDIICommand == "Oil Temp") {
        _OBDIIOilTemp = _OBDIIValue;
    }
    else if (_OBDIICommand == "Battery") {
        _OBDIIBattery = _OBDIIValue;
    }
    else if (_OBDIICommand == "Fuel Input") {
        _OBDIIFuelInput = _OBDIIValue;
    }
    else if ("Boost") {
        _OBDIIBoost = _OBDIIValue;
    }

    //emit dataAvailable();
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

    /* FOR TESTING */
/*
    FILE *f = fopen("/home/fs/Desktop/autosafe-cornell/user-interface/Prototype/Prototype/outFile.txt", "a");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(f, "%f,%f,%f,%f,%f,%f,%f,%f,%f,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n",_AccelerometerX,_AccelerometerY,_AccelerometerZ, _GyroX, _GyroY, _GyroZ, _MagX, _MagY, _MagZ);
    fclose(f);
*/
}

double DataController::getDirection() {
    SensorData temp;
    int avg_magx, avg_magy;
    int size = prevData.size();
    double direction;
    if(prevData.isEmpty()){
        qDebug() << "getDirection:Previous data list is empty.";
    } else {
        for(int i=0; i < size; ++i) {
            temp = prevData.at(i);
            avg_magx += temp.magX;
            avg_magy += temp.magY;
        }
        avg_magx = avg_magx/size;
        avg_magy = avg_magy/size;
        direction = tan(avg_magy)/tan(avg_magx);
        qDebug() << "Direction" << direction;
        return direction;
    }
    return -1;
}

double DataController::deg2rad(double d) {
    return d * M_PI/180;
}

double DataController::rad2deg(double r) {
    return r * 180/M_PI;
}

int DataController::getRhumbLineBearing() {
    double dLon, dPhi, lon1, lat1, lon2, lat2;
    int size = prevData.size();
    if (size > 50 && _PositionFix == 1) {
    lon1 = prevData.at(size - 50).longitude;
    lat1 = prevData.at(size - 50).lattitude;
    lon2 = prevData.last().longitude;
    lat2 = prevData.last().lattitude;
    //difference in longitudinal coordinates
    //dLon = deg2rad(lon2) - deg2rad(lon1);
    dLon = (sin(deg2rad(lon2)-deg2rad(lon1) * cos(deg2rad(lat2))));
    //difference in the phi of latitudinal coordinates
    //dPhi = log(tan(deg2rad(lat2) / 2 + M_PI / 4) / tan(deg2rad(lat1) / 2 + M_PI / 4));
    dPhi = (cos(deg2rad(lat1)) * sin(deg2rad(lat2)) - sin(deg2rad(lat1)) * cos(deg2rad(lat2)) - cos(deg2rad(lon2)-deg2rad(lon1)));


    //we need to recalculate dLon if it is greater than pi
    if(abs(dLon) > M_PI) {
        if(dLon > 0) {
            dLon = (2 * M_PI - dLon) * -1;
        } else {
            dLon = 2 * M_PI + dLon;
        }
    }
    return (int)(rad2deg(atan2(dLon, dPhi)));
    }
    return 0;
}

QString DataController::getCompassDir(int bearing) {
    int tmp = bearing / 22.5;
    QString dir;
    switch (tmp) {
    case 1:
        dir = "NNE";
        break;
    case 2:
         dir = "NE";
         break;
    case 3:
         dir = "ENE";
         break;
    case 4:
         dir = "E";
         break;
    case 5:
         dir = "ESE";
         break;
    case 6:
         dir = "SE";
         break;
    case 7:
         dir = "SSE";
         break;
    case 8:
         dir = "S";
         break;
    case 9:
         dir = "SSW";
         break;
    case 10:
         dir = "SW";
         break;
    case 11:
         dir = "WSW";
         break;
    case 12:
         dir = "W";
         break;
    case 13:
         dir = "WNW";
         break;
    case 14:
         dir = "NW";
         break;
    case 15:
         dir = "NNW";
         break;
    default:
        dir = "N";
        break;
    }
    return dir;
}

void DataController::handleData(){

    //dumpData();

    int bearing = getRhumbLineBearing();
    //qDebug() << "Bearing:" << bearing;
    //qDebug() << "Lon:" << _CoordinatesLong << "Lat:" << _CoordinatesLat;
    //qDebug() << "Direction:" << getCompassDir(bearing);

    //control UI if necessary 0 = nothing 1 = left 2 = right
    if (_KnobTurn == 1) {
        emit tabLeft();
    }

    else if (_KnobTurn == 2) {
        emit tabRight();
    }

    emit updateDiagnosticInfo();

    SensorData data = getDataPacket();
    emit sendToCrashDetection(data);
}

//function used to send necessary data to crash detection
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

    prevData.append(data);
    if (prevData.size() > 500) {
        prevData.removeFirst();
    }
    return data;
}

//vehicle info displayed on UI
QStringList DataController::getVehicleInfoList()
{
    _DiagnosticDataList.clear();
    //_DiagnosticDataList.append("Date: " + QString::number(_Month) + "/" + QString::number(_Day) + "/" + QString::number(_Year));
    //_DiagnosticDataList.append("Time: " + QString::number(_Hour) + ":" + QString::number(_Minute) + ":" + QString::number(_Second));
    //_DiagnosticDataList.append("Accelerometer - X: " + QString::number(_AccelerometerX) + " Y: " + QString::number(_AccelerometerY) + " Z: " + QString::number(_AccelerometerZ));
    //_DiagnosticDataList.append("Gyroscope - X: " + QString::number(_GyroX) + " Y: " + QString::number(_GyroY) + " Z: " + QString::number(_GyroZ));
    //_DiagnosticDataList.append("Magnetometer - X: " + QString::number(_MagX) + " Y: " + QString::number(_MagY) + "Z: " + QString::number(_MagZ));

    _DiagnosticDataList.append("Speed: " + QString::number(int(_Speed)) + " MPH");
    _DiagnosticDataList.append("RPM: " + QString::number(_OBDIIRPMS));
//    _DiagnosticDataList.append("Speed (ODBII): " + QString::number(_OBDIISpeed) + " km/h");
//    _DiagnosticDataList.append("Fuel Pressure: " + QString::number(_OBDIIFuelPressure) + " kPa");
//    _DiagnosticDataList.append("Oil Temperature: " + QString::number(_OBDIIOilTemp) + " degrees celsius");
//    _DiagnosticDataList.append("Hybrid Battery: " + QString::number(_OBDIIBattery) + " % remaining");
//    _DiagnosticDataList.append("Fuel: " + QString::number(_OBDIIFuelInput) + " % remaining");
//    _DiagnosticDataList.append("Boost: " + QString::number(_OBDIIBoost));

    _DiagnosticDataList.append("Fuel Pressure: 15 PSI");
    _DiagnosticDataList.append("Oil Temperature: 180 F");
    _DiagnosticDataList.append("Hybrid Battery: N/A");
    _DiagnosticDataList.append("Fuel: 88 % remaining");
    _DiagnosticDataList.append("Boost: 10 PSI");
    _DiagnosticDataList.append("GPS Position Fix: " + QString::number(_PositionFix));
    _DiagnosticDataList.append("GPS Lat: " + QString::number(_CoordinatesLat) + " GPS Long: " + QString::number(_CoordinatesLong));

    return _DiagnosticDataList;
}

//OBDII codes displayed on UI
QStringList DataController::getVehicleAlertsList(){

    _VehicleAlertList.clear();

    if (_OBDIITRBL != 0) {
        _VehicleAlertList.append("Trouble codes received:\n");
        _VehicleAlertList.append(QString::number(_OBDIITRBL, 16));
    }

    else {
        _VehicleAlertList.append("No trouble codes, presently.");
    }

    return _VehicleAlertList;
}

//Network messages displayed on UI
QStringList DataController::getMessageList(){
    return _MessageList;
}

void DataController::getMessage(const QString &msg){
    qDebug() << "getting message, appending to list for UI" << msg;
    QRegExp sep(":");
    qDebug() << "dir:" << msg.section(sep,0,0);
    int dir = msg.section(sep,0,0).toInt();
    if (dir > getRhumbLineBearing() - 90 && dir < getRhumbLineBearing() + 90){
        qDebug() << "Same Direction" << dir;
    }
    //msg.append(" To the ");
    //msg.append(getCompassDir(dir));
    _MessageList.clear();
    _MessageList.append(msg.section(sep,1,2) + " To the " + getCompassDir(dir));
    qDebug() << _MessageList;
    emit updateMessages();
    emit alertDriverToIncidentAhead();
}

void DataController::handleCrash(const QString &msg){
    //allow user to cancel crash

    emit sendMessage("!" + msg, NEW_MSG);
}


/**** UI TEST FUNCTIONS - disregard when connected to sensors/network ****/

void DataController::handleTestCrashFromQML(){
    emit sendMessage("!Crash detected!", NEW_MSG);
}

void DataController::handleTestNetworkMessageFromQML(){
    emit alertDriverToIncidentAhead();
}

void DataController::handleTabLeftFromQML(){
    emit tabLeft();
}

void DataController::handleTabRightFromQML(){
    emit tabRight();
}


/************************ THREAD SETUP **************************************/

void DataController::runSensorThread()
{
    //qDebug() << "UIDataController: Running sensor thread.";
    SensorThread *sensorThread = new SensorThread(this);
    connect(sensorThread, SIGNAL(bufferReady(char*)), this, SLOT(parseData(char*)));
    sensorThread->start();
}

void DataController::runCrashDetectionThread()
{
    //qDebug() << "DataController: Running crash detection thread.";
    CrashDetectionThread *crashDetectionThread = new CrashDetectionThread(this);
    connect(this, SIGNAL(sendToCrashDetection(SensorData)), crashDetectionThread, SLOT(analyzeData(SensorData)));
    connect(crashDetectionThread, SIGNAL(crashDetected(QString)), this, SLOT(handleCrash(QString)));
    crashDetectionThread->start();
}

void DataController::runNetworkThread()
{
    //qDebug() << "Running Network thread.";
    NetworkThread *networkThread = new NetworkThread(this);
    connect(networkThread, SIGNAL(messageReceived(QString)), this, SLOT(getMessage(QString)));
    connect(this, SIGNAL(sendMessage(QString, int)), networkThread, SLOT(sendMessage(QString, int)));
    networkThread->start();
}
