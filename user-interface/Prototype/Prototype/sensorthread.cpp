#include "sensorthread.h"

/* Sensor Thread functions */
SensorThread::SensorThread(QObject *)
{

}

void SensorThread::run() Q_DECL_OVERRIDE
{
    QString result;
    qDebug() << "Sensor thread is running!";

    int numRead = 0, numReadTotal = 0;
    char buffer[128];

    QSerialPort *serial = new QSerialPort("/dev/ttyACM0");

    forever {
        numRead  = serial->read(buffer, 128);

        //do stuff with the data!
        qDebug() << "Buffer data: " << buffer;

        numReadTotal += numRead;
        if (numRead == 0 && !serial->waitForReadyRead(5000))
            break;
    }

    result = "done!";
    emit resultReady(result);
}

SensorThread::~SensorThread()
{

}
