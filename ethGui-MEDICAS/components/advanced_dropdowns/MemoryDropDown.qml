import QtQuick 2.0
import QtQuick.Layouts 1.3

ColumnLayout{
    RowLayout{
        Text {
            Layout.topMargin: 5
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            color: "white"
            font.pixelSize: 12
            Layout.fillWidth: true
            text: qsTr("SSD: ") + _translator.emptyString
        }

        Text {
            id: ssdText
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            Layout.alignment: Qt.AlignRight
            color: "white"
            font.pixelSize: 12
            verticalAlignment: Qt.AlignRight
        }
    }

    RowLayout{
        Text {
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            color: "white"
            font.pixelSize: 12
            Layout.fillWidth: true
            text: qsTr("SD Card: ") + _translator.emptyString
        }

        Text {
            id: sdText
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            Layout.alignment: Qt.AlignRight
            color: "white"
            font.pixelSize: 12
            verticalAlignment: Qt.AlignRight
        }
    }

    RowLayout{
        Text {
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            color: "white"
            font.pixelSize: 12
            Layout.fillWidth: true
            text: qsTr("Flash: ") + _translator.emptyString
        }

        Text {
            id: flashText
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            Layout.bottomMargin: 5
            Layout.alignment: Qt.AlignRight
            color: "white"
            font.pixelSize: 12
            verticalAlignment: Qt.AlignRight
        }
    }

    Connections{
        target: _controllerCore
        function onDriveStateChanged(ssd, ssdCap, ssdFree, sd, sdCap, sdFree, flash, flashCap, flashFree){
            var gig = 1024*1024*1024;
            ssdText.text = ssd
            sdText.text = sd;
            flashText.text = flash;
            if(ssd === "READY")
            {
                ssdText.text += Math.round(ssdCap/gig) + " GB  " + Math.round(ssdFree) + "% free";
            }
            if(sd === "READY")
            {
                sdText.text += Math.round(sdCap/gig) + " GB  " + Math.round(sdFree) + "% free";
            }
            if(flash === "READY")
            {
                flashText.text += Math.round(flashCap/gig) + " GB  " + Math.round(flashFree) + "% free";
            }
        }
    }
}

