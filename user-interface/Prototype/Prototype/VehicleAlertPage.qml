import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.1

Item {

    width: parent.width
    height: parent.height

    ListView {
        id: listView
        anchors.fill: parent
        spacing: 40
        delegate: Item {
            Rectangle {
                color: "#404040"
                Text {
                    id: elementLabel
                    anchors.left: parent.left
                    text: appData.getVehicleAlertsList()[index]
                    font.pointSize: 16
                    color: "white"
                }
            }
        }

        Connections{
            target: appData
            onUpdateDiagnosticInfo : {
                listView.model = appData.getVehicleAlertsList()
            }
        }
    }
}
