import QtQuick 2.12
import QtQuick.Controls 2.12

SpinBox {
    id: control
    value: 2
    editable: false
    width: parent.width
    font.bold: true
    from: 2
    to: 30

    contentItem: TextInput {
        z: 2

        id: controlText

        text: control.textFromValue(control.value, control.locale) + " min"

        font: control.font
        color: "black"
        selectionColor: "orange"
        selectedTextColor: "orange"
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter

        readOnly: !control.editable
        validator: control.validator
        inputMethodHints: Qt.ImhFormattedNumbersOnly
        Connections{
            target: control
            onValueChanged:{
                    controlText.text = control.value + " min"
            }
        }
    }

    up.indicator: Rectangle {
        x: control.mirrored ? 0 : parent.width - width
        height: parent.height
        implicitWidth: 40
        anchors.rightMargin: 2
        implicitHeight: 30
        color: control.up.pressed ? "gray" : "black"
        border.color: enabled ? "orange" : "orange"


        Text {
            text: "+"
            font.pixelSize: control.font.pixelSize * 2
            color: "orange"
            anchors.fill: parent
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    down.indicator: Rectangle {
        x: control.mirrored ? parent.width - width : 0
        height: parent.height
        implicitWidth: 40
        implicitHeight: 30
        anchors.leftMargin: 2
        color: control.down.pressed ? "gray" : "black"
        border.color: enabled ? "orange" : "orange"

        Text {
            text: "-"
            font.pixelSize: control.font.pixelSize * 2
            color: "orange"
            anchors.fill: parent
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    background: Rectangle {
        implicitWidth: 140
        implicitHeight: 30
        anchors.rightMargin: 5
        anchors.leftMargin: 5
        border.color: "#bdbebf"
    }
}
