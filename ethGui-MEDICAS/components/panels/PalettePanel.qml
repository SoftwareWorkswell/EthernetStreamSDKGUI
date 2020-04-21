import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "../"

Rectangle{
    Rectangle{
        id: palette
        function applyPaletteValues(){
            var data = _controllerCore.paletteValues;
            var newStops = [];
            gradientPalette.stops = [];
            for (var i = 0; i < data.length;i++)
            {
                var s1 = stopComponent.createObject(palette, {"position":1-(1/data.length)*(i+1),"color":Qt.rgba(data[i].r/255, data[i].g/255, data[i].b/255,1)});
                newStops.push(s1);
            }
            gradientPalette.stops = newStops;
            palette.update();
        }
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 20
        anchors.left: parent.left
        anchors.topMargin: 20
        anchors.leftMargin: 5
        anchors.bottomMargin: 10
        gradient: Gradient {
            id: gradientPalette
            GradientStop { position: 0.0; color: "white"}
            GradientStop { position: 0.1; color: "black" }
            GradientStop { position: 0.3; color: "white"}
        }
        Connections{
            target: _controllerCore
            onPaletteValuesChanged: palette.applyPaletteValues()
        }
        Rectangle{
            id: alarmPaletteAboveRect
            color: colorAboveComboBox.currentText
            width: palette.width
            anchors.top: parent.top
            visible: false
            function reDraw()
            {
                if (!_controllerCore.compareFw("1.1.1", _controllerCore.firmwareVersion))
                    return;
                var height;
                var ptop = Math.round(_controllerCore.paletteTop*10)/10
                var pbottom = Math.round(_controllerCore.paletteBottom*10)/10
                if (rangeAlarmSlider.second.value < ptop)
                {
                    height = palette.height * (ptop - rangeAlarmSlider.second.value)  / (ptop - pbottom);
                    alarmPaletteAboveRect.height = height > palette.height ?  palette.height :  height;
                }   else alarmPaletteAboveRect.height = 0;
                if (rangeAlarmSlider.first.value > pbottom)
                {
                    height = palette.height * (rangeAlarmSlider.first.value - pbottom)  / (ptop - pbottom);
                    alarmPaletteBelowRect.height = height > palette.height ?  palette.height :  height;
                }   else alarmPaletteBelowRect.height = 0;
                alarmPaletteAboveRect.visible = false;
                alarmPaletteBetweenRect.visible = false;
                alarmPaletteBelowRect.visible = false;
                if(alarmModeAbove.checked)
                    alarmPaletteAboveRect.visible = true;
                else if(alarmModeBetween.checked)
                    alarmPaletteBetweenRect.visible = true;
                else if(alarmModeBelow.checked)
                    alarmPaletteBelowRect.visible = true;
                else if (alarmModeOutside.checked)
                {
                    alarmPaletteAboveRect.visible = true;
                    alarmPaletteBelowRect.visible = true;
                }
            }
        }
        Connections{
            target: _controllerCore
            onPaletteTopChanged: alarmPaletteAboveRect.reDraw();
        }
        Connections{
            target: _controllerCore
            onPaletteBottomChanged: alarmPaletteAboveRect.reDraw();
        }
        Rectangle{
            id: alarmPaletteBetweenRect
            color: colorBetweenComboBox.currentText
            width: palette.width
            anchors.top: alarmPaletteAboveRect.bottom
            anchors.bottom: alarmPaletteBelowRect.top

            visible: false
        }
        Rectangle{
            id: alarmPaletteBelowRect
            color: colorBelowComboBox.currentText
            width: palette.width
            anchors.bottom: parent.bottom
            visible: false
        }
    }
    Text{
        id: rangeModeText
        text: _controllerCore.rangeMode === 0 ? "A" : (_controllerCore.rangeMode === 1 ? "M" : "S")
        font.bold: true
        anchors.bottom: palette.top
        anchors.left: palette.left
        color: "white"
        anchors.bottomMargin: 2
        anchors.leftMargin: 5
    }
    Rectangle{
        anchors.left: palette.right
        anchors.leftMargin: 5
        height: parent.height
        anchors.bottomMargin: 10
        anchors.topMargin: 30
        visible: _controllerCore.type != mainWindow.securityType
        Text{
            text: "°C"
            color: "white"
            font.bold: true
            y: 5
        }
        Text {
            text: Math.round(_controllerCore.paletteTop*10)/10
            color: "white"
            y: 18
            Layout.fillWidth: true;
        }
        Text {
            text: Math.round((_controllerCore.paletteBottom +(_controllerCore.paletteTop - _controllerCore.paletteBottom)/4*3)*10)/10
            color: "white"
            y: 10 + palette.height/4
            Layout.fillWidth: true;
        }
        Text {
            text:  Math.round((_controllerCore.paletteBottom +(_controllerCore.paletteTop - _controllerCore.paletteBottom)/2)*10)/10
            color: "white"
            y: 10 + palette.height/2
            Layout.fillWidth: true;
        }
        Text {
            text: Math.round((_controllerCore.paletteBottom +(_controllerCore.paletteTop - _controllerCore.paletteBottom)/4)*10)/10
            color: "white"
            y: 10 + palette.height*3/4
            Layout.fillWidth: true;
        }
        Text {
            text:  Math.round(_controllerCore.paletteBottom*10)/10
            color: "white"
            y: 5 + palette.height
            Layout.fillWidth: true;
        }
    }
}
