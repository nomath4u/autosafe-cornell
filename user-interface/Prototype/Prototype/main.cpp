#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QStringListModel>

#include "datacontroller.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    DataController appData;
    //appData.runSensorThread();
    //appData.runCrashDetectionThread();
    //appData.runNetworkThread();

    engine.rootContext()->setContextProperty("appData", &appData);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    app.exec();

    return 0;
}
