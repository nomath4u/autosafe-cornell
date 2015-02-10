#include "sensorthread.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

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
    struct termios oldtio, newtio;

    fd = open("/dev/ttyACM0", O_RDONLY | O_NOCTTY);
    if (fd == -1)
    {
        qDebug() << "Failed to open port.";
    }

    tcgetattr(fd,&oldtio); /* save current serial port settings */
    bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR | ICRNL;
    newtio.c_oflag = 0;
    newtio.c_lflag = ICANON;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);

    return (fd);
}

void SensorThread::run(){

    int fd, n, e;
    char buffer[128];

    fd = openPort();


    forever{
        n = read(fd, &buffer, sizeof(buffer));
        if (n == -1)
        {
            e = errno;
            qDebug() << "Read error: " << e;
        }

        emit bufferReady(buffer);
    }
}
