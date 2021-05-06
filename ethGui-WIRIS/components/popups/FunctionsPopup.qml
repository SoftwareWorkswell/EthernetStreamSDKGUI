import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "../"

ControlPopup{
    ColumnLayout{
        width: parent.width
        PopupItem{

            width: parent.width
            itemText: "ZOOM"
            Layout.preferredHeight: 25
        }
        // temporarily not available for gis
        Rectangle{
            Layout.preferredHeight: 0
            visible: false
            Layout.fillWidth:  true
            RowLayout{
                spacing: 5
                anchors.centerIn: parent
                Text {
                    y: 5
                    color: "white"
                    text: "Thermal:"
                }
                ZoomSpinBox{
                    Layout.preferredWidth:  120
                    id: zoomSpinBox
                    editable: false

                    Connections{
                        target: zoomSpinBox.up
                        function onPressedChanged(){
                            if(zoomSpinBox.up.pressed)
                                _controllerCore.changeZoomThermal(true);
                        }
                    }
                    Connections{
                        target: zoomSpinBox.down
                        function onPressedChanged(){
                            if(zoomSpinBox.down.pressed)
                                _controllerCore.changeZoomThermal(false);
                        }
                    }

                    Connections{
                        target: _controllerCore
                        function onCurrentZoomThermalChanged(){
                            zoomSpinBox.zoomValue = Math.round(_controllerCore.currentZoomThermal * 10)/10;
                        }
                    }
                }
            }
            color: "transparent"
        }

        Rectangle{
            Layout.preferredHeight: 40
            Layout.fillWidth:  true
            RowLayout{
                spacing: 5
                anchors.centerIn: parent
                Text {
                    //Layout.preferredHeight: 30
                    y: 5
                    color: "white"
                    text: "Visible: "
                }
                ZoomSpinBox{
                    Layout.preferredWidth:  120
                    id: zoomVisibleSpinBox
                    editable: false

                    Connections{
                        target: zoomVisibleSpinBox.up
                        function onPressedChanged(){
                            if(zoomVisibleSpinBox.up.pressed)
                                _controllerCore.changeZoomVisible(true);
                        }
                    }
                    Connections{
                        target: zoomVisibleSpinBox.down
                        function onPressedChanged(){
                            if(zoomVisibleSpinBox.down.pressed)
                                _controllerCore.changeZoomVisible(false);
                        }
                    }

                    Connections{
                        target: _controllerCore
                        function onCurrentZoomVisibleChanged(){
                            zoomVisibleSpinBox.zoomValue = Math.round(_controllerCore.currentZoomVisible * 10)/10;
                        }
                    }
                }
            }
            color: "transparent"
        }

        PopupItem{

            width: parent.width
            itemText: "Main camera"
            Layout.preferredHeight: 25
        }
        RowLayout {
            Layout.preferredHeight: 20
            Layout.alignment: Qt.AlignHCenter
            Layout.margins: 10
            CustomRadioButton1{
                id: mainCameraThermalRadio
                text: "Thermal"
                checked: mainWindow.mainStreamThermal
                onClicked: mainWindow.mainStreamThermal = true
            }

            CustomRadioButton1{
                id: mainCameraVisibleRadio
                text: "Visible"
                checked: !mainWindow.mainStreamThermal
                onClicked: mainWindow.mainStreamThermal = false
            }
        }
        PopupItem{

            width: parent.width
            itemText: "Focus"
            Layout.preferredHeight: 25
            visible: _controllerCore.type === mainWindow.gisType
        }
        Rectangle{
            Layout.preferredWidth: 200
            Layout.preferredHeight: 50
            Layout.alignment: Qt.AlignHCenter
            color: "transparent"
            anchors.margins: 50
            visible: _controllerCore.type === mainWindow.gisType
            PopupButton{
                anchors.left: parent.left
                width: parent.width * 0.45
                text: "Auto"
                onClicked:{
                    _controllerCore.autoFocus()
                }
                enabled: _controllerCore.type === mainWindow.gisType
            }

            PopupButton{
                anchors.right: parent.right
                width: parent.width * 0.45
                text: "To Infinity"
                enabled: _controllerCore.type === mainWindow.gisType
                onClicked: {
                    _controllerCore.infiniteFocus()
                }

            }
        }
        PopupItem{

            width: parent.width
            itemText: "Modes"
            Layout.preferredHeight: 25
            visible: _controllerCore.type === mainWindow.gisType
        }
        RowLayout {
            Layout.preferredHeight: 20
            Layout.alignment: Qt.AlignLeft
            visible: _controllerCore.type === mainWindow.gisType
            CustomRadioButton1{
                id: diffDef
                checked: true
                text: "Default"
                onClicked:
                {
                    highSensRadio.checked = false
                    diffRadio.checked = false
                    _controllerCore.toggleDifferentialMode(false)
                    _controllerCore.toggleHighSensitivityMode(false)
                }

            }
        }
        RowLayout {
            Layout.preferredHeight: 20
            Layout.alignment: Qt.AlignLeft
            visible: _controllerCore.type === mainWindow.gisType
            Connections{
                target: _controllerCore
                function onDifferentialModeChanged(val)
                {
                    diffRadio.checked = val
                    if(val)
                        diffDef.checked = false
                    else if(!highSensRadio.checked)
                        diffDef.checked = true
                }
                function onHighSensitivityModeChanged(val)
                {
                    highSensRadio.checked = val
                    if(val)
                        diffDef.checked = false
                    else if(!diffRadio.checked)
                        diffDef.checked = true
                }
            }

            CustomRadioButton1{
                id: diffRadio
                checked: false
                text: "Differential"
                onClicked:
                {
                    highSensRadio.checked = false
                    diffDef.checked = false
                    _controllerCore.toggleDifferentialMode(checked)
                }

            }

            CustomRadioButton1{
                id: highSensRadio
                checked: false
                text: "High Sensitivity"
                onClicked:
                {
                    diffRadio.checked = false
                    diffDef.checked = false
                    _controllerCore.toggleHighSensitivityMode(checked)
                }
            }

        }
    }
}
