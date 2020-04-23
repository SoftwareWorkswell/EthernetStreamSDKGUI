import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "../"

ControlPopup{
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
            onCheckedChanged: _controllerCore.showCrossMax = checked
        }
        PopupCheckBox{
            checked: false
            text: "Show Cross Center"
            onCheckedChanged: _controllerCore.showCrossCenter = checked
        }
        PopupItem{

            width: parent.width
            itemText: "Blackbody ROI"
            Layout.preferredHeight: 25
        }
        PopupCheckBox{
            text: "Show Blackbody ROI"
            checked: false
            visible: true
            enabled: true
            id: showBlackbodyCheck
            onCheckedChanged: _controllerCore.showBlackbodyRoi = checked
        }
        RowLayout {
            Layout.preferredHeight: 50
            Layout.alignment: Qt.AlignHCenter
            PopupButton{
                width: parent.width * 0.65
                height: parent.height * 0.55
                anchors.centerIn: parent
                text: "Set Position"
                onClicked:{
                    showBlackbodyCheck.checked = true
                    mainviewParent.setBlackbodyRectVisible(true)
                    mainviewParent.setAlarmsSliderVisible(false)
                    mainviewParent.setManRangeSliderVisible(false)
                    controlPanel.hideAllPopups()

                }
            }
        }

        PopupItem{

            width: parent.width
            itemText: "Alarm"
            visible: true
            Layout.preferredHeight: 25
        }

        RowLayout {
            Layout.preferredHeight: 30
            Layout.alignment: Qt.AlignHCenter
            function setAlarmMode(val, from, to)
            {
                if(from < 30 || to > 42)
                    return
                _controllerCore.alarmMode = val;
                _controllerCore.alarmPreventive = from;
                _controllerCore.alarmCritical = to;
            }
            function setAlarmFirstVal(val)
            {
                if(val < 30)
                    return;
                _controllerCore.alarmPreventive = val;
            }
            function setAlarmSecondVal(val)
            {
                if(val > 42)
                    return;
                _controllerCore.alarmCritical = val;
            }
            CustomRadioButton1{
                checked: true
                id: alarmModeOff
                text: "Off"
                onClicked: {
                    if(checked)
                        _controllerCore.alarmMode = 1
                    else
                        _controllerCore.alarmMode = 0;
                }

            }
            CustomRadioButton1{
                id: alarmModeOn
                text: "On"
                onClicked:{
                    if(checked)
                        _controllerCore.alarmMode = 0
                    else
                        _controllerCore.alarmMode = 1;
                }
            }

            Connections{
                target: _controllerCore
                onAlarmModeChanged:{
                    switch(val)
                    {
                    case 0:
                        alarmModeOn.checked = true;
                        alarmModeOff.checked = false;
                        break;
                    case 1:
                        alarmModeOn.checked = false;
                        alarmModeOff.checked = true;
                    }
                }
            }
        }
        RowLayout{
            Layout.fillWidth: true
            Layout.margins: 5
            Text{
                id: preventiveValText
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                color: "white"
                text: "Preventive Value: "
                font.pixelSize: 12
                font.bold: true
            }
            Rectangle{
                id: alarmPreventiveBackground
                Layout.preferredHeight: 15
                Layout.preferredWidth: 50
                TextInput {
                    anchors.fill: parent
                    id: alarmPreventiveValue
                    text: _controllerCore.alarmPreventive.toFixed(2)
                    font.bold: true
                    //validator: RegExpValidator { regExp: /^0\.\d\d/}
                    onAccepted: {
                        _controllerCore.alarmPreventive = parseFloat(alarmPreventiveValue.text)
                    }
                }
                Connections{
                    target: _controllerCore
                    onAlarmPreventiveChanged: {
                        alarmPreventiveValue.text = _controllerCore.alarmPreventive.toFixed(2)
                    }
                }
            }
        }
        RowLayout{
            Layout.fillWidth: true
            Layout.margins: 5
            Text{
                id: criticalValText
                color: "white"
                text: "Critival Value: "
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                font.pixelSize: 12
                font.bold: true
            }
            Rectangle{
                id: alarmCriticalBackground
                Layout.preferredHeight: 15
                Layout.preferredWidth: 50
                TextInput {
                    anchors.fill: parent
                    id: alarmCriticalValue
                    text: _controllerCore.alarmCritical.toFixed(2)
                    font.bold: true
                    //validator: RegExpValidator { regExp: /^0\.\d\d/}
                    onAccepted: {
                        _controllerCore.alarmCritical = parseFloat(alarmCriticalValue.text)
                    }
                }
                Connections{
                    target: _controllerCore
                    onAlarmCriticalChanged: {
                        alarmCriticalValue.text = _controllerCore.alarmCritical.toFixed(2)
                    }
                }
            }
        }
        ColumnLayout{
            Layout.alignment: Qt.AlignHCenter
            visible: alarmsDropDown.activated
            RowLayout
            {
                Layout.fillWidth: true
                Layout.margins: 5
                Text{
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: true
                    text: "Preventive Color: "
                    color: "white"
                    font.pixelSize: 12
                    font.bold: true
                }
                CustomComboBox{
                    id: colorPreventiveComboBox
                    property bool skipHandler: false
                    model: ["RED", "GREEN", "BLUE", "LIME", "YELLOW", "CYAN", "MAGENTA", "MAROON", "OLIVE", "PURPLE", "TEAL", "NAVY", "BROWN"]
                    onCurrentTextChanged:{
                        if(skipHandler)
                        {
                            skipHandler = false;
                            return
                        }
                        _controllerCore.setAlarmColors(colorPreventiveComboBox.currentText, _controllerCore.criticalColor)
                    }
                }

            }
            RowLayout
            {
                Layout.fillWidth: true
                Layout.margins: 5
                Text{
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: true

                    text: "Critical Color: "
                    color: "white"
                    font.pixelSize: 12
                    font.bold: true
                }
                CustomComboBox{
                    id: colorCriticalComboBox
                    property bool skipHandler: false
                    model: ["RED", "GREEN", "BLUE", "LIME", "YELLOW", "CYAN", "MAGENTA", "MAROON", "OLIVE", "PURPLE", "TEAL", "NAVY", "BROWN"]
                    onCurrentTextChanged:{
                        if(skipHandler)
                        {
                            skipHandler = false;
                            return
                        }
                        _controllerCore.setAlarmColors(_controllerCore.preventiveColor, colorCriticalComboBox.currentText)
                    }
                }
            }
            Connections{
                target: _controllerCore
                onPreventiveColorChanged:{
                    colorPreventiveComboBox.skipHandler = true
                    colorPreventiveComboBox.currentIndex = colorPreventiveComboBox.model.indexOf(val)
                }
                onCriticalColorChanged:{
                    colorCriticalComboBox.skipHandler = true
                    var idx = colorCriticalComboBox.model.indexOf(val)
                    console.log('critical col ' + idx  + " " + val)
                    colorCriticalComboBox.currentIndex = colorCriticalComboBox.model.indexOf(val)
                }
            }
            RowLayout{
                Layout.fillWidth: true
                Layout.margins: 5
                Text{
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: true
                    color: "white"
                    font.pixelSize: 12
                    text: "Opacity: "
                    font.bold: true
                }
                Rectangle{
                    id: opacityBackground
                    Layout.preferredHeight: 15
                    Layout.preferredWidth: 50
                    TextInput {
                        anchors.fill: parent
                        id: alarmOpacityValue
                        text: _controllerCore.alarmOpacity
                        font.bold: true
                        //validator: RegExpValidator { regExp: /^0\.\d\d/}
                        onTextChanged: {
                            _controllerCore.opacity = alarmOpacityValue.text
                        }
                    }
                    Connections{
                        target: _controllerCore
                        onAlarmOpacityChanged: {
                            alarmOpacityValue.text = _controllerCore.alarmOpacity
                        }
                    }
                }
            }
        }
        Rectangle{
            visible: true
            Layout.fillWidth: true
            height: 1
            color: "black"
        }
    }
    Rectangle{
        color: "transparent"
        Layout.preferredHeight: 40
        Layout.preferredWidth: parent.width-4
    }

}

