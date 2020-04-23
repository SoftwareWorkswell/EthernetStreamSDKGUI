import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "../"
import "../popups/"


Rectangle {
    function toggleControlPanel()
    {
        if (controlPanel.visible)
        {
            controlPanel.visible = false;
            controlPanel.width = 0;
            hideControlManelImg.rotation = 270;
        }
        else {
            controlPanel.width = 70;
            controlPanel.visible = true;
            hideControlManelImg.rotation = 90;
        }
        mainView.onResized(mainviewParent.width, mainviewParent.height);
    }
    function isPopupVisible(id) {
        for(var i = 0; i < controlPanelPopup.children.length; ++i)
            if(controlPanelPopup.children[i].objectName === id)
                return controlPanelPopup.children[i].visible;
        return false

    }

    function setPopupVisible(id, visible) {
        for(var i = 0; i < controlPanelPopup.children.length; ++i)
            if(controlPanelPopup.children[i].objectName === id)
                return controlPanelPopup.children[i].visible = visible;
    }

    function getPopup(id) {
        for(var i = 0; i < controlPanelPopup.children.length; ++i)
            if(controlPanelPopup.children[i].objectName=== id)
                return controlPanelPopup.children[i]
        return []
    }

    function setMainViewSource(source)
    {
        mainView.source = source
    }

    function setManRangeSliderVisible(visible)
    {
        quickRangeSliderRect.visible = visible
    }

    function setAlarmsSliderVisible(visible)
    {
        quickAlarmsRect.visible = visible
    }
    function setBlackbodyRectVisible(visible)
    {
        quickBlackbodyRect.visible = visible
    }

    Image{
        id: mainView
        function onResized(parentWidth, parentHeight)
        {
            var cwidth = _controllerCore.mainCamera === "THERMAL" ? 5 : 16;
            var cheight = _controllerCore.mainCamera === "THERMAL" ? 4 : 9;
            if(parentWidth/cwidth*cheight < parentHeight)
            {
                mainView.width = parentWidth;
                mainView.height = mainView.width/cwidth*cheight;
            }
            else{
                mainView.height = parentHeight;
                mainView.width = mainView.height/cheight*cwidth;
            }
        }
        anchors.centerIn: parent
        height: 400
        width: 500
        source: "/img/screen/init.png"
        Connections{
            target: _controllerCore
            onStreamFrameChanged:{
                if (_controllerCore.mainCamera === "THERMAL")
                    mainView.source = "image://streamFramesProvider/500/500/?seed=" + Math.random();
                else
                {
                    if(streamWindow.visible)
                        separateView.source = "image://streamFramesProvider/500/500/?seed=" + Math.random();
                    else
                        rightPanel.setSecondarySource( "image://streamFramesProvider/500/500/?seed=" + Math.random())
                }
            }
        }
        Connections{
            target: mainviewParent
            onHeightChanged: mainView.onResized(mainviewParent.width, mainviewParent.height)
        }
        Connections{
            target: mainviewParent
            onWidthChanged: mainView.onResized(mainviewParent.width, mainviewParent.height)
        }
    }
    MouseArea{
        width: 20
        hoverEnabled: true
        height: 20
        opacity: containsMouse ? 1.0 : 0.5
        x: 10
        y: 10
        Image{
            id: hideControlManelImg
            source: "/img/png/ico_arrow_down.png"
            height: parent.height
            fillMode: Image.PreserveAspectFit
            rotation: 90
        }
        onClicked: {
            mainviewParent.toggleControlPanel();
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
            id: quickPauseImg
            source: "/img/png/button-pause.png"
            height: parent.height
            fillMode: Image.PreserveAspectFit
        }
        onClicked: {
            if(!_controllerCore.mainCamera === "THERMAL")
            {
                _controllerCore.toggleStreamVisible();
                if(_controllerCore.streamingVisible())
                    quickPauseImg.source = "/img/png/button-pause.png"
                else quickPauseImg.source = "/img/png/button-play.png"
            }
            else {
                _controllerCore.toggleStreamThermal();
                if(_controllerCore.streamingThermal())
                    quickPauseImg.source = "/img/png/button-pause.png"
                else quickPauseImg.source = "/img/png/button-play.png"
            }
        }
    }
    Rectangle{
        height: 30
        width: 120
        visible: false
        x: parent.width - 130
        y: 20
        id: nucPopup
        color: "#88000000"
        Text {
            anchors.centerIn: parent
            color: "red";
            text: "NUC in progress..."
        }
    }
    ColumnLayout{
        id: controlPanelPopup
        width: 260
        height: parent.height

        RangePopup{
            id: rangePopup
            width: parent.width
            objectName: "rangePopup"
        }
        FunctionsPopup{
            id: functionsPopup
            width: parent.width
            objectName: "functionsPopup"

        }
        PalettePopup{
            id: palettePopup
            width: parent.width
            objectName: "palettePopup"

        }
        CapturePopup{
            id: capturePopup
            width: parent.width
            objectName: "capturePopup"
        }
        MeasurePopup {
            id: measurePopup
            width: parent.width
            objectName: "measurePopup"
        }
        AdvancedPopup{
            id: advancedPopup
            width: parent.width
            objectName: "advancedPopup"

        }
    }
    Rectangle{
        id: quickRangeSliderRect
        visible: false
        height: 35
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        color: "#DD444444"
        ManualRangeSlider
        {
            id: quickManSlider
            anchors.fill: parent
        }
    }
    Rectangle{
        id: quickAlarmsRect
        visible: false
        height: 35
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        color: "#DD444444"
        AlarmsSliderQuick
        {
            id: quickAlarmSlider
            anchors.fill: parent
        }
    }
    Rectangle{
        id: quickBlackbodyRect
        visible: false
        height: 42
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        color: "#DD444444"
        Rectangle{
            color: "transparent"
            Layout.preferredHeight: 40
            Layout.preferredWidth: parent.width
            RowLayout
            {
                anchors.fill: parent
                RowLayout{
                    Layout.fillWidth: true
                    Layout.margins: 5
                    Text{
                        Layout.alignment: Qt.AlignLeft
                        color: "white"
                        font.pixelSize: 12
                        text: "X: "
                        font.bold: true
                    }
                    Rectangle{
                        id: blackbodyXBackground
                        Layout.preferredHeight: 20
                        Layout.preferredWidth: 120
                        SpinBox {
                            anchors.fill: parent
                            id:  blackbodyXValue
                            value: parseInt(_controllerCore.blackbodyRoiX)
                            font.bold: true
                            to: 639
                            editable: true
                            //validator: RegExpValidator { regExp: /^0\.\d\d/}
                            onValueModified: {
                                _controllerCore.blackbodyRoiX = value
                            }
                        }
                        Connections{
                            target: _controllerCore
                            onBlackbodyRoiXChanged: {
                                 blackbodyXValue.value = _controllerCore.blackbodyRoiX
                            }
                        }
                    }
                }

                RowLayout{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.margins: 5
                    Text{
                        Layout.alignment: Qt.AlignLeft
                        color: "white"
                        font.pixelSize: 12
                        text: "Y: "
                        font.bold: true
                    }
                    Rectangle{
                        id: blackbodyYBackground
                        Layout.preferredHeight: 20
                        Layout.preferredWidth: 120
                        SpinBox {
                            anchors.fill: parent
                            id:  blackbodyYValue
                            font.bold: true
                            to: 511
                            editable: true
                            value: parseInt(_controllerCore.blackbodyRoiX)
                            onValueModified: {
                                _controllerCore.blackbodyRoiY = value
                            }
                        }
                        Connections{
                            target: _controllerCore
                            onBlackbodyRoiYChanged: {
                                 blackbodyYValue.value = _controllerCore.blackbodyRoiY
                            }
                        }
                    }
                }
                RowLayout {
                    Layout.fillHeight: true
                    PopupButton{
                        text: "Confirm"
                        onClicked: _controllerCore.sendBlackbodyCoordinates()
                    }
                }
                RowLayout {
                    Layout.fillHeight: true
                    PopupButton{
                        text: "Hide"
                        onClicked:{
                            _controllerCore.fetchBlackbodyCoordinates()
                            setBlackbodyRectVisible(false)
                        }
                    }
                }
            }
       }
    }
}
