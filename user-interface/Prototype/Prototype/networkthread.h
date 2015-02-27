#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QThread>

#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data


class NetworkThread : public QThread
{
    Q_OBJECT
    void run();

public:
    NetworkThread(QObject *parent = 0);
    ~NetworkThread();

    void die(char *s);
    QString crypt (unsigned char* message);
    int send_message(char* who, char message[BUFLEN]);


signals:
    void bufferReady(char* buffer);
};

#endif // NETWORKTHREAD_H
