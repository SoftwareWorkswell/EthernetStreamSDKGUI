import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "../"
import "../../"


Rectangle{
    function setSecondarySource(source)
    {
        visibleRect.source = source
    }
    function refreshSSDState(state)
    {
        sideSsdText.text = state
    }

    Column{
        anchors.fill: parent
        Image{
            id: visibleRect
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: _controllerCore.mainCamera === "THERMAL" ? width/16*9 : width/5*4
            //color: "green"
            Connections{
                target: _controllerCore
                onStreamFrameVisibleChanged:{
                    if (_controllerCore.mainCamera === "THERMAL")
                    {
                        if (streamWindow.visible)
                            separateView.source = "image://streamFramesProviderVisible/500/500/?seed=" + Math.random();
                        else
                            visibleRect.source = "image://streamFramesProviderVisible/500/500/?seed=" + Math.random();
                    }
                    else
                        mainviewParent.setMainViewSource("image://streamFramesProviderVisible/500/500/?seed=" + Math.random())
                }
            }
            MouseArea{
                width: 20
                hoverEnabled: true
                height: 20
                opacity: containsMouse ? 1.0 : 0.5
                x: 50
                y: parent.height - 30
                Image{
                    source: "/img/png/ico_arrow_both.png"
                    height: parent.height
                    fillMode: Image.PreserveAspectFit
                    rotation: 90
                }
                onClicked: {
                    if(_controllerCore.mainCamera === "THERMAL")
                        _controllerCore.mainCamera = "VISIBLE"
                    else
                        _controllerCore.mainCamera = "THERMAL"
                    separateView.onResizedSeparateWindow(streamWindow.width, streamWindow.height);
                }
            }
            MouseArea{
                width: 30
                hoverEnabled: true
                height: 30
                opacity: containsMouse ? 1.0 : 0.5
                x: 10
                y: parent.height - 35
                Image{
                    id: sidePanelPauseImg
                    source: "/img/png/button-pause.png"
                    height: parent.height
                    fillMode: Image.PreserveAspectFit
                }
                onClicked: {
                    if(_controllerCore.mainCamera === "THERMAL")
                    {
                        _controllerCore.toggleStreamVisible();
                        if(_controllerCore.streamingVisible())
                            sidePanelPauseImg.source = "/img/png/button-pause.png"
                        else sidePanelPauseImg.source = "/img/png/button-play.png"
                    }
                    else {
                        _controllerCore.toggleStreamThermal();
                        if(_controllerCore.streamingThermal())
                            sidePanelPauseImg.source = "/img/png/button-pause.png"
                        else sidePanelPauseImg.source = "/img/png/button-play.png"
                    }
                }
            }
            MouseArea{
                width: 20
                hoverEnabled: true
                height: 20
                opacity: containsMouse ? 1.0 : 0.5
                x: 80
                y: parent.height - 30
                Image{
                    source: "/img/png/ico_fullscreen.png"
                    height: parent.height
                    fillMode: Image.PreserveAspectFit
                }
                onClicked: {
                    if (streamWindow.visible)
                    {
                        visibleRect.fillMode = Image.Stretch;
                        streamWindow.hide();
                    }
                    else {
                        visibleRect.fillMode = Image.PreserveAspectFit;
                        visibleRect.source = "/img/png/logo-workswell.png"
                        streamWindow.show();
                    }
                }
            }

        }
        ScrollView {
            anchors.top: visibleRect.bottom
            width: parent.width
            height: parent.height - visibleRect.height
            clip: true
            Rectangle{
                id: sideHeader
                height: 2
                width: parent.width
                color: "orange"
                Rectangle{
                    color: "#16151D"
                    anchors.fill: parent
                    anchors.topMargin: 1
                    Text {
                        id: sideHeaderText
                        anchors.centerIn: parent

                        color: "orange"
                        font.bold: true
                    }
                }
            }
            Rectangle{
                id: infoPanel
                anchors.top: sideHeader.bottom
                anchors.left: parent.left
                width: parent.width
                height: parent.height - sideHeader.height - logo.height
                color: "#33333D"
                onHeightChanged: {
                    if (infoPanel.height > 500)
                    {
                        basicInfoColumn.width = Qt.binding(function() { return infoPanel.width});
                        advancedInfoColumn.width = Qt.binding(function() { return infoPanel.width});
                        basicInfoColumn.height = Qt.binding(function() { return infoPanel.height / 2});
                        advancedInfoColumn.height = Qt.binding(function() { return infoPanel.height / 2});
                        advancedInfoColumn.anchors.top = Qt.binding(function() { return basicInfoColumn.bottom});
                    }
                    else {
                        basicInfoColumn.width = Qt.binding(function() { return infoPanel.width/2});
                        advancedInfoColumn.width = Qt.binding(function() { return infoPanel.width/2});
                        basicInfoColumn.height = Qt.binding(function() { return infoPanel.height});
                        advancedInfoColumn.height = Qt.binding(function() { return infoPanel.height});
                        advancedInfoColumn.anchors.top = Qt.binding(function() { return infoPanel.top});
                    }
                }
                ColumnLayout{
                    id: basicInfoColumn
                    height: parent.height
                    anchors.top: parent.top
                    anchors.left: parent.left
                    width: parent.width / 2
                    Rectangle{
                        Layout.fillHeight: true
                        Layout.fillWidth:  true
                        Row{
                            height: parent.height
                            spacing: 5
                            anchors.centerIn: parent
                            Text {
                                text: _controllerCore.type
                                font.pixelSize: 13
                                color: "Orange"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                        color: "#16151D"
                    }
                    CustomHr{}
                    Rectangle{
                        Layout.fillHeight: true
                        Layout.fillWidth:  true
                        color: "#16151D"
                        Row{
                            height: parent.height
                            spacing: 5
                            anchors.centerIn: parent
                            Text {
                                id: preventiveText
                                font.pixelSize: 13
                                color: _controllerCore.preventiveColor
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                        Connections{
                            target: _controllerCore
                            onPreventiveColorChanged:
                            {
                                preventiveText.color = val
                            }
                            onAlarmPreventiveChanged:{
                                preventiveText.text = "Preventive: " + val.toFixed(2) + " °C"
                            }
                        }
                    }
                    CustomHr{}
                    Rectangle{
                        Layout.fillHeight: true
                        Layout.fillWidth:  true
                        Row{
                            height: parent.height
                            spacing: 5
                            anchors.centerIn: parent
                            Text {
                                id: criticalText
                                font.pixelSize: 13
                                color: _controllerCore.criticalColor
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                        Connections{
                            target: _controllerCore
                            onCriticalColorChanged:
                            {
                                criticalText.color = val
                            }
                            onAlarmCriticalChanged:{
                                criticalText.text = "Critical: " + val.toFixed(2) + " °C"
                            }
                        }
                        color: "#16151D"
                    }
                    CustomHr{
                        visible: true
                    }
                    Rectangle{
                        visible: _controllerCore.type != mainWindow.securityType
                        Layout.fillHeight: true
                        Layout.fillWidth:  true
                        Row{
                            height: parent.height
                            spacing: 5
                            anchors.centerIn: parent
                            Text {
                                id: blackbodyText
                                font.pixelSize: 13
                                color: "white"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                        color: "#16151D"

                        Connections{
                            target: _controllerCore
                            onBlackbodyStatusChanged:{
                                blackbodyText.text = _controllerCore.blackbodyStatus
                            }
                        }
                    }
                    CustomHr{
                        visible: _controllerCore.type != mainWindow.securityType
                    }
                    Rectangle{
                        Layout.fillHeight: true
                        Layout.fillWidth:  true
                        visible: _controllerCore.type != mainWindow.securityType
                        Row{
                            height: parent.height
                            spacing: 5
                            anchors.centerIn: parent
                            Image{
                                height: 20
                                anchors.verticalCenter: parent.verticalCenter
                                source: "/img/png/button_measure.png"
                                fillMode: Image.PreserveAspectFit

                            }
                            Text {
                                id: sideAlarmText
                                function setAlarmText(){
                                    if (_controllerCore.alarmMode == 0)
                                        sideAlarmText.text = "OFF";
                                    else
                                        sideAlarmText.text = "ON";

                                }
                                color: "white"
                                anchors.verticalCenter: parent.verticalCenter
                                font.pixelSize: 12
                                Connections{
                                    target: _controllerCore
                                    onAlarmModeChanged: sideAlarmText.setAlarmText()
                                }
                                Connections{
                                    target: _controllerCore
                                    onAlarmPreventiveChanged: sideAlarmText.setAlarmText()
                                }
                                Connections{
                                    target: _controllerCore
                                    onAlarmCriticalChanged: sideAlarmText.setAlarmText()
                                }
                            }
                        }
                        color: "#16151D"
                    }
                    CustomHr{
                        visible:true
                    }
                    Rectangle{
                        visible: true
                        Layout.fillHeight: true
                        Layout.fillWidth:  true
                        Row{
                            height: parent.height
                            spacing: 5
                            anchors.centerIn: parent
                            Image{
                                height: 20
                                anchors.verticalCenter: parent.verticalCenter
                                source: "/img/png/ico_memory.png"
                                fillMode: Image.PreserveAspectFit
                            }
                            Text {
                                id: sideSsdText
                                text: "SSD Free: N/A"
                                color: "white"
                                font.pixelSize: 12
                                anchors.verticalCenter: parent.verticalCenter

                            }
                        }
                        color: "#16151D"
                    }
                    CustomHr{
                        color: "orange"
                    }
                }
                ColumnLayout{
                    id: advancedInfoColumn
                    height: parent.height
                    anchors.right: parent.right
                    width: parent.width / 2
                    Rectangle{
                        Layout.fillHeight: true
                        Layout.fillWidth:  true
                        Row{
                            height: parent.height
                            spacing: 5
                            anchors.centerIn: parent
                            Text {
                                text: "IP: " + _controllerCore.ipAddress
                                color: "white"
                                font.pixelSize: 12
                                anchors.verticalCenter: parent.verticalCenter

                            }
                        }
                        color: "#16151D"
                    }
                    CustomHr{}
                    Rectangle{
                        Layout.fillHeight: true
                        Layout.fillWidth:  true
                        Row{
                            height: parent.height
                            spacing: 5
                            anchors.centerIn: parent
                            Text {
                                text: "CPU: " + Math.round(_controllerCore.cpuTemp) + "°C"
                                color: "white"
                                font.pixelSize: 12
                                anchors.verticalCenter: parent.verticalCenter

                            }
                        }
                        color: "#16151D"
                    }
                    CustomHr{}
                    Rectangle{
                        Layout.fillHeight: true
                        Layout.fillWidth:  true
                        Row{
                            height: parent.height
                            spacing: 5
                            anchors.centerIn: parent
                            Text {
                                text: "IR Core: " + Math.round(_controllerCore.camTemp) + "°C"
                                color: "white"
                                font.pixelSize: 12
                                anchors.verticalCenter: parent.verticalCenter

                            }
                        }
                        color: "#16151D"
                    }
                    CustomHr{}
                    Rectangle{
                        Layout.fillHeight: true
                        Layout.fillWidth:  true
                        Row{
                            height: parent.height
                            spacing: 5
                            anchors.centerIn: parent
                            Text {
                                text: "Firmware: " + _controllerCore.firmwareVersion;
                                color: "white"
                                font.pixelSize: 12
                                anchors.verticalCenter: parent.verticalCenter

                            }
                        }
                        color: "#16151D"
                    }
                    CustomHr{
                        visible: _controllerCore.type != mainWindow.securityType
                    }
                    Rectangle{
                        visible: _controllerCore.type != mainWindow.securityType
                        Layout.fillHeight: true
                        Layout.fillWidth:  true
                        Row{
                            height: parent.height
                            spacing: 5
                            anchors.centerIn: parent
                            Image{
                                height: 20
                                anchors.verticalCenter: parent.verticalCenter
                                source: "/img/png/ico_memory.png"
                                fillMode: Image.PreserveAspectFit

                            }
                            Text {
                                text: sideSsdText.text
                                color: "white"
                                font.pixelSize: 12
                                anchors.verticalCenter: parent.verticalCenter

                            }
                        }
                        color: "#16151D"
                    }
                    CustomHr{
                    }
                    Rectangle{
                        Layout.fillHeight: true
                        Layout.fillWidth:  true
                        Row{
                            height: parent.height
                            spacing: 5
                            anchors.centerIn: parent
                            Text {
                                id: connectionText
                                color: "white"
                                font.pixelSize: 12
                                anchors.verticalCenter: parent.verticalCenter
                                Connections{
                                    target: _controllerCore
                                    onConnectedChanged: connectionText.text = "Connection: " + (_controllerCore.connected ? "OK" : "NONE")
                                }
                            }
                        }
                        color: "#16151D"
                    }
                    CustomHr{
                        color: "orange"
                        Layout.preferredWidth: parent.width
                    }
                }
            }
            Rectangle{
                id: logo
                height: 40
                width: parent.width
                //color: "#FF7604"//"#16151D"

                gradient: Gradient {
                    GradientStop { position: 0.0; color: "white" }
                    GradientStop { position: 0.1; color: "#FF7604" }
                    GradientStop { position: 1.0; color: "orange" }
                }
                anchors.bottom: parent.bottom
                Image{
                    anchors.horizontalCenter: parent.horizontalCenter
                    y: 5
                    source: "/img/png/logo-workswell.png"
                    fillMode: Image.PreserveAspectFit
                    height: 20
                }
            }
        }
    }
}
