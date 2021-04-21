import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "../"

ColumnLayout{
    RowLayout{
        Layout.margins: 5
        Layout.fillWidth: true
        Text{
            Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: true
            color: "white"
            font.pixelSize: 12
            text: qsTr("Gamma: ") + _translator.emptyString
            font.bold: true
        }
        CustomComboBox{
            Layout.alignment: Qt.AlignCenter
            id: gammaCombo
            model: ["STANDARD", "STRAIGHT", "NARROW", "WIDE"]
            onActivated: {
                _controllerCore.gamma = currentIndex
            }
        }
        Connections{
            target: _controllerCore
            function onGammaChanged(val) {
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
            text: qsTr("White Balance: ") + _translator.emptyString
            font.bold: true
        }
        CustomComboBox{
            Layout.alignment: Qt.AlignRight
            id: wBaCombo
            model: ["NARROW", "INDOOR", "OUTDOOR", "WIDE", "ONE_PUSH_TRIGGER"]
            onActivated: {
                _controllerCore.whiteBalance = currentIndex
            }
        }
        Connections{
            target: _controllerCore
            function onWhiteBalanceChanged(val)
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
            text: qsTr("Iris: ") + _translator.emptyString
            font.bold: true
        }
        CustomComboBox{
            Layout.alignment: Qt.AlignRight
            id: irisCombo
            model: [ "F22", "F16", "F14", "F11", "F9.6", "F8", "F6.8", "F5.6", "F4.8", "F4", "F3.4", "F2.8","F2.4",  "F2", "F1.8" ]
            onActivated: {
                // iris modes start idexing at 0x03 - check customtools.h
                _controllerCore.iris = (currentIndex)
            }
        }
        Connections{
            target: _controllerCore
            function onIrisChanged(val)
            {
                irisCombo.currentIndex = (val)
            }
        }
    }
    PopupCheckBox{
        id: backlightCompensationCheck
        checked: false
        text: qsTr("Backlight Compensation") + _translator.emptyString
        onClicked: {
            _controllerCore.backlightCompensation = checked
        }
    }
    Connections{
        target: _controllerCore
        function onBacklightCompensationChanged(val) {
            backlightCompensationCheck.checked = val
        }
    }

    PopupCheckBox{
        id: dynamicRangeModeButton
        checked: false
        text: qsTr("Wide Dynamic Range Mode") + _translator.emptyString
        onClicked: {
            _controllerCore.wideDynamicRangeMode = checked
        }
    }
    Connections{
        target: _controllerCore
        function onWideDynamicRangeModeChanged() {
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
            text: qsTr("Noise Reduction 2D: ") + _translator.emptyString
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
                onEditingFinished: {
                    _controllerCore.noiseReduction2D = noiseReduction2DValue.text
                }
            }
            Connections{
                target: _controllerCore
                function onNoiseReduction2DChanged(val) {
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
            text: qsTr("Noise Reduction 2D: ") + _translator.emptyString
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
                onEditingFinished: {
                    _controllerCore.noiseReduction3D = noiseReduction3DValue.text
                }
            }
            Connections{
                target: _controllerCore
                function onNoiseReduction2DChanged() {
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
            text: qsTr("Exposure Mode: ") + _translator.emptyString
            font.bold: true
        }
        CustomComboBox{
            Layout.alignment: Qt.AlignRight
            id: exposureCombo
            model: ["AUTO", "SHUTTER_PRIORITY", "IRIS_PRIORITY", "MANUAL"]
            onActivated:{
                _controllerCore.exposureMode = currentIndex
            }
        }
        Connections{
            target: _controllerCore
            function onExposureModeChanged(val){
                exposureCombo.currentIndex = val
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
            text: qsTr("Exposure Compensation: ") + _translator.emptyString
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
                onEditingFinished: {
                    _controllerCore.exposureCompensation = exposureCompensationValue.text
                }
            }
            Connections{
                target: _controllerCore
                function onExposureCompensationChanged(val) {
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
            text: qsTr("Color Gain: ") + _translator.emptyString
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
                onEditingFinished: {
                    _controllerCore.colorGain = colorGainValue.text
                }
            }
            Connections{
                target: _controllerCore
                function onColorGainChanged() {
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
            text: qsTr("Shutter: ") + _translator.emptyString
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
                onEditingFinished: {
                    _controllerCore.visShutterPeriod = parseInt(shutterValue.text)
                }
            }
            Connections{
                target: _controllerCore
                function onVisShutterPeriodChanged() {
                    shutterValue.text = _controllerCore.visShutterPeriod
                }
            }
        }
    }

}

