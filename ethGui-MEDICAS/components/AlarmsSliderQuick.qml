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
        from: 35.0
        to: 50.0
        first.value: _controllerCore.alarmPreventive()
        second.value: _controllerCore.alarmCritical()
        first.onMoved: _controllerCore.alarmPreventive = first.value.toFixed(2)
        second.onMoved: _controllerCore.alarmCritical = second.value.toFixed(2)

    }
    Connections{
        target: _controllerCore
        onAlarmPreventiveChanged:{
            quickManSlider.first.value = val
        }
    }
    Connections{
        target: _controllerCore
        onAlarmCriticalChanged:{
            quickManSlider.second.value = val
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
