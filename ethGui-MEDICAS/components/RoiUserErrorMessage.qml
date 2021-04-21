import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import "../"

Dialog {
    id: roiUserErrorDialog
    title: qsTr("User ROI Position Error") + _translator.emptyString
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
            text: qsTr("Error Setting User ROI Position.\nPlease make sure that the ROI\nis placed in marked area and\nROI size is at least 3x3 pixels.") + _translator.emptyString
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
                    roiUserErrorDialog.close()
                }
            }
        }
    }
}
