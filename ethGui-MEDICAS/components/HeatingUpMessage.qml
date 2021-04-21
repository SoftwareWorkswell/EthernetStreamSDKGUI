import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import "../"

Dialog {
    id: heatingUpDialog
    title: qsTr("Heating Up") + _translator.emptyString
    parent: measurePopup
    implicitWidth: parent.width
    implicitHeight: 150
    modal: true
    background: Rectangle {
        color: "#33333D"
        opacity: 75
    }
    contentItem: ColumnLayout {
        Layout.minimumWidth: 400
        Text {
            Layout.fillWidth: true
            text: qsTr("Camera is Currently Heating Up.\nPlease wait up to 15 minutes\nor cancel heating up\nfrom the camera firmware.") + _translator.emptyString
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
                onClicked: {
                    mainviewParent.hideAllQuickRects()
                    heatingUpDialog.close()
                }
            }
        }
    }
}
