import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "./components"

Window {
    id: mainWindow

    function setSliderRanges()
    {
        if(range1.checked)
        {
            rangeAlarmSlider.from = range1.lowerThreshold;
            rangeAlarmSlider.to = range1.upperThreshold;

            rangeManSlider.from = range1.lowerThreshold;
            rangeManSlider.to = range1.upperThreshold;
        }
        else {
            rangeAlarmSlider.from = range2.lowerThreshold;
            rangeAlarmSlider.to = range2.upperThreshold;

            rangeManSlider.from = range2.lowerThreshold;
            rangeManSlider.to = range2.upperThreshold;
        }

        rangeAlarmSlider.first.value = rangeAlarmSlider.from;
        rangeAlarmSlider.second.value = rangeAlarmSlider.to;

        rangeManSlider.firstVal = rangeManSlider.from;
        rangeManSlider.secondVal = rangeManSlider.to;

    }

    property string securityType: "WIRIS Security"

    property bool mainStreamThermal: true;
    onMainStreamThermalChanged: {
        mainView.onResized(mainviewParent.width, mainviewParent.height);
        if (separateView)
            separateView.onResizedSeparateWindow(streamWindow.width, streamWindow.height);
    }
    visible: true
    minimumWidth: 960
    minimumHeight: 540
    title: qsTr("WIRIS Ethernet Stream SDK GUI 0.2.0")

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
                                            onTypeChanged: {
                                                if (type == mainWindow.securityType)
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
                                 onStreamFrameChanged:{
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
                                     else quickPauseImg.source = "img/png/button-play.png"
                                 }
                                 else {
                                     _controllerCore.toggleStreamThermal();
                                     if(_controllerCore.streamingThermal())
                                         quickPauseImg.source = "img/png/button-pause.png"
                                     else quickPauseImg.source = "img/png/button-play.png"
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

                             ControlPopup{
                                 id: rangePopup
                                 width: parent.width


                                 ColumnLayout{
                                     width: parent.width


                                     PopupItem{

                                         width: parent.width
                                         itemText: "Mode"
                                         Layout.preferredHeight: 25
                                     }
                                     RowLayout {
                                         id: rangeModeRowLayout

                                         Layout.preferredHeight: 35
                                         CustomRadioButton1{
                                             id: rangeModeAuto
                                             text: "Automatic"
                                             onClicked: {
                                                 _controllerCore.rangeMode = 0;
                                                 quickRangeButton.highlighted = false;
                                             }
                                         }

                                         CustomRadioButton1{
                                             id: rangeModeMan
                                             text: "Manual"
                                             onClicked: {
                                                 _controllerCore.rangeMode = 1;
                                             }
                                         }

                                         CustomRadioButton1{
                                             id: rangeModeSpan
                                             text: "Span"
                                             onClicked: {
                                                 _controllerCore.rangeMode = 2;
                                                 quickRangeButton.highlighted = false;
                                             }
                                         }

                                         Connections{
                                             target: _controllerCore
                                             onRangeModeChanged: {
                                                 alarmPaletteAboveRect.reDraw();
                                                 rangeModeAuto.checked = false;
                                                 rangeModeMan.checked = false;
                                                 rangeModeSpan.checked = false;
                                                 switch(mode)
                                                 {
                                                    case 0:
                                                        rangeModeAuto.checked = true;
                                                        break;
                                                    case 1:
                                                        rangeModeMan.checked = true;
                                                        break;
                                                    case 2:
                                                        rangeModeSpan.checked = true;
                                                        break;
                                                 }
                                             }
                                         }

                                     }


                                     ManualRangeSlider{
                                         id: rangeManSlider
                                     }

                                     Row{
                                         visible: rangeModeSpan.checked
                                         anchors.centerIn: parent
                                         height: 40

                                         Text {
                                             anchors.verticalCenter: parent.verticalCenter;
                                             text: "Window: "
                                             color: "white"
                                         }

                                         TextField{
                                            id: spanWindowTextField
                                            width: 60
                                            height: 30
                                            text: _controllerCore.spanWindow
                                            validator: RegExpValidator { regExp: /^(\d)+\.\d/}
                                            background: Rectangle{color: "#66000000"; border.color: "orange"; border.width: 1}
                                            color: "orange"
                                            onTextChanged: {
                                                if (spanWindowTextField.text == "" || spanWindowTextField.text.substr(spanWindowTextField.text.length -1 ) == ".")
                                                {
                                                    spanWindowTextField.color = "red";
                                                    return;
                                                }
                                                 if ((+spanWindowTextField.text/2) + (+spanCenterTextField.text) <= _controllerCore.enviromentUpperThreshold() && +spanCenterTextField.text - (+spanWindowTextField.text/2) >= _controllerCore.enviromentLowerThreshold()){
                                                     _controllerCore.spanWindow = spanWindowTextField.text
                                                     spanWindowTextField.color = "orange";
                                                 }
                                                 else spanWindowTextField.color = "red";
                                            }
                                         }

                                         Text {
                                             anchors.verticalCenter: parent.verticalCenter;
                                             text: " Center: "
                                             color: "white"
                                         }

                                         TextField{
                                             id: spanCenterTextField
                                             width: 60
                                             height: 30
                                             text: _controllerCore.spanCenter
                                             background: Rectangle{color: "#66000000"; border.color: "orange"; border.width: 1}
                                             color: "orange"
                                             validator: RegExpValidator { regExp: /^[+-]?(\d)+\.\d/}
                                             onTextChanged: {
                                                 if (spanCenterTextField.text == "" || spanCenterTextField.text.substr(spanCenterTextField.text.length -1 ) == ".")
                                                 {
                                                     spanCenterTextField.color = "red";
                                                     return;
                                                 }
                                                  if ((+spanWindowTextField.text/2) + (+spanCenterTextField.text) <= _controllerCore.enviromentUpperThreshold() && +spanCenterTextField.text - (+spanWindowTextField.text/2) >= _controllerCore.enviromentLowerThreshold()){
                                                      _controllerCore.spanCenter = spanCenterTextField.text
                                                      spanCenterTextField.color = "orange";
                                                  }
                                                  else spanCenterTextField.color = "red";
                                             }
                                         }

                                         Connections{
                                             target: _controllerCore
                                             onSpanCenterChanged:{
                                                spanCenterTextField.text = Math.round(_controllerCore.spanCenter*10)/10
                                             }
                                         }
                                         Connections{
                                             target: _controllerCore
                                             onSpanWindowChanged:{
                                                spanWindowTextField.text = Math.round(_controllerCore.spanWindow*10)/10
                                             }
                                         }
                                     }


                                     PopupItem{

                                         width: parent.width
                                         itemText: "Environment"
                                         Layout.preferredHeight: 25
                                     }
                                     RowLayout {
                                         Layout.preferredHeight: 20
                                         CustomRadioButton1{
                                             id: range1
                                             checked: _controllerCore.isCurrentEnviroment(range1.envValue)
                                             text: "-25-°C-150°C"
                                             property int upperThreshold: 150
                                             property int lowerThreshold: -25

                                             property string envValue

                                             Connections{
                                                 target: _controllerCore
                                                 onEnviroment1Changed: {
                                                     range1.envValue = _controllerCore.enviroment1
                                                     range1.text = "-25 - 150°C"//Math.round(_controllerCore.enviromentLowerThreshold()) + " - " + Math.round(_controllerCore.enviromentUpperThreshold());
                                                 }
                                             }

                                             onClicked: _controllerCore.setEnviroment1(range1.envValue);
                                         }

                                         CustomRadioButton1{
                                             id: range2
                                             checked: _controllerCore.isCurrentEnviroment(range2.envValue)
                                             text: "-40°C-550°C"
                                             property int upperThreshold: 550
                                             property int lowerThreshold: -40
                                             property string envValue

                                             onClicked: _controllerCore.setEnviroment2(range2.envValue);

                                             Connections{
                                                 target: _controllerCore
                                                 onEnviroment2Changed: {
                                                     range2.envValue = _controllerCore.enviroment2
                                                     range2.text = "-40 - 550°C" //Math.round(_controllerCore.enviromentLowerThreshold()) + " - " + Math.round(_controllerCore.enviromentUpperThreshold());
                                                 }
                                             }
                                         }

                                         Connections{
                                             target: _controllerCore
                                             onCurrentEnviromentChanged: {
                                                 range1.checked = _controllerCore.isCurrentEnviroment(range1.envValue);
                                                 range2.checked = _controllerCore.isCurrentEnviroment(range2.envValue);

                                                 if (range1.checked)
                                                 {
                                                     range1.upperThreshold = _controllerCore.enviromentUpperThreshold();
                                                     range1.lowerThreshold = _controllerCore.enviromentLowerThreshold();
                                                 }
                                                 else if (range2.checked)
                                                 {
                                                     range2.upperThreshold = _controllerCore.enviromentUpperThreshold();
                                                     range2.lowerThreshold = _controllerCore.enviromentLowerThreshold();
                                                 }
                                             }
                                         }

                                         Component.onCompleted: setSliderRanges()

                                     }
                                 }

                             }
                             ControlPopup{
                                id: functionsPopup
                                width: parent.width
                                ColumnLayout{
                                    width: parent.width


                                    PopupItem{

                                        width: parent.width
                                        itemText: "ZOOM"
                                        Layout.preferredHeight: 25
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
                                                text: "Thermal:"
                                            }
                                            ZoomSpinBox{
                                                Layout.preferredWidth:  120
                                                id: zoomSpinBox
                                                editable: false

                                                Connections{
                                                    target: zoomSpinBox.up
                                                    onPressedChanged:{
                                                     if(zoomSpinBox.up.pressed)
                                                         _controllerCore.changeZoomThermal(true);
                                                    }
                                                }
                                                Connections{
                                                    target: zoomSpinBox.down
                                                    onPressedChanged:{
                                                     if(zoomSpinBox.down.pressed)
                                                         _controllerCore.changeZoomThermal(false);
                                                    }
                                                }

                                                Connections{
                                                    target: _controllerCore
                                                    onCurrentZoomThermalChanged:{
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
                                                    onPressedChanged:{
                                                     if(zoomVisibleSpinBox.up.pressed)
                                                         _controllerCore.changeZoomVisible(true);
                                                    }
                                                }
                                                Connections{
                                                    target: zoomVisibleSpinBox.down
                                                    onPressedChanged:{
                                                     if(zoomVisibleSpinBox.down.pressed)
                                                         _controllerCore.changeZoomVisible(false);
                                                    }
                                                }

                                                Connections{
                                                    target: _controllerCore
                                                    onCurrentZoomVisibleChanged:{
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


                                }
                             }
                             ControlPopup{
                                 id: palettePopup
                                 width: parent.width

                                 ColumnLayout{
                                     width: parent.width
                                     height: parent.height

                                     PopupItem{

                                         width: parent.width
                                         itemText: "Palettes"
                                         Layout.preferredHeight: 25
                                     }


                                     Component{
                                         id: paletteDelegate
                                         PaletteButton {
                                            id: delegateButton
                                            text: value
                                            highlighted: _controllerCore.currentPalette == value
                                            onClicked: _controllerCore.setCurrentPaletteByIndex(index)
                                         }
                                     }

                                     ListView {
                                         id: palettesListView
                                         Layout.fillHeight: true
                                         Layout.fillWidth: true

                                         model: _controllerCore.palettes
                                         delegate: paletteDelegate
                                         Connections{
                                             target: _controllerCore
                                             onPalettesChanged: {
                                                 palettesListView.model = _controllerCore.palettes;
                                             }
                                         }
                                     }
                                 }
                             }
                             ControlPopup{
                                 id: capturePopup
                                 width: parent.width

                                 ColumnLayout{
                                     width: parent.width

                                     PopupItem{

                                         width: parent.width
                                         itemText: "Capture & Record"
                                         Layout.preferredHeight: 25
                                     }

                                     Rectangle{
                                         Layout.preferredWidth: 200
                                         Layout.preferredHeight: 50
                                         Layout.alignment: Qt.AlignHCenter
                                         color: "transparent"
                                         anchors.margins: 50
                                         PopupButton{
                                             anchors.left: parent.left
                                             width: parent.width * 0.45
                                             text: "Capture"
                                             onClicked: quickCaptureButton.onClicked();
                                         }

                                         PopupButton{
                                             anchors.right: parent.right
                                             width: parent.width * 0.45
                                             text: "Record"

                                             onClicked: {
                                                 if(visVidCheck.checked || radVidCheck.checked || quickRecordText.recording)
                                                    _controllerCore.toggleRecording();

                                             }

                                         }

                                     }
                                     PopupCheckBox{
                                         id: periodicCaptureCheck
                                         checked: false
                                         text: "Periodec image capture"
                                         onCheckedChanged: {
                                             if (!checked)
                                                 periodicCaptureTimer.stop();
                                         }

                                     }

                                     CustomSpinBox{
                                         id: periodicCaptureSpinBox
                                         visible: periodicCaptureCheck.checked
                                         from: 2
                                         to: 60
                                         value: 5
                                         editable: false;
                                         append: " seconds"
                                         Layout.preferredWidth: 160
                                         Layout.alignment: Qt.AlignHCenter
                                         Layout.preferredHeight: 25

                                     }
                                     Text {
                                         text: "In progress..."
                                         color: "red"
                                         font.bold: true;
                                         Layout.alignment: Qt.AlignHCenter
                                         Layout.preferredHeight: 25
                                         visible: periodicCaptureTimer.running
                                     }
                                 }
                                 Timer{
                                     id: periodicCaptureTimer
                                     repeat: true
                                     triggeredOnStart: false
                                     interval: periodicCaptureSpinBox.value * 1000
                                     onTriggered: {
                                         if(periodicCaptureCheck.checked)
                                             _controllerCore.triggerCapture();
                                     }

                                 }
                             }
                             ControlPopup{
                                 id: measurePopup
                                 width: parent.width

                                 ColumnLayout{
                                     width: parent.width

                                     PopupItem{
                                         width: parent.width
                                         itemText: "Measure"
                                         Layout.preferredHeight: 25
                                     }

                                     PopupCheckBox{
                                         id: showMaxCheck
                                         text: "Show Cross Max"
                                         checked: false
                                         onCheckedChanged: _controllerCore.paintMax(checked)
                                     }

                                     PopupCheckBox{
                                         id: showMinCheck
                                         text: "Show Cross Min"
                                         checked: false
                                         onCheckedChanged: _controllerCore.paintMin(checked)
                                     }

                                     PopupCheckBox{
                                         checked: false
                                         text: "Show Cross Center"
                                         onCheckedChanged: _controllerCore.paintCenter(checked)
                                     }

                                     PopupCheckBox{
                                         text: "Show Temperatures"
                                         checked: false
                                         visible: _controllerCore.type != mainWindow.securityType
                                         enabled: showMaxCheck.checked || showMinCheck.checked
                                         onCheckedChanged: _controllerCore.paintVals(checked)
                                     }

                                     PopupItem{

                                         width: parent.width
                                         itemText: "Alarm"
                                         visible: _controllerCore.type != mainWindow.securityType
                                         Layout.preferredHeight: 25
                                     }
                                     Grid {
                                         visible: _controllerCore.type != mainWindow.securityType
                                         id: alarmLayout
                                         Layout.preferredHeight: 70
                                         columns: 3
                                         function setAlarmMode(val, from, to)
                                         {
                                             _controllerCore.alarmMode = val;
                                             _controllerCore.alarmFrom = from;
                                             _controllerCore.alarmTo = to;
                                         }

                                         function setAlarmFirstVal(val)
                                         {
                                             _controllerCore.alarmFrom = val;
                                         }
                                         function setAlarmSecondVal(val)
                                         {
                                             _controllerCore.alarmTo = val;
                                         }

                                         CustomRadioButton1{
                                             checked: true
                                             id: alarmModeOff
                                             text: "Off"
                                             onCheckedChanged: if(checked) alarmLayout.setAlarmMode(0, rangeAlarmSlider.first.value, rangeAlarmSlider.second.value);

                                         }


                                         CustomRadioButton1{
                                             id: alarmModeAbove
                                             text: "Above"
                                             onCheckedChanged: if(checked) alarmLayout.setAlarmMode(1, rangeAlarmSlider.from, rangeAlarmSlider.second.value);
                                         }

                                         CustomRadioButton1{
                                             id: alarmModeBelow
                                             text: "Below"
                                             onClicked: {rangeAlarmSlider.second.value = rangeAlarmSlider.to;}
                                             onCheckedChanged: if(checked) alarmLayout.setAlarmMode(2, rangeAlarmSlider.first.value, rangeAlarmSlider.to);
                                         }

                                         CustomRadioButton1{
                                             id: alarmModeBetween
                                             text: "Between"
                                             onCheckedChanged: if(checked) alarmLayout.setAlarmMode(3, rangeAlarmSlider.first.value, rangeAlarmSlider.second.value);
                                         }

                                         CustomRadioButton1{
                                             id: alarmModeOutside
                                             text: "Outside"
                                             onCheckedChanged: if(checked) alarmLayout.setAlarmMode(4, rangeAlarmSlider.first.value, rangeAlarmSlider.second.value);
                                         }

                                        Connections{
                                            target: _controllerCore
                                            onAlarmModeChanged:{
                                                switch(val)
                                                {
                                                case 0:
                                                    alarmModeOff.checked = true;
                                                    break;
                                                case 1:
                                                    alarmModeAbove.checked = true;
                                                    break;
                                                case 2:
                                                    alarmModeBelow.checked = true;
                                                    break;
                                                case 3:
                                                    alarmModeBetween.checked = true;
                                                    break;
                                                case 4:
                                                    alarmModeOutside.checked = true;
                                                    break;
                                                }
                                            }
                                        }

                                     }
                                     Rectangle{
                                         color: "transparent"
                                         Layout.preferredHeight: 40
                                         Layout.preferredWidth: parent.width-4


                                         TextField {
                                             id: alarmFromText
                                             anchors.left: parent.left
                                             width: parent.width*1/6
                                             color: "orange"
                                             background: Rectangle{color: "#66000000"; border.color: "orange"; border.width: 1}
                                             height: 30
                                             y: 5
                                             validator: RegExpValidator { regExp: /^[+-]?(\d)+\.\d/}
                                             horizontalAlignment: Text.AlignHCenter
                                             verticalAlignment: Text.AlignVCenter
                                             visible: rangeAlarmSlider.visible
                                             text:  Math.round(rangeAlarmSlider.first.value*10)/10
                                             onTextChanged: {
                                                 if (+alarmFromText.text === Math.round(rangeAlarmSlider.first.value*10)/10)
                                                 {
                                                     alarmFromText.color = "orange";
                                                     return;
                                                 }
                                                 if (+alarmFromText.text < +alarmToText.text && +alarmFromText.text >= rangeAlarmSlider.from && alarmFromText.text != "")
                                                 {
                                                     rangeAlarmSlider.first.value = alarmFromText.text;
                                                     alarmFromText.color = "orange";
                                                 }
                                                 else alarmFromText.color = "red";
                                             }
                                         }

                                         RangeSlider {
                                             id: rangeAlarmSlider
                                             enabled: !alarmModeOff.checked
                                             visible: enabled
                                             Connections {
                                                 target: rangeAlarmSlider.second
                                                 onValueChanged: {
                                                     if (alarmModeBelow.checked && rangeAlarmSlider.second.value != rangeAlarmSlider.to) {
                                                         rangeAlarmSlider.second.value = rangeAlarmSlider.to;

                                                     }
                                                     if (!alarmModeBelow.checked)
                                                     {
                                                         alarmSecondDebounceTimer.stop();
                                                         alarmSecondDebounceTimer.start();
                                                     }

                                                 }
                                             }
                                             Connections {
                                                 target: rangeAlarmSlider.first
                                                 onValueChanged: {
                                                     if (alarmModeAbove.checked && rangeAlarmSlider.first.value != rangeAlarmSlider.from)
                                                     {
                                                         rangeAlarmSlider.first.value = rangeAlarmSlider.from;
                                                     }
                                                     if (!alarmModeAbove.checked)
                                                     {
                                                         alarmFirstDebounceTimer.stop();
                                                         alarmFirstDebounceTimer.start();
                                                     }
                                                 }
                                             }

                                             Connections{
                                                 target: _controllerCore
                                                 onAlarmFromChanged:{
                                                     rangeAlarmSlider.first.value = val;
                                                     alarmPaletteAboveRect.reDraw();
                                                 }
                                             }
                                             Connections{
                                                 target: _controllerCore
                                                 onAlarmToChanged:{
                                                     rangeAlarmSlider.second.value = val;
                                                     alarmPaletteAboveRect.reDraw();
                                                 }
                                             }

                                             Timer{
                                                 id: alarmFirstDebounceTimer
                                                 interval: 300
                                                 repeat: false
                                                 onTriggered: alarmLayout.setAlarmFirstVal(rangeAlarmSlider.first.value)
                                             }
                                             Timer{
                                                 id: alarmSecondDebounceTimer
                                                 interval: 300
                                                 repeat: false
                                                 onTriggered: alarmLayout.setAlarmSecondVal(rangeAlarmSlider.second.value)
                                             }


                                             anchors.left: alarmFromText.right
                                             width: parent.width*2/3
                                             from: 1
                                             to: 100
                                             first.value: 25
                                             second.value: 75


                                             //style
                                             background: Rectangle {
                                                     x: rangeAlarmSlider.leftPadding
                                                     y: rangeAlarmSlider.topPadding + rangeAlarmSlider.availableHeight / 2 - height / 2
                                                     implicitWidth: 200
                                                     implicitHeight: 4
                                                     width: rangeAlarmSlider.availableWidth
                                                     height: implicitHeight
                                                     radius: 2
                                                     color: colorBelowComboBox.currentText

                                                     Rectangle {
                                                         x: rangeAlarmSlider.first.visualPosition * parent.width
                                                         width: rangeAlarmSlider.second.visualPosition * parent.width - x
                                                         height: parent.height
                                                         color: colorBetweenComboBox.currentText
                                                         radius: 2
                                                     }

                                                     Rectangle {
                                                         x: rangeAlarmSlider.second.visualPosition * parent.width
                                                         width: rangeAlarmSlider.availableWidth - x
                                                         height: parent.height
                                                         color: colorAboveComboBox.currentText
                                                         radius: 2
                                                     }
                                                 }
                                         }


                                         TextField {
                                             id: alarmToText
                                             anchors.left: rangeAlarmSlider.right
                                             width: parent.width*1/6
                                             color: "orange"
                                             background: Rectangle{color: "#66000000"; border.color: "orange"; border.width: 1}
                                             height: 30
                                             y: 5
                                             validator: RegExpValidator { regExp: /^[+-]?(\d)+\.\d/}
                                             horizontalAlignment: Text.AlignHCenter
                                             verticalAlignment: Text.AlignVCenter
                                             visible: rangeAlarmSlider.visible
                                             text:  Math.round(rangeAlarmSlider.second.value*10)/10
                                             onTextChanged: {
                                                 if (+alarmToText.text === Math.round(rangeAlarmSlider.second.value*10)/10){
                                                     alarmToText.color = "orange";
                                                     return;
                                                 }
                                                 if (+alarmFromText.text < +alarmToText.text && +alarmToText.text <= rangeAlarmSlider.to && alarmToText.text != "")
                                                 {
                                                     rangeAlarmSlider.second.value = alarmToText.text;
                                                     alarmToText.color = "orange";
                                                 }
                                                 else alarmToText.color = "red";
                                             }
                                         }

                                     }
                                 }

                             }
                             ControlPopup{
                                 id: advancedPopup
                                 width: parent.width

                                 ColumnLayout{
                                     width: parent.width
                                     PopupDropDown{
                                         id: thermalDropDown
                                         dropdownText: "Thermal Camera"
                                         visible: _controllerCore.type != mainWindow.securityType
                                     }
                                     ColumnLayout{
                                         Layout.alignment: Qt.AlignHCenter
                                         visible: thermalDropDown.activated

                                         Text{
                                             color: "white"
                                             font.pixelSize: 12
                                             text: "Emissivity: "
                                             font.bold: true
                                         }

                                        Rectangle{
                                            id: emissivityBackground
                                            Layout.preferredHeight: 15
                                            Layout.preferredWidth: 50
                                             TextInput {
                                                 anchors.fill: parent
                                                 id: emissivityValue
                                                 text: Math.round(_controllerCore.emissivity*100)/100
                                                 font.bold: true
                                                  validator: RegExpValidator { regExp: /^0\.\d\d/}
                                                 onTextChanged: {

                                                      if (emissivityValue.text > 0.5 && emissivityValue.text <= 1){
                                                          _controllerCore.emissivity = emissivityValue.text
                                                          emissivityValue.color = "black";
                                                      }
                                                      else emissivityValue.color = "red";
                                                 }
                                                 Connections{
                                                    target: _controllerCore
                                                    onEmissivityChanged: emissivityValue.text = Math.round(_controllerCore.emissivity*100)/100
                                                 }
                                             }
                                        }

                                         Text{
                                             id: thermalText
                                             color: "white"
                                             text: "Shutter: "
                                             font.pixelSize: 12
                                             font.bold: true
                                         }

                                         PopupCheckBox{
                                             id: syncShutter
                                             text: "Synchronous Correction"
                                             checked: _controllerCore.syncShutter
                                             onCheckedChanged: _controllerCore.syncShutter = checked;

                                             Connections{
                                                 target: _controllerCore
                                                 onSyncShutterChanged:{
                                                     syncShutter.checked = _controllerCore.syncShutter;
                                                 }
                                             }
                                         }

                                         ShutterSpinBox{
                                            id: shutterPeriodSpinbox


                                            function applyShutter()
                                            {
                                                _controllerCore.shutterPeriod = shutterPeriodSpinbox.value;

                                            }

                                            Layout.preferredHeight: 25
                                            visible: syncShutter.checked
                                            onValueChanged: {
                                                shutterDebounceTimer.stop();
                                                shutterDebounceTimer.start();
                                            }

                                            Connections{
                                                target: _controllerCore
                                                onShutterPeriodChanged:{
                                                    shutterPeriodSpinbox.value = _controllerCore.shutterPeriod;
                                                }
                                            }

                                            Timer{
                                                id: shutterDebounceTimer
                                                interval: 5000
                                                repeat: false
                                                onTriggered: shutterPeriodSpinbox.applyShutter()
                                            }


                                         }
                                     }

                                     Rectangle{
                                         Layout.fillWidth: true
                                         height: 1
                                         color: "black"

                                     }

                                     PopupDropDown{
                                         id: imageVidDropDown
                                         dropdownText: "Images & Video"
                                     }

                                     ColumnLayout{
                                         id: imageVidColumn

                                         function setImageVid()
                                         {
                                             _controllerCore.setCaptureVideoSettings(radJpgCheck.checked, radTiffCheck.checked, srImgCheck.checked, visJpgCheck.checked, radVidCheck.checked, visVidCheck.checked)
                                         }

                                         Layout.alignment: Qt.AlignHCenter
                                         visible: imageVidDropDown.activated

                                         PopupCheckBox{
                                             id: radJpgCheck
                                             text: _controllerCore.type == mainWindow.securityType ? "Image IR JPEG" : "Image Radiometric JPEG"
                                             onCheckedChanged: imageVidColumn.setImageVid();
                                         }

                                         PopupCheckBox{
                                             id: radTiffCheck
                                             text: "Image Radiometric TIFF"
                                             onCheckedChanged: imageVidColumn.setImageVid();
                                            visible: _controllerCore.type != mainWindow.securityType
                                         }

                                         PopupCheckBox{
                                             id: srImgCheck
                                             text: "Super‐resolution Image"
                                             onCheckedChanged: imageVidColumn.setImageVid();
                                             visible: _controllerCore.type != mainWindow.securityType
                                         }

                                         PopupCheckBox{
                                             id: visJpgCheck
                                             text: "Image Visible JPEG"
                                             onCheckedChanged: imageVidColumn.setImageVid();
                                         }

                                         Text{
                                             text: "Save Images to: "
                                             font.pixelSize: 12
                                             color: "white"
                                             font.bold: true
                                         }


                                         CustomComboBox{
                                             id: selectedStorage
                                             model: ["SSD", "SD", "FLASH"]
                                             onCurrentTextChanged: {

                                             }
                                         }

                                         Connections{
                                            target: _controllerCore
                                            onDriveStateChanged:{
                                                var gig = 1024*1024*1024;
                                                ssdText.text = "SSD: " + ssd
                                                sdText.text = "SD Card: " + sd;
                                                flashText.text = "Flash: " + flash;

                                                var arr = [];
                                                if(ssd == "READY")
                                                {
                                                    arr.push("SSD");
                                                    ssdText.text += " " + Math.round(ssdCap/gig) + " GB " + Math.round(ssdFree) + "% free";
                                                    sideSsdText.text = "SSD Free: "+ Math.round(ssdFree) + "%"
                                                }
                                                if(sd == "READY")
                                                {
                                                    arr.push("SD_CARD");
                                                    sdText.text += " " + Math.round(sdCap/gig) + " GB " + Math.round(sdFree) + "% free";
                                                }
                                                if(flash == "READY")
                                                {
                                                    arr.push("FLASH_DRIVE");
                                                    flashText.text += " " + Math.round(flashCap/gig) + " GB " + Math.round(flashFree) + "% free";
                                                }
                                                selectedStorage.model = arr;
                                                selectedStorage.update();
                                                selectedStorage.customBackground = _controllerCore.changeStorage(selectedStorage.currentText) ? "white" : "red";

                                            }
                                         }

                                         PopupCheckBox{
                                             id: radVidCheck
                                             enabled: !quickRecordText.recording
                                             text: _controllerCore.type == mainWindow.securityType ? "Video IR (SSD)" :"Video Radiometric (SSD)"
                                             onCheckedChanged: imageVidColumn.setImageVid();
                                         }

                                         PopupCheckBox{
                                             id: visVidCheck
                                             enabled: !quickRecordText.recording
                                             text: "Video Visible (SSD)"
                                             onCheckedChanged: imageVidColumn.setImageVid();
                                         }


                                         Connections{
                                             target: _controllerCore
                                             onCapRecSettingChanged: {
                                                 radJpgCheck.checked = radJpg;
                                                 radTiffCheck.checked = radTiff;
                                                 srImgCheck.checked = srImg;
                                                 visJpgCheck.checked = visJpg;
                                                 radVidCheck.checked = radVid;
                                                 visVidCheck.checked = visVid;
                                             }
                                         }

                                     }

                                     Rectangle{
                                         Layout.fillWidth: true
                                         height: 1
                                         color: "black"
                                     }

                                     PopupDropDown{
                                         id: alarmsDropDown
                                         dropdownText: "Alarms"
                                     }
                                     ColumnLayout{
                                         Layout.alignment: Qt.AlignHCenter
                                         visible: alarmsDropDown.activated

                                         Text{
                                             text: "Alarm Above: "
                                             color: "white"
                                             font.pixelSize: 12
                                             font.bold: true
                                         }
                                         CustomComboBox{
                                             id: colorAboveComboBox
                                             model: ["RED", "GREEN", "BLUE"]
                                             onCurrentTextChanged: _controllerCore.setAlarmColors(colorAboveComboBox.currentText, colorBetweenComboBox.currentText, colorBelowComboBox.currentText)
                                         }

                                         Text{
                                             text: "Alarm Below: "
                                             color: "white"
                                             font.pixelSize: 12
                                             font.bold: true
                                         }
                                         CustomComboBox{
                                             id: colorBelowComboBox
                                             model: ["RED", "GREEN", "BLUE"]
                                             onCurrentTextChanged: _controllerCore.setAlarmColors(colorAboveComboBox.currentText, colorBetweenComboBox.currentText, colorBelowComboBox.currentText)
                                         }

                                         Text{
                                             text: "Alarm Between: "
                                             color: "white"
                                             font.pixelSize: 12
                                             font.bold: true
                                         }
                                         CustomComboBox{
                                             id: colorBetweenComboBox
                                             model: ["RED", "GREEN", "BLUE"]
                                             onCurrentTextChanged: _controllerCore.setAlarmColors(colorAboveComboBox.currentText, colorBetweenComboBox.currentText, colorBelowComboBox.currentText)
                                         }

                                         Connections{
                                             target: _controllerCore
                                             onAlarmColorsChanged:{
                                                 colorAboveComboBox.currentIndex = above;
                                                 colorBetweenComboBox.currentIndex = between;
                                                 colorBelowComboBox.currentIndex = below;
                                             }
                                         }


                                     }
                                     Rectangle{
                                         visible: alarmsDropDown.visible
                                         Layout.fillWidth: true
                                         height: 1
                                         color: "black"
                                     }


                                     PopupDropDown{
                                         id: memoryDropDown
                                         dropdownText: "Memory"
                                     }
                                     ColumnLayout{
                                         Layout.alignment: Qt.AlignHCenter
                                         visible: memoryDropDown.activated

                                         Text{
                                             id: ssdText
                                             color: "white"
                                             font.bold: true
                                         }

                                         Text{
                                             id: sdText
                                             color: "white"
                                             font.bold: true
                                         }

                                         Text{
                                             id: flashText
                                             color: "white"
                                             font.bold: true
                                         }

                                     }
                                     Rectangle{
                                         Layout.fillWidth: true
                                         height: 1
                                         color: "black"
                                     }


                                     PopupDropDown{
                                         id: systemDropDown
                                         dropdownText: "System"
                                     }

                                     ColumnLayout{
                                         Layout.alignment: Qt.AlignHCenter
                                         visible: systemDropDown.activated

                                         Text{
                                             color: "white"
                                             font.pixelSize: 12
                                             text: "Current time: " + _controllerCore.systemTime
                                         }

                                         Text{
                                             color: "white"
                                             font.pixelSize: 12
                                             text: "Current Date: " + _controllerCore.systemDate
                                         }
                                         Text{
                                             color: "white"
                                             font.pixelSize: 12
                                             text: "Units: °C"
                                         }
                                         Text{
                                             color: "white"
                                             font.pixelSize: 12
                                             text: "Languge: English"
                                         }

                                         Text{
                                             id: rebtText
                                             color: maRbt.containsMouse ? "orange" : "white"
                                             font.bold: true
                                             font.pixelSize: 12
                                             text: "Reboot System"
                                             MouseArea {
                                                 id: maRbt
                                                 anchors.fill: parent
                                                 hoverEnabled: true
                                                 onClicked:{
                                                    rebootWarningText.visible = true;
                                                    rebootRow.visible = true;
                                                 }
                                             }
                                         }


                                             Text{
                                                 id: rebootWarningText
                                                 text: "Reboot?\nThis will restart the app\nand make WIRIS\nunreachable for a while"
                                                 color: "red"
                                                 visible: false
                                                 font.pixelSize: 12
                                             }
                                             RowLayout{
                                                 id: rebootRow
                                                 visible: false
                                                 PopupButton{
                                                     text: "Reboot"
                                                    Layout.preferredHeight: 20
                                                    Layout.preferredWidth: 60
                                                    onClicked: _controllerCore.reboot();
                                                 }
                                                 PopupButton{
                                                     Layout.preferredHeight: 20
                                                    Layout.preferredWidth: 60
                                                     text: "Cancel"
                                                     onClicked: {
                                                         rebootWarningText.visible = false;
                                                         rebootRow.visible = false;
                                                     }
                                                 }
                                             }


                                         Connections{
                                             target: _controllerCore
                                             onFirmwareVersionChanged:{
                                                 if (!_controllerCore.compareFw("1.1.1", val))
                                                 {
                                                     rebtText.visible = false;
                                                     alarmsDropDown.visible = false;
                                                 }
                                             }
                                         }

                                         Text{
                                             color: maSds.containsMouse ? "orange" : "white"
                                             font.bold: true
                                             font.pixelSize: 12
                                             text: "Set Default Settings"

                                             MouseArea {
                                                 id: maSds
                                                 anchors.fill: parent
                                                 hoverEnabled: true
                                                 onClicked:{
                                                    defaultWarningText.visible = true;
                                                    defaultRow.visible = true;
                                                 }
                                             }
                                         }
                                         Text{
                                             id: defaultWarningText
                                             text: "Set default settigns?\nThis will restart the app"
                                             color: "red"
                                             visible: false
                                             font.pixelSize: 12
                                         }
                                         RowLayout{
                                             id: defaultRow
                                             visible: false
                                             PopupButton{
                                                 text: "Set"
                                                Layout.preferredHeight: 20
                                                Layout.preferredWidth: 60
                                                onClicked: _controllerCore.setDefaultSettings()
                                             }
                                             PopupButton{
                                                 Layout.preferredHeight: 20
                                                Layout.preferredWidth: 60
                                                 text: "Cancel"
                                                 onClicked: {
                                                     defaultWarningText.visible = false;
                                                     defaultRow.visible = false;
                                                 }
                                             }
                                         }

                                     }
                                     Rectangle{
                                         Layout.fillWidth: true
                                         height: 1
                                         color: "black"
                                     }
                                     PopupDropDown{
                                         id: infoDropdown
                                         dropdownText: "Info"
                                     }
                                     InfoPanel{}
                                 }
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

                                visible: false
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
                                color: quickCaptureText.capturing ? "red" : (quickCaptureButton.hovered ? "orange" : "white")
                                font.bold: true
                                x: 5
                                font.pixelSize: 14
                                anchors.verticalCenter: parent.verticalCenter

                            }
                        }
                        }
                        onClicked: {
                            if (periodicCaptureCheck.checked)
                            {
                                if(periodicCaptureTimer.running)
                                    periodicCaptureTimer.stop();
                                else{
                                    _controllerCore.triggerCapture();
                                    periodicCaptureTimer.start();
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
                                color: quickRecordText.recording ? "red" : ((visVidCheck.checked || radVidCheck.checked) ? (quickRecordButton.hovered ? "orange" : "white") : "gray")
                                font.bold: true
                                font.pixelSize: 14
                                x: 5
                                anchors.verticalCenter: parent.verticalCenter

                            }
                        }
                        }
                        onClicked: if(visVidCheck.checked || radVidCheck.checked) _controllerCore.toggleRecording()
                    }

                    Connections{
                        target: _controllerCore
                        onCaptureRecordingChanged:
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

                        onClicked: shutterPeriodSpinbox.applyShutter();
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

                        onClicked: {
                            rangeModeMan.onClicked();
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
                        onStreamFrameVisibleChanged:{
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
                            {
                                _controllerCore.toggleStreamVisible();
                                if(_controllerCore.streamingVisible())
                                    sidePanelPauseImg.source = "img/png/button-pause.png"
                                else sidePanelPauseImg.source = "img/png/button-play.png"
                            }
                            else {
                                _controllerCore.toggleStreamThermal();
                                if(_controllerCore.streamingThermal())
                                    sidePanelPauseImg.source = "img/png/button-pause.png"
                                else sidePanelPauseImg.source = "img/png/button-play.png"
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
                                            onGpsChanged: gpsText.text = val
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
                                       text: mainWindow.mainStreamThermal ? "A: "+ _controllerCore.currentZoomVisible +"x" : "D: "+ _controllerCore.currentZoomThermal +"x"
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
                                        text: (syncShutter.checked ? ( shutterPeriodSpinbox.value+ " min")  : "async")
                                        color: "white"
                                         font.pixelSize: 12
                                         anchors.verticalCenter: parent.verticalCenter
                                        Connections{
                                            target: _controllerCore
                                            onShutterDataChanged:{
                                                if (!_controllerCore.compareFw("1.1.1", _controllerCore.firmwareVersion) || _controllerCore.type == mainWindow.securityType)
                                                    return;

                                                if (syncShutter.checked)
                                                    shutterText.text = next;
                                                else shutterText.text = last;

                                                if (last == "0:00" || last == "0:01" || next == "0:01" || next == "0:00")
                                                    nucPopup.visible = true;
                                                else nucPopup.visible = false;
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
                                            onCurrentEnviromentChanged: sideRangeText.text = Math.round(_controllerCore.enviromentLowerThreshold()) + " - " + Math.round(_controllerCore.enviromentUpperThreshold()) + "°C"
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
                                            if (alarmModeOff.checked)
                                                sideAlarmText.text = "OFF";
                                            else if (alarmModeAbove.checked)
                                                sideAlarmText.text = "Above: " + Math.round(rangeAlarmSlider.second.value*10)/10 + "°C";
                                            else if (alarmModeBetween.checked)
                                                sideAlarmText.text = "B: " + Math.round(rangeAlarmSlider.first.value*10)/10 + " - " + Math.round(rangeAlarmSlider.second.value*10)/10 + "°C";
                                            else if (alarmModeBelow.checked)
                                                sideAlarmText.text = "Below: " + Math.round(rangeAlarmSlider.first.value*10)/10 + "°C";
                                            else if (alarmModeOutside.checked)
                                                sideAlarmText.text = "O: " + Math.round(rangeAlarmSlider.first.value*10)/10 + " - " + Math.round(rangeAlarmSlider.second.value*10)/10 + "°C";
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
                                            onAlarmFromChanged: sideAlarmText.setAlarmText()
                                        }
                                        Connections{
                                            target: _controllerCore
                                            onAlarmToChanged: sideAlarmText.setAlarmText()
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
         title: qsTr("WIRIS Ethernet Stream SDK GUI")
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



}
