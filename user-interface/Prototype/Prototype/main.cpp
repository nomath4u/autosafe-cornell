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

    UIData appData;
    appData.updateData();

    app.exec(); //main GUI thread

    return 0;
}
