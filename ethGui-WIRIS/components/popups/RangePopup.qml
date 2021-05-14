import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "../"


ControlPopup{
    property alias from: rangeManSlider.from
    property alias to: rangeManSlider.to
    property alias firstVal: rangeManSlider.firstVal
    property alias secondVal: rangeManSlider.secondVal

    function setSliderRanges()
    {
        var thresholds = environmentsSwitch.thresholds()
        measurePopup.setRangeAlarmSliders(thresholds)
        rangeManSlider.from = thresholds[0]
        rangeManSlider.to = thresholds[1]
        rangeManSlider.firstVal = rangeManSlider.from;
        rangeManSlider.secondVal = rangeManSlider.to;
        rangeManSlider.setTexts()
    }
    function rangeManSliderSetFirst(val)
    {
        rangeManSlider.setFirstVal(val);
    }
    function rangeManSliderSetSecond(val)
    {
        rangeManSlider.setSecondVal(val);
    }
    function rangeModeManChecked()
    {
        return rangeModeMan.checked
    }

    function rangeModeManClicked()
    {
        rangeModeMan.onClicked();
    }

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
                function onRangeModeChanged(mode)
                {
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
                validator: RegExpValidator { regExp: /^(\d)+\.?\d?/}
                background: Rectangle{color: "#66000000"; border.color: "orange"; border.width: 1}
                color: "orange"
                onAccepted: {
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
                onFocusChanged: {
                    if(!focus)
                    {
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
                validator: RegExpValidator { regExp: /^[+-]?(\d)+\.?\d?/}
                onAccepted: {
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
                onFocusChanged: {
                    if(!focus)
                    {
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
            }

            Connections{
                target: _controllerCore
                function onSpanCenterChanged()
                {
                    spanCenterTextField.text = Math.round(_controllerCore.spanCenter*10)/10
                }
            }
            Connections{
                target: _controllerCore
                function onSpanWindowChanged()
                {
                    spanWindowTextField.text = Math.round(_controllerCore.spanWindow*10)/10
                }
            }
        }
        PopupItem{

            width: parent.width
            itemText: "Environment"
            Layout.preferredHeight: 25
        }
        EnvironmentsSwitch
        {
            id: environmentsSwitch
            Layout.preferredHeight: 20
            Component.onCompleted: setSliderRanges()
        }
    }
}

