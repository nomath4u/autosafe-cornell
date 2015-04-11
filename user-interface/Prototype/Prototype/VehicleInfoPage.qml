import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.1

Item {

    width: parent.width
    height: parent.height

    /* for UI testing only! static list of values */
    Item {
       ListModel {
           id: dataModel

           ListElement {
               label: "RPM"
               value: "1000"
           }

           ListElement {
               label: "Speed"
               value: "10 MPH"
           }
           ListElement {
               label: "Stuff"
               value: "More stuff..."
           }
       }
    }

    ListView {
        id: listView
        model: dataModel
        anchors.fill: parent
        spacing: 5
        delegate: Rectangle {
                width: parent.width
                height: 40
                Text {
                    id: elementLabel
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    text: model.label + " " + model.value
                    font.pointSize: 18
            }
        }
    }

//    ListView {
//        id: listView
//        anchors.fill: parent
//        spacing: 40
//        delegate: Item {
//            Rectangle {
//                color: "#404040"
//                Text {
//                    id: elementLabel
//                    anchors.left: parent.left
//                    text: appData.getList()[index]
//                    font.pointSize: 16
//                    color: "white"
//                }
//            }
//        }

//        Connections{
//            target: appData
//            onUpdateDiagnosticInfo : { /* must have "on" and camel case signal name! - slot registers automatically */
//                listView.model = appData.getList()
//            }
//        }
//    }

}
