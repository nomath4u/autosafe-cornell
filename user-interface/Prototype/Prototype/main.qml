import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.1

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    //dark background
    Rectangle {
        color: "#212126"
        anchors.fill: parent
    }

    toolBar: BorderImage {
        border.bottom: 8
        source: "toolbar.png"
        width: parent.width
        height: 75

        Text {
            font.pixelSize: 42
            anchors.verticalCenter: parent.verticalCenter
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            color: "white"
            text: "Auto Safe"
        }
    }

    Item {
        width: parent.width
        height: parent.height

        TabView {
            id: navigationTabView
            anchors.fill: parent
            style: tabViewStyle

            Tab {
                id: diagnosticTab
                title: "Diagnostic Information"
                DiagnosticPage{ visible: true }
            }

            Tab {
                id: alertTab
                title: "Alerts"
                AlertsPage{ visible: true }
            }

            Tab {
                id: conditionTab
                title: "Road Conditions"
                ConditionsPage{ visible: true }
            }
        }

        Component {
            id: tabViewStyle
            TabViewStyle {
                tabsAlignment: Qt.AlignVCenter
                tabOverlap: 0

                tab: Item {
                    implicitWidth: control.width/control.count
                    implicitHeight: 50
                    BorderImage {
                        anchors.fill: parent
                        border.bottom: 8
                        border.top: 8
                        source: styleData.selected ? "tab_selected.png":"tabs_standard.png"
                        Text {
                            anchors.centerIn: parent
                            color: "white"
                            text: styleData.title.toUpperCase()
                            font.pixelSize: 16
                        }
                    }
                }

            }
        }
    }
}

