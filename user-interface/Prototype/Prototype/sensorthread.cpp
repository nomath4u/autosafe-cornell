#include "sensorthread.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#ifdef DEMO

#include<arpa/inet.h>
#include<sys/socket.h>

#define SERVER "10.0.0.1"
#define BUFLEN 512  //Max length of buffer
#define PORT 8989   //The port on which to send data

#endif

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
    char buffer[152];

#ifdef DEMO
    qDebug() << "DEMO!";

    struct sockaddr_in si_me, si_other;

    int s, i, slen = sizeof(si_other) , recv_len;
    int bcast = 1;
    char buf[BUFLEN];

    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        qDebug() << "Socket";
    }

    if ((setsockopt(s, SOL_SOCKET, SO_BROADCAST, &bcast, sizeof(bcast))) == -1)
    {
        qDebug() << "setopts";
    }

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = INADDR_ANY;
    //si_me.sin_addr.s_addr = INADDR_BROADCAST;

    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        qDebug() << "bind";
    }

    //keep listening for data
    while(1)
    {

        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, (socklen_t *) &slen)) == -1)
        {
            qDebug() << "recvfrom()";
        }

        //printf("Data: %s\n" , buf);

        emit bufferReady(buf);

    }

    close(s);
#else

    fd = openPort();

    forever{
        n = read(fd, &buffer, sizeof(buffer));
        if (n == -1)
        {
            e = errno;
            qDebug() << "Read error: " << e;
        }

        //qDebug() << "buffer contents: " << buffer;
        emit bufferReady(buffer);
    }
#endif
}
