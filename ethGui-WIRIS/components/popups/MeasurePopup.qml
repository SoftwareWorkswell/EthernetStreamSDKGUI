import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "../"

ControlPopup{
    id: measurePopup
    width: parent.width
    function setRangeAlarmSliders(vals)
    {
        rangeAlarmSlider.from = vals[0];
        rangeAlarmSlider.to = vals[1];

        rangeAlarmSlider.first.value = rangeAlarmSlider.from;
        rangeAlarmSlider.second.value = rangeAlarmSlider.to;
    }
    function rangeAlarmSliderSecond()
    {
        return rangeAlarmSlider.second.value
    }
    function rangeAlarmSliderFirst()
    {
        return rangeAlarmSlider.first.value
    }
    function alarmModeAboveChecked()
    {
        return alarmModeAbove.checked
    }
    function alarmModeBetweenChecked()
    {
        return alarmModeBetween.checked
    }
    function alarmModeBelowChecked()
    {
        return alarmModeBelow.checked
    }
    function alarmModeOutsideChecked()
    {
        return alarmModeOutside.checked
    }
    function alarmModeOffChecked()
    {
        return alarmModeOff.checked
    }

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
            id: showCenterCheck
            checked: false
            text: "Show Cross Center"
            onCheckedChanged: _controllerCore.paintCenter(checked)
        }

        PopupCheckBox{
            text: "Show Temperatures"
            checked: false
            visible: _controllerCore.type != mainWindow.securityType
            enabled: showMaxCheck.checked || showMinCheck.checked || showCenterCheck.checked
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
                if(!_controllerCore.isSetup)
                    return;
                _controllerCore.alarmMode = val;
                _controllerCore.alarmFrom = from;
                _controllerCore.alarmTo = to;
            }

            function setAlarmFirstVal(val)
            {
                if(!_controllerCore.isSetup)
                    return;
                _controllerCore.alarmFrom = val;
            }
            function setAlarmSecondVal(val)
            {
                if(!_controllerCore.isSetup)
                    return;
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
                function onAlarmModeChanged(val)
                {
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
                onAccepted: {
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
                onFocusChanged:
                {
                    if(!focus)
                    {
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
            }

            RangeSlider {
                id: rangeAlarmSlider
                enabled: !alarmModeOff.checked
                visible: enabled
                Connections {
                    target: rangeAlarmSlider.second
                    function onValueChanged()
                    {
                        if (alarmModeBelow.checked && rangeAlarmSlider.second.value != rangeAlarmSlider.to)
                            rangeAlarmSlider.second.value = rangeAlarmSlider.to;
                        if (!alarmModeBelow.checked)
                        {
                            alarmSecondDebounceTimer.stop();
                            alarmSecondDebounceTimer.start();
                        }

                    }
                }
                Connections {
                    target: rangeAlarmSlider.first
                    function onValueChanged() {
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
                    function onAlarmFromChanged(val){
                        rangeAlarmSlider.first.value = val;
                        alarmPaletteAboveRect.reDraw();
                    }
                }
                Connections{
                    target: _controllerCore
                    function onAlarmToChanged(val){
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
                    color:  alarmModeBelowChecked() || alarmModeOutsideChecked() ? advancedPopup.alarmColorBelow()  : "gray"

                    Rectangle {
                        x: rangeAlarmSlider.first.visualPosition * parent.width
                        width: rangeAlarmSlider.second.visualPosition * parent.width - x
                        height: parent.height
                        color: alarmModeBetweenChecked() ? advancedPopup.alarmColorBetween() : "gray"
                        radius: 2
                    }

                    Rectangle {
                        x: rangeAlarmSlider.second.visualPosition * parent.width
                        width: rangeAlarmSlider.availableWidth - x
                        height: parent.height
                        color: alarmModeAboveChecked() || alarmModeOutsideChecked() ? advancedPopup.alarmColorAbove() : "gray"
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
                onAccepted: {
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
                onFocusChanged:
                {
                    if(!focus)
                    {
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

}

