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
    property int roiIdx: -1
    onRoiIdxChanged: {
        _controllerCore.updateRoiAreaLimits()
        userRoiXValue.value = parseInt(_controllerCore.getUserRoiX(root.roiIdx, selectedCorner.currentIndex))
        userRoiYValue.value = parseInt(_controllerCore.getUserRoiY(root.roiIdx, selectedCorner.currentIndex))
        _controllerCore.setUserRoiSelectedCorner(root.roiIdx, selectedCorner.currentIndex)
    }
    RoiUserErrorMessage{
        id: errorMessage
    }
        RowLayout{
            anchors.fill: parent
            spacing: 5
            RowLayout{
                Layout.alignment: Qt.AlignLeft
                Text{
                    color: "white"
                    font.pixelSize: 12
                    text: qsTr("Corner:") + _translator.emptyString
                    font.bold: true
                }
                CustomComboBox {
                    id: selectedCorner
                    Layout.preferredHeight: 30
                    Layout.preferredWidth: 150
                    Layout.minimumWidth: 50
                    Layout.maximumWidth: 200
                    Layout.fillWidth: true
                    model: ["TOP-LEFT", "TOP-RIGHT", "BOTTOM-LEFT", "BOTTOM-RIGHT"]
                    onCurrentIndexChanged: {
                        _controllerCore.setUserRoiSelectedCorner(root.roiIdx, currentIndex)
                        userRoiXValue.value = parseInt(_controllerCore.getUserRoiX(root.roiIdx, selectedCorner.currentIndex))
                        userRoiYValue.value = parseInt(_controllerCore.getUserRoiY(root.roiIdx, selectedCorner.currentIndex))
                    }
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignCenter
                Text{
                    color: "white"
                    font.pixelSize: 12
                    text: "X: "
                    font.bold: true
                }
                SpinBox {
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 120
                    Layout.minimumWidth: 120
                    Layout.maximumWidth: 200
                    Layout.fillWidth: true
                    id:  userRoiXValue
                    value: parseInt(_controllerCore.getUserRoiX(root.roiIdx, selectedCorner.currentIndex))
                    font.bold: true
                    from: _controllerCore.roiAreaMinX
                    to: _controllerCore.roiAreaMaxX
                    editable: true

                    //validator: RegExpValidator { regExp: /^0\.\d\d/}
                    onValueModified: {
                        _controllerCore.setUserRoiX(root.roiIdx, selectedCorner.currentIndex, value)
                        _controllerCore.updateRoiAreaLimits()
                    }
                }

                Text{
                    color: "white"
                    font.pixelSize: 12
                    text: "Y: "
                    font.bold: true
                }

                SpinBox {
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 120
                    Layout.minimumWidth: 120
                    Layout.maximumWidth: 200
                    Layout.fillWidth: true
                    id:  userRoiYValue
                    font.bold: true
                    from: _controllerCore.roiAreaMinY
                    to: _controllerCore.roiAreaMaxY
                    editable: true
                    value: parseInt(_controllerCore.getUserRoiY(root.roiIdx, selectedCorner.currentIndex))
                    onValueModified: {
                        _controllerCore.setUserRoiY(root.roiIdx, selectedCorner.currentIndex, value)
                        _controllerCore.updateRoiAreaLimits()
                    }
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignRight
                PopupButton{
                    Layout.minimumWidth: 40
                    Layout.maximumWidth: 100
                    Layout.fillWidth: true
                    text: qsTr("Confirm") + _translator.emptyString
                    onClicked:{
                        if(_controllerCore.sendUserRoiCoordinates(root.roiIdx))
                        {
                            _controllerCore.setCurrentRoiIdx(-1)
                            root.closingForm()
                        }
                        else
                            errorMessage.open()
                    }
                }

                PopupButton{
                    Layout.minimumWidth: 40
                    Layout.maximumWidth: 100
                    Layout.fillWidth: true
                    text: qsTr("Hide") + _translator.emptyString
                    onClicked:{
                        _controllerCore.sendOriginalUserRoiCoordinates(root.roiIdx)
                        _controllerCore.setCurrentRoiIdx(-1)
                        root.closingForm()
                    }
                }
            }
        }

        Action{
            shortcut: "W"
            enabled: root.visible
            onTriggered:{
                userRoiYValue.decrease()
                userRoiYValue.valueModified()
            }
        }
        Action{
            shortcut: "S"
            enabled: root.visible
            onTriggered:{
                userRoiYValue.increase()
                userRoiYValue.valueModified()
            }
        }
        Action{
            shortcut: "A"
            enabled: root.visible
            onTriggered:{
                userRoiXValue.decrease()
                userRoiXValue.valueModified()
            }
        }

        Action{
            shortcut: "D"
            enabled: root.visible
            onTriggered:{
                userRoiXValue.increase()
                userRoiXValue.valueModified()
            }
        }

        Action{
            shortcut: StandardKey.InsertParagraphSeparator
            enabled: root.visible
            onTriggered:{
                if(_controllerCore.sendUserRoiCoordinates(root.roiIdx))
                {
                    _controllerCore.setCurrentRoiIdx(-1)
                    root.closingForm()
                }
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
