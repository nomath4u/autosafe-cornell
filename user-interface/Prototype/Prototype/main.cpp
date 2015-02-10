#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QStringListModel>

#include "uidatacontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    UIDataController appData;
    appData.runSensorThread(); //this is working fine

    engine.rootContext()->setContextProperty("appData", &appData);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    app.exec();

    return 0;
}

