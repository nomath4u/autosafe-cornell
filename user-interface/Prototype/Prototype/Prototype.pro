TEMPLATE = app

QT += qml quick widgets serialport

SOURCES += main.cpp \
    sensorthread.cpp \
    networkthread.cpp \
    crashdetectionthread.cpp \
    datacontroller.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    sensorthread.h \
    networkthread.h \
    crashdetectionthread.h \
    datacontroller.h


LIBS += -L/usr/include/openssl -lcrypto
