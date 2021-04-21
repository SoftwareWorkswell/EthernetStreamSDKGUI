import QtQuick 2.0
import QtQuick.Layouts 1.12

ColumnLayout{

    Text{
        Layout.leftMargin: 5
        Layout.rightMargin: 5
        color: "white"
        text: _controllerCore.type;
        font.pixelSize: 12
    }
    RowLayout{
        Text {
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            color: "white"
            font.pixelSize: 12
            Layout.fillWidth: true
            text: qsTr("Version :") + _translator.emptyString
        }

        Text {
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            Layout.alignment: Qt.AlignRight
            color: "white"
            font.pixelSize: 12
            text: _controllerCore.firmwareVersion
        }
    }

    RowLayout{
        Text {
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            color: "white"
            font.pixelSize: 12
            Layout.fillWidth: true
            text: qsTr("SN:") + _translator.emptyString
        }

        Text {
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            Layout.alignment: Qt.AlignRight
            color: "white"
            font.pixelSize: 12
            text: _controllerCore.serialNumber
        }
    }

    RowLayout{
        Text {
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            color: "white"
            font.pixelSize: 12
            Layout.fillWidth: true
            text: qsTr("AN:") + _translator.emptyString
        }

        Text {
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            Layout.alignment: Qt.AlignRight
            color: "white"
            font.pixelSize: 12
            text: _controllerCore.articleNumber
        }
    }

    Text{
        color: "white"
        text: "Workswell s.r.o."
        font.pixelSize: 12
        Layout.alignment: Qt.AlignCenter
        Layout.leftMargin: 5
        Layout.rightMargin: 5
        Layout.topMargin: 15
    }

    Text{
        color: maPageLink.containsMouse ? "orange" : "white"
        text: "https://www.workswell.eu"
        font.pixelSize: 12
        Layout.alignment: Qt.AlignCenter
        Layout.leftMargin: 5
        Layout.rightMargin: 5
        MouseArea{
            id: maPageLink
            anchors.fill: parent
            hoverEnabled: true
            onClicked: _controllerCore.openPage()
        }
    }
}
