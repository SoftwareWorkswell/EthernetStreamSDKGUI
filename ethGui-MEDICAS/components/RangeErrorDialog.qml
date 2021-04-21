import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import "../"

Dialog {
    id: rangeErrorDialog
    title: "Range Error"
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
                text:  qsTr("Selected range is not allowed.\nPlease make sure that\nthe selected range follows these rules:\n\nMinimum could not to be set\nbelow 0 °C (32 °F).\nMaximum could not be set\nabove 50 °C (122 °F).\nMinimum has to be lower\nthan Maximum and their difference\nhas to be above or equal 10 °C (18 °F).") + _translator.emptyString
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
                onClicked: rangeErrorDialog.close()
            }
        }
    }
}
