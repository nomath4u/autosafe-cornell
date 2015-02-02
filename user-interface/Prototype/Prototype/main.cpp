#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QObject>
#include <QThread>
#include <QString>

#include "uidata.h"
#include "sensorthread.h"
#include "networkthread.h"

int main(int argc, char *argv[])
{
    //UI Data Model
    UIData data;

    //Network & Sensor Threads
    NetworkThread network;
    SensorThread sensors;

    //connecting signals & slots ex: QObject::connect(object, SIGNAL(signal), object, SLOT(slot));

//    QObject::connect(&data, SIGNAL(getSensorData()), &sensors, SLOT(getData())); //ui -> refresh sensor data
//    QObject::connect(&sensors, SIGNAL(dataReceived(UIData)), &data, SLOT(updateUI(UIData))); //sensors -> new data, update UI
//    QObject::connect(&network, SIGNAL(messageReceived(QString)), &data, SLOT(updateConditions(QString))); //network -> new data, update UI


    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));    


    //Run threads

    app.exec(); //main GUI thread

    network.start();
    sensors.start();


    return 0;
}
