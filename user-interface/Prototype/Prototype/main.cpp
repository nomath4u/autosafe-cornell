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
    appData.parseData();

    //appData.runSensorThread();

    //QStringListModel list;
    //list.setStringList(appData.getList());
    //engine.rootContext()->setContextProperty("dataModel", &list);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    app.exec();

    return 0;
}

