TEMPLATE = app

QT += qml quick widgets serialport

SOURCES += main.cpp \
    parse_data.cpp \
    sensorthread.cpp \
    uidatacontroller.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    parse_data.h \
    sensorthread.h \
    uidatacontroller.h
