import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.1

ApplicationWindow {
    visible: true
    width: 800
    height: 600

//Auto Safe header
//    toolBar: BorderImage {
//        border.bottom: 8
//        source: "toolbar.png"
//        width: parent.width
//        height: 75

//        Text {
//            font.pixelSize: 42
//            anchors.verticalCenter: parent.verticalCenter
//            anchors.top: parent.top
//            anchors.topMargin: 10
//            anchors.left: parent.left
//            anchors.leftMargin: 10
//            color: "white"
//            text: "Auto Safe"
//        }
//    }

    Item {
        focus: true
        width: parent.width
        height: parent.height

        Keys.onPressed: {
                if (event.key === Qt.Key_Left) {
                    //console.log("move left - tab left");
                    appData.handleTabLeftFromQML();
                    event.accepted = true;
                }
                else if (event.key === Qt.Key_Right) {
                    //console.log("move right - tab right");
                    appData.handleTabRightFromQML();
                    event.accepted = true;
                }
                else if (event.key === Qt.Key_Up) {
                    //console.log("move up - testCrash");
                    appData.handleTestCrashFromQML();
                    event.accepted = true;
                }
                else if (event.key === Qt.Key_Down) {
                    //console.log("move down - testNetworkMessage");
                    appData.handleTestNetworkMessageFromQML();
                    event.accepted = true;
                }
        }

        TabView {
            id: navigationTabView
            anchors.fill: parent
            style: tabViewStyle

            Tab {
                id: vehicleInfoTab
                //title: "Vehicle Information"
                title: "Diagnostic Information"
                VehicleInfoPage { visible: true }
            }

            Tab {
                id: vehicleAlertTab
                title: "Vehicle Alerts"
                VehicleAlertPage { visible: true }
            }

            Tab {
                id: networkAlertTab
                //title: "Network Alerts"
                title: "Road Conditions"
                NetworkAlertPage { visible: true }
            }

            Connections {
                target: appData
                onTabLeft : {
                    //console.log("Made it to Tab Left in QML!");
                    if(navigationTabView.currentIndex - 1 < 0){
                        navigationTabView.currentIndex = navigationTabView.count - 1;
                    }
                    else {
                        navigationTabView.currentIndex = navigationTabView.currentIndex - 1;
                    }
                }
                onTabRight : {
                    //console.log("Made it to Tab Right in QML!");
                    if(navigationTabView.currentIndex + 1 > navigationTabView.count){
                        navigationTabView.currentIndex = 0;
                    }
                    else {
                        navigationTabView.currentIndex = navigationTabView.currentIndex + 1;
                    }
                }
            }
        }

        Component {
            id: tabViewStyle
            TabViewStyle {
                tabsAlignment: Qt.AlignVCenter
                tabOverlap: 0
                tab: Rectangle {
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
                            font.pixelSize: 18
                        }
                    }
                }
                frame: Rectangle { color: "#312F2F" }
            }

        }

        MessageDialog {
            id: messageDialog
            onAccepted: {
                //cancel network alert?
            }
        }

        Connections {
            target: appData
            onConfirmLocalIncident : {
                //console.log("Local crash detected!");

//                messageDialog.visible = true
//                messageDialog.icon = StandardIcon.Warning
//                messageDialog.title = "Crash detected!"
//                messageDialog.text = "Alerting network.\nPress knob to cancel."
            }
            onAlertDriverToIncidentAhead : {
                //console.log("Network message received, changing tab to 'Road Conditions'!");
                navigationTabView.currentIndex = 2;
                //flash title text to red 3 times??
            }
        }
    }
}
