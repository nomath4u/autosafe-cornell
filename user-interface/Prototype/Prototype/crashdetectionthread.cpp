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
    //complementaryFilter(data);

    /* check for rollover */


    /* check for spinout */


    if ((data.accelerometerX * 10) >= ROLLOVER_MAX){
        qDebug() << "Looks like the vehicle is rolling";
    }
    qDebug() << abs((data.accelerometerY * 10));
    if (abs((data.accelerometerY * 10)) >= ROLLOVER_MAX){
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
    //prevData.append(data;
}

//void CrashDetectionThread::run(){
//    forever {
//        //run thread
//    }
//}

//hacky code for week 10 checkoffs
void CrashDetectionThread::run(){

    //reading from simulation output
    qDebug() << "in CrashDetectionThread::run()";

    SensorData data;
    char buf[128];
    int n;
    int fd;
    int lineCount;

    fd = open("/home/fs/Desktop/autosafe-cornell/user-interface/Prototype/Prototype/roll.txt", O_RDONLY);
    if(fd == -1){
        qDebug() << "failed to open file!";
    }

    n = 1;
    lineCount = 0;

    while(n > 0){
        n = read(fd, &buf, sizeof(buf));
        lineCount++;
        if(lineCount % 20 == 0){
            qDebug() << "\n<----- 2 SECONDS PASSED------>\n";
        }
        data = parseData(buf);
        dumpData(data);
        //analyze data
    }

    close(fd);
}

//hacky code for week 10 checkoffs
double CrashDetectionThread::strToDouble(char* str)
{
    char* endptr;
    double value = strtod(str, &endptr);
    if (*endptr) return 0;
    return value;
}

//hacky code for week 10 checkoffs
SensorData CrashDetectionThread::parseData(char *buffer){

    SensorData data;
    char *token = (char *)malloc(10*sizeof(char));
    token = strtok(buffer, ",");
    for (int i=0; token != NULL; i++) {
        if (i == 0) {
            data.accelerometerX = strToDouble(token);
        }
        else if (i == 1) {
            data.accelerometerY = strToDouble(token);
        }
        else if (i == 2) {
            data.accelerometerZ = strToDouble(token);
        }
        else if (i == 3) {
            data.gyroX = strToDouble(token);
        }
        else if (i == 4) {
            data.gyroY = strToDouble(token);
        }
        else if (i == 5) {
            data.gyroZ = strToDouble(token);
        }
        else if (i == 6) {
            data.fix = atoi(token);
        }
        else if (i == 7) {
            data.satellites = atoi(token);
        }
        else if (i == 8) {
            data.hour = (time_t)atoi(token);
        }
        else if (i == 9) {
            data.minute = (time_t)atoi(token);
        }
        else if (i == 10) {
            data.second = (time_t)atoi(token);
        }
        else if (i == 11) {
            data.month = (time_t)atoi(token);
        }
        else if (i == 12) {
            data.day = (time_t)atoi(token);
        }
        else if (i == 13) {
            data.year = (time_t)atoi(token);
        }
        else if (i == 14) {
            data.lattitude = strToDouble(token);
        }
        else if (i == 15) {
            data.longitude = strToDouble(token);
        }
        else if (i == 16) {
            data.speed = atoi(token);
        }
        else if (i == 17) {
            //strcpy(data.command, token);
        }
        else if (i == 18) {
            //data.value = atoi(token);
        }

        token = strtok(NULL, ",");
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
//    qDebug() << "Time Hour: "        << (int)data.hour;
//    qDebug() << "Time Minutes: "     << (int)data.minute;
//    qDebug() << "Time Seconds: "     << (int)data.second;
//    qDebug() << "Date Month: "       << (int)data.month;
//    qDebug() << "Date Day: "         << (int)data.day;
//    qDebug() << "Date Year: "        << (int)data.year;
}
