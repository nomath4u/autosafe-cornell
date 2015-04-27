#include "crashdetectionthread.h"
#include "datacontroller.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <QFile>

CrashDetectionThread::CrashDetectionThread(QObject *)
{

}

CrashDetectionThread::~CrashDetectionThread()
{

}

void CrashDetectionThread::analyzeData(const SensorData &data)
{

    //impact detection
    if(abs(data.accelerometerX) >= 1.5 || abs(data.accelerometerY) >= 1.5){
        emit crashDetected("Vehicle has crashed!");
    }

#ifdef RCDEMO
    //roll detection
    if(data.accelerometerZ > 0.69){
        qDebug() << "FLIP!";
        emit crashDetected("Vehicle has flipped!");
    }
#endif

#ifndef RCDEMO
    //roll detection
    if(data.accelerometerZ < -0.69){
        qDebug() << "FLIP!";
        emit crashDetected("Vehicle has flipped!");
    }
#endif

    //spinning out
    if(data.gyroZ >= SPIN_MAX){
        emit crashDetected("Vehicle has spunout!");
    }
}

double CrashDetectionThread::getDirection() {
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

double CrashDetectionThread::deg2rad(double d) {
    return d * M_PI/180;
}

double CrashDetectionThread::rad2deg(double r) {
    return r * 180/M_PI;
}

int CrashDetectionThread::getRhumbLineBearing() {
    double dLon, dPhi, lon1, lat1, lon2, lat2;
    int size = prevData.size();
    lon1 = prevData.at(size - 3).longitude;
    lat1 = prevData.at(size - 3).lattitude;
    lon2 = prevData.last().longitude;
    lat2 = prevData.last().lattitude;
    //difference in longitudinal coordinates
    dLon = deg2rad(lon2) - deg2rad(lon1);
    //difference in the phi of latitudinal coordinates
    dPhi = log(tan(deg2rad(lat2) / 2 + M_PI / 4) / tan(deg2rad(lat1) / 2 + M_PI / 4));

    //we need to recalculate dLon if it is greater than pi
    if(abs(dLon) > M_PI) {
        if(dLon > 0) {
            dLon = (2 * M_PI - dLon) * -1;
        } else {
            dLon = 2 * M_PI + dLon;
        }
    }
    return (int)(rad2deg(atan2(dLon, dPhi)) + 360) % 360;
}

QString CrashDetectionThread::getCompassDir(double bearing) {
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

void CrashDetectionThread::run(){
    forever {
        //run thread
    }
}
