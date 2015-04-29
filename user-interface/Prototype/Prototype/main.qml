import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.1

ApplicationWindow {
    id: appWindow
    title: "Auto Safe"
    visible: true
    width: 1280
    height: 720

    signal networkTabUpdate

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
                    implicitHeight: 150
                    BorderImage {
                        anchors.fill: parent
                        border.bottom: 15
                        border.top: 15
                        source: styleData.selected ? "tab_selected5.png":"tabs_standard3.png"
                        Text {
                            anchors.centerIn: parent
                            color: "white"
                            text: styleData.title.toUpperCase()

                            font.pixelSize: 28
                        }
                    }
                }
                frame: Rectangle {
                    color: "black"
                    border.color: "white"
                    border.width: 2
                }
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
                networkTabUpdate();
                //flash title text to red 3 times??
            }
        }
    }
}
