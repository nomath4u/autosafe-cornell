import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.1

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    Rectangle {
        color: "#212126"
        anchors.fill: parent
    }

    toolBar: BorderImage {
        border.bottom: 8
        source: "toolbar.png"
        width: parent.width
        height: 50

        /* need to update these with real data */

        Text {
            font.pixelSize: 24
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            color: "white"
            text: "NE"
        }

        Text {
            font.pixelSize: 24
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            color: "white"
            text: "43 out"
        }


    }

    Item {
        width: parent.width
        height: parent.height

        TabView {
            anchors.fill: parent
            style: tabStyle

            Tab {
                title: "Diagnostic Information"
            }

            Tab {
                title: "Alerts"
            }

            Tab {
                title: "Road Conditions"
            }
        }

        Component {
            id: tabStyle
            TabViewStyle {
                tabsAlignment: Qt.AlignVCenter
                tabOverlap: 0
                frame: Item { }
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
                        Rectangle {
                            visible: index > 0
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            anchors.margins: 10
                            width:1
                            color: "#3a3a3a"
                        }
                    }
                }
            }
        }
    }
}

