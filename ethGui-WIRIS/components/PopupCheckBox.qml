import QtQuick 2.12
import QtQuick.Controls 2.12

CheckBox {
    id: control
    text: qsTr("CheckBox")
    checked: true
    font.bold: true
    font.pixelSize: 12

    indicator: Rectangle {
        implicitWidth: 16
        implicitHeight: 16
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: 3
        border.color: control.down ? "red" : "orange"

        Rectangle {
            width: 10
            height: 10
            x: 3
            y: 3
            radius: 2
            color: control.down ? "red" : "orange"
            visible: control.checked
        }
    }

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.checked ? "orange" : "white"
        verticalAlignment: Text.AlignVCenter
        leftPadding: control.indicator.width + control.spacing
    }
}
