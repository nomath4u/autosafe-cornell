#include "crashdetectionthread.h"
#include "datacontroller.h"

CrashDetectionThread::CrashDetectionThread(QObject *)
{

}

CrashDetectionThread::~CrashDetectionThread()
{

}

void CrashDetectionThread::analyzeData(const SensorData &data) /*this is a slot called once we've parsed the data from the sensors*/
{
    if (data.roll > ROLLOVER_MAX){
        qDebug() << "Looks like the vehicle is rolling";
    }
    if (data.pitch > ROLLOVER_MAX){
        qDebug() << "Looks like the vehicle is flipping";
    }

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
    forever{
        //this just runs the thread...
    }
}
