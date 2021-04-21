import QtQuick 2.0
import QtQuick.Layouts 1.3
import "../"

ColumnLayout {
    RowLayout {
        Text {
            Layout.margins: 5
            color: "white"
            font.pixelSize: 12
            Layout.fillWidth: true
            text: qsTr("DIO connected:") + _translator.emptyString
        }

        Text {
            Layout.margins: 5
            Layout.alignment: Qt.AlignRight
            color: "white"
            font.pixelSize: 12
            font.bold: true
            text: (_controllerCore.dioConnected ? qsTr("TRUE") + _translator.emptyString : qsTr(
                                                      "FALSE") + _translator.emptyString)
        }
    }
    RowLayout {
        Layout.margins: 5
        Layout.fillWidth: true
        enabled: _controllerCore.dioConnected
        Text {
            text: qsTr("Trigger mode: ") + _translator.emptyString
            font.pixelSize: 12
            Layout.fillWidth: true
            color: "white"
            font.bold: true
        }
        CustomComboBox {
            id: selectedTrigger
            model: ["NON-TRIGGERED", "RISING-EDGE"]
            onActivated: _controllerCore.dioTriggerMode = currentText
            enabled: _controllerCore.dioConnected
            Layout.preferredWidth: 135
        }
        Connections {
            target: _controllerCore
            function onDioTriggerModeChanged() {
                if (_controllerCore.dioTriggerMode === "NON-TRIGGERED") {
                    selectedTrigger.currentIndex = 0
                } else if (_controllerCore.dioTriggerMode === "RISING-EDGE") {
                    selectedTrigger.currentIndex = 1
                }
            }
        }
    }

    PopupCheckBox {
        id: picOnTrigCheck
        enabled: ((_controllerCore.dioTriggerMode === "RISING-EDGE")
                  && _controllerCore.dioConnected)
        text: qsTr("Picture on trigger") + _translator.emptyString
        Layout.fillWidth: false
        Layout.margins: 5
        onClicked: _controllerCore.dioPictureOnTrigger = picOnTrigCheck.checked
        checked: _controllerCore.dioPictureOnTrigger
    }

    RowLayout {
        Layout.fillWidth: true
        Layout.margins: 5
        enabled: _controllerCore.dioConnected
        Text {
            Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: true
            color: "white"
            font.pixelSize: 12
            text: qsTr("Minimum pulse width: ") + _translator.emptyString
            font.bold: true
        }
        Rectangle {
            id: minimumPulseWidthBackground
            Layout.preferredHeight: 15
            Layout.preferredWidth: 50
            TextInput {
                anchors.fill: parent
                id: minimumPulseWidthValue
                text: _controllerCore.dioMinimumPulseWidth
                font.bold: true
                enabled: _controllerCore.dioConnected
                onAccepted: _controllerCore.dioMinimumPulseWidth = minimumPulseWidthValue.text
            }
        }
    }

    RowLayout {
        Layout.fillHeight: false
        Layout.fillWidth: true
        Layout.margins: 5
        enabled: _controllerCore.dioConnected
        Text {
            text: qsTr("DIO signal: ") + _translator.emptyString
            font.pixelSize: 12
            Layout.fillWidth: true
            color: "white"
            font.bold: true
        }
        CustomComboBox {
            id: selectedSignal
            model: ["INTERNAL", "EXTERNAL"]
            enabled: _controllerCore.dioConnected
            onActivated: _controllerCore.dioSignal = currentText
            Layout.preferredWidth: 135
        }
        Connections {
            target: _controllerCore
            function onDioSignalChanged() {
                if (_controllerCore.dioSignal === "INTERNAL") {
                    selectedSignal.currentIndex = 0
                } else if (_controllerCore.dioSignal === "EXTERNAL") {
                    selectedSignal.currentIndex = 1
                }
            }
        }
    }

    RowLayout {
        Layout.fillWidth: true
        enabled: _controllerCore.dioConnected
        Layout.margins: 5
        Text {
            text: qsTr("Trigger delay: ") + _translator.emptyString
            font.pixelSize: 12
            Layout.fillWidth: true
            color: "white"
            font.bold: true
        }
        CustomComboBox {
            id: selectedDelay
            model: ["0", "1", "2", "3", "5", "10"]
            enabled: _controllerCore.dioConnected
            onActivated: _controllerCore.dioTriggerDelay = currentText
            Layout.preferredWidth: 135
        }
        Connections {
            target: _controllerCore
            function onDioTriggerDelayChanged() {
                if (_controllerCore.dioTriggerDelay === "0") {
                    selectedDelay.currentIndex = 0
                } else if (_controllerCore.dioTriggerDelay === "1") {
                    selectedDelay.currentIndex = 1
                } else if (_controllerCore.dioTriggerDelay === "2") {
                    selectedDelay.currentIndex = 2
                } else if (_controllerCore.dioTriggerDelay === "3") {
                    selectedDelay.currentIndex = 3
                } else if (_controllerCore.dioTriggerDelay === "5") {
                    selectedDelay.currentIndex = 4
                } else if (_controllerCore.dioTriggerDelay === "10") {
                    selectedDelay.currentIndex = 5
                }
            }
        }
    }

    RowLayout {
        Layout.fillWidth: true
        Layout.margins: 5
        enabled: _controllerCore.dioConnected
        Text {
            Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: true
            color: "white"
            font.pixelSize: 12
            text: qsTr("Minimal detection time: ") + _translator.emptyString
            font.bold: true
        }
        Rectangle {
            id: minimalDetectionTimeBackground
            Layout.preferredHeight: 15
            Layout.preferredWidth: 50
            TextInput {
                anchors.fill: parent
                id: minimalDetectionTimeValue
                text: _controllerCore.dioMinDetectionTime
                font.bold: true
                enabled: _controllerCore.dioConnected
                onAccepted: _controllerCore.dioMinDetectionTime = minimalDetectionTimeValue.text
            }
        }
    }

    RowLayout {
        Layout.fillWidth: true
        Layout.margins: 5
        enabled: _controllerCore.dioConnected
        Text {
            Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: true
            color: "white"
            font.pixelSize: 12
            text: qsTr("Temperature hysteresis: ") + _translator.emptyString
            font.bold: true
        }
        Rectangle {
            id: tempHysteresisBackground
            Layout.preferredHeight: 15
            Layout.preferredWidth: 50
            TextInput {
                anchors.fill: parent
                id: tempHysteresisValue
                text: _controllerCore.dioTempHysteresis
                font.bold: true
                enabled: _controllerCore.dioConnected
                onAccepted: _controllerCore.dioTempHysteresis = tempHysteresisValue.text
            }
        }
    }

    RowLayout {
        Layout.fillWidth: true
        Layout.margins: 5
        enabled: _controllerCore.dioConnected
        Text {
            Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: true
            color: "white"
            font.pixelSize: 12
            text: qsTr("Minimal alarm time: ") + _translator.emptyString
            font.bold: true
        }
        Rectangle {
            id: minAlarmTimeBackground
            Layout.preferredHeight: 15
            Layout.preferredWidth: 50
            TextInput {
                anchors.fill: parent
                id: minAlarmTimeValue
                text: _controllerCore.dioMinAlarmTime
                font.bold: true
                enabled: _controllerCore.dioConnected
                onAccepted: _controllerCore.dioMinAlarmTime = minAlarmTimeValue.text
            }
        }
    }
    RowLayout {
        enabled: _controllerCore.dioConnected
        Layout.fillWidth: true
        Layout.margins: 5
        Text {
            text: qsTr("DIO logic: ") + _translator.emptyString
            font.pixelSize: 12
            Layout.fillWidth: true
            color: "white"
            font.bold: true
        }
        CustomComboBox {
            id: selectedLogic
            model: ["OPEN", "CLOSED"]
            enabled: _controllerCore.dioConnected
            onActivated: _controllerCore.dioLogic = currentText
            Layout.preferredWidth: 135
        }
        Connections {
            target: _controllerCore
            function onDioLogicChanged() {
                if (_controllerCore.dioLogic === "OPEN") {
                    selectedLogic.currentIndex = 0
                } else if (_controllerCore.dioLogic === "CLOSED") {
                    selectedLogic.currentIndex = 1
                }
            }
        }
    }

    RowLayout {
        enabled: _controllerCore.dioConnected
        Layout.fillWidth: true
        Layout.margins: 5
        Text {
            text: qsTr("Gate open time: ") + _translator.emptyString
            font.pixelSize: 12
            Layout.fillWidth: true
            color: "white"
            font.bold: true
        }
        CustomComboBox {
            id: selectedGateOpenTime
            model: ["500", "1000", "2000", "3000", "5000", "10000"]
            enabled: _controllerCore.dioConnected
            onActivated: _controllerCore.dioGateOpenTime = currentText
            Layout.preferredWidth: 135
        }
        Connections {
            target: _controllerCore
            function onDioGateOpenTimeChanged() {
                if (_controllerCore.dioGateOpenTime === "500") {
                    selectedGateOpenTime.currentIndex = 0
                } else if (_controllerCore.dioGateOpenTime === "1000") {
                    selectedGateOpenTime.currentIndex = 1
                } else if (_controllerCore.dioGateOpenTime === "2000") {
                    selectedGateOpenTime.currentIndex = 2
                } else if (_controllerCore.dioGateOpenTime === "3000") {
                    selectedGateOpenTime.currentIndex = 3
                } else if (_controllerCore.dioGateOpenTime === "5000") {
                    selectedGateOpenTime.currentIndex = 4
                } else if (_controllerCore.dioGateOpenTime === "10000") {
                    selectedGateOpenTime.currentIndex = 5
                }
            }
        }
    }

    PopupCheckBox {
        id: showDioStatusCheck
        text: qsTr("Show DIO status") + _translator.emptyString
        checked: _controllerCore.dioShowStatus
        onClicked: _controllerCore.dioShowStatus = showDioStatusCheck.checked
        enabled: _controllerCore.dioConnected
        Layout.margins: 5
    }

    Text {
        id: resetText
        color: _controllerCore.dioConnected ? (malogout.containsMouse ? "orange" : "white") : "gray"
        font.bold: true
        font.pixelSize: 12
        Layout.fillWidth: true
        Layout.margins: 5
        text: qsTr("Restart DIO") + _translator.emptyString
        MouseArea {
            id: malogout
            anchors.fill: parent
            hoverEnabled: true
            enabled: _controllerCore.dioConnected
            onClicked: _controllerCore.dioReset()
        }
    }
}
