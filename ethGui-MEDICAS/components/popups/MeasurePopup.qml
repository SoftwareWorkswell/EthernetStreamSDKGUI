import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "../"

ControlPopup {
    id: measurePopup
    function showHeatingUpMessage()
    {
        heatingUpDialog.open()
    }

    BlackbodyErrorMessage {
        id: bbErrorDialog
        Connections {
            target: _controllerCore
            function onBlackbodyError() {
                bbErrorDialog.open()
            }
        }
    }
    HeatingUpMessage {
        id: heatingUpDialog
        Connections{
            target: measurePopup
            onOpened:
            {
                console.log('measure opened')
            }
        }
    }
    AlarmErrorDialog {
        id: alarmErrorDialog
        Connections {
            target: _controllerCore
            function onAlarmError() {
                alarmErrorDialog.open()
            }
        }
    }
    ScrollView {
        anchors.fill: parent
        clip: true
        contentHeight: modeColumn.y + modeColumn.height + 30
        contentWidth: width
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.width: 10
        contentChildren: [
            ColumnLayout {
                id: measColumn
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                PopupItem {
                    Layout.preferredWidth: parent.width
                    itemText: qsTr("Measure") + _translator.emptyString
                    Layout.preferredHeight: 25
                }
                PopupCheckBox {
                    id: showMaxCheck
                    text: qsTr("Show Cross Max") + _translator.emptyString
                    enabled: !alarmModeOff.checked
                    checked: false
                    Layout.rightMargin: 20
                    property bool setProgramatically: false
                    onClicked: {
                        if (!setProgramatically)
                            _controllerCore.showCrossMax = checked
                    }
                    Connections {
                        target: _controllerCore
                        function onShowCrossMaxChanged(val) {
                            showMaxCheck.setProgramatically = true
                            showMaxCheck.checked = val
                            showMaxCheck.setProgramatically = false
                        }
                    }
                }
                PopupCheckBox {
                    text: qsTr("Show Blackbody ROI") + _translator.emptyString
                    checked: false
                    visible: true
                    enabled: true
                    Layout.rightMargin: 20
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
                PopupCheckBox {
                    id: crossUserCheck
                    checked: false
                    text: qsTr("Show Cross User") + _translator.emptyString
                    Layout.rightMargin: 20
                    property bool setProgramatically: false
                    onCheckedChanged: {
                        if (!setProgramatically)
                            _controllerCore.showCrossUser = checked
                    }
                    Connections {
                        target: _controllerCore
                        function onShowCrossUserChanged(val) {
                            crossUserCheck.setProgramatically = true
                            crossUserCheck.checked = val
                            crossUserCheck.setProgramatically = false
                        }
                    }
                }
                RowLayout {
                    Layout.preferredHeight: 50
                    Layout.rightMargin: 20
                    Layout.alignment: Qt.AlignHCenter
                    PopupButton {
                        width: parent.width * 0.65
                        height: parent.height * 0.55
                        //anchors.centerIn: parent
                        Layout.alignment: Qt.AlignCenter | Qt.AlignHCenter
                        text: qsTr("Set Position") + _translator.emptyString
                        onClicked: {
                            showBlackbodyCheck.checked = true
                            mainviewParent.hideAllQuickRects()
                            mainviewParent.setBlackbodyRectVisible(true)
                            controlPanel.hideAllPopups()
                            _controllerCore.setSpecialPheripheralOutput(true)
                            _controllerCore.setCurrentRoiIdx(6)
                        }
                    }
                }
            },
            ColumnLayout {
                id: alarmColumn
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: measColumn.bottom
                PopupItem {
                    Layout.preferredWidth: parent.width
                    itemText: qsTr("Alarm") + _translator.emptyString
                    visible: true
                    Layout.preferredHeight: 25
                }
                RowLayout {
                    Layout.preferredHeight: 30
                    Layout.alignment: Qt.AlignHCenter
                    Layout.maximumWidth: measurePopup.width
                    spacing: 5
                    function setAlarmMode(val, from, to) {
                        if (from < 30 || to > 42)
                            return
                        _controllerCore.alarmMode = val
                        _controllerCore.alarmPreventive = from
                        _controllerCore.alarmCritical = to
                    }
                    function setAlarmFirstVal(val) {
                        if (val < 30)
                            return
                        _controllerCore.alarmPreventive = val
                    }
                    function setAlarmSecondVal(val) {
                        if (val > 42)
                            return
                        _controllerCore.alarmCritical = val
                    }
                    CustomRadioButton1 {
                        checked: true
                        id: alarmModeOff
                        text: qsTr("Off") + _translator.emptyString
                        Layout.fillWidth: true
                        onClicked: {
                            if (checked) {
                                _controllerCore.alarmMode = 0
                                _controllerCore.showCrossMax = false
                            } else
                                _controllerCore.alarmMode = 1
                        }
                    }
                    CustomRadioButton1 {
                        id: alarmModeOn
                        text: qsTr("Image") + _translator.emptyString
                        Layout.fillWidth: true
                        onClicked: {
                            if (checked)
                                _controllerCore.alarmMode = 1
                            else
                                _controllerCore.alarmMode = 0
                        }
                    }
                    CustomRadioButton1 {
                        id: alarmModeROI
                        text: qsTr("Inside ROIs") + _translator.emptyString
                        Layout.fillWidth: true
                        Layout.rightMargin: 20
                        onClicked: {
                            if (checked)
                                _controllerCore.alarmMode = 2
                            else
                                _controllerCore.alarmMode = 1
                        }
                    }

                    Connections {
                        target: _controllerCore
                        function onAlarmModeChanged(val) {
                            switch (val) {
                            case 1:
                                alarmModeOn.checked = true
                                alarmModeOff.checked = false
                                alarmModeROI.checked = false
                                break
                            case 0:
                                alarmModeOn.checked = false
                                alarmModeOff.checked = true
                                alarmModeROI.checked = false
                                break
                            case 2:
                                alarmModeROI.checked = true
                                alarmModeOn.checked = false
                                alarmModeOff.checked = false
                                break
                            }
                        }
                    }
                }
                RowLayout {
                    Layout.fillWidth: true
                    Layout.maximumWidth: parent.width - 20
                    Layout.margins: 5
                    Layout.rightMargin: 20
                    Text {
                        id: preventiveValText
                        Layout.alignment: Qt.AlignLeft
                        color: "white"
                        Layout.fillWidth: true
                        text: qsTr("Preventive Value: ") + _translator.emptyString
                        font.pixelSize: 12
                        font.bold: true
                    }
                    Rectangle {
                        id: alarmPreventiveBackground
                        Layout.alignment: Qt.AlignRight
                        Layout.preferredHeight: 15
                        Layout.preferredWidth: 50
                        TextInput {
                            anchors.fill: parent
                            id: alarmPreventiveValue
                            text: _controllerCore.alarmPreventive.toFixed(2)
                            font.bold: true
                            //validator: RegExpValidator { regExp: /^0\.\d\d/}
                            onAccepted: {
                                _controllerCore.alarmPreventive = parseFloat(
                                            alarmPreventiveValue.text)
                            }
                        }
                        Connections {
                            target: _controllerCore
                            function onAlarmPreventiveChanged() {
                                alarmPreventiveValue.text = _controllerCore.alarmPreventive.toFixed(
                                            2)
                            }
                        }
                    }
                }
                RowLayout {
                    Layout.fillWidth: true
                    Layout.maximumWidth: parent.width - 20
                    Layout.margins: 5
                    Layout.rightMargin: 20
                    Text {
                        id: criticalValText
                        color: "white"
                        Layout.fillWidth: true
                        text: qsTr("Critival Value: ") + _translator.emptyString
                        Layout.alignment: Qt.AlignLeft
                        font.pixelSize: 12
                        font.bold: true
                    }
                    Rectangle {
                        id: alarmCriticalBackground
                        Layout.alignment: Qt.AlignRight
                        Layout.preferredHeight: 15
                        Layout.preferredWidth: 50
                        TextInput {
                            anchors.fill: parent
                            id: alarmCriticalValue
                            text: _controllerCore.alarmCritical.toFixed(2)
                            font.bold: true
                            //validator: RegExpValidator { regExp: /^0\.\d\d/}
                            onAccepted: {
                                _controllerCore.alarmCritical = parseFloat(
                                            alarmCriticalValue.text)
                            }
                        }
                        Connections {
                            target: _controllerCore
                            function onAlarmCriticalChanged() {
                                alarmCriticalValue.text = _controllerCore.alarmCritical.toFixed(
                                            2)
                            }
                        }
                    }
                }
                Rectangle {
                    visible: true
                    Layout.fillWidth: true
                    Layout.rightMargin: 20
                    height: 1
                    color: "black"
                }
            },
            ColumnLayout {
                id: roiColumn
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: alarmColumn.bottom
                PopupItem {
                    Layout.preferredWidth: parent.width
                    Layout.rightMargin: 20
                    itemText: qsTr("User ROIs") + _translator.emptyString
                    visible: true
                    Layout.preferredHeight: 25
                }
                RowLayout {
                    Layout.fillWidth: true
                    Layout.rightMargin: 20
                    Layout.margins: 5
                    Text {
                        Layout.alignment: Qt.AlignLeft
                        Layout.fillWidth: true
                        color: "white"
                        font.pixelSize: 12
                        text: qsTr("Show ROI1: ") + _translator.emptyString
                        font.bold: true
                    }
                    CustomComboBox {
                        Layout.alignment: Qt.AlignRight
                        id: roi1Combo
                        Layout.rightMargin: 15
                        model: ["OFF", "BORDER", "CORNERS"]
                        onActivated: {
                            _controllerCore.showUserRoi1 = currentIndex
                        }
                    }
                }
                RowLayout {
                    Layout.fillWidth: true
                    Layout.margins: 5
                    Layout.rightMargin: 20
                    Text {
                        Layout.alignment: Qt.AlignLeft
                        Layout.fillWidth: true
                        color: "white"
                        font.pixelSize: 12
                        text: qsTr("Show ROI2: ") + _translator.emptyString
                        font.bold: true
                    }
                    CustomComboBox {
                        Layout.alignment: Qt.AlignRight
                        id: roi2Combo
                        Layout.rightMargin: 15
                        model: ["OFF", "BORDER", "CORNERS"]
                        onActivated: {
                            _controllerCore.showUserRoi2 = currentIndex
                        }
                    }
                }
                RowLayout {
                    Layout.fillWidth: true
                    Layout.margins: 5
                    Layout.rightMargin: 20
                    Text {
                        Layout.alignment: Qt.AlignLeft
                        Layout.fillWidth: true
                        color: "white"
                        font.pixelSize: 12
                        text: qsTr("Show ROI3: ") + _translator.emptyString
                        font.bold: true
                    }
                    CustomComboBox {
                        Layout.alignment: Qt.AlignRight
                        id: roi3Combo
                        Layout.rightMargin: 15
                        model: ["OFF", "BORDER", "CORNERS"]
                        onActivated: {
                            _controllerCore.showUserRoi3 = currentIndex
                        }
                    }
                }
                RowLayout {
                    Layout.fillWidth: true
                    Layout.margins: 5
                    Layout.rightMargin: 20
                    Text {
                        Layout.alignment: Qt.AlignLeft
                        Layout.fillWidth: true
                        color: "white"
                        font.pixelSize: 12
                        text: qsTr("Show ROI4: ") + _translator.emptyString
                        font.bold: true
                    }
                    CustomComboBox {
                        Layout.alignment: Qt.AlignRight
                        id: roi4Combo
                        Layout.rightMargin: 15
                        model: ["OFF", "BORDER", "CORNERS"]
                        onActivated: {
                            _controllerCore.showUserRoi4 = currentIndex
                        }
                    }
                }
                Connections {
                    target: _controllerCore
                    function onShowUserRoiChanged() {
                        roi1Combo.currentIndex = _controllerCore.showUserRoi1
                        roi2Combo.currentIndex = _controllerCore.showUserRoi2
                        roi3Combo.currentIndex = _controllerCore.showUserRoi3
                        roi4Combo.currentIndex = _controllerCore.showUserRoi4
                    }
                }

                Rectangle {
                    visible: true
                    Layout.fillWidth: true
                    height: 1
                    color: "black"
                    Layout.rightMargin: 20
                }
            },
            ColumnLayout {
                id: modeColumn
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: roiColumn.bottom
                PopupItem {
                    Layout.preferredWidth: parent.width
                    itemText: qsTr("Temperature Mode") + _translator.emptyString
                    visible: true
                    Layout.preferredHeight: 25
                }
                RowLayout {
                    Layout.fillWidth: true
                    Layout.margins: 5
                    Layout.rightMargin: 20
                    Text {
                        Layout.alignment: Qt.AlignLeft
                        Layout.fillWidth: true
                        color: "white"
                        font.pixelSize: 12
                        text: qsTr("Mode: ") + _translator.emptyString
                        font.bold: true
                    }
                    CustomComboBox {
                        Layout.alignment: Qt.AlignRight
                        id: tempModeCombo
                        Layout.rightMargin: 10
                        model: ["CBT", "SST", "ISO", "AXI", "LONG-SST", "LONG-CBT", "LONG-AXI"]
                        onActivated: {
                            _controllerCore.temperatureMode = currentIndex
                        }
                    }
                    Connections {
                        target: _controllerCore
                        function onTemperatureModeChanged(val) {
                            tempModeCombo.currentIndex = val
                        }
                    }
                }
                Rectangle {
                    color: "transparent"
                    Layout.preferredHeight: 40
                    Layout.preferredWidth: parent.width - 4
                    Layout.rightMargin: 20
                }
            }
        ]
    }
}
