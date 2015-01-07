QT += qml quick
TARGET = AutoSafeUI
!no_desktop: QT += widgets

include(src/src.pri)
include(../shared/shared.pri)

OTHER_FILES += \
    main.qml

RESOURCES += \
    resources.qrc

DISTFILES += \
    content/DiagnosticInformation.qml \
    content/Alerts.qml \
    content/RoadConditions.qml
