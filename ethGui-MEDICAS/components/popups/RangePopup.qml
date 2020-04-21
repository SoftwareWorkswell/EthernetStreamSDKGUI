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
                text: "Min Temperature: "
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
                        _controllerCore.manualRange1 = minTemperatureValue.text
                    }
                }
                Connections{
                    target: _controllerCore
                    onManualRange1Changed: {
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
                text: "Max Temperature: "
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
                        _controllerCore.manualRange2 = maxTemperatureValue.text
                    }
                }
                Connections{
                    target: _controllerCore
                    onManualRange2Changed: {
                        maxTemperatureValue.text =  parseFloat(_controllerCore.manualRange2).toFixed(2)
                    }
                }
            }
        }
    }

}
