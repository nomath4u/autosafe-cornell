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

//source: http://www.pieter-jan.com/node/11
void CrashDetectionThread::complementaryFilter(SensorData data)
{
    float rollAcc = 0;
    float pitchAcc = 0;

    if(prevData.isEmpty()){
        data.roll = atan2(data.accelerometerY, data.accelerometerZ) * 180 / M_PI;
        data.pitch  = atan2(data.accelerometerX, data.accelerometerY)  * 180 / M_PI;
    }

    //complementary filter approximation - to account for drift over longer period of time...
    //take the integral of the gyro data: int(angularSpeed) = angle, combine with acceleration angle
    if(!prevData.isEmpty()){
        //angle of rotation determined from accelerometer values
        rollAcc = atan2(data.accelerometerY, data.accelerometerZ) * 180 / M_PI;
        pitchAcc = atan2(data.accelerometerX, data.accelerometerY)  * 180 / M_PI;

        //dt assumes packets sent every 100ms
        data.roll = (0.98 * (prevData.last().roll + data.gyroX * 0.001)) + (0.02 * rollAcc);
        data.pitch = (0.98 * (prevData.last().pitch + data.gyroY * 0.001)) + (0.02 * pitchAcc);
    }

    /* only save last 100 data packets */
    if(prevData.size() >= 100){
        prevData.removeFirst();
    }

    prevData.append(data);

//    qDebug() << "Roll: " << data.roll;
//    qDebug() << "Pitch: " << data.pitch;
}

void CrashDetectionThread::analyzeData(const SensorData &data) /*this is a slot called once we've parsed the data from the sensors*/
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
        emit situationDetected("Vehicle has flipped!");
    }

    /* check for spinout */
    //normal driving gyro values - x: 20 y: 15 z: 25
    //spinout test gyro values - x:20 y:20 z:50

    /* naive spinout catch... */

    //we suspect spinning when the gyro is high
    if(data.gyroZ >= SPIN_MAX){
        qDebug() << "SPINOUT likely.";
        emit situationDetected("Vehicle has spunout!");

//        int avgAccAngle;
//        if(!prevData.isEmpty()){
//            avgAccAngle = (tan(prevData[0].accelerometerX/prevData[0].accelerometerY) + tan(prevData[1].accelerometerX/prevData[1].accelerometerY) + tan(prevData[2].accelerometerX/prevData[2].accelerometerY)) / 3;
//        }

//        //if current acceleration angle is different the average --> we're turning
//        if(tan(data.accelerometerX/data.accelerometerY) != avgAccAngle){
//            qDebug() << "We think we're turning.";
//        }
//        //if current acceleration angle is the same --> we're not turning
//        else if(tan(data.accelerometerX/data.accelerometerY) == avgAccAngle){
//            qDebug() << "Not turning, you're having a bad time!";
//        }
    }

//    //save last 3 packet data
//    if(prevData.size() >= 3){
//        prevData.removeFirst();
//    }

//    prevData.append(data);

    else{
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
