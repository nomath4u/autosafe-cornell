#include "sensorthread.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

SensorThread::SensorThread(QObject *){

}

SensorThread::~SensorThread(){

}

double SensorThread::strToDouble(char* str)
{
    char* endptr;
    double value = strtod(str, &endptr);
    if (*endptr) return 0;
    return value;
}

int SensorThread::openPort()
{
    int fd;

    fd = open("/dev/ttyACM0", O_RDONLY | O_NOCTTY);
    if (fd == -1)
    {
        qDebug() << "Failed to open port.";
    }

    return (fd);
}

void SensorThread::run() Q_DECL_OVERRIDE
{
    qDebug() << "Made it to parseData()";

    int fd, n, e;
    char buffer[128];

    fd = openPort();

    forever{
        n = read(fd, buffer, sizeof(buffer));
        if (n == -1)
        {
            e = errno;
            qDebug() << "Read error: " << e;
        }

        emit resultReady(buffer);
    }
}


//void SensorThread::run() Q_DECL_OVERRIDE
//{
//    char data[128];
//    int numRead;

//    QSerialPort serial;
//    serial.setPortName("/dev/ttyACM0");
//    if(!serial.setBaudRate(serial.Baud9600) || //8n1 - data parity stop
//       !serial.setParity(QSerialPort::UnknownParity) ||
//       !serial.setStopBits(QSerialPort::UnknownStopBits) ||
//       !serial.setDataBits(QSerialPort::UnknownDataBits) ||
//       !serial.setFlowControl(QSerialPort::NoFlowControl))
//    {
//       qDebug() << "Unable to configure serial port!";
//    }

//    if(serial.error() != QSerialPort::NoError)
//    {
//      qDebug() << "Some error occurred with the serial port!";
//    }

//    if(serial.open(serial.ReadOnly)) //while
//    {
//        qDebug() << "Port opened successfully!";

//        //forever {
//            numRead = serial.read(data, 128);
//            if(numRead == -1)
//            {
//                qDebug() << "Error: reading past the end of the stream!";
//            }
//            else if(numRead == 0)
//            {
//                qDebug() << "No data received!";

//            }
//            else
//            {
//                qDebug() << "numRead = " << numRead;
//            }
//       //}
//       //emit signal to parsing code

//    }
//    else
//    {
//        qDebug() << "Port failed to open!";
//    }
//}
