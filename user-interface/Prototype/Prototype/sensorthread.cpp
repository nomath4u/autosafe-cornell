#include "sensorthread.h"
//#include "parse_data.h"

/* Sensor Thread functions */
SensorThread::SensorThread(QObject *){

}

SensorThread::~SensorThread(){

}

/*
void SensorThread::run() Q_DECL_OVERRIDE //calling Sam's code
{
    //parse_data();

}
*/

void SensorThread::run() Q_DECL_OVERRIDE
{
    char data[128];
    int numRead;

    QSerialPort serial;
    serial.setPortName("/dev/ttyACM0");
    if(!serial.setBaudRate(serial.Baud9600) ||
       !serial.setParity(QSerialPort::UnknownParity) ||
       !serial.setStopBits(QSerialPort::UnknownStopBits) ||
       !serial.setDataBits(QSerialPort::UnknownDataBits) ||
       !serial.setFlowControl(QSerialPort::UnknownFlowControl))
    {
       qDebug() << "Unable to configure serial port!";
    }

    if(serial.error() != QSerialPort::NoError)
    {
      qDebug() << "Some error occurred with the serial port!";
    }

    if(serial.open(serial.ReadOnly))
    {
        qDebug() << "Port opened successfully!";

        //forever {
            numRead = serial.read(data, 128);
            if(numRead == -1)
            {
                qDebug() << "Error: reading past the end of the stream!";
            }
            else if(numRead == 0)
            {
                qDebug() << "No data received!";

            }
            else
            {
                qDebug() << "numRead = " << numRead;
            }
       //}
       //emit signal to parsing code

    }
    else
    {
        qDebug() << "Port failed to open!";
    }
}
