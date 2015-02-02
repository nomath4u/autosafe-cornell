#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H

#include <QThread>

class NetworkThread : public QThread
{
    Q_OBJECT

public:
    NetworkThread(QObject * parent = 0 );
    ~NetworkThread();

protected:
    void run();

    public slots:
         void sendMessage(); //this will be called by the crash detection code only

    signals:
         void messageReceived();
};

#endif // NETWORKTHREAD_H
