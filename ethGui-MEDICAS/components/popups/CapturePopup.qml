import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "../"

ControlPopup{
    id: root
    signal captureToggled()
    signal recordingToggled()

    function isPeriodicCaptureChecked() {
        return periodicCaptureCheck.checked
    }

    function isPeriodicCaptureRunning() {
        return periodicCaptureTimer.running
    }

    function setPeriodicCaptureRunning(running)
    {
        if(running)
            periodicCaptureTimer.start()
        else
            periodicCaptureTimer.stop()
    }

    ColumnLayout{
        width: parent.width
        PopupItem{

            width: parent.width
            itemText: qsTr("Capture & Record") + _translator.emptyString
            Layout.preferredHeight: 25
        }
        Rectangle{
            Layout.preferredWidth: 200
            Layout.preferredHeight: 50
            Layout.alignment: Qt.AlignHCenter
            color: "transparent"
            anchors.margins: 50
            PopupButton{
                Layout.alignment: Qt.AlignHCenter
                width: parent.width * 0.45
                text: qsTr("Capture") + _translator.emptyString
                onClicked: root.captureToggled()
                enabled: quickMenuPanel.captureEnabled
            }
        }
        PopupCheckBox{
            id: periodicCaptureCheck
            checked: false
            text: qsTr("Periodic image capture") + _translator.emptyString
            enabled: quickMenuPanel.captureEnabled
            onCheckedChanged:{
                periodicCaptureTimer.restart()
                periodicCaptureTimer.tick = 0
                countdownText.text =  qsTr("Next trigger in ") + _translator.emptyString + (periodicCaptureSpinBox.value - tick)
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
            id: countdownText
            text: qsTr("Next trigger in ") + _translator.emptyString + (periodicCaptureSpinBox.value - periodicCaptureTimer.tick)
            color: "white"
            font.pointSize: 10
            font.bold: true;
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: 25
            visible: periodicCaptureTimer.running
        }
    }
    Timer{
        id: periodicCaptureTimer
        repeat: true
        running: periodicCaptureCheck.checked
        triggeredOnStart: false
        interval: 1000
        property var tick: 0
        onTriggered: {
            tick += 1
            countdownText.text =  "Next trigger in " + (periodicCaptureSpinBox.value - tick)
            if(tick == periodicCaptureSpinBox.value)
            {
                _controllerCore.triggerCapture();
                tick = 0;
                countdownText.text =  "Next trigger in " + (periodicCaptureSpinBox.value - tick)
            }
        }
    }
}
