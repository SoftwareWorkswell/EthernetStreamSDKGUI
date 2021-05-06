import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Rectangle{
    color: "transparent"
    visible: rangePopup.rangeModeManChecked()
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
        text:  Math.round(quickManSlider.first.value*10)/10+ _controllerCore.thermalUnitSign
    }

    RangeSlider {
        id: quickManSlider
        anchors.left: sliderFrom.right
        width: parent.width*2/3
        from: rangePopup.from
        to: rangePopup.to
        first.value: rangePopup.fVal
        second.value: rangePopup.sVal
    }

    Connections{
        target: quickManSlider.first
        function onValueChanged()
        {
            rangePopup.rangeManSliderSetFirst(quickManSlider.first.value);
        }
    }
    Connections{
        target: quickManSlider.second
        function onValueChanged()
        {
            rangePopup.rangeManSliderSetSecond(quickManSlider.second.value);
        }
    }
    Connections{
        target: _controllerCore
        function onManualRange1Changed()
        {
            quickManSlider.first.value = _controllerCore.manualRange1
        }
    }
    Connections{
        target: _controllerCore
        function onManualRange2Changed()
        {
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
        text: Math.round(quickManSlider.second.value*10)/10 + _controllerCore.thermalUnitSign
    }

}
