import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import "../"

Dialog {
    id: alarmErrorDialog
    title: qsTr("Range Error") + _translator.emptyString
    //parent: rangePopup
    implicitWidth: parent.width
    implicitHeight: 250
    modal: true
    background: Rectangle {
        color: "#33333D"
        opacity: 75
    }
    contentItem: ColumnLayout {
        Layout.minimumWidth: 400
            Text {
                Layout.fillWidth: true
                text:  qsTr("Selected alarm values are not allowed.\nPlease make sure that\nthe alarm values follow these rules:\n\nPreventive could not to be set\nbelow 35 °C (95 °F).\nCritical could not be set\nabove 42 °C (107.6 °F).\nPreventive has to be lower\nthan Critical.") + _translator.emptyString
                font.pixelSize: 12
                color: "white"
                font.bold: true
            }
    }
    footer: RowLayout{
        Layout.fillWidth: true
        Text {
            color: maClose.containsMouse ? "orange" : "white"
            font.bold: true
            font.pixelSize: 12
            text: qsTr("OK") + _translator.emptyString
            Layout.alignment: Qt.AlignCenter
            Layout.leftMargin: 10
            Layout.bottomMargin: 10
            MouseArea {
                id: maClose
                anchors.fill: parent
                hoverEnabled: true
                onClicked: alarmErrorDialog.close()
            }
        }
    }
}
