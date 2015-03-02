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

    void die(const char *s);
    QString crypt (unsigned char* message);
    int send_message(const char *who, QString send);
    void message_data(QString s, QStringList* l);
    bool check_recv(QString s, QStringList* l);
    bool verify_hash(QString s, QString h);


signals:
    void bufferReady(char* buffer);
};

#endif // NETWORKTHREAD_H
