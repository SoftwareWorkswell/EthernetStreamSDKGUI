import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "./components"
import "./components/popups"

Window {
    id: mainWindow
    onClosing:{
        _controllerCore.close()
    }
    function setSliderRanges()
    {
        console.log('set slider ranges')
        if(_controllerCore.isSetup)
            rangePopup.setSliderRanges()
    }

    function onAutoReconnected() {
        disconnectedWindowShown = false
        connectionWindowShown = false
    }

    function showConnectionWindow() {
        var component = Qt.createComponent("components/ConnectionWindow.qml")
        var window = component.createObject(mainWindow)
        window.show()
        mainWindow.hide()
        disconnectedWindowShown = false
        connectionWindowShown = true
    }

    function showDisconnectedWindow() {
        if (disconnectedWindowShown == false
                && connectionWindowShown == false) {
            var component = Qt.createComponent(
                        "components/CameraDisconnectedMessage.qml")
            var window = component.createObject(mainWindow)
            window.show()
            mainWindow.disconnectedWindowShown = true
        }
    }
    property string securityType: "WIRIS Security"
    property string gisType: "GIS-320"
    property bool disconnectedWindowShown: false
    property bool connectionWindowShown: false
    property var separateWindow: Qt.createComponent(
                                     "./components/SeparateWindow.qml").createObject(
                                     "streamWindow")
    property bool mainStreamThermal: true;
    onMainStreamThermalChanged: {
        if(mainStreamThermal)
            _controllerCore.setMainCamera("THERMAL")
        else
            _controllerCore.setMainCamera("VISIBLE")
        mainView.onResized(mainviewParent.width, mainviewParent.height);
        if (separateView)
            separateView.onResizedSeparateWindow(streamWindow.width, streamWindow.height);
    }
    visible: true
    minimumWidth: 960
    minimumHeight: 540
    title: qsTr("WIRIS & GIS Ethernet Stream SDK GUI 1.0.0B5")

    Rectangle{
        anchors.fill: parent

        Row{
            id: mainRow
            anchors.fill: parent

            Rectangle{
                anchors.left: parent.left;
                anchors.top: parent.top;
                width: parent.width *7/10
                height: parent.height
                color: "red"

                Row{
                    anchors.fill: parent
                    Rectangle{
                        anchors.top: parent.top
                        anchors.left: parent.left
                        width: parent.width
                        height: parent.height - 40

                        Rectangle{
                            anchors.fill: parent

                            Rectangle{
                                id: controlPanel

                                function hideAllPopups()
                                {
                                    rangePopup.visible = false;
                                    rangeImg.source = "img/png/button_range_c.png";
                                    rangeButton.highlighted = false;

                                    functionsPopup.visible = false;
                                    functionsImg.source = "img/png/button_functions.png";
                                    functionsButton.highlighted = false;

                                    palettePopup.visible = false;
                                    paletteImg.source = "img/png/button_palette.png";
                                    paletteButton.highlighted = false;

                                    capturePopup.visible = false;
                                    captureImg.source = "img/png/button_capture.png";
                                    captureButton.highlighted = false;

                                    measurePopup.visible = false;
                                    measureImg.source = "img/png/button_measure.png";
                                    measureButton.highlighted = false;

                                    advancedPopup.visible = false;
                                    advancedImg.source = "img/png/button_settings.png";
                                    advancedButton.highlighted = false;
                                }

                                anchors.top: parent.top
                                anchors.left: parent.left
                                height: parent.height
                                width: 70

                                Rectangle{
                                    anchors.fill: parent
                                    color: "#33333D"


                                    ColumnLayout{
                                        width: parent.width
                                        height: parent.height
                                        ControlButton {
                                            id: rangeButton
                                            text: "Range"
                                            visible: _controllerCore.type != mainWindow.securityType

                                            Layout.fillWidth: true
                                            Layout.preferredHeight: 54
                                            Image{
                                                id: rangeImg
                                                source: "img/png/button_range_c.png"
                                                fillMode: Image.PreserveAspectFit
                                                height: 25
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                y: 7
                                            }

                                            onClicked: {
                                                if (!rangePopup.visible && _controllerCore.type != mainWindow.securityType)
                                                {
                                                    controlPanel.hideAllPopups();
                                                    rangePopup.visible = true;
                                                    rangeButton.highlighted = true;
                                                    rangeImg.source = "img/png/button_range_c_selected.png"
                                                }

                                                else{
                                                    controlPanel.hideAllPopups();
                                                }
                                            }
                                            Connections{
                                                target: _controllerCore
                                                function onTypeChanged(type)
                                                {
                                                    if (type === mainWindow.securityType)
                                                        rangeButton.backgroundColor = "#555555"
                                                }
                                            }
                                        }
                                        ControlButton {
                                            id: functionsButton
                                            text: "Functions"

                                            Layout.fillWidth: true
                                            Layout.preferredHeight: 54
                                            Image{
                                                id: functionsImg
                                                source: "img/png/button_functions.png"
                                                fillMode: Image.PreserveAspectFit
                                                height: 25
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                y: 7
                                            }

                                            onClicked: {
                                                if (!functionsPopup.visible)
                                                {
                                                    controlPanel.hideAllPopups();
                                                    functionsPopup.visible = true;
                                                    functionsButton.highlighted = true;
                                                    functionsImg.source = "img/png/button_functions_selected.png"
                                                }

                                                else{
                                                    controlPanel.hideAllPopups();
                                                }
                                            }
                                        }

                                        ControlButton {
                                            id: paletteButton
                                            text: "Palette"

                                            Layout.fillWidth: true
                                            Layout.preferredHeight: 54
                                            Image{
                                                id: paletteImg
                                                source: "img/png/button_palette.png"
                                                fillMode: Image.PreserveAspectFit
                                                height: 25
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                y: 7
                                            }

                                            onClicked: {
                                                if (!palettePopup.visible)
                                                {
                                                    controlPanel.hideAllPopups();
                                                    palettePopup.visible = true;
                                                    paletteButton.highlighted = true;
                                                    paletteImg.source = "img/png/button_palette_selected.png"
                                                }

                                                else{
                                                    controlPanel.hideAllPopups();
                                                }
                                            }
                                        }

                                        ControlButton {
                                            id: captureButton
                                            text: "Capture"
                                            Layout.fillWidth: true
                                            Layout.preferredHeight: 54
                                            Image{
                                                id: captureImg
                                                source: "img/png/button_capture.png"
                                                fillMode: Image.PreserveAspectFit
                                                height: 25
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                y: 7
                                            }

                                            onClicked: {
                                                if (!capturePopup.visible)
                                                {
                                                    controlPanel.hideAllPopups();
                                                    capturePopup.visible = true;
                                                    captureButton.highlighted = true;
                                                    captureImg.source = "img/png/button_capture_selected.png"
                                                }

                                                else{
                                                    controlPanel.hideAllPopups();
                                                }
                                            }
                                        }

                                        ControlButton {
                                            id: measureButton
                                            text: "Measure"

                                            Layout.fillWidth: true
                                            Layout.preferredHeight: 54
                                            Image{
                                                id: measureImg
                                                source: "img/png/button_measure.png"
                                                fillMode: Image.PreserveAspectFit
                                                height: 25
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                y: 7
                                            }

                                            onClicked: {
                                                if (!measurePopup.visible)
                                                {
                                                    controlPanel.hideAllPopups();
                                                    measurePopup.visible = true;
                                                    measureButton.highlighted = true;
                                                    measureImg.source = "img/png/button_measure_selected.png"
                                                }

                                                else{
                                                    controlPanel.hideAllPopups();
                                                }
                                            }
                                        }


                                        ControlButton {
                                            id: advancedButton
                                            text: "Advanced"

                                            Layout.fillWidth: true
                                            Layout.preferredHeight: 54
                                            Image{
                                                id: advancedImg
                                                source: "img/png/button_settings.png"
                                                fillMode: Image.PreserveAspectFit
                                                height: 25
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                y: 7
                                            }

                                            onClicked: {
                                                if (!advancedPopup.visible)
                                                {
                                                    controlPanel.hideAllPopups();
                                                    advancedPopup.visible = true;
                                                    advancedButton.highlighted = true;
                                                    advancedImg.source = "img/png/button_settings_selected.png"
                                                }

                                                else{
                                                    controlPanel.hideAllPopups();
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            Rectangle {
                                id: mainviewParent
                                InfoComp{
                                    id: infotainment
                                    width: 500
                                    height: 80
                                    anchors.top: parent.top
                                    anchors.topMargin: 50
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    z: mainView.z + 11
                                    function displayInfoMessage(message){
                                        infotainment.infoMess = message;
                                        infotainment.stateVisible = true;
                                    }
                                    Connections{
                                        target: _controllerCore
                                        function onShowInfoMessage(message)
                                        {
                                            infotainment.displayInfoMessage(message)
                                        }
                                    }
                                }
                                Rectangle{
                                    id: progressRect
                                    visible: progressRect.on
                                    width: 300
                                    height: 80
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.top: parent.top
                                    anchors.topMargin: 50
                                    property bool on: false
                                    property bool terminated: false
                                    z: mainView.z + 11
                                    color: "#343437"
                                    border.color: "#606060"
                                    border.width: 3
                                    Connections
                                    {
                                        target: _controllerCore
                                        function onEnvChangeStarted()
                                        {
                                            progressRect.on = true
                                        }
                                        function onCurrentEnviromentChanged()
                                        {
                                            progressRect.on = false
                                        }
                                    }

                                    GridLayout
                                    {
                                        anchors.fill: parent
                                        Text {
                                            Layout.row: 0
                                            Layout.column: 1
                                            color: "white"
                                            font.family: "Helvetica"
                                            font.pointSize: 9
                                            Layout.alignment: Qt.AlignLeft
                                            verticalAlignment: Text.AlignVCenter
                                            font.bold: true
                                            text: "Changing environment..."
                                        }
                                        Image {
                                            id: container
                                            Layout.row: 0
                                            Layout.column: 0
                                            Layout.preferredWidth: 35
                                            Layout.preferredHeight: 35
                                            Layout.alignment: Qt.AlignRight
                                            source: "../img/png/busy2.png"; visible: progressRect.on
                                            fillMode: Image.PreserveAspectFit
                                            NumberAnimation on rotation { running: progressRect.on; from: 0; to: -360; loops: Animation.Infinite; duration: 2000 }
                                        }
                                    }
                                }


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

                                height: parent.height
                                anchors.left: controlPanel.right
                                anchors.right: palettePanel.left

                                Image{
                                    id: mainView


                                    function onResized(parentWidth, parentHeight)
                                    {
                                        var cwidth = mainWindow.mainStreamThermal ? 5 : 16;
                                        var cheight = mainWindow.mainStreamThermal ? 4 : 9;
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
                                    source: "img/screen/init.png"
                                    Connections{
                                        target: _controllerCore
                                        function onStreamFrameChanged(){
                                            if (mainWindow.mainStreamThermal)
                                                mainView.source = "image://streamFramesProvider/500/500/?seed=" + Math.random();
                                            else
                                            {
                                                if (streamWindow.visible)
                                                    separateView.source = "image://streamFramesProvider/500/500/?seed=" + Math.random();
                                                else visibleRect.source = "image://streamFramesProvider/500/500/?seed=" + Math.random();
                                            }
                                        }
                                    }

                                    Connections{
                                        target: mainviewParent
                                        function onHeightChanged(){ mainView.onResized(mainviewParent.width, mainviewParent.height) }
                                    }
                                    Connections{
                                        target: mainviewParent
                                        function onWidthChanged(){ mainView.onResized(mainviewParent.width, mainviewParent.height) }
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
                                        source: "img/png/ico_arrow_down.png"
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
                                        source: "img/png/button-pause.png"
                                        height: parent.height
                                        fillMode: Image.PreserveAspectFit
                                    }
                                    onClicked: {
                                        if(!mainWindow.mainStreamThermal)
                                        {
                                            _controllerCore.toggleStreamVisible();
                                            if(_controllerCore.streamingVisible())
                                                quickPauseImg.source = "img/png/button-pause.png"
                                            else
                                                quickPauseImg.source = "img/png/button-play.png"
                                        }
                                        else {
                                            _controllerCore.toggleStreamThermal();
                                            if(_controllerCore.streamingThermal())
                                                quickPauseImg.source = "img/png/button-pause.png"
                                            else
                                                quickPauseImg.source = "img/png/button-play.png"
                                        }
                                    }
                                }
                                MouseArea {
                                    width: 20
                                    hoverEnabled: true
                                    height: 20
                                    opacity: containsMouse ? 1.0 : 0.5
                                    x: 40
                                    y: parent.height - 30
                                    Image {
                                        id: snapImg
                                        source: "/img/png/button_functions_selected.png"
                                        height: parent.height
                                        fillMode: Image.PreserveAspectFit
                                    }
                                    onClicked: {
                                        _controllerCore.mainCameraSnapshot()
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
                                    }
                                    FunctionsPopup{
                                        id: functionsPopup
                                        width: parent.width
                                    }
                                    PalettePopup{
                                        id: palettePopup
                                        width: parent.width
                                    }
                                    CapturePopup{
                                        id: capturePopup
                                        width: parent.width
                                    }
                                    MeasurePopup{
                                        id: measurePopup
                                        width: parent.width
                                    }
                                    AdvancedPopup{
                                        id: advancedPopup
                                        width: parent.width
                                    }

                                }


                                Rectangle{
                                    height: 35
                                    width: parent.width
                                    anchors.bottom: parent.bottom
                                    anchors.left: parent.left

                                    visible: quickRangeButton.highlighted

                                    color: "#DD444444"
                                    ManualRangeSliderQuick
                                    {
                                        id: quickManSlider
                                        anchors.fill: parent
                                    }
                                }

                            }

                            Component
                            {
                                id:stopComponent
                                GradientStop {}
                            }

                            Rectangle{
                                id: palettePanel
                                anchors.right: parent.right
                                width: 60
                                height: parent.height
                                color: "#444444"
                                //visible: _controllerCore.type !== mainWindow.gisType ? true : _controllerCore.cooldownDone
                                Rectangle{
                                    id: palette
                                    //visible: _controllerCore.type !== mainWindow.gisType ? true : _controllerCore.cooldownDone
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
                                        function onPaletteValuesChanged(){ palette.applyPaletteValues()}
                                    }

                                    Rectangle{
                                        id: alarmPaletteAboveRect
                                        color: advancedPopup.alarmColorAbove()
                                        width: palette.width
                                        anchors.top: parent.top
                                        function reDraw()
                                        {
                                            if (!_controllerCore.compareFw("1.1.1", _controllerCore.firmwareVersion))
                                                return;
                                            var height;
                                            var ptop = Math.round(_controllerCore.paletteTop*10)/10
                                            var pbottom = Math.round(_controllerCore.paletteBottom*10)/10

                                            if (measurePopup.rangeAlarmSliderSecond() < ptop)
                                            {
                                                height = palette.height * (ptop - measurePopup.rangeAlarmSliderSecond())  / (ptop - pbottom);
                                                alarmPaletteAboveRect.height = height > palette.height ?  palette.height :  height;
                                            }   else alarmPaletteAboveRect.height = 0;

                                            if (measurePopup.rangeAlarmSliderFirst() > pbottom)
                                            {
                                                height = palette.height * (measurePopup.rangeAlarmSliderFirst() - pbottom)  / (ptop - pbottom);
                                                alarmPaletteBelowRect.height = height > palette.height ?  palette.height :  height;
                                            }   else alarmPaletteBelowRect.height = 0;


                                            alarmPaletteAboveRect.visible = false;
                                            alarmPaletteBetweenRect.visible = false;
                                            alarmPaletteBelowRect.visible = false;

                                            if(measurePopup.alarmModeAboveChecked())
                                                alarmPaletteAboveRect.visible = true;
                                            else if(measurePopup.alarmModeBetweenChecked())
                                                alarmPaletteBetweenRect.visible = true;
                                            else if(measurePopup.alarmModeBelowChecked())
                                                alarmPaletteBelowRect.visible = true;
                                            else if (measurePopup.alarmModeOutsideChecked())
                                            {
                                                alarmPaletteAboveRect.visible = true;
                                                alarmPaletteBelowRect.visible = true;
                                            }
                                        }

                                        visible: false
                                    }

                                    Connections{
                                        target: _controllerCore
                                        function onPaletteTopChanged(){ alarmPaletteAboveRect.reDraw(); }
                                    }

                                    Connections{
                                        target: _controllerCore
                                        function onPaletteBottomChanged(){ alarmPaletteAboveRect.reDraw(); }
                                    }

                                    Rectangle{
                                        id: alarmPaletteBetweenRect
                                        color: advancedPopup.alarmColorBetween()
                                        width: palette.width
                                        anchors.top: alarmPaletteAboveRect.bottom
                                        anchors.bottom: alarmPaletteBelowRect.top

                                        visible: false
                                    }

                                    Rectangle{
                                        id: alarmPaletteBelowRect
                                        color: advancedPopup.alarmColorBelow()
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
                                    visible: _controllerCore.type !== mainWindow.gisType ? true : _controllerCore.cooldownDone
                                }
                                Rectangle{

                                    anchors.left: palette.right
                                    anchors.leftMargin: 5
                                    height: parent.height
                                    anchors.bottomMargin: 10
                                    anchors.topMargin: 30
                                    visible:{
                                        if(_controllerCore.type == mainWindow.securityType)
                                            return false;
                                        return _controllerCore.type !== mainWindow.gisType ? true : _controllerCore.cooldownDone
                                    }

                                    Text{
                                        id: unitText
                                        text: _controllerCore.thermalUnitSign
                                        color: "white"
                                        font.bold: true
                                        y: 5
                                    }
                                    Connections{
                                        target: _controllerCore
                                        function onThermalUnitChanged()
                                        {
                                            unitText.text = _controllerCore.thermalUnitSign
                                        }
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

                        }


                    }

                    Rectangle{
                        color: "#16151D"
                        width: parent.width
                        anchors.bottom: parent.bottom
                        height: 40

                        Rectangle{
                            id: quickMenuRect
                            width: controlPanel.width
                            anchors.top: parent.top
                            height: parent.height
                            Text{
                                font.bold: true
                                anchors.centerIn: parent
                                text: "Quick\nMenu"
                                color: "white"
                            }
                            gradient: Gradient {
                                GradientStop { position: 0.0; color: "white" }
                                GradientStop { position: 0.1; color: "#FF7604" }
                                GradientStop { position: 1.0; color: "orange" }
                            }
                        }

                        RowLayout{

                            anchors.left: quickMenuRect.right
                            width: parent.width - controlPanel.width
                            height: parent.height
                            Button{
                                id: quickCaptureButton
                                property bool stream: false
                                Layout.fillWidth: true
                                Layout.preferredHeight: 40
                                Rectangle{
                                    anchors.fill: parent
                                    color: "#16151D"


                                    Row{
                                        height: parent.height
                                        spacing: 5
                                        anchors.centerIn: parent
                                        Image{
                                            height: 20
                                            y: 10

                                            source: quickCaptureText.capturing ? "img/png/overlay_captured_active.png" : "img/png/overlay_captured.png"
                                            fillMode: Image.PreserveAspectFit

                                        }
                                        Text {
                                            id: quickCaptureText
                                            property bool capturing
                                            text: "Capture"
                                            color:{
                                                var res
                                                if(_controllerCore.type !== mainWindow.securityType)
                                                    res = quickCaptureText.capturing ? "red" : ((advancedPopup.imageCaptureEnabled()) ? (quickCaptureButton.hovered ? "orange" : "white")  : "gray")
                                                else
                                                    res = quickCaptureText.capturing ? "red" : ((advancedPopup.imagaCaptureSecEnabled()) ? (quickCaptureButton.hovered ? "orange" : "white")  : "gray")
                                                return res;
                                            }

                                            font.bold: true
                                            x: 5
                                            font.pixelSize: 14
                                            anchors.verticalCenter: parent.verticalCenter

                                        }
                                    }
                                }
                                onClicked: {
                                    if (capturePopup.periodicCaptureChecked())
                                    {
                                        if(capturePopup.isPeriodicCaptureRunning())
                                            capturePopup.setPeriodicCaptureRunning(false)
                                        else{
                                            _controllerCore.triggerCapture();
                                            capturePopup.setPeriodicCaptureRunning(true)
                                        }
                                    }
                                    else _controllerCore.triggerCapture();
                                }
                            }
                            Rectangle{
                                color: "gray"
                                Layout.preferredHeight: 20
                                Layout.preferredWidth: 2
                                radius: 1
                            }
                            Button{
                                id: quickRecordButton
                                Layout.fillWidth: true
                                Layout.preferredHeight: 40
                                Rectangle{
                                    anchors.fill: parent
                                    color: "#16151D"
                                    Row{
                                        height: parent.height
                                        spacing: 5
                                        anchors.centerIn: parent
                                        Image{
                                            height: 20
                                            y: 10

                                            source: quickRecordText.recording ? "img/png/overlay_video_active.png" : "img/png/overlay_video.png"
                                            fillMode: Image.PreserveAspectFit
                                        }
                                        Text {
                                            id: quickRecordText
                                            property bool recording
                                            text: "Record"
                                            color: quickRecordText.recording ? "red" : ((advancedPopup.visVidChecked() || advancedPopup.radVidChecked) ? (quickRecordButton.hovered ? "orange" : "white") : "gray")
                                            font.bold: true
                                            font.pixelSize: 14
                                            x: 5
                                            anchors.verticalCenter: parent.verticalCenter

                                        }
                                    }
                                }
                                onClicked: if(advancedPopup.visVidChecked() || advancedPopup.radVidChecked()) _controllerCore.toggleRecording()
                            }

                            Connections{
                                target: _controllerCore
                                function onCaptureRecordingChanged(capturing, recording)
                                {
                                    quickRecordText.recording = recording
                                    quickCaptureText.capturing = capturing
                                }
                            }

                            Rectangle{
                                color: "gray"
                                Layout.preferredHeight: 20
                                Layout.preferredWidth: 2
                                radius: 1
                                visible: _controllerCore.type != mainWindow.securityType

                            }
                            Button{
                                id: quickShutterButton
                                Layout.fillWidth: true
                                Layout.preferredHeight: 40
                                visible: _controllerCore.type != mainWindow.securityType
                                Rectangle{
                                    anchors.fill: parent
                                    color: "#16151D"


                                    Row{
                                        height: parent.height
                                        spacing: 5
                                        anchors.centerIn: parent
                                        Image{
                                            height: 20
                                            y: 10

                                            source: "img/png/overlay_shutter.png"
                                            fillMode: Image.PreserveAspectFit

                                        }
                                        Text {
                                            text: "NUC"
                                            color: quickShutterButton.hovered ? "orange" : "white"
                                            font.bold: true
                                            x: 5
                                            font.pixelSize: 14
                                            anchors.verticalCenter: parent.verticalCenter
                                        }
                                    }
                                }

                                onClicked: advancedPopup.applyShutter();
                            }
                            Rectangle{
                                color: "gray"
                                Layout.preferredHeight: 20
                                Layout.preferredWidth: 2
                                radius: 1
                                visible: _controllerCore.type != mainWindow.securityType

                            }
                            Button{
                                id: quickRangeButton
                                Layout.fillWidth: true
                                Layout.preferredHeight: 40
                                visible: _controllerCore.type != mainWindow.securityType
                                Rectangle{
                                    anchors.fill: parent
                                    color: "#16151D"


                                    Row{
                                        height: parent.height
                                        spacing: 5
                                        anchors.centerIn: parent
                                        Image{
                                            height: 20
                                            y: 10

                                            source: quickRangeButton.highlighted ? "img/png/overlay_range_active.png" : "img/png/overlay_range.png"
                                            fillMode: Image.PreserveAspectFit

                                        }
                                        Text {
                                            text: "Man. range"
                                            color: quickRangeButton.highlighted ? "red" : (quickRangeButton.hovered ? "orange" : "white")
                                            font.bold: true
                                            font.pixelSize: 14
                                            x: 5
                                            anchors.verticalCenter: parent.verticalCenter

                                        }
                                    }
                                }
                                property int origMode: 0
                                onClicked: {
                                    if(!highlighted) // set manual when opening
                                    {
                                        origMode = _controllerCore.rangeMode
                                        rangePopup.rangeModeManClicked();
                                    }
                                    if(highlighted) // set orig when closing
                                        _controllerCore.rangeMode = origMode
                                    quickRangeButton.highlighted = !quickRangeButton.highlighted;
                                }
                            }

                        }

                    }
                }
            }

            Rectangle{
                anchors.right: parent.right;
                anchors.top: parent.top;
                width: parent.width *3/10
                height: parent.height
                color: "black"

                Column{
                    anchors.fill: parent

                    Image{
                        id: visibleRect
                        anchors.top: parent.top
                        anchors.left: parent.left
                        width: parent.width
                        height: mainWindow.mainStreamThermal ? width/16*9 : width/5*4
                        //color: "green"
                        Connections{
                            target: _controllerCore
                            function onStreamFrameVisibleChanged()
                            {
                                if (mainWindow.mainStreamThermal)
                                {
                                    if (streamWindow.visible)
                                        separateView.source = "image://streamFramesProviderVisible/500/500/?seed=" + Math.random();
                                    else visibleRect.source = "image://streamFramesProviderVisible/500/500/?seed=" + Math.random();
                                }
                                else
                                    mainView.source = "image://streamFramesProviderVisible/500/500/?seed=" + Math.random();
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
                                source: "img/png/ico_arrow_both.png"
                                height: parent.height
                                fillMode: Image.PreserveAspectFit
                                rotation: 90
                            }
                            onClicked: {
                                mainWindow.mainStreamThermal = !mainWindow.mainStreamThermal;
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
                                source: "img/png/button-pause.png"
                                height: parent.height
                                fillMode: Image.PreserveAspectFit
                            }
                            onClicked: {
                                if(mainWindow.mainStreamThermal)
                                    _controllerCore.toggleStreamVisible();
                                else
                                    _controllerCore.toggleStreamThermal();
                                setImg();
                            }
                            function setImg()
                            {
                                if(mainWindow.mainStreamThermal)
                                {
                                    if(_controllerCore.streamingVisible())
                                        sidePanelPauseImg.source = "img/png/button-pause.png"
                                    else
                                        sidePanelPauseImg.source = "img/png/button-play.png"
                                }
                                else {
                                    if(_controllerCore.streamingThermal())
                                        sidePanelPauseImg.source = "img/png/button-pause.png"
                                    else
                                        sidePanelPauseImg.source = "img/png/button-play.png"
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
                                source: "img/png/ico_fullscreen.png"
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
                                    visibleRect.source = "img/png/logo-workswell.png"
                                    streamWindow.show();
                                }
                            }
                        }
                        MouseArea {
                            width: 20
                            hoverEnabled: true
                            height: 20
                            opacity: containsMouse ? 1.0 : 0.5
                            x: 110
                            y: parent.height - 30
                            Image {
                                source: "/img/png/button_functions_selected.png"
                                height: parent.height
                                fillMode: Image.PreserveAspectFit
                            }
                            onClicked: {
                                _controllerCore.secondaryCameraSnapshot()
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
                                        Image{
                                            height: 20
                                            anchors.verticalCenter: parent.verticalCenter
                                            source: "img/png/ico_gps.png"
                                            fillMode: Image.PreserveAspectFit

                                        }
                                        Text {
                                            id: gpsText
                                            font.pixelSize: 10
                                            color: "white"
                                            anchors.verticalCenter: parent.verticalCenter
                                            Connections{
                                                target: _controllerCore
                                                function onGpsChanged(val)
                                                {
                                                    gpsText.text = val
                                                }
                                            }

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
                                        Image{
                                            height: 20
                                            anchors.verticalCenter: parent.verticalCenter
                                            source: "img/png/ico_zoom.png"
                                            fillMode: Image.PreserveAspectFit

                                        }
                                        Text {
                                            text:"Thermal: " + _controllerCore.currentZoomThermal +"x"
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
                                        Image{
                                            height: 20
                                            anchors.verticalCenter: parent.verticalCenter
                                            source: "img/png/ico_shutter.png"
                                            fillMode: Image.PreserveAspectFit
                                        }
                                        Text {
                                            id: shutterText
                                            text: (advancedPopup.syncShutterChecked() ? ( advancedPopup.shutterPeriodSpinboxValue() + " min")  : "async")
                                            color: "white"
                                            font.pixelSize: 12
                                            anchors.verticalCenter: parent.verticalCenter
                                            Connections{
                                                target: _controllerCore
                                                function onShutterDataChanged(next, last)
                                                {
                                                    if (!_controllerCore.compareFw("1.1.1", _controllerCore.firmwareVersion) || _controllerCore.type == mainWindow.securityType)
                                                        return;
                                                    if(_controllerCore.type == mainWindow.gisType)
                                                    {
                                                        if(_controllerCore.cooldownDone)
                                                        {
                                                            shutterText.text = last;
                                                            nucPopup.visible = false;
                                                        }
                                                        else
                                                        {
                                                            shutterText.text = "N/A";
                                                            nucPopup.visible = false;
                                                        }
                                                        return;
                                                    }
                                                    if (advancedPopup.syncShutterChecked())
                                                        shutterText.text = next;
                                                    else
                                                        shutterText.text = last;
                                                    if (last == "0:00" || last == "0:01" || next == "0:01" || next == "0:00")
                                                        nucPopup.visible = true;
                                                    else
                                                        nucPopup.visible = false;
                                                }
                                            }

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
                                            source: "img/png/ico_range.png"
                                            fillMode: Image.PreserveAspectFit

                                        }
                                        Text {
                                            id: sideRangeText
                                            color: "white"
                                            anchors.verticalCenter: parent.verticalCenter
                                            font.pixelSize: 12
                                            Connections{
                                                target: _controllerCore
                                                function onCurrentEnviromentChanged()
                                                {
                                                    sideRangeText.text = Math.round(_controllerCore.enviromentLowerThreshold()) + " - " + Math.round(_controllerCore.enviromentUpperThreshold()) + _controllerCore.thermalUnitSign
                                                }
                                            }

                                        }
                                    }
                                    color: "#16151D"
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
                                            source: "img/png/button_measure.png"
                                            fillMode: Image.PreserveAspectFit

                                        }
                                        Text {
                                            id: sideAlarmText
                                            function setAlarmText(){
                                                if (measurePopup.alarmModeOffChecked())
                                                    sideAlarmText.text = "OFF";
                                                else if (measurePopup.alarmModeAboveChecked())
                                                    sideAlarmText.text = "Above: " + Math.round(measurePopup.rangeAlarmSliderSecond()*10)/10 + _controllerCore.thermalUnitSign;
                                                else if (measurePopup.alarmModeBetweenChecked())
                                                    sideAlarmText.text = "B: " + Math.round(measurePopup.rangeAlarmSliderFirst()*10)/10 + " - " + Math.round(measurePopup.rangeAlarmSliderSecond()*10)/10 + _controllerCore.thermalUnitSign;
                                                else if (measurePopup.alarmModeBelowChecked())
                                                    sideAlarmText.text = "Below: " + Math.round(measurePopup.rangeAlarmSliderFirst()*10)/10 + _controllerCore.thermalUnitSign;
                                                else if (measurePopup.alarmModeOutsideChecked())
                                                    sideAlarmText.text = "O: " + Math.round(measurePopup.rangeAlarmSliderFirst()*10)/10 + " - " + Math.round(measurePopup.rangeAlarmSliderSecond()*10)/10 + _controllerCore.thermalUnitSign;
                                            }
                                            color: "white"
                                            anchors.verticalCenter: parent.verticalCenter
                                            font.pixelSize: 12
                                            Connections{
                                                target: _controllerCore
                                                function onAlarmModeChanged(){ sideAlarmText.setAlarmText()}
                                            }
                                            Connections{
                                                target: _controllerCore
                                                function onAlarmFromChanged(){ sideAlarmText.setAlarmText()}
                                            }
                                            Connections{
                                                target: _controllerCore
                                                function onAlarmToChanged(){ sideAlarmText.setAlarmText()}
                                            }

                                        }
                                    }
                                    color: "#16151D"
                                }
                                CustomHr{
                                    visible: _controllerCore.type == mainWindow.securityType
                                }
                                Rectangle{
                                    visible: _controllerCore.type == mainWindow.securityType
                                    Layout.fillHeight: true
                                    Layout.fillWidth:  true
                                    Row{
                                        height: parent.height
                                        spacing: 5
                                        anchors.centerIn: parent
                                        Image{
                                            height: 20
                                            anchors.verticalCenter: parent.verticalCenter
                                            source: "img/png/ico_memory.png"
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
                                            text: ""
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
                                    Row{
                                        height: parent.height
                                        spacing: 5
                                        anchors.centerIn: parent
                                        Text {
                                            text: "CPU: " + Math.round(_controllerCore.cpuTemp) + _controllerCore.thermalUnitSign
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
                                            text: "IR Core: " + Math.round(_controllerCore.camTemp) + _controllerCore.thermalUnitSign
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
                                        Image{
                                            height: 20
                                            anchors.verticalCenter: parent.verticalCenter
                                            source: "img/png/ico_zoom.png"
                                            fillMode: Image.PreserveAspectFit

                                        }
                                        Text {
                                            text: "Visible: " +_controllerCore.currentZoomVisible +"x"
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
                                            source: "img/png/ico_memory.png"
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
                                                function onConnectedChanged(){ connectionText.text = "Connection: " + (_controllerCore.connected ? "OK" : "NONE")}
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
                                source: _controllerCore.type == mainWindow.securityType ? "img/png/logo_workswell_wws.png" : "img/png/logo_workswell_wwp.png"
                                fillMode: Image.PreserveAspectFit
                                height: 30
                            }
                        }
                    }
                }
            }
        }
    }
    Component.onCompleted: {
        setSliderRanges();
        var component = Qt.createComponent("components/ConnectionWindow.qml");
        var window    = component.createObject(mainWindow);
        window.show();
        mainWindow.hide();
    }
    ApplicationWindow {
        id: streamWindow
        title: qsTr("WIRIS & GIS Ethernet Stream SDK GUI 1.0.0B5")
        minimumWidth: 640
        minimumHeight: 480

        onHeightChanged: separateView.onResizedSeparateWindow(streamWindow.width, streamWindow.height)
        onWidthChanged: separateView.onResizedSeparateWindow(streamWindow.width, streamWindow.height)
        Image {
            id: separateView

            function onResizedSeparateWindow(parentWidth, parentHeight)
            {
                var cwidth = mainWindow.mainStreamThermal ? 16 : 5;
                var cheight = mainWindow.mainStreamThermal ? 9 : 4;
                if(parentWidth/cwidth*cheight < parentHeight)
                {
                    separateView.width = parentWidth;
                    separateView.height = separateView.width/cwidth*cheight;
                }
                else{
                    separateView.height = parentHeight;
                    separateView.width = separateView.height/cheight*cwidth;
                }
            }
            anchors.centerIn: parent
        }
    }

    Connections {
        target: _controllerCore
        function onCameraDisconnected() {
            showDisconnectedWindow()
        }
    }
}
