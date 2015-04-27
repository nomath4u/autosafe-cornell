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
    /* naive checks for rollover */
    if ((data.accelerometerX * 10) >= ROLLOVER_MAX){
        qDebug() << "Looks like the vehicle is rolling";
    }

    if (abs((data.accelerometerY * 10)) >= ROLLOVER_MAX){
        qDebug() << "Looks like the vehicle is flipping";
    }

    if(data.accelerometerZ*10 <= -8 && data.accelerometerZ*10 >= -10){
        qDebug() << "FLIP!";
        emit crashDetected("Vehicle has flipped!");
    }

    //we suspect spinning when the gyro is high
    if(data.gyroZ >= SPIN_MAX){
        qDebug() << "SPINOUT likely.";
        emit crashDetected("Vehicle has spunout!");
    } else{
        qDebug() << "normal driving";
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

void CrashDetectionThread::run(){
    forever {
        //run thread
    }
}
