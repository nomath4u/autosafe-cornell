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


    signals:

};

#endif // NETWORKTHREAD_H
