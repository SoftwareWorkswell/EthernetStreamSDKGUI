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

        PopupDropDown{
            id: visibleDropDown
            dropdownText: "Visible Camera"
        }
        ColumnLayout{
            visible: visibleDropDown.activated
            RowLayout{
                Layout.margins: 5
                Layout.fillWidth: true
                Text{
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: true
                    color: "white"
                    font.pixelSize: 12
                    text: "Gamma: "
                    font.bold: true
                }
                CustomComboBox{
                    Layout.alignment: Qt.AlignCenter
                    id: gammaCombo
                    model: ["STANDARD", "STRAIGHT", "NARROW", "WIDE"]
                    onCurrentTextChanged: {
                        _controllerCore.gamma = currentIndex
                    }
                }
                Connections{
                    target: _controllerCore
                    onGammaChanged: {
                        gammaCombo.currentIndex = val
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
                    text: "White Balance: "
                    font.bold: true
                }
                CustomComboBox{
                    Layout.alignment: Qt.AlignRight
                    id: wBaCombo
                    model: ["NARROW", "INDOOR", "OUTDOOR", "WIDE", "ONE_PUSH_TRIGGER"]
                    onCurrentTextChanged: {
                        _controllerCore.whiteBalance = currentIndex
                    }
                }
                Connections{
                    target: _controllerCore
                    onWhiteBalanceChanged:
                    {
                        wBaCombo.currentIndex = val
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
                    text: "Iris: "
                    font.bold: true
                }
                CustomComboBox{
                    Layout.alignment: Qt.AlignRight
                    id: irisCombo
                    model: [ "F22", "F16", "F14", "F11", "F9.6", "F8", "F6.8", "F5.6", "F4.8", "F4", "F3.4", "F2.8","F2.4",  "F2", "F1.8" ]
                    onCurrentTextChanged: {
                        // iris modes start idexing at 0x03 - check customtools.h
                        _controllerCore.iris = (currentIndex)
                    }
                }
                Connections{
                    target: _controllerCore
                    onIrisChanged:
                        irisCombo.currentIndex = (val)
                }
            }
            PopupCheckBox{
                id: backlightCompensationCheck
                checked: false
                text: "Backlight Compensation"
                onClicked: {
                    _controllerCore.backlightCompensation = checked
                }
            }
            Connections{
                target: _controllerCore
                onBacklightCompensationChanged: {
                    backlightCompensationCheck.checked = val
                }
            }

            PopupCheckBox{
                id: dynamicRangeModeButton
                checked: false
                text: "Wide Dynamic Range Mode"
                onClicked: {
                    _controllerCore.wideDynamicRangeMode = checked
                }
            }
            Connections{
                target: _controllerCore
                onWideDynamicRangeModeChanged: {
                    dynamicRangeModeButton.checked = val
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
                    text: "Noise Reduction 2D: "
                    font.bold: true
                }
                Rectangle{
                    id: noiseReduction2DBackground
                    Layout.preferredHeight: 15
                    Layout.preferredWidth: 50
                    TextInput {
                        anchors.fill: parent
                        id: noiseReduction2DValue
                        text: _controllerCore.noiseReduction2D
                        font.bold: true
                        //validator: RegExpValidator { regExp: /^0\.\d\d/}
                        onTextChanged: {
                            _controllerCore.noiseReduction2D = noiseReduction2DValue.text
                        }
                    }
                    Connections{
                        target: _controllerCore
                        onNoiseReduction2DChanged: {
                            noiseReduction2DValue.text = _controllerCore.noiseReduction2D
                        }
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
                    text: "Noise Reduction 2D: "
                    font.bold: true
                }
                Rectangle{
                    id: noiseReduction3DBackground
                    Layout.preferredHeight: 15
                    Layout.preferredWidth: 50
                    TextInput {
                        anchors.fill: parent
                        id: noiseReduction3DValue
                        text: _controllerCore.noiseReduction3D
                        font.bold: true
                        //validator: RegExpValidator { regExp: /^0\.\d\d/}
                        onTextChanged: {
                            _controllerCore.noiseReduction3D = noiseReduction3DValue.text
                        }
                    }
                    Connections{
                        target: _controllerCore
                        onNoiseReduction2DChanged: {
                            noiseReduction3DValue.text = _controllerCore.noiseReduction3D
                        }
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
                    text: "Exposure Mode: "
                    font.bold: true
                }
                CustomComboBox{
                    Layout.alignment: Qt.AlignRight
                    id: exposureCombo
                    model: ["AUTO", "SHUTTER_PRIORITY", "IRIS_PRIORITY", "MANUAL"]
                    onCurrentTextChanged: {
                        _controllerCore.exposureMode = currentIndex
                    }
                }
                Connections{
                    target: _controllerCore
                    onExposureModeChanged:
                        exposureCombo.currentIndex = val
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
                    text: "Exposure Compensation: "
                    font.bold: true
                }
                Rectangle{
                    id: exposureCompensationBackground
                    Layout.preferredHeight: 15
                    Layout.preferredWidth: 50
                    TextInput {
                        anchors.fill: parent
                        id: exposureCompensationValue
                        text: _controllerCore.exposureCompensation.toFixed(2)
                        font.bold: true
                        //validator: RegExpValidator { regExp: /^0\.\d\d/}
                        onTextChanged: {
                            _controllerCore.exposureCompensation = exposureCompensation.text
                        }
                    }
                    Connections{
                        target: _controllerCore
                        onExposureCompensationChanged: {
                            exposureCompensationValue.text = val
                        }
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
                    text: "Color Gain: "
                    font.bold: true
                }
                Rectangle{
                    id: colorGainBackground
                    Layout.preferredHeight: 15
                    Layout.preferredWidth: 50
                    TextInput {
                        anchors.fill: parent
                        id: colorGainValue
                        text: _controllerCore.colorGain.toFixed(2)
                        font.bold: true
                        //validator: RegExpValidator { regExp: /^0\.\d\d/}
                        onTextChanged: {
                            _controllerCore.colorGain = colorGainValue.text
                        }
                    }
                    Connections{
                        target: _controllerCore
                        onColorGainChanged: {
                            colorGainValue.text = _controllerCore.colorGain
                        }
                    }
                }
            }
            RowLayout{
                Layout.fillWidth: true
                Layout.margins: 5
                Text{
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: true
                    id: thermalText
                    color: "white"
                    text: "Shutter: "
                    font.pixelSize: 12
                    font.bold: true
                }
                Rectangle{
                    id: shutterBackground
                    Layout.preferredHeight: 15
                    Layout.preferredWidth: 50
                    TextInput {
                        anchors.fill: parent
                        id: shutterValue
                        text: _controllerCore.visShutterPeriod.toFixed(2)
                        font.bold: true
                        //validator: RegExpValidator { regExp: /^0\.\d\d/}
                        onTextChanged: {
                            _controllerCore.visShutterPeriod = parseInt(shutterValue.text)
                        }
                    }
                    Connections{
                        target: _controllerCore
                        onVisShutterPeriodChanged: {
                            shutterValue.text = _controllerCore.visShutterPeriod
                        }
                    }
                }
            }

            Rectangle{
                Layout.fillWidth: true
                height: 1
                color: "black"
            }
        }
        PopupDropDown{
            id: imageVidDropDown
            dropdownText: "Images & Video"
        }
        ColumnLayout{
            id: imageVidColumn
            function setImageVid()
            {
                _controllerCore.setCaptureVideoSettings(radJpgCheck.checked, visJpgCheck.checked, radVidCheck.checked, visVidCheck.checked)
            }
            Layout.alignment: Qt.AlignHCenter
            visible: imageVidDropDown.activated
            PopupCheckBox{
                id: radJpgCheck
                text: _controllerCore.type == mainWindow.securityType ? "Image IR JPEG" : "Image Radiometric JPEG"
                onCheckedChanged: imageVidColumn.setImageVid();
            }
            PopupCheckBox{
                id: visJpgCheck
                text: "Image Visible JPEG"
                onCheckedChanged: imageVidColumn.setImageVid();
            }
            RowLayout
            {
                Text{
                    text: "Save Images to: "
                    font.pixelSize: 12
                    color: "white"
                    font.bold: true
                }
                CustomComboBox{
                    id: selectedStorage
                    model: ["SSD", "SD", "FLASH"]
                    onCurrentTextChanged: {

                    }
                }
            }
            Connections{
                target: _controllerCore
                onDriveStateChanged:{
                    var gig = 1024*1024*1024;
                    ssdText.text = "SSD: " + ssd
                    sdText.text = "SD Card: " + sd;
                    flashText.text = "Flash: " + flash;
                    var arr = [];
                    if(ssd == "READY")
                    {
                        arr.push("SSD");
                        ssdText.text += " " + Math.round(ssdCap/gig) + " GB " + Math.round(ssdFree) + "% free";
                        rightPanel.refreshSSDState("SSD Free: "+ Math.round(ssdFree) + "%")
                    }
                    if(sd == "READY")
                    {
                        arr.push("SD_CARD");
                        sdText.text += " " + Math.round(sdCap/gig) + " GB " + Math.round(sdFree) + "% free";
                    }
                    if(flash == "READY")
                    {
                        arr.push("FLASH_DRIVE");
                        flashText.text += " " + Math.round(flashCap/gig) + " GB " + Math.round(flashFree) + "% free";
                    }
                    selectedStorage.model = arr;
                    selectedStorage.update();
                    selectedStorage.customBackground = _controllerCore.changeStorage(selectedStorage.currentText) ? "white" : "red";
                }
            }
            PopupCheckBox{
                id: radVidCheck
                enabled: !quickRecordText.recording
                text: "Video Radiometric (SSD)"
                onCheckedChanged: imageVidColumn.setImageVid();
            }
            PopupCheckBox{
                id: visVidCheck
                enabled: !quickRecordText.recording
                text: "Video Visible (SSD)"
                onCheckedChanged: imageVidColumn.setImageVid();
            }
            Connections{
                target: _controllerCore
                onCapRecSettingChanged: {
                    radJpgCheck.checked = radJpg;
                    visJpgCheck.checked = visJpg;
                    radVidCheck.checked = encIRVid;
                    visVidCheck.checked = visVid;
                }
            }

        }
        Rectangle{
            Layout.fillWidth: true
            height: 1
            color: "black"
        }
        PopupDropDown{
            id: memoryDropDown
            dropdownText: "Memory"
        }
        ColumnLayout{
            Layout.alignment: Qt.AlignHCenter
            visible: memoryDropDown.activated
            Text{
                id: ssdText
                color: "white"
                font.bold: true
            }
            Text{
                id: sdText
                color: "white"
                font.bold: true
            }
            Text{
                id: flashText
                color: "white"
                font.bold: true
            }
        }
        Rectangle{
            Layout.fillWidth: true
            height: 1
            color: "black"
        }
        PopupDropDown{
            id: systemDropDown
            dropdownText: "System"
        }
        ColumnLayout{
            Layout.alignment: Qt.AlignHCenter
            visible: systemDropDown.activated
            Text{
                color: "white"
                font.pixelSize: 12
                text: "Current time: " + _controllerCore.systemTime
            }
            Text{
                color: "white"
                font.pixelSize: 12
                text: "Current Date: " + _controllerCore.systemDate
            }
            Text{
                color: "white"
                font.pixelSize: 12
                text: "Units: °C"
            }
            Text{
                color: "white"
                font.pixelSize: 12
                text: "Languge: English"
            }
            Text{
                id: rebtText
                color: maRbt.containsMouse ? "orange" : "white"
                font.bold: true
                font.pixelSize: 12
                text: "Reboot System"
                MouseArea {
                    id: maRbt
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked:{
                        rebootWarningText.visible = true;
                        rebootRow.visible = true;
                    }
                }
            }
            Text{
                id: rebootWarningText
                text: "Reboot?\nThis will restart the app\nand make MEDICAS\nunreachable for a while"
                color: "red"
                visible: false
                font.pixelSize: 12
            }
            RowLayout{
                id: rebootRow
                visible: false
                PopupButton{
                    text: "Reboot"
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 60
                    onClicked: _controllerCore.reboot();
                }
                PopupButton{
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 60
                    text: "Cancel"
                    onClicked: {
                        rebootWarningText.visible = false;
                        rebootRow.visible = false;
                    }
                }
            }
            Connections{
                target: _controllerCore
                onFirmwareVersionChanged:{
                    if (!_controllerCore.compareFw("1.1.1", val))
                    {
                        rebtText.visible = false;
                    }
                }
            }
            Text{
                color: maSds.containsMouse ? "orange" : "white"
                font.bold: true
                font.pixelSize: 12
                text: "Set Default Settings"
                MouseArea {
                    id: maSds
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked:{
                        defaultWarningText.visible = true;
                        defaultRow.visible = true;
                    }
                }
            }
            Text{
                id: defaultWarningText
                text: "Set default settigns?\nThis will restart the app"
                color: "red"
                visible: false
                font.pixelSize: 12
            }
            RowLayout{
                id: defaultRow
                visible: false
                PopupButton{
                    text: "Set"
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 60
                    onClicked: _controllerCore.setDefaultSettings()
                }
                PopupButton{
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 60
                    text: "Cancel"
                    onClicked: {
                        defaultWarningText.visible = false;
                        defaultRow.visible = false;
                    }
                }
            }
        }
        Rectangle{
            Layout.fillWidth: true
            height: 1
            color: "black"
        }
        PopupDropDown{
            id: infoDropdown
            dropdownText: "Info"
        }
        InfoPanel{}
    }
}

