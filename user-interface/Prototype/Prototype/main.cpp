#define _CRT_SECURE_NO_WARNINGS

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>

#include "networkthread.h"
#include "sensorthread.h"
#include "uidata.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));    

    app.exec(); //main GUI thread

    UIData appData;

    int count = 0;
    while (count < 10){
        appData.updateData();
        count++;
    }

    return 0;
}
