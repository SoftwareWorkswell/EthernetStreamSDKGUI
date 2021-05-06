import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "../"

ControlPopup{
    id: capturePopup
    width: parent.width
    function periodicCaptureChecked()
    {
        return periodicCaptureCheck.checked
    }
    function isPeriodicCaptureRunning()
    {
        return periodicCaptureTimer.running
    }
    function setPeriodicCaptureRunning(val)
    {
        if(val)
           periodicCaptureTimer.start();
        else
            periodicCaptureTimer.stop();
    }


    ColumnLayout{
        width: parent.width

        PopupItem{

            width: parent.width
            itemText: "Capture & Record"
            Layout.preferredHeight: 25
        }
        Text{
            Layout.margins: 10
            text: "Set saved image and sequence\nformats in Advanced settings"
            color: "white"
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
                onClicked:{
                    quickCaptureButton.onClicked();
                }
                enabled: {
                    var res
                    if(_controllerCore.type !== mainWindow.securityType)
                        res = quickCaptureText.capturing ? false : ((advancedPopup.imageCaptureEnabled()) ? true  : false)
                    else
                        res = quickCaptureText.capturing ? false : ((advancedPopup.imagaCaptureSecEnabled()) ? true  : false)
                    return res;
                }
            }

            PopupButton{
                anchors.right: parent.right
                width: parent.width * 0.45
                text: "Record"
                enabled: advancedPopup.visVidChecked() || advancedPopup.radVidChecked() || quickRecordText.recording
                onClicked: {
                    if(advancedPopup.visVidChecked() || advancedPopup.radVidChecked() || quickRecordText.recording)
                        _controllerCore.toggleRecording();

                }

            }

        }
        PopupCheckBox{
            id: periodicCaptureCheck
            checked: false
            text: "Periodic image capture"
            enabled: {
                var res
                if(_controllerCore.type !== mainWindow.securityType)
                    res = quickCaptureText.capturing ? false : ((advancedPopup.imageCaptureEnabled()) ? true  : false)
                else
                    res = quickCaptureText.capturing ? false : ((advancedPopup.imagaCaptureSecEnabled()) ? true  : false)
                return res;
            }
            onCheckedChanged: {
                if (!checked)
                    periodicCaptureTimer.stop();
            }

        }

        CustomSpinBox{
            id: periodicCaptureSpinBox
            visible: periodicCaptureCheck.checked
            from: _controllerCore.type == mainWindow.gisType ? 1 : 2
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

