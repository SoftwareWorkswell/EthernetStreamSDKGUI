import QtQuick 2.0
import QtQuick.Layouts 1.12

ColumnLayout{
    Layout.alignment: Qt.AlignHCenter
    visible: infoDropdown.activated

    Text{
        color: "white"
        text: _controllerCore.type;
        font.pixelSize: 12
    }

    Text{
        color: "white"
        text: "Version : " + _controllerCore.firmwareVersion
        font.pixelSize: 12

    }

    Text{
        color: "white"
        text: "SN: " + _controllerCore.serialNumber
        font.pixelSize: 12
    }

    Text{
        color: "white"
        text: "AN: " + _controllerCore.articleNumber
        font.pixelSize: 12
    }



    Text{
        color: "white"
        text: "Workswell s.r.o.\nhttps://www.workswell.eu"
        font.pixelSize: 12
    }



}
