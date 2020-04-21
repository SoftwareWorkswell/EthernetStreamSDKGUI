import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "../"

ControlPopup{

    ColumnLayout{
        width: parent.width
        PopupItem{

            width: parent.width
            itemText: "Main camera"
            Layout.preferredHeight: 25
        }
        RowLayout {
            Layout.preferredHeight: 30
            Layout.alignment: Qt.AlignHCenter
            CustomRadioButton1{
                id: mainCameraThermalRadio
                text: "Thermal"
                checked: _controllerCore.mainCamera === "THERMAL"
                onClicked:{
                    if(checked)
                        _controllerCore.mainCamera = "THERMAL"
                }
            }

            CustomRadioButton1{
                id: mainCameraVisibleRadio
                text: "Visible"
                checked: _controllerCore.mainCamera === "VISIBLE"
                onClicked:{
                    if(checked)
                        _controllerCore.mainCamera = "VISIBLE"
                }
            }
        }
        PopupItem{

            width: parent.width
            itemText: "Calibration"
            Layout.preferredHeight: 25
        }
        RowLayout {
            Layout.preferredHeight: 50
            Layout.alignment: Qt.AlignHCenter
            PopupButton{
                width: parent.width * 0.65
                height: parent.height * 0.55
                anchors.centerIn: parent
                text: "Force"
                onClicked: _controllerCore.forceCalibration()
            }
        }
        PopupItem{

            width: parent.width
            itemText: "Level Of Acception"
            Layout.preferredHeight: 25
        }
        RowLayout {
            Layout.preferredHeight: 20
            Layout.alignment: Qt.AlignHCenter

            Rectangle{
                id: acceptionBackground
                Layout.preferredHeight: 15
                Layout.preferredWidth: 50
                TextInput {
                    anchors.fill: parent
                    id: acceptionValue
                    text: _controllerCore.levelOfAcception.toFixed(2)
                    font.bold: true
                    //validator: RegExpValidator { regExp: /^0\.\d\d/}
                    onTextChanged: {
                        _controllerCore.levelOfAcception = acceptionValue.text
                    }
                }
                Connections{
                    target: _controllerCore
                    onLevelOfAcceptionChanged: {
                        acceptionValue.text = _controllerCore.levelOfAcception
                    }
                }
            }
        }
    }
}
