#include "crashdetectionthread.h"
#include "datacontroller.h"

CrashDetectionThread::CrashDetectionThread(QObject *)
{

}

CrashDetectionThread::~CrashDetectionThread()
{

}

void CrashDetectionThread::analyzeData(const SensorData &prev_data, const SensorData &data)
{
    if (data.accelerometerX > ROLLOVER_MAX){
        qDebug() << "Looks like the vehical is rolling";
    }
    if (data.accelerometerY > ROLLOVER_MAX){
        qDebug() << "Looks like the vehical is flipping";
    }
    if (data.gyroZ < STOP_FORCE){
        qDebug() << "Looks like the vehical has colidded";
    }
    if (data.gyroX > abs(TBONE_FORCE)){
        qDebug() << "Looks like the vehical had been Tboned";
    }

    //Copy data to prev_data

}

void CrashDetectionThread::run(){
    SensorData prev_data;
    forever{
        //do stuff
        analyzeData(prev_data, DataController::getDataPacket());
    }
}

