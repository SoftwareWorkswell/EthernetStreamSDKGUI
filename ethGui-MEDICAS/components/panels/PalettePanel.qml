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
            function onPaletteValuesChanged(){
                palette.applyPaletteValues()
            }
        }
        Rectangle{
            id: alarmPaletteAboveRect
            color: _controllerCore.criticalColor
            width: _controllerCore.alarmMode === 1 ? palette.width : (palette.width / 2)
            anchors.left: parent.left
            y: parent.y
            visible: false
            function reDraw()
            {
                if(_controllerCore.alarmMode > 0)
                {
                    var height;
                    // top and bottom temp of palette
                    var ptop = Math.round(_controllerCore.paletteTop*10)/10
                    var pbottom = Math.round(_controllerCore.paletteBottom*10)/10
                    var criticalMax = Math.round(_controllerCore.getCriticalUpperBoundary() *10)/10
                    if (_controllerCore.alarmCritical < ptop)
                    {
                        console.log('redraw ' + (ptop - criticalMax) + ' ' + (ptop - _controllerCore.alarmCritical))
                        height = palette.height * (((ptop - _controllerCore.alarmCritical)- (ptop - criticalMax))  / (ptop - pbottom));
                        alarmPaletteAboveRect.y = palette.height * (((ptop - criticalMax))  / (ptop - pbottom));
                        alarmPaletteAboveRect.height = height > palette.height ?  palette.height :  height;
                    }
                    else
                        alarmPaletteAboveRect.height = 0;
                    if (_controllerCore.alarmPreventive > pbottom)
                    {
                        alarmPaletteBetweenRect.height = palette.height * (((ptop - _controllerCore.alarmPreventive) - (ptop - _controllerCore.alarmCritical)) / (ptop - pbottom))
                        alarmPaletteBetweenRect.y =  palette.height * (((ptop - _controllerCore.alarmCritical))  / (ptop - pbottom));
                    }
                    else
                        alarmPaletteBetweenRect.height = 0;
                    alarmPaletteAboveRect.visible = true;
                    alarmPaletteBetweenRect.visible = true;
                }
                else
                {
                    alarmPaletteAboveRect.visible = false;
                    alarmPaletteBetweenRect.visible = false;
                }
            }
        }
        Connections{
            target: _controllerCore
            function onPaletteTopChanged(){
                alarmPaletteAboveRect.reDraw();
            }
            function onAlarmPreventiveChanged()
            {
                alarmPaletteAboveRect.reDraw();
            }
            function onAlarmCriticalChanged()
            {
                alarmPaletteAboveRect.reDraw();
            }
            function onAlarmModeChanged()
            {
                alarmPaletteAboveRect.reDraw();
            }
            function onPaletteBottomChanged(){
                alarmPaletteAboveRect.reDraw();
            }
            function onTemperatureModeChanged(){
                alarmPaletteAboveRect.reDraw();
            }
        }
        Rectangle{
            id: alarmPaletteBetweenRect
            color: _controllerCore.preventiveColor
            width: _controllerCore.alarmMode === 1 ? palette.width : (palette.width / 2)
            anchors.top: alarmPaletteAboveRect.bottom
            anchors.left: parent.left
            visible: false
        }
    }

    Rectangle{
        anchors.left: palette.right
        anchors.leftMargin: 5
        height: parent.height
        anchors.bottomMargin: 10
        anchors.topMargin: 30
        visible: _controllerCore.type !== mainWindow.securityType
        Text{
            text: _controllerCore.units()
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
