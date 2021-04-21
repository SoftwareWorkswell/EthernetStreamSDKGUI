import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import "../"
import "../../"

Dialog {
    id: roiNamesDialog
    title: "Set Roi Names"
    parent: userDropDown
    enabled: visible
    focus: false
    implicitWidth: parent.width
    implicitHeight: 150
    modal: true
    background: Rectangle {
        color: "#33333D"
        opacity: 75
    }
    contentItem: ColumnLayout {
        Layout.minimumWidth: 400
        RowLayout {
            Layout.fillWidth: true
            Text {
                id: roi1Label
                text: qsTr("ROI1: ") + _translator.emptyString
                font.pixelSize: 12
                Layout.fillWidth: true
                color: "white"
                font.bold: true
            }

            Rectangle {
                Layout.preferredHeight: roi1Label.height
                Layout.preferredWidth: 140
                TextInput {
                    anchors.fill: parent
                    anchors.leftMargin: 3
                    maximumLength: 15
                    id: roi1Text
                    font.bold: true
                    verticalAlignment: Qt.AlignVCenter
                    text: "ROI1"
                    color: "gray"
                    MouseArea {
                        anchors.fill: parent
                        id: maRoi1
                        onClicked: {
                            roi1Text.forceActiveFocus()
                            roi1Text.text = ""
                            roi1Text.color = "black"
                        }
                    }
                    onFocusChanged: if (!activeFocus) {
                                        roi1Text.color = "gray"
                                    }
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Text {
                id: roi2Label
                text: qsTr("ROI2: ") + _translator.emptyString
                font.pixelSize: 12
                Layout.fillWidth: true
                color: "white"
                font.bold: true
            }

            Rectangle {
                Layout.preferredHeight: roi1Label.height
                Layout.preferredWidth: 140
                TextInput {
                    anchors.fill: parent
                    anchors.leftMargin: 3
                    maximumLength: 15
                    id: roi2Text
                    font.bold: true
                    verticalAlignment: Qt.AlignVCenter
                    text: "ROI2"
                    color: "gray"
                    MouseArea {
                        anchors.fill: parent
                        id: maRoi2
                        onClicked: {
                            roi2Text.forceActiveFocus()
                            roi2Text.text = ""
                            roi2Text.color = "black"
                        }
                    }
                    onFocusChanged: if (!activeFocus) {
                                        roi2Text.color = "gray"
                                    }
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Text {
                text: qsTr("ROI3: ") + _translator.emptyString
                font.pixelSize: 12
                Layout.fillWidth: true
                color: "white"
                font.bold: true
            }

            Rectangle {
                Layout.preferredHeight: roi1Label.height
                Layout.preferredWidth: 140
                TextInput {
                    anchors.fill: parent
                    anchors.leftMargin: 3
                    maximumLength: 15
                    id: roi3Text
                    font.bold: true
                    verticalAlignment: Qt.AlignVCenter
                    text: "ROI3"
                    color: "gray"
                    MouseArea {
                        anchors.fill: parent
                        id: maRoi3
                        onClicked: {
                            roi3Text.forceActiveFocus()
                            roi3Text.text = ""
                            roi3Text.color = "black"
                        }
                    }
                    onFocusChanged: if (!activeFocus) {
                                        roi3Text.color = "gray"
                                    }
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Text {
                text: qsTr("ROI4: ") + _translator.emptyString
                font.pixelSize: 12
                Layout.fillWidth: true
                color: "white"
                font.bold: true
            }

            Rectangle {
                Layout.preferredHeight: roi1Label.height
                Layout.preferredWidth: 140
                TextInput {
                    anchors.fill: parent
                    anchors.leftMargin: 3
                    maximumLength: 15
                    id: roi4Text
                    font.bold: true
                    verticalAlignment: Qt.AlignVCenter
                    text: "ROI4"
                    color: "gray"
                    MouseArea {
                        anchors.fill: parent
                        id: maRoi4
                        onClicked: {
                            roi4Text.forceActiveFocus()
                            roi4Text.text = ""
                            roi4Text.color = "black"
                        }
                    }
                    onFocusChanged: if (!activeFocus) {
                                        roi4Text.color = "gray"
                                    }
                }
            }
        }
    }
    footer: RowLayout {
        Layout.fillWidth: true
        Text {
            color: maClose.containsMouse ? "orange" : "white"
            font.bold: true
            font.pixelSize: 12
            text: qsTr("Cancel") + _translator.emptyString
            Layout.alignment: Qt.AlignLeft
            Layout.leftMargin: 10
            Layout.bottomMargin: 10
            MouseArea {
                id: maClose
                anchors.fill: parent
                hoverEnabled: true
                onClicked: roiNamesDialog.close()
            }
        }
        Text {
            color: maSaveBtn.containsMouse ? "orange" : "white"
            font.bold: true
            font.pixelSize: 12
            text: qsTr("Save") + _translator.emptyString
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 10
            Layout.bottomMargin: 10
            MouseArea {
                id: maSaveBtn
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    if (_controllerCore.setRoiNames(roi1Text.text,
                                                    roi2Text.text,
                                                    roi3Text.text,
                                                    roi4Text.text)) {
                        roiNamesDialog.close()
                    }
                }
            }
        }
    }
    Action {
        shortcut: StandardKey.InsertParagraphSeparator
        enabled: roiNamesDialog.opened
        onTriggered: if (_controllerCore.setRoiNames(roi1Text.text,
                                                     roi2Text.text,
                                                     roi3Text.text,
                                                     roi4Text.text)) {
                         roiNamesDialog.close()
                     }
    }
    Action {
        shortcut: StandardKey.Cancel
        enabled: roiNamesDialog.opened
        onTriggered: roiNamesDialog.close()
    }
}
