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
        qDebug() << "Direction" << directio
        return direction;
    }
    return -1;
}

void CrashDetectionThread::run(){
    forever {
        //run thread
    }
}

//hacky code for week 10 checkoffs -- using simulation data
//void CrashDetectionThread::run(){

//    SensorData data;

//    /* lab computer paths */
//    //QString filename = "/home/cornell/Desktop/autosafe-cornell/user-interface/Prototype/Prototype/roll.txt";
//    //QString filename = "/home/cornell/Desktop/autosafe-cornell/user-interface/Prototype/Prototype/spinout.txt";

//    /* faith's home computer paths */
//    //QString filename = "/home/fs/Desktop/autosafe-cornell/user-interface/Prototype/Prototype/roll.txt";
//    QString filename = "/home/fs/Desktop/autosafe-cornell/user-interface/Prototype/Prototype/spinout.txt";

//    int lineNum = 0;
//    QFile inputFile(filename);
//    if(inputFile.open(QIODevice::ReadOnly)){

//        QTextStream in(&inputFile);
//        while(!in.atEnd()){

//            QString line = in.readLine();
//            lineNum++;
//            //qDebug() << "Parsing line: " << lineNum;
//            data = parseData(line);
//            //dumpData(data);

//            /* naive checks for rollover */
//            if ((data.accelerometerX * 10) >= ROLLOVER_MAX){
//                qDebug() << "Looks like the vehicle is rolling";
//            }

//            if (abs((data.accelerometerY * 10)) >= ROLLOVER_MAX){
//                qDebug() << "Looks like the vehicle is flipping";
//            }

//            if(data.accelerometerZ*10 <= -8 && data.accelerometerZ*10 >= -10){
//                qDebug() << "ROLLOVER LIKELY.";
//                emit situationDetected("Vehicle has flipped!");
//            }

//            //max normal driving gyro values - x: 20 y: 15 z: 25
//            //max spinout simulation gyro values - x:20 y:20 z:50

//            /* naive spinout catch... */

//            //we suspect spinning when the gyro is high
//            if(abs(data.gyroZ) >= SPIN_MAX){
//                qDebug() << "SPINOUT SUSPECTED.";

//                /* smarter algorithm... not working... */
//                int avgAccAngle;
//                if(prevData.size() == 3){
//                    avgAccAngle = (tan(prevData[0].accelerometerX/prevData[0].accelerometerY) + tan(prevData[1].accelerometerX/prevData[1].accelerometerY) + tan(prevData[2].accelerometerX/prevData[2].accelerometerY)) / 3;

//                    //if current acceleration angle is different the average --> we're turning
//                    if(tan(data.accelerometerX/data.accelerometerY) != avgAccAngle){
//                        qDebug() << "We're probably just turning... false positive.";
//                    }

//                    //if current acceleration angle is the same --> we're not turning
//                    if(tan(data.accelerometerX/data.accelerometerY) == avgAccAngle){
//                        qDebug() << "Spinout... you're having a bad time!";
//                    }

//                }

//                //save last 3 packet data
//                if(prevData.size() >= 3){
//                    prevData.removeFirst();
//                }

//                prevData.append(data);
//            }
//        }

//        inputFile.close();
//    }
//}

//hacky code for week 10 checkoffs
SensorData CrashDetectionThread::parseData(QString line){

    SensorData data;
    QStringList splitLine = line.split(",");
    //qDebug() << "line: " << splitLine;
    //qDebug() << splitLine.size();
    for(int i = 0; i < splitLine.size(); ++i){
        //qDebug() << "item " << i << " : " << splitLine[i];

        if (i == 0) {
            data.accelerometerX = splitLine[i].toDouble();
        }
        else if (i == 1) {
            data.accelerometerY = splitLine[i].toDouble();
        }
        else if (i == 2) {
            data.accelerometerZ = splitLine[i].toDouble();
        }
        else if (i == 3) {
            data.gyroX = splitLine[i].toDouble();
        }
        else if (i == 4) {
            data.gyroY = splitLine[i].toDouble();
        }
        else if (i == 5) {
            data.gyroZ = splitLine[i].toDouble();
        }

//        else if (i == 6){
//            data.magX = splitLine[i].toDouble();
//        }
//        else if (i == 7) {
//            data.magY = splitLine[i].toDouble();
//        }
//        else if (i == 8) {
//            data.magZ = splitLine[i].toDouble();
//        }

        else{
            break;
        }
    }

    return data;
}

//hacky code for week 10 checkoffs
void CrashDetectionThread::dumpData(SensorData data){

    qDebug() << "Accelerometer X: "  << data.accelerometerX;
    qDebug() << "Accelerometer Y: "  << data.accelerometerY;
    qDebug() << "Accelerometer Z: "  << data.accelerometerZ;
    qDebug() << "Gyro X: "           << data.gyroX;
    qDebug() << "Gyro Y: "           << data.gyroY;
    qDebug() << "Gyro Z: "           << data.gyroZ;
//    qDebug() << "Mag X: "            << data.magX;
//    qDebug() << "Mag Y: "            << data.magY;
//    qDebug() << "Mag Z: "            << data.magZ;

}
