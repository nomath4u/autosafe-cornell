#include "crashdetectionthread.h"
#include "datacontroller.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>

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
    float rotationX = 0;
    float rotationY = 0;

    //angle of rotation determined from accelerometer values
    rollAcc = atan2(data.accelerometerY, data.accelerometerZ) * 180 / M_PI;
    pitchAcc = atan2(data.accelerometerX, data.accelerometerY)  * 180 / M_PI;

    //complementary filter approximation - to account for drift over longer period of time...
    //this should be outputting the angle of rotation about x and y in degrees...

    //take the integral of the gyro data: int(angularSpeed) = angle, combine with acceleration angle
    //dt assumes packets are sent every 100ms --is this accurate?
    //131 is our gyro sensitivit?? need to confirm this...
    rotationX = 0.98 * (data.gyroX / 131) * 0.001 + (0.02 * rollAcc);
    rotationY = 0.98 * (data.gyroY / 131) * 0.001 + (0.02 * pitchAcc);

    qDebug() << "Rotation x: " << rotationX;
    qDebug() << "Rotation y: " << rotationY;
}

void CrashDetectionThread::analyzeData(const SensorData &data) /*this is a slot called once we've parsed the data from the sensors*/
{
    complementaryFilter(data);

    /* check for rollover */


    /* check for spinout */


//    if (data.roll > ROLLOVER_MAX){
//        qDebug() << "Looks like the vehicle is rolling";
//    }
//    if (data.pitch > ROLLOVER_MAX){
//        qDebug() << "Looks like the vehicle is flipping";
//    }

//    if (data.gyroZ < STOP_FORCE){
//        qDebug() << "Looks like the vehical has colidded";
//    }
//    if (data.gyroX > abs(TBONE_FORCE)){
//        qDebug() << "Looks like the vehical had been Tboned";
//    }

    //do some comparisons to the previous data?

    //save the data
    prevData = data;
}

void CrashDetectionThread::run(){
    forever {
        //run thread
    }
}
