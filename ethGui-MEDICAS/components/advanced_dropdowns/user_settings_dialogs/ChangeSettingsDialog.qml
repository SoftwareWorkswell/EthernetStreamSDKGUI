import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import "../"
import "../../"

Dialog {
    id: changeSettingsDialog
    title: "Change user settings"
    enabled: visible
    focus: false
    parent: userDropDown
    implicitWidth: parent.width
    implicitHeight: 400
    modal: true
    background: Rectangle {
        color: "#33333D"
        opacity: 75
    }
    contentItem: ColumnLayout {
        Layout.fillWidth: true
        RowLayout {
            Layout.fillWidth: true
            Text {
                id: userText
                text: qsTr("User: ") + _translator.emptyString
                font.pixelSize: 12
                Layout.fillWidth: true
                color: "white"
                font.bold: true
            }
            CustomComboBox {
                Layout.rightMargin: 20
                id: selectedUser
                model: ["USER1", "USER2", "USER3", "USER4"]
                currentIndex: 0
                onCurrentTextChanged: _controllerCore.fetchUser(currentText)
            }
        }
        RowLayout{
            Layout.fillWidth: true
            ColumnLayout{
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.fillWidth: true
                PopupCheckBox{
                    id: rangeCheck
                    text: qsTr("Range") + _translator.emptyString
                    checked: _controllerCore.currentUserSettings.range
                }
                PopupCheckBox{
                    id: functionsCheck
                    text: qsTr("Functions") + _translator.emptyString
                    checked: _controllerCore.currentUserSettings.functions
                }
                PopupCheckBox{
                    id: captureCheck
                    text: qsTr("Capture") + _translator.emptyString
                    checked: _controllerCore.currentUserSettings.capture
                }
                PopupCheckBox{
                    id: paletteCheck
                    text: qsTr("Palette") + _translator.emptyString
                    checked: _controllerCore.currentUserSettings.palette
                }
                PopupCheckBox{
                    id: measureCheck
                    text: qsTr("Measure") + _translator.emptyString
                    checked: _controllerCore.currentUserSettings.measure
                }
                PopupCheckBox{
                    id: advancedVisibleCheck
                    text: qsTr("Advanced") + _translator.emptyString
                    checked: _controllerCore.currentUserSettings.advancedVisible
                }

            }
            ColumnLayout{
                Layout.alignment: Qt.AlignTop | Qt.AlignRight
                Layout.fillWidth: true
                PopupCheckBox{
                    id: advancedImageVisibleCheck
                    text: qsTr("Advanced image") + _translator.emptyString
                    checked: _controllerCore.currentUserSettings.advancedImageVisible
                }
                PopupCheckBox{
                    id: advancedAlarmsCheck
                    text: qsTr("Advanced alarms") + _translator.emptyString
                    checked: _controllerCore.currentUserSettings.advancedAlarms
                }
                PopupCheckBox{
                    id: advancedMemoryCheck
                    text: qsTr("Advanced memory") + _translator.emptyString
                    checked: _controllerCore.currentUserSettings.advancedMemory
                }
                PopupCheckBox{
                    id: advancedSystemCheck
                    text: qsTr("Advanced system") + _translator.emptyString
                    checked: _controllerCore.currentUserSettings.advancedSystem
                }
                PopupCheckBox{
                    id: advancedRoiCheck
                    text: qsTr("Advanced ROI") + _translator.emptyString
                    checked: _controllerCore.currentUserSettings.advancedRoi
                }
                PopupCheckBox{
                    id: advancedDioCheck
                    text: qsTr("Advanced DIO") + _translator.emptyString
                    checked: _controllerCore.currentUserSettings.advancedDio
                }
                PopupCheckBox{
                    id: advancedLayoutCheck
                    text: qsTr("Advanced layout") + _translator.emptyString
                    checked: _controllerCore.currentUserSettings.advancedLayout
                }
            }
            Connections{
                target: _controllerCore
                function onCurrentUserSettingsChanged(){
                    rangeCheck.checked = _controllerCore.currentUserSettings.range
                    functionsCheck.checked = _controllerCore.currentUserSettings.functions
                    captureCheck.checked = _controllerCore.currentUserSettings.capture
                    paletteCheck.checked = _controllerCore.currentUserSettings.palette
                    measureCheck.checked = _controllerCore.currentUserSettings.measure
                    advancedVisibleCheck.checked = _controllerCore.currentUserSettings.advancedVisible
                    advancedImageVisibleCheck.checked = _controllerCore.currentUserSettings.advancedImageVisible
                    advancedAlarmsCheck.checked = _controllerCore.currentUserSettings.advancedAlarms
                    advancedMemoryCheck.checked = _controllerCore.currentUserSettings.advancedMemory
                    advancedSystemCheck.checked = _controllerCore.currentUserSettings.advancedSystem
                    advancedRoiCheck.checked = _controllerCore.currentUserSettings.advancedRoi
                    advancedDioCheck.checked = _controllerCore.currentUserSettings.advancedDio
                    advancedLayoutCheck.checked = _controllerCore.currentUserSettings.advancedLayout
                }
            }
        }

    }
    footer: RowLayout{
        Layout.fillWidth: true
        Text {
            color: maCloseLogin.containsMouse ? "orange" : "white"
            font.bold: true
            font.pixelSize: 12
            text: qsTr("Close") + _translator.emptyString
            Layout.alignment: Qt.AlignLeft
            Layout.leftMargin: 10
            Layout.bottomMargin: 10
            MouseArea {
                id: maCloseLogin
                anchors.fill: parent
                hoverEnabled: true
                onClicked: changeSettingsDialog.close()
            }
        }
        Text {
            color: maLoginBtn.containsMouse ? "orange" : "white"
            font.bold: true
            font.pixelSize: 12
            text: qsTr("Apply") + _translator.emptyString
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 10
            Layout.bottomMargin: 10
            MouseArea {
                id: maLoginBtn
                anchors.fill: parent
                hoverEnabled: true
                onClicked: _controllerCore.setCurrentUserSettings(selectedUser.currentText,
                                                                  rangeCheck.checked,
                                                                  functionsCheck.checked,
                                                                  captureCheck.checked,
                                                                  paletteCheck.checked,
                                                                  measureCheck.checked,
                                                                  advancedVisibleCheck.checked,
                                                                  advancedImageVisibleCheck.checked,
                                                                  advancedAlarmsCheck.checked,
                                                                  advancedMemoryCheck.checked,
                                                                  advancedSystemCheck.checked,
                                                                  advancedRoiCheck.checked,
                                                                  advancedDioCheck.checked,
                                                                  advancedLayoutCheck.checked)
            }
        }
    }
    Action{
        shortcut: StandardKey.InsertParagraphSeparator
        enabled: changeSettingsDialog.opened
        onTriggered:{
            _controllerCore.setCurrentUserSettings(selectedUser.currentText,
                                                                             rangeCheck.checked,
                                                                             functionsCheck.checked,
                                                                             captureCheck.checked,
                                                                             paletteCheck.checked,
                                                                             measureCheck.checked,
                                                                             advancedVisibleCheck.checked,
                                                                             advancedImageVisibleCheck.checked,
                                                                             advancedAlarmsCheck.checked,
                                                                             advancedMemoryCheck.checked,
                                                                             advancedSystemCheck.checked,
                                                                             advancedRoiCheck.checked,
                                                                             advancedDioCheck.checked,
                                                                             advancedLayoutCheck.checked)
        }
    }
    Action{
        shortcut: StandardKey.Cancel
        enabled: changeSettingsDialog.opened
        onTriggered: changeSettingsDialog.close()
    }
}

