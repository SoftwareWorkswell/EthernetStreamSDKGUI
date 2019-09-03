import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Rectangle{
    color: "transparent"
    visible: rangeModeMan.checked
    Layout.preferredHeight: 40
    Layout.preferredWidth: parent.width

    Text {
        anchors.left: parent.left
        width: parent.width*1/6
        id: sliderFrom
        color: "orange"
        height: parent.height
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text:  Math.round(quickManSlider.first.value*10)/10+ " °C"
    }

    RangeSlider {
        id: quickManSlider

        anchors.left: sliderFrom.right
        width: parent.width*2/3
        from: rangeManSlider.from
        to: rangeManSlider.to
        first.value: rangeManSlider.fVal
        second.value: rangeManSlider.sVal

    }

    Connections{
        target: quickManSlider.first
        onValueChanged: {
            rangeManSlider.setFirstVal(quickManSlider.first.value);
        }
    }
    Connections{
        target: quickManSlider.second
        onValueChanged: {
            rangeManSlider.setSecondVal(quickManSlider.second.value);
        }
    }
    Connections{
        target: _controllerCore
        onManualRange1Changed:{
            quickManSlider.first.value = _controllerCore.manualRange1
        }
    }
    Connections{
        target: _controllerCore
        onManualRange2Changed:{
            quickManSlider.second.value = _controllerCore.manualRange2
        }
    }



    Text {
        anchors.left: quickManSlider.right
        width: parent.width*1/6
        id: sliderTo
        color: "orange"
        height: parent.height
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: Math.round(quickManSlider.second.value*10)/10+ " °C"
    }

}
