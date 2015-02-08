#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QStringListModel>

#include "uidata.h"

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    UIData appData;
    appData.updateData();

    QStringListModel list;
    list.setStringList(appData.getList());
    engine.rootContext()->setContextProperty("dataModel", &list);


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    app.exec(); //main GUI thread

    return 0;
}

