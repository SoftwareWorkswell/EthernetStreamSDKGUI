import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "../"
import "../popups/"

Rectangle {
    id: root
    color: "transparent"
    signal closingForm
    RowLayout {
        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 5
            Text {
                Layout.alignment: Qt.AlignLeft
                color: "white"
                font.pixelSize: 12
                text: "X: "
                font.bold: true
            }
            Rectangle {
                id: blackbodyXBackground
                Layout.preferredHeight: 20
                Layout.preferredWidth: 120
                SpinBox {
                    anchors.fill: parent
                    id: blackbodyXValue
                    value: parseInt(_controllerCore.blackbodyRoiX)
                    font.bold: true
                    from: _controllerCore.blackBodyMinX
                    to: _controllerCore.blackBodyMaxX
                    editable: true
                    //validator: RegExpValidator { regExp: /^0\.\d\d/}
                    onValueModified: {
                        _controllerCore.blackbodyRoiX = value
                        _controllerCore.updateBlackBodyLimits()
                    }
                }
                Connections {
                    target: _controllerCore
                    function onBlackbodyRoiXChanged() {
                        blackbodyXValue.value = _controllerCore.blackbodyRoiX
                        _controllerCore.updateBlackBodyLimits()
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 5
            Text {
                Layout.alignment: Qt.AlignLeft
                color: "white"
                font.pixelSize: 12
                text: "Y: "
                font.bold: true
            }
            Rectangle {
                id: blackbodyYBackground
                Layout.preferredHeight: 20
                Layout.preferredWidth: 120
                SpinBox {
                    anchors.fill: parent
                    id: blackbodyYValue
                    font.bold: true
                    from: _controllerCore.blackBodyMinY
                    to: _controllerCore.blackBodyMaxY
                    editable: true
                    value: parseInt(_controllerCore.blackbodyRoiX)
                    onValueModified: {
                        _controllerCore.blackbodyRoiY = value
                        _controllerCore.updateBlackBodyLimits()
                    }
                }
                Connections {
                    target: _controllerCore
                    function onBlackbodyRoiYChanged() {
                        blackbodyYValue.value = _controllerCore.blackbodyRoiY
                        _controllerCore.updateBlackBodyLimits()
                    }
                }
            }
        }
        RowLayout {
            Layout.fillHeight: true
            PopupButton {
                text: qsTr("Confirm") + _translator.emptyString
                onClicked: {
                    _controllerCore.sendBlackbodyCoordinates()
                    _controllerCore.setCurrentRoiIdx(-1)
                    root.closingForm()
                }
            }
        }
        RowLayout {
            Layout.fillHeight: true
            PopupButton {
                text: qsTr("Hide") + _translator.emptyString
                onClicked: {
                    _controllerCore.setCurrentRoiIdx(-1)
                    _controllerCore.sendOriginalBlackbodyCoordinates()
                    root.closingForm()
                }
            }
        }
    }

    Action {
        shortcut: "W"
        enabled: root.visible
        onTriggered: {
            blackbodyYValue.decrease()
            blackbodyYValue.valueModified()
        }
    }
    Action {
        shortcut: "S"
        enabled: root.visible
        onTriggered: {
            blackbodyYValue.increase()
            blackbodyYValue.valueModified()
        }
    }
    Action {
        shortcut: "A"
        enabled: root.visible
        onTriggered: {
            blackbodyXValue.decrease()
            blackbodyXValue.valueModified()
        }
    }

    Action {
        shortcut: "D"
        enabled: root.visible
        onTriggered: {
            blackbodyXValue.increase()
            blackbodyXValue.valueModified()
        }
    }

    Action {
        shortcut: StandardKey.InsertParagraphSeparator
        enabled: root.visible
        onTriggered: {
            _controllerCore.sendBlackbodyCoordinates()
            _controllerCore.setCurrentRoiIdx(-1)
            root.closingForm()
        }
    }

    Action {
        shortcut: StandardKey.Cancel
        enabled: root.visible
        onTriggered: {
            _controllerCore.setCurrentRoiIdx(-1)
            _controllerCore.setSpecialPheripheralOutput(false)
            root.closingForm()
        }
    }
}
