import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.1

Item {
    ScrollView {

        width: parent.width
        height: parent.height

        flickableItem.interactive: true

        ListView {
            id: messageListView
            anchors.fill: parent
            spacing: 40
            delegate: Item {
                Rectangle {
                    color: "#404040"
                    Text {
                        id: elementLabel
                        anchors.left: parent.left
                        text: appData.getMessageList()[index]
                        font.pointSize: 16
                        color: "white"
                    }
                }
            }

            Connections{
                target: appData
                onUpdateMessages : {
                    messageListView.model = appData.getMessageList()
                }
            }
        }

        style: ScrollViewStyle {
                transientScrollBars: true
                handle: Item {
                    implicitWidth: 14
                    implicitHeight: 26
                    Rectangle {
                        color: "#424246"
                        anchors.fill: parent
                        anchors.topMargin: 6
                        anchors.leftMargin: 4
                        anchors.rightMargin: 4
                        anchors.bottomMargin: 6
                    }
                }
                scrollBarBackground: Item {
                    implicitWidth: 14
                    implicitHeight: 26
                }
            }
    }
}
