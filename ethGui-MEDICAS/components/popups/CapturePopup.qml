import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "../"

ControlPopup{
    ColumnLayout{
        width: parent.width
        PopupItem{

            width: parent.width
            itemText: "Capture & Record"
            Layout.preferredHeight: 25
        }
        Rectangle{
            Layout.preferredWidth: 200
            Layout.preferredHeight: 50
            Layout.alignment: Qt.AlignHCenter
            color: "transparent"
            anchors.margins: 50
            PopupButton{
                anchors.left: parent.left
                width: parent.width * 0.45
                text: "Capture"
                onClicked: quickCaptureButton.onClicked();
            }
            PopupButton{
                anchors.right: parent.right
                width: parent.width * 0.45
                text: "Record"

                onClicked: {
                    if(visVidCheck.checked || radVidCheck.checked || quickRecordText.recording)
                        _controllerCore.toggleRecording();

                }
            }
        }
        PopupCheckBox{
            id: periodicCaptureCheck
            checked: false
            text: "Periodic image capture"
            onCheckedChanged: {
                if (!checked)
                    periodicCaptureTimer.stop();
            }

        }
        CustomSpinBox{
            id: periodicCaptureSpinBox
            visible: periodicCaptureCheck.checked
            from: 2
            to: 60
            value: 5
            editable: false;
            append: " seconds"
            Layout.preferredWidth: 160
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: 25

        }
        Text {
            text: "In progress..."
            color: "red"
            font.bold: true;
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: 25
            visible: periodicCaptureTimer.running
        }
    }
    Timer{
        id: periodicCaptureTimer
        repeat: true
        triggeredOnStart: false
        interval: periodicCaptureSpinBox.value * 1000
        onTriggered: {
            if(periodicCaptureCheck.checked)
                _controllerCore.triggerCapture();
        }
    }
}
