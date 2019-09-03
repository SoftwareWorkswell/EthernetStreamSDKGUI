import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Rectangle{
    color: "transparent"
    visible: rangeModeMan.checked
    Layout.preferredHeight: 40
    Layout.preferredWidth: parent.width - 10
    Layout.alignment: Qt.AlignHCenter

    function setFirstVal(val)
    {
        rangeManSlider.first.value = val;
        rangeFirstDebounceTimer.stop();
        rangeFirstDebounceTimer.start();
    }

    function setSecondVal(val)
    {
        rangeManSlider.second.value = val;
        rangeSecondDebounceTimer.stop();
        rangeSecondDebounceTimer.start();
    }

    property alias from: rangeManSlider.from
    property alias to: rangeManSlider.to
    property alias firstVal: rangeManSlider.fVal
    property alias secondVal: rangeManSlider.sVal



    TextField {
        id: sliderFrom
        anchors.left: parent.left
        width: parent.width*1/6
        color: "orange"
        background: Rectangle{color: "#66000000"; border.color: "orange"; border.width: 1}
        height: 30
        y: 5
        validator: RegExpValidator { regExp: /^[+-]?(\d)+\.\d/}
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        visible: rangeManSlider.visible
        text:  Math.round(rangeManSlider.first.value*10)/10
        onTextChanged: {
            if (+sliderFrom.text === Math.round(rangeManSlider.first.value*10)/10)
            {
                sliderFrom.color = "orange";
                return;
            }
            if (+sliderFrom.text < +sliderTo.text && +sliderFrom.text >= rangeManSlider.from && sliderFrom.text != "")
            {
                rangeManSlider.first.value = sliderFrom.text;
                sliderFrom.color = "orange";
            }
            else sliderFrom.color = "red";
        }
    }

    RangeSlider {
        id: rangeManSlider

        property double fVal: rangeManSlider.first.value
        property double sVal: rangeManSlider.second.value

        function setFirstRange(val)
        {
           _controllerCore.manualRange1 = val;
        }
        function setSecondRange(val)
        {
           _controllerCore.manualRange2 = val;
        }


        anchors.left: sliderFrom.right
        width: parent.width*2/3
        from: 1
        to: 100
        first.value: _controllerCore.manualRange1
        second.value: _controllerCore.manualRange2


        Component.onCompleted: setSliderRanges()

        Connections{
            target: range1
            onCheckedChanged: setSliderRanges()
        }

        Connections{
            target: range2
            onCheckedChanged: setSliderRanges()
        }

        Connections{
            target: rangeManSlider.first
            onValueChanged: {
                rangeFirstDebounceTimer.stop();
                rangeFirstDebounceTimer.start();
            }
        }
        Connections{
            target: rangeManSlider.second
            onValueChanged: {
                rangeSecondDebounceTimer.stop();
                rangeSecondDebounceTimer.start();
            }
        }

        Connections{
            target: _controllerCore
            onManualRange1Changed:{
                rangeManSlider.first.value = _controllerCore.manualRange1
            }
        }
        Connections{
            target: _controllerCore
            onManualRange2Changed:{
                rangeManSlider.second.value = _controllerCore.manualRange2
            }
        }

        Timer{
            id: rangeFirstDebounceTimer
            interval: 1000
            repeat: false
            onTriggered: rangeManSlider.setFirstRange(rangeManSlider.first.value)
        }
        Timer{
            id: rangeSecondDebounceTimer
            interval: 1000
            repeat: false
            onTriggered: rangeManSlider.setSecondRange(rangeManSlider.second.value)
        }



    }

    TextField {
        id: sliderTo
        anchors.left: rangeManSlider.right
        width: parent.width*1/6
        color: "orange"
        background: Rectangle{color: "#66000000"; border.color: "orange"; border.width: 1}
        height: 30
        y: 5
        validator: RegExpValidator { regExp: /^[+-]?(\d)+\.\d/}
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        visible: rangeManSlider.visible
        text:  Math.round(rangeManSlider.second.value*10)/10
        onTextChanged: {
            if (+sliderTo.text === Math.round(rangeManSlider.second.value*10)/10){
                sliderTo.color = "orange";
                return;
            }
            if (+sliderFrom.text < +sliderTo.text && +sliderTo.text <= rangeManSlider.to && sliderTo.text != "")
            {
                rangeManSlider.second.value = sliderTo.text;
                sliderTo.color = "orange";
            }
            else sliderTo.color = "red";
        }
    }

}
