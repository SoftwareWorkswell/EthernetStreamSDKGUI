import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "../"

Rectangle{
    function toggleCaptureClick(){
        quickCaptureButton.onClicked();
    }

    function disableHighlights()
    {
        quickRangeButton.highlighted = false;
        quickAlarmsButton.highlighted = false;
    }

    property bool recordingEnabled: false
    property bool captureEnabled: false
    Rectangle{
        id: quickMenuRect
        width: controlPanel.width
        anchors.top: parent.top
        height: parent.height
        Text{
            font.bold: true
            anchors.centerIn: parent
            text: qsTr("Quick\nMenu") + _translator.emptyString
            color: "white"
        }
        gradient: Gradient {
            GradientStop { position: 0.0; color: "white" }
            GradientStop { position: 0.1; color: "#FF7604" }
            GradientStop { position: 1.0; color: "orange" }
        }
    }
    RowLayout{
        anchors.left: quickMenuRect.right
        width: parent.width - controlPanel.width
        height: parent.height
        Button{
            id: quickCaptureButton
            property bool stream: false
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            Rectangle{
                anchors.fill: parent
                color: "#16151D"
                Row{
                    height: parent.height
                    spacing: 5
                    anchors.centerIn: parent
                    Image{
                        height: 20
                        y: 10
                        source: quickCaptureText.capturing ? "/img/png/overlay_captured_active.png" : "/img/png/overlay_captured.png"
                        fillMode: Image.PreserveAspectFit
                    }
                    Text {
                        id: quickCaptureText
                        property bool capturing
                        text: qsTr("Capture") + _translator.emptyString
                        enabled: captureEnabled
                        color:  enabled ? (quickCaptureText.capturing ? "red" : (quickCaptureButton.hovered ? "orange" : "white")) : "gray"
                        font.bold: true
                        x: 5
                        font.pixelSize: 14
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            onClicked: {
                if (mainviewParent.isPeriodicCaptureChecked())
                {
                    if(mainviewParent.isPeriodicCaptureRunning())
                        mainviewParent.setPeriodicCaptureRunning(false)
                    else{
                        _controllerCore.triggerCapture();
                        mainviewParent.setPeriodicCaptureRunning(true)
                    }
                }
                else
                    _controllerCore.triggerCapture();
            }
        }
        Connections{
            target: _controllerCore
            function onCaptureRecordingChanged(capturing, recording)
            {
                quickCaptureText.capturing = capturing
            }
        }
        Rectangle{
            color: "gray"
            Layout.preferredHeight: 20
            Layout.preferredWidth: 2
            radius: 1
            visible: _controllerCore.type !== mainWindow.securityType
        }
        Button{
            id: quickRangeButton
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            visible: _controllerCore.type !== mainWindow.securityType

            Rectangle{
                anchors.fill: parent
                color: "#16151D"
                Row{
                    height: parent.height
                    spacing: 5
                    anchors.centerIn: parent
                    Image{
                        height: 20
                        y: 10
                        source: quickRangeButton.highlighted ? "/img/png/overlay_range_active.png" : "/img/png/overlay_range.png"
                        fillMode: Image.PreserveAspectFit
                    }
                    Text {
                        text: qsTr("Range") + _translator.emptyString
                        color: quickRangeButton.highlighted ? "red" : (quickRangeButton.hovered ? "orange" : "white")
                        font.bold: true
                        font.pixelSize: 14
                        x: 5
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            onClicked: {
                quickRangeButton.highlighted = !quickRangeButton.highlighted;
                mainviewParent.setManRangeSliderVisible(quickRangeButton.highlighted)
                if(quickRangeButton.highlighted)
                {
                    quickAlarmsButton.highlighted = false
                    mainviewParent.setAlarmsSliderVisible(false)
                    mainviewParent.setBlackbodyRectVisible(false)
                    _controllerCore.setCurrentRoiIdx(-1)
                }
            }
        }
        Rectangle{
            color: "gray"
            Layout.preferredHeight: 20
            Layout.preferredWidth: 2
            radius: 1
            visible: _controllerCore.type !== mainWindow.securityType
        }
        Button{
            id: quickAlarmsButton
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            visible: _controllerCore.type !== mainWindow.securityType
            Rectangle{
                anchors.fill: parent
                color: "#16151D"
                Row{
                    height: parent.height
                    spacing: 5
                    anchors.centerIn: parent
                    Image{
                        height: 20
                        y: 10
                        source: quickAlarmsButton.highlighted ? "/img/png/overlay_range_active.png" : "/img/png/overlay_range.png"
                        fillMode: Image.PreserveAspectFit
                    }
                    Text {
                        text: qsTr("Alarms") + _translator.emptyString
                        color: quickAlarmsButton.highlighted ? "red" : (quickAlarmsButton.hovered ? "orange" : "white")
                        font.bold: true
                        font.pixelSize: 14
                        x: 5
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            onClicked: {
                quickAlarmsButton.highlighted = !quickAlarmsButton.highlighted;
                mainviewParent.setAlarmsSliderVisible(quickAlarmsButton.highlighted)
                if(quickAlarmsButton.highlighted)
                {
                    mainviewParent.setManRangeSliderVisible(false)
                    mainviewParent.setBlackbodyRectVisible(false)
                    quickRangeButton.highlighted = false
                    _controllerCore.setCurrentRoiIdx(-1)
                }
            }
        }
        Rectangle{
            color: "gray"
            Layout.preferredHeight: 20
            Layout.preferredWidth: 2
            radius: 1
            visible: _controllerCore.type !== mainWindow.securityType
        }
        Button{
            id: quickShutterButton
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            Rectangle{
                anchors.fill: parent
                color: "#16151D"
                Row{
                    height: parent.height
                    spacing: 5
                    anchors.centerIn: parent
                    Image{
                        height: 20
                        y: 10

                        source: "/img/png/overlay_shutter.png"
                        fillMode: Image.PreserveAspectFit

                    }
                    Text {
                        text: qsTr("Disconnect") + _translator.emptyString
                        color: quickShutterButton.hovered ? "orange" : "white"
                        font.bold: true
                        x: 5
                        font.pixelSize: 14
                        anchors.verticalCenter: parent.verticalCenter

                    }
                }
            }
            onClicked:{
                _controllerCore.close()
                showConnectionWindow()
            }
        }
    }
}
