import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import "../"

Dialog {
    id: crossUserErrorDialog
    title: qsTr("Cross User Position Error") + _translator.emptyString
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
            text: qsTr("Cross User cannot be placed\nover Black Body ROI.") + _translator.emptyString
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
                    crossUserErrorDialog.close()
                }
            }
        }
    }
}
