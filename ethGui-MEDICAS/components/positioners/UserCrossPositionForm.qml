import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "../"
import "../popups/"

Rectangle{
    id: root
    color: "transparent"
    signal closingForm()
    CrossUserErrorMessage{
        id: errorMessage
    }
        RowLayout
        {
            anchors.fill: parent
            RowLayout{
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                Layout.margins: 5
                spacing: 5
                Text{
                    Layout.alignment: Qt.AlignLeft
                    color: "white"
                    font.pixelSize: 12
                    text: "X: "
                    font.bold: true
                }
                SpinBox {
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 120
                    id:  crossUserXValue
                    value: parseInt(_controllerCore.crossUserX)
                    font.bold: true
                    from: _controllerCore.userCrossMinX
                    to: _controllerCore.userCrossMaxX
                    editable: true
                    //validator: RegExpValidator { regExp: /^0\.\d\d/}
                    onValueModified: {
                        _controllerCore.crossUserX = value
                        _controllerCore.updateUserCrossLimits()
                    }
                }
                Connections{
                    target: _controllerCore
                    function onCrossUserXChanged(val) {
                        crossUserXValue.value = val
                        _controllerCore.updateUserCrossLimits()
                    }
                }
                Text{
                    Layout.alignment: Qt.AlignLeft
                    color: "white"
                    font.pixelSize: 12
                    text: "Y: "
                    font.bold: true
                }
                SpinBox {
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 120
                    id:  crossUserYValue
                    font.bold: true
                    from: _controllerCore.userCrossMinY
                    to: _controllerCore.userCrossMaxY
                    editable: true
                    value:  parseInt(_controllerCore.crossUserY)
                    onValueModified:{
                        _controllerCore.crossUserY = value
                        _controllerCore.updateUserCrossLimits()
                    }
                }
                Connections{
                    target: _controllerCore
                    function onCrossUserYChanged(val) {
                        crossUserYValue.value = val
                        _controllerCore.updateUserCrossLimits()
                    }
                }

            }
            RowLayout {
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignRight
                spacing: 5
                PopupButton{
                    text: qsTr("Confirm") + _translator.emptyString
                    onClicked: {
                        if(_controllerCore.sendCrossUserCoordinates())
                            root.closingForm()
                        else
                            errorMessage.open()
                    }
                }
                PopupButton{
                    text: qsTr("Hide") + _translator.emptyString
                    onClicked:{
                        root.closingForm()
                    }
                }
            }
        }

        Action{
            shortcut: "W"
            enabled: root.visible
            onTriggered:{
                crossUserYValue.decrease()
                crossUserYValue.valueModified()
            }
        }
        Action{
            shortcut: "S"
            enabled: root.visible
            onTriggered:{
                crossUserYValue.increase()
                crossUserYValue.valueModified()
            }
        }
        Action{
            shortcut: "A"
            enabled: root.visible
            onTriggered:{
                crossUserXValue.decrease()
                crossUserXValue.valueModified()
            }
        }

        Action{
            shortcut: "D"
            enabled: root.visible
            onTriggered:{
                crossUserXValue.increase()
                crossUserXValue.valueModified()
            }
        }

        Action{
            shortcut: StandardKey.InsertParagraphSeparator
            enabled: root.visible
            onTriggered:{
                if(_controllerCore.sendCrossUserCoordinates())
                    root.closingForm()
                else
                    errorMessage.open()
            }
        }


        Action{
            shortcut: StandardKey.Cancel
            enabled: root.visible
            onTriggered:{
                _controllerCore.setCurrentRoiIdx(-1)
                root.closingForm()
            }
        }
}
