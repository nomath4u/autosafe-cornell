#include "networkthread.h"
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include<openssl/hmac.h>
#include<time.h>
#include <QStringList>
#include <QString>

char cryptkey[] = "cornellcup2015";

NetworkThread::NetworkThread(QObject *){

}

NetworkThread::~NetworkThread(){

}


void NetworkThread::die(char* s)
{
    perror(s);
    exit(1);
}


QString NetworkThread::crypt (unsigned char* message)
{
    unsigned char* digest;
    unsigned int md_len;

    digest = HMAC(EVP_sha1(), cryptkey, strlen(cryptkey), message, strlen((char*)message), NULL, &md_len);
    char mdString[(md_len*2)+1];
    for(int i = 0; i < md_len; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    mdString[41] = '\0';
    QString s(mdString);
    //qDebug() << "Digest:" << s;
    return s;
}

int NetworkThread::send_message(char* who, char message[BUFLEN])
{
    struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
    QString send;
    send = crypt((unsigned char*)message);
    send.replace(1,1,'v');
    send.append(":");
    send.append(message);
    //send.append(QString(":%1").arg(time(NULL)));
    qDebug() << "SEND:" << send;



    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("Socket creation failed: Send Message");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);


    if (inet_aton(who , &si_other.sin_addr) == 0)
    {
        die("inet_aton() failed\n");
    }

    //send the message
    if (sendto(s, send.toStdString().c_str(), send.length() , 0 , (struct sockaddr *) &si_other, slen)==-1)
    {
        die("sendto() failed: Send Message");
    }

    shutdown(s, SHUT_RDWR);
    return 0;
}

void NetworkThread::message_data(QString s, QStringList* l)
{
    l->append(s);
    if (l->length() > 40){
        l->removeFirst();
    }
}

bool NetworkThread::check_recv(QString s, QStringList* l){
    qDebug() << "IN CHECK " << s << *l;
    return l->contains(s);
}

bool NetworkThread::verify_hash(QString s, QString h){
    if(QString::compare(h, crypt((unsigned char*)qstrdup(s.toLocal8Bit()))) == 0){
        qDebug() << "VERIFIED";
        return true;
    } else {
        return false;
    }
}

void NetworkThread::run(){
    qDebug() << "IN NETWORK";
    QStringList* ds=new QStringList;
    QString hash, mess;
    QString Wes = "Hello this is my message";
    Wes.append(QString(":%1").arg(time(NULL)));
    send_message("10.0.0.2", (char*)qstrdup(Wes.toLocal8Bit()));
    //send_message("10.0.0.2", "Hello this is my message:14250228");



        /*

        struct sockaddr_in si_me, si_other;
        QString recv;
        int s, i, slen = sizeof(si_other) , recv_len;
        char buf[BUFLEN];

        //create a UDP socket
        if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        {
            die("Socket creation failed: Server");
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
            die("bind");
        }

        //keep listening for data
        while(1)
        {
            //for (i=0; i < BUFLEN ; i++) {
            //    buf[i] = "";
            //}
            printf("Waiting for data...");
            fflush(stdout);

            //try to receive some data, this is a blocking call
            if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, (socklen_t*)&slen)) == -1)
            {
                die("recvfrom()");
            }

            recv.clear();
            for (int i = 0; i < recv_len; ++i){
                 recv += buf[i];
            }

            //recv.fromLocal8Bit(buf);

            si_other.sin_addr.s_addr = INADDR_BROADCAST;
            si_other.sin_family = AF_INET;
            //si_other.sin_port = htons(PORT);

            //print details of the client/peer and the data received
            printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
            printf("Data: %s\n" , buf);
            recv = buf[1];

            QRegExp sep(":");
            qDebug() << "REG:" << recv.section(sep,0,0);
            hash = recv.section(sep,0,0);
            mess = recv.section(sep,1,2);
            if(verify_hash(mess, hash)){
                if ( ! check_recv(hash, ds)){
                    qDebug() << "UINQE message";
                    message_data(hash, ds);
                } else {
                    qDebug() << "NON message";
                }
            }
            qDebug() << *ds;


            // Parse Message and do stuff
            if (buf[0] == '!') {
                printf("Crash!\n");
                fflush(stdout);
                send_message("10.0.0.2", "OH NO!");
                send_message("10.0.0.3", "OH NO!");
                //exit(1);
            }

            // Send message to others
            //send_message(buf);
        }

        shutdown(s, SHUT_RDWR);
        */

}