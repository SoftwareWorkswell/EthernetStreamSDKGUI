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
            itemText: qsTr("Main camera") + _translator.emptyString
            Layout.preferredHeight: 25
        }
        RowLayout {
            Layout.preferredHeight: 30
            Layout.alignment: Qt.AlignHCenter
            CustomRadioButton1{
                id: mainCameraThermalRadio
                text: qsTr("Thermal") + _translator.emptyString
                checked: _controllerCore.mainCamera === "THERMAL"
                onClicked:{
                    if(checked)
                        _controllerCore.mainCamera = "THERMAL"

                    rightPanel.setPauseIcon("/img/png/button-pause.png")
                    mainviewParent.setPauseIcon("/img/png/button-pause.png")
                    if(!_controllerCore.streamingThermal()) {
                        _controllerCore.toggleStreamThermal();
                    }
                    if(!_controllerCore.streamingVisible()) {
                        _controllerCore.toggleStreamVisible();
                    }
                }
            }

            CustomRadioButton1{
                id: mainCameraVisibleRadio
                text: qsTr("Visible") + _translator.emptyString
                checked: _controllerCore.mainCamera === "VISIBLE"
                onClicked:{
                    if(checked)
                        _controllerCore.mainCamera = "VISIBLE"

                    rightPanel.setPauseIcon("/img/png/button-pause.png")
                    mainviewParent.setPauseIcon("/img/png/button-pause.png")
                    if(!_controllerCore.streamingThermal()) {
                        _controllerCore.toggleStreamThermal();
                    }
                    if(!_controllerCore.streamingVisible()) {
                        _controllerCore.toggleStreamVisible();
                    }
                }
            }
        }
        PopupItem{

            width: parent.width
            itemText: qsTr("Calibration") + _translator.emptyString
            Layout.preferredHeight: 25
        }
        RowLayout {
            Layout.preferredHeight: 50
            Layout.alignment: Qt.AlignHCenter
            PopupButton{
                width: parent.width * 0.65
                height: parent.height * 0.55
                Layout.alignment: Qt.AlignCenter | Qt.AlignHCenter
                text: qsTr("Force") + _translator.emptyString
                onClicked: _controllerCore.forceCalibration()
            }
        }
        PopupItem{

            width: parent.width
            itemText: qsTr("Blackbody ROI") + _translator.emptyString
            Layout.preferredHeight: 25
        }
        PopupCheckBox{
            text: qsTr("Show Blackbody ROI") + _translator.emptyString
            checked: false
            visible: true
            enabled: true
            id: showBlackbodyCheck
            property bool setProgramatically: false
            onCheckedChanged: {
                if (!setProgramatically)
                    _controllerCore.showBlackbodyRoi = checked
            }
            Connections {
                target: _controllerCore
                function onShowBlackbodyRoiChanged(val) {
                    showBlackbodyCheck.setProgramatically = true
                    showBlackbodyCheck.checked = val
                    showBlackbodyCheck.setProgramatically = false
                }
            }
        }
        RowLayout{
            Layout.fillWidth: true
            Layout.margins: 5
            Text{
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                color: "white"
                font.pixelSize: 12
                text: qsTr("Blackbody Detection: ") + _translator.emptyString
                font.bold: true
            }
            CustomComboBox{
                Layout.alignment: Qt.AlignRight
                id: bbAreaCombo
                model: ["STANDARD", "SENSITIVE", "OFF"]
                onActivated: {
                    _controllerCore.blackbodyDetection = currentIndex
                }
            }
            Connections{
                target: _controllerCore
                function onBlackbodyDetectionChanged(val)
                {
                    bbAreaCombo.currentIndex = val
                }
            }
        }
        RowLayout{
            Layout.fillWidth: true
            Layout.margins: 5
            Text{
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                color: "white"
                font.pixelSize: 12
                text: qsTr("Black body size coefficient: ") + _translator.emptyString
                font.bold: true
            }
            Rectangle{
                Layout.preferredHeight: 15
                Layout.preferredWidth: 50
                id: bbSizeBackground
                TextInput {
                    anchors.fill: parent
                    id: bbSizeCoef
                    font.bold: true
                   // validator: RegExpValidator { regExp: /^0\.\d\d/}
                    onAccepted: {
                        _controllerCore.blackBodyRoiScale = parseInt(text)
                    }
                }
                Connections{
                    target: _controllerCore
                    function onBlackBodyRoiScaleChanged() {
                        bbSizeCoef.text =  parseInt(_controllerCore.blackBodyRoiScale)
                    }
                }
            }
        }
        RowLayout {
            Layout.preferredHeight: 50
            Layout.alignment: Qt.AlignHCenter
            PopupButton{
                width: parent.width * 0.65
                height: parent.height * 0.55
                Layout.alignment: Qt.AlignCenter
                text: qsTr("Set Position") + _translator.emptyString
                onClicked:{
                    showBlackbodyCheck.checked = true
                    mainviewParent.hideAllQuickRects()
                    controlPanel.hideAllPopups()
                    mainviewParent.setBlackbodyRectVisible(true)
                    _controllerCore.setSpecialPheripheralOutput(true)
                    _controllerCore.setCurrentRoiIdx(6)
                }
            }
        }
    }
}
