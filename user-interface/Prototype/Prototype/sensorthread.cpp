#include "sensorthread.h"

/* Sensor Thread functions */
SensorThread::SensorThread(QObject *)
{

}

void SensorThread::run() Q_DECL_OVERRIDE
{
    char data[128];
    //QByteArray data;
    int numRead;

    QSerialPort serial;
    serial.setPortName("/dev/ttyACM0");
    if(!serial.setBaudRate(serial.Baud9600) ||
       !serial.setParity(QSerialPort::NoParity) ||
       !serial.setStopBits(QSerialPort::OneStop) ||
       !serial.setDataBits(QSerialPort::Data8) ||
       !serial.setFlowControl(QSerialPort::NoFlowControl))
    {
       qDebug() << "Unable to configure serial port!";
    }

    if(serial.error() != QSerialPort::NoError)
    {
      qDebug() << "Some error occurred with the serial port!";
    }

    if (serial.open(serial.ReadOnly))
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

SensorThread::~SensorThread()
{

}
