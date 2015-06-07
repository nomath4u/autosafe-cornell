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



void CrashDetectionThread::run(){
    forever {
        //run thread
    }
}
