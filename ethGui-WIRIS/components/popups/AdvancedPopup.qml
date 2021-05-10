import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "../"

ControlPopup{
    function visVidChecked()
    {
        return visVidCheck.checked
    }
    function radVidChecked()
    {
        return radVidCheck.checked
    }
    function imageCaptureEnabled()
    {
        return radJpgCheck.checked || radTiffCheck.checked || visJpgCheck.checked || srImgCheck.checked
    }
    function imagaCaptureSecEnabled()
    {
        return radJpgCheck.checked || visJpgCheck.checked
    }

    function applyShutter()
    {
        shutterPeriodSpinbox.applyShutter();
    }

    function shutterPeriodSpinboxValue()
    {
        return shutterPeriodSpinbox.value
    }

    function syncShutterChecked()
    {
        return syncShutter.checked
    }

    function alarmColorAbove()
    {
        return colorAboveComboBox.currentText
    }

    function alarmColorBetween()
    {
        return colorBetweenComboBox.currentText
    }

    function alarmColorBelow()
    {
        return colorBelowComboBox.currentText
    }

    ColumnLayout{
        width: parent.width
        PopupDropDown{
            id: thermalDropDown
            dropdownText: "Thermal Camera"
            visible: _controllerCore.type != mainWindow.securityType
        }
        ColumnLayout{
            Layout.alignment: Qt.AlignHCenter
            visible: thermalDropDown.activated

            Text{
                color: "white"
                font.pixelSize: 12
                text: "Emissivity: "
                font.bold: true
            }

            Rectangle{
                id: emissivityBackground
                Layout.preferredHeight: 15
                Layout.preferredWidth: 50
                TextInput {
                    anchors.fill: parent
                    id: emissivityValue
                    text: Math.round(_controllerCore.emissivity*100)/100
                    font.bold: true
                    validator: RegExpValidator { regExp: /^0\.\d\d?/}
                    onAccepted: {
                        if(!_controllerCore.isSetup)
                            return
                        if (emissivityValue.text > 0.5 && emissivityValue.text <= 1){
                            _controllerCore.emissivity = emissivityValue.text
                            emissivityValue.color = "black";
                        }
                        else emissivityValue.color = "red";
                    }
                    Connections{
                        target: _controllerCore
                        function onEmissivityChanged(){ emissivityValue.text = Math.round(_controllerCore.emissivity*100)/100 }
                    }
                }
            }

            Text{
                color: "white"
                font.pixelSize: 12
                text: "Reflected Temperature: "
                font.bold: true
            }

            Rectangle{
                id: refTempBackground
                Layout.preferredHeight: 15
                Layout.preferredWidth: 50
                TextInput {
                    anchors.fill: parent
                    id: refTempValue
                    text: Math.round(_controllerCore.reflectedTemp*100)/100
                    font.bold: true
                    validator: RegExpValidator { regExp: /^\-?\d?\d?\d?\d\.?\d?\d?/}
                    onAccepted: {
                        if(!_controllerCore.isSetup)
                            return
                        if (refTempValue.text > -50 && refTempValue.text <= 1500){
                            _controllerCore.reflectedTemp = refTempValue.text
                            refTempValue.color = "black";
                        }
                        else refTempValue.color = "red";
                    }
                    Connections{
                        target: _controllerCore
                        function onReflectedTempChanged(){ refTempValue.text = Math.round(_controllerCore.reflectedTemp*100)/100 }
                    }
                }
            }

            Text{
                color: "white"
                font.pixelSize: 12
                text: "Atmospheric Temperature"
                font.bold: true
            }

            Rectangle{
                id: atmTempBackground
                Layout.preferredHeight: 15
                Layout.preferredWidth: 50
                TextInput {
                    anchors.fill: parent
                    id: atmTempValue
                    text: Math.round(_controllerCore.atmosphericTemp*100)/100
                    font.bold: true
                    validator: RegExpValidator { regExp: /^\-?\d?\d?\d?\d\.?\d?\d?/}
                    onAccepted: {
                        if(!_controllerCore.isSetup)
                            return
                        if (atmTempValue.text > -50 && atmTempValue.text <= 1500){
                            _controllerCore.atmosphericTemp = atmTempValue.text
                            atmTempValue.color = "black";
                        }
                        else atmTempValue.color = "red";
                    }
                    Connections{
                        target: _controllerCore
                        function onAtmosphericTempChanged(){ atmTempValue.text = Math.round(_controllerCore.atmosphericTemp*100)/100 }
                    }
                }
            }

            Text{
                id: thermalText
                color: "white"
                text: "Shutter: "
                font.pixelSize: 12
                font.bold: true
                visible: _controllerCore.type !== mainWindow.gisType
            }

            PopupCheckBox{
                id: syncShutter
                text: "Synchronous Correction"
                onClicked: _controllerCore.syncShutter = checked;
                visible: _controllerCore.type !== mainWindow.gisType
                Connections{
                    target: _controllerCore
                    function onSyncShutterChanged()
                    {
                        syncShutter.checked = _controllerCore.syncShutter;
                    }
                }
            }

            ShutterSpinBox{
                id: shutterPeriodSpinbox
                function applyShutter()
                {
                    _controllerCore.triggerShutter()
                }
                Layout.preferredHeight: 25
                visible: _controllerCore.type !== mainWindow.gisType && syncShutter.checked
                onValueChanged: {
                    shutterDebounceTimer.stop();
                    shutterDebounceTimer.start();
                }

                Connections{
                    target: _controllerCore
                    function onShutterPeriodChanged()
                    {
                        shutterPeriodSpinbox.value = _controllerCore.shutterPeriod;
                    }
                }

                Timer{
                    id: shutterDebounceTimer
                    interval: 5000
                    repeat: false
                    onTriggered: shutterPeriodSpinbox.applyShutter()
                }


            }
            PopupCheckBox{
                id: interpolation
                text: "Image Interpolation"
                onClicked: _controllerCore.interpolation = checked;
                visible: _controllerCore.type === mainWindow.gisType
                Connections{
                    target: _controllerCore
                    function onInterpolationChanged(val)
                    {
                        interpolation.checked = _controllerCore.interpolation;
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
            id: imageVidDropDown
            dropdownText: "Images & Video"
        }

        ColumnLayout{
            id: imageVidColumn

            function setImageVid()
            {
                _controllerCore.setCaptureVideoSettings(radJpgCheck.checked, radTiffCheck.checked, srImgCheck.checked, visJpgCheck.checked, radVidCheck.checked, visVidCheck.checked)
            }

            Layout.alignment: Qt.AlignHCenter
            visible: imageVidDropDown.activated

            PopupCheckBox{
                id: radJpgCheck
                text: _controllerCore.type == mainWindow.securityType ? "Image IR JPEG" : "Image Radiometric JPEG"
                onClicked: imageVidColumn.setImageVid();
            }

            PopupCheckBox{
                id: radTiffCheck
                text: "Image Radiometric TIFF"
                onClicked: imageVidColumn.setImageVid();
                visible: _controllerCore.type != mainWindow.securityType
            }

            PopupCheckBox{
                id: srImgCheck
                text: "Super‐resolution Image"
                onClicked:  imageVidColumn.setImageVid();
                visible: _controllerCore.type != mainWindow.securityType &&  _controllerCore.type != mainWindow.gisType
            }

            PopupCheckBox{
                id: visJpgCheck
                text: "Image Visible JPEG"
                onClicked: imageVidColumn.setImageVid();
            }

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

            Connections{
                target: _controllerCore
                function onDriveStateChanged(ssd, ssdCap,  ssdFree, sd, sdCap, sdFree, flash, flashCap, flashFree)
                {
                    var gig = 1024*1024*1024;
                    ssdText.text = "SSD: " + ssd
                    sdText.text = "SD Card: " + sd;
                    flashText.text = "Flash: " + flash;

                    var arr = [];
                    if(ssd == "READY")
                    {
                        arr.push("SSD");
                        ssdText.text += " " + Math.round(ssdCap/gig) + " GB " + Math.round(ssdFree) + "% free";
                        sideSsdText.text = "SSD Free: "+ Math.round(ssdFree) + "%"
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
                enabled: !(quickRecordText.recording)
                text: _controllerCore.type == mainWindow.securityType ? "Encoded IR (SSD)" :"Video Radiometric (SSD)"
                onClicked: imageVidColumn.setImageVid();
            }

            PopupCheckBox{
                id: visVidCheck
                enabled: !quickRecordText.recording
                text: "Video Visible (SSD)"
                onClicked: imageVidColumn.setImageVid();
            }

            Connections{
                target: _controllerCore
                function onCapRecSettingChanged(radJpg, radTiff, srImg, visJpg, radVid, visVid)
                {
                    radJpgCheck.checked = radJpg;
                    radTiffCheck.checked = radTiff;
                    srImgCheck.checked = srImg;
                    visJpgCheck.checked = visJpg;
                    radVidCheck.checked = radVid;
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
            id: alarmsDropDown
            dropdownText: "Alarms"
        }
        ColumnLayout{
            Layout.alignment: Qt.AlignHCenter
            visible: alarmsDropDown.activated

            Text{
                text: "Alarm Above: "
                color: "white"
                font.pixelSize: 12
                font.bold: true
            }
            CustomComboBox{
                id: colorAboveComboBox
                model: ["RED", "GREEN", "BLUE"]
                onActivated: _controllerCore.setAlarmColors(colorAboveComboBox.currentText, colorBetweenComboBox.currentText, colorBelowComboBox.currentText)
            }

            Text{
                text: "Alarm Below: "
                color: "white"
                font.pixelSize: 12
                font.bold: true
            }
            CustomComboBox{
                id: colorBelowComboBox
                model: ["RED", "GREEN", "BLUE"]
                onActivated: _controllerCore.setAlarmColors(colorAboveComboBox.currentText, colorBetweenComboBox.currentText, colorBelowComboBox.currentText)
            }

            Text{
                text: "Alarm Between: "
                color: "white"
                font.pixelSize: 12
                font.bold: true
            }
            CustomComboBox{
                id: colorBetweenComboBox
                model: ["RED", "GREEN", "BLUE"]
                onActivated: _controllerCore.setAlarmColors(colorAboveComboBox.currentText, colorBetweenComboBox.currentText, colorBelowComboBox.currentText)
            }

            Connections{
                target: _controllerCore
                function onAlarmColorsChanged(above, between, below)
                {
                    colorAboveComboBox.currentIndex = above;
                    colorBetweenComboBox.currentIndex = between;
                    colorBelowComboBox.currentIndex = below;
                }
            }
        }
        Rectangle{
            visible: alarmsDropDown.visible
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
                text: "Reboot?\nThis will CLOSE the app\nand make WIRIS\nunreachable for a while\nApplication needs to\nbe restarted manually"
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
            Text{
                text: "Units: "
                color: "white"
                font.pixelSize: 12
                font.bold: true
                visible: _controllerCore.type != mainWindow.securityType
            }
            CustomComboBox{
                id: unitsComboBox
                model: ["CELSIUS", "FAHRENHEIT", "KELVIN"]
                onActivated:  _controllerCore.thermalUnit = currentText
                visible: _controllerCore.type != mainWindow.securityType
                Connections{
                    target: _controllerCore
                    function onThermalUnitChanged()
                    {
                        unitsComboBox.currentIndex = unitsComboBox.model.indexOf(_controllerCore.thermalUnit)
                    }
                }
            }

            Connections{
                target: _controllerCore
                function onFirmwareVersionChanged(val)
                {
                    if(!_controllerCore.compareFw("1.1.1", val))
                    {
                        rebtText.visible = false;
                        alarmsDropDown.visible = false;
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
