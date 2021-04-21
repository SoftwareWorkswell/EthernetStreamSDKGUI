import QtQuick 2.0
import QtQuick.Layouts 1.3
import "../"

ColumnLayout{
    RowLayout
    {
        Layout.fillWidth: true
        Layout.margins: 5
        Text{
            Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: true
            text: qsTr("Preventive Color: ") + _translator.emptyString
            color: "white"
            font.pixelSize: 12
            font.bold: true
        }
        CustomComboBox{
            id: colorPreventiveComboBox
            property bool skipHandler: false
            model: ["RED", "GREEN", "BLUE", "LIME", "YELLOW", "CYAN", "MAGENTA", "MAROON", "OLIVE", "PURPLE", "TEAL", "NAVY", "BROWN"]
            onActivated:{
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

            text: qsTr("Critical Color: ")
            color: "white"
            font.pixelSize: 12
            font.bold: true
        }
        CustomComboBox{
            id: colorCriticalComboBox
            property bool skipHandler: false
            model: ["RED", "GREEN", "BLUE", "LIME", "YELLOW", "CYAN", "MAGENTA", "MAROON", "OLIVE", "PURPLE", "TEAL", "NAVY", "BROWN"]
            onActivated:{
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
        function onPreventiveColorChanged(val){
            colorPreventiveComboBox.skipHandler = true
            colorPreventiveComboBox.currentIndex = colorPreventiveComboBox.model.indexOf(val)
        }
        function onCriticalColorChanged(val){
            colorCriticalComboBox.skipHandler = true
            var idx = colorCriticalComboBox.model.indexOf(val)
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
            text: qsTr("Opacity: ") + _translator.emptyString
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
                onAccepted: {
                    _controllerCore.opacity = alarmOpacityValue.text
                }
            }
            Connections{
                target: _controllerCore
                function onAlarmOpacityChanged() {
                    alarmOpacityValue.text = _controllerCore.alarmOpacity
                }
            }
        }
    }
}
