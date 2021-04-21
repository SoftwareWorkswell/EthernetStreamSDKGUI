import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "../"

ControlPopup{
    id: rangePopup
    width: parent.width

    RangeErrorDialog
    {
        id: rangeErrorDialog
        Connections {
            target: _controllerCore
            function onRangeError() {
                rangeErrorDialog.open()
            }
        }
    }

    ColumnLayout{
        width: parent.width

        PopupItem{

            width: parent.width
            Layout.preferredHeight: 25
        }

        RowLayout{
            Layout.fillWidth: true
            Layout.margins: 5
            Text{
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                color: "white"
                font.pixelSize: 12
                text: qsTr("Min Temperature: ") + _translator.emptyString
                font.bold: true
            }
            Rectangle{
                id: minTemperaturBackground
                Layout.preferredHeight: 15
                Layout.preferredWidth: 50
                TextInput {
                    anchors.fill: parent
                    id: minTemperatureValue
                    font.bold: true
                   // validator: RegExpValidator { regExp: /^0\.\d\d/}
                    onAccepted: {
                        var inText = minTemperatureValue.text
                        if(inText.includes(','))
                            inText = inText.replace(',', '.')
                        _controllerCore.manualRange1 = inText
                    }
                }
                Connections{
                    target: _controllerCore
                    function onManualRange1Changed() {
                        minTemperatureValue.text =  parseFloat(_controllerCore.manualRange1).toFixed(2)
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
                text: qsTr("Max Temperature: ") + _translator.emptyString
                font.bold: true
            }
            Rectangle{
                id: maxTemperaturBackground
                Layout.preferredHeight: 15
                Layout.preferredWidth: 50
                TextInput {
                    anchors.fill: parent
                    id: maxTemperatureValue
                    font.bold: true
                  //  validator: RegExpValidator { regExp: /^0\.\d\d/}
                    onAccepted: {
                        var inText = maxTemperatureValue.text
                        if(inText.includes(','))
                            inText = inText.replace(',', '.')
                        _controllerCore.manualRange2 = inText
                    }
                }
                Connections{
                    target: _controllerCore
                    function onManualRange2Changed() {
                        maxTemperatureValue.text =  parseFloat(_controllerCore.manualRange2).toFixed(2)
                    }
                }
            }
        }
    }

}
