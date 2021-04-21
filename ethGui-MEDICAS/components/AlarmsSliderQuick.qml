import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Rectangle {
    color: "transparent"
    Layout.preferredHeight: 40
    Layout.preferredWidth: parent.width - 10
    Layout.alignment: Qt.AlignHCenter

    function setFirstVal(val) {
        rangeManSlider.first.value = val
        rangeFirstDebounceTimer.stop()
        rangeFirstDebounceTimer.start()
    }

    function setSecondVal(val) {
        rangeManSlider.second.value = val
        rangeSecondDebounceTimer.stop()
        rangeSecondDebounceTimer.start()
    }

    property alias from: rangeManSlider.from
    property alias to: rangeManSlider.to
    property alias firstVal: rangeManSlider.fVal
    property alias secondVal: rangeManSlider.sVal

    TextField {
        id: sliderFrom
        anchors.left: parent.left
        width: parent.width * 1 / 6
        color: "orange"
        background: Rectangle {
            color: "#66000000"
            border.color: "orange"
            border.width: 1
        }
        height: 30
        y: 5
        //validator: RegExpValidator { regExp: /^[+-]?(\d)+\.\d/}
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        visible: rangeManSlider.visible
        text: rangeManSlider.first.value.toFixed(2)
        onTextChanged: {
            if (+sliderFrom.text === Math.round(
                        rangeManSlider.first.value * 100) / 100) {
                sliderFrom.color = "orange"
                return
            }
            if (+sliderFrom.text < +sliderTo.text
                    && +sliderFrom.text >= rangeManSlider.from
                    && sliderFrom.text != "") {
                rangeManSlider.first.value = sliderFrom.text
                sliderFrom.color = "orange"
            } else
                sliderFrom.color = "red"
        }
    }

    RangeSlider {
        id: rangeManSlider

        property double fVal: rangeManSlider.first.value
        property double sVal: rangeManSlider.second.value

        anchors.left: sliderFrom.right
        width: parent.width * 2 / 3
        from: _controllerCore._controllerCore.getAlarmMinLimit()
        to: _controllerCore._controllerCore.getAlarmMaxLimit()
        first.value: _controllerCore.alarmPreventive
        second.value: _controllerCore.alarmCritical

        Connections {
            target: rangeManSlider.first
            function onValueChanged() {
                rangeFirstDebounceTimer.stop()
                rangeFirstDebounceTimer.start()
            }
        }
        Connections {
            target: rangeManSlider.second
            function onValueChanged() {
                rangeSecondDebounceTimer.stop()
                rangeSecondDebounceTimer.start()
            }
        }

        Connections {
            target: _controllerCore
            function onAlarmPreventiveChanged(val) {
                rangeManSlider.first.value = val
            }
            function onAlarmCriticalChanged(val) {
                rangeManSlider.second.value = val
            }
            function onUnitsChanged(val) {
                rangeManSlider.from = _controllerCore.getAlarmMinLimit()
                rangeManSlider.to = _controllerCore.getAlarmMaxLimit()
            }
        }
        Timer {
            id: rangeFirstDebounceTimer
            interval: 500
            repeat: false
            onTriggered: _controllerCore.alarmPreventive = rangeManSlider.first.value
        }
        Timer {
            id: rangeSecondDebounceTimer
            interval: 500
            repeat: false
            onTriggered: _controllerCore.alarmCritical = rangeManSlider.second.value
        }
    }

    TextField {
        id: sliderTo
        anchors.left: rangeManSlider.right
        width: parent.width * 1 / 6
        color: "orange"
        background: Rectangle {
            color: "#66000000"
            border.color: "orange"
            border.width: 1
        }
        height: 30
        y: 5
        //validator: RegExpValidator { regExp: /^[+-]?(\d)+\.\d/}
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        visible: rangeManSlider.visible
        text: Math.round(rangeManSlider.second.value * 100) / 100
        onTextChanged: {
            if (+sliderTo.text === Math.round(
                        rangeManSlider.second.value * 100) / 100) {
                sliderTo.color = "orange"
                return
            }
            if (+sliderFrom.text < +sliderTo.text
                    && +sliderTo.text <= rangeManSlider.to
                    && sliderTo.text != "") {
                rangeManSlider.second.value = sliderTo.text
                sliderTo.color = "orange"
            } else
                sliderTo.color = "red"
        }
    }
}
