import QtQuick 2.0
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "../"
import "./user_settings_dialogs"

ColumnLayout {
    Layout.fillWidth: true
    Layout.leftMargin: 5

    RowLayout {
        Text {
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            color: "white"
            font.pixelSize: 12
            Layout.fillWidth: true
            text: qsTr("Current time:") + _translator.emptyString
        }

        Text {
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            Layout.alignment: Qt.AlignRight
            color: "white"
            font.pixelSize: 12
            text: _controllerCore.systemTime
        }
    }

    RowLayout {
        Text {
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            color: "white"
            font.pixelSize: 12
            Layout.fillWidth: true
            text: qsTr("Current Date:") + _translator.emptyString
        }

        Text {
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            Layout.alignment: Qt.AlignRight
            color: "white"
            font.pixelSize: 12
            text: _controllerCore.systemDate
        }
    }

    Text {
        id: changeDateTime
        color: maChangeDateTime.containsMouse ? "orange" : "white"
        font.bold: true
        font.pixelSize: 12
        text: qsTr("Change date and time") + _translator.emptyString
        Layout.fillWidth: true
        Layout.margins: 5
        MouseArea {
            id: maChangeDateTime
            anchors.fill: parent
            hoverEnabled: true
            onClicked: timeDateDialog.open()
        }
    }

    RowLayout {
        Layout.fillWidth: true
        Layout.margins: 5
        Text {
            text: qsTr("Units: ") + _translator.emptyString
            font.pixelSize: 12
            Layout.fillWidth: true
            color: "white"
            font.bold: false
        }

        CustomComboBox {
            id: selectedUnit
            model: ["CELSIUS", "FAHRENHEIT"]
            onActivated: _controllerCore.units = currentText
            Connections {
                target: _controllerCore
                function onUnitsChanged() {
                    if (_controllerCore.units === " °C") {
                        selectedUnit.currentIndex = 0
                    } else if (_controllerCore.units === " °F") {
                        selectedUnit.currentIndex = 1
                    }
                }
            }
        }
    }

    Text {
        text: qsTr("MEDICAS language: ") + _translator.emptyString
        font.pixelSize: 12
        Layout.fillWidth: true
        color: "white"
        font.bold: true
        Layout.margins: 5
        Layout.bottomMargin: 0
    }
    CustomComboBox {
        id: selectedLanguage
        model: ["ENGLISH", "CZECH", "DUTCH", "PORTUGUESE", "SPANISH", "THAI", "TURKISH", "BULGARIAN", "CHINESE-SIMPLIFIED"]
        Layout.preferredWidth: parent.width - 40
        Layout.alignment: Qt.AlignRight
        Layout.margins: 5
        Layout.topMargin: 0
        onActivated: {
            _translator.selectLanguage(currentText)
            _controllerCore.language = currentText
        }
        Connections {
            target: _controllerCore
            function onLanguageChanged() {
                _translator.selectLanguage(_controllerCore.language)
                if (_controllerCore.language === "ENGLISH") {
                    selectedLanguage.currentIndex = 0
                } else if (_controllerCore.language === "CZECH") {
                    selectedLanguage.currentIndex = 1
                } else if (_controllerCore.language === "DUTCH") {
                    selectedLanguage.currentIndex = 2
                } else if (_controllerCore.language === "PORTUGUESE") {
                    selectedLanguage.currentIndex = 3
                } else if (_controllerCore.language === "SPANISH") {
                    selectedLanguage.currentIndex = 4
                } else if (_controllerCore.language === "THAI") {
                    selectedLanguage.currentIndex = 5
                } else if (_controllerCore.language === "TURKISH") {
                    selectedLanguage.currentIndex = 6
                } else if (_controllerCore.language === "BULGARIAN") {
                    selectedLanguage.currentIndex = 7
                } else if (_controllerCore.language === "CHINESE-SIMPLIFIED") {
                    selectedLanguage.currentIndex = 8
                }
            }
        }
    }
    Text {
        id: languageWarningText
        text: "Selected language is not supported\nby GUI application. Language will be\nchanged only in camera."
        visible: (selectedLanguage.currentText !== "CZECH"
                  && selectedLanguage.currentText !== "ENGLISH")
        Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
        color: "red"
        font.pointSize: 10
        font.bold: true
    }

    Text {
        id: rebtText
        color: maRbt.containsMouse ? "orange" : "white"
        font.bold: true
        font.pixelSize: 12
        Layout.topMargin: 5
        Layout.leftMargin: 5
        Layout.rightMargin: 5
        text: qsTr("Reboot System") + _translator.emptyString
        MouseArea {
            id: maRbt
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                rebootWarningText.visible = true
                rebootRow.visible = true
            }
        }
    }
    Text {
        id: rebootWarningText
        text: qsTr("Reboot?\nThis will restart the app\nand make MEDICAS\nunreachable for a while")
              + _translator.emptyString
        color: "red"
        visible: false
        font.pixelSize: 12
    }
    RowLayout {
        id: rebootRow
        visible: false
        Layout.leftMargin: 5
        Layout.rightMargin: 5
        PopupButton {
            text: qsTr("Reboot") + _translator.emptyString
            Layout.preferredHeight: 20
            Layout.preferredWidth: 60
            onClicked: _controllerCore.reboot()
        }
        PopupButton {
            Layout.preferredHeight: 20
            Layout.preferredWidth: 60
            text: qsTr("Cancel") + _translator.emptyString
            onClicked: {
                rebootWarningText.visible = false
                rebootRow.visible = false
            }
        }
    }
    Connections {
        target: _controllerCore
        function onFirmwareVersionChanged(val) {
            if (!_controllerCore.compareFw("1.1.1", val)) {
                rebtText.visible = false
            }
        }
    }
    Text {
        color: maSds.containsMouse ? "orange" : "white"
        font.bold: true
        font.pixelSize: 12
        Layout.leftMargin: 5
        Layout.rightMargin: 5
        Layout.bottomMargin: 5
        text: qsTr("Set Default Settings") + _translator.emptyString
        MouseArea {
            id: maSds
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                defaultWarningText.visible = true
                defaultRow.visible = true
            }
        }
    }
    Text {
        id: defaultWarningText
        text: qsTr("Set default settigns?\nThis will restart the app") + _translator.emptyString
        color: "red"
        visible: false
        font.pixelSize: 12
        Layout.leftMargin: 5
        Layout.rightMargin: 5
    }
    RowLayout {
        id: defaultRow
        visible: false
        Layout.margins: 5
        PopupButton {
            text: qsTr("Set") + _translator.emptyString
            Layout.preferredHeight: 20
            Layout.preferredWidth: 60
            onClicked: _controllerCore.setDefaultSettings()
        }
        PopupButton {
            Layout.preferredHeight: 20
            Layout.preferredWidth: 60
            text: qsTr("Cancel") + _translator.emptyString
            onClicked: {
                defaultWarningText.visible = false
                defaultRow.visible = false
            }
        }
    }

    ChangeSystemTimeDialog {
        id: timeDateDialog
    }
}
