import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.1

ScrollView {

    width: parent.width
    height: parent.height

    flickableItem.interactive: true

//    Item{
//        ListModel {
//            id: dataModel

//            ListElement {
//                label: "RPM"
//                value: "1000"
//            }

//            ListElement {
//                label: "Speed"
//                value: "10 MPH"
//            }

//        }
//    }

    ListView {
        model: dataModel
        anchors.fill: parent
        spacing: 40

        delegate: Item {
            Rectangle {
                color: "#404040"
                Text {
                    id: elementLabel
                    anchors.left: parent.left
                    text: edit
                    font.pointSize: 16
                    color: "white"
                }

//                Text {
//                    id: elementValue
//                    anchors.left: elementLabel.right
//                    anchors.leftMargin: 400 - elementLabel.width
//                    text: value
//                    font.pointSize: 16
//                    color: "white"
//                }
            }
        }
    }

//    style: ScrollViewStyle {
//            transientScrollBars: true
//            handle: Item {
//                implicitWidth: 14
//                implicitHeight: 26
//                Rectangle {
//                    color: "#424246"
//                    anchors.fill: parent
//                    anchors.topMargin: 6
//                    anchors.leftMargin: 4
//                    anchors.rightMargin: 4
//                    anchors.bottomMargin: 6
//                }
//            }
//            scrollBarBackground: Item {
//                implicitWidth: 14
//                implicitHeight: 26
//            }
//        }
}
