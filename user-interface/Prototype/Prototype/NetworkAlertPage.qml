import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.1

Item {
    width: parent.width
    height: parent.height

    ListView {
        id: messageListView
        anchors.fill: parent
        spacing: 40
        delegate: Item {
            Rectangle {
                Text {
                    id: elementLabel
                    anchors.left: parent.left
                    text: appData.getMessageList()[index]
                    font.pointSize: 16
                    color: "red"
                }
            }
        }

        Connections{
            target: appWindow
            onNetworkTabUpdate : {
                console.log("Updating messages");
                messageListView.model = appData.getMessageList()
                console.log(appData.getMessageList());

            }
        }
    }
}
