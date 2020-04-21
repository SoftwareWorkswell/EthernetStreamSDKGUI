import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4


RadioButton {
    id: control
    text: qsTr("RadioButton")
    font.pixelSize: 13

    indicator: Rectangle {
        implicitWidth: 20
        implicitHeight: 20
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: 10
        border.color: control.down ? "red" : "orange"

        Rectangle {
            width: 14
            height: 14
            x: 3
            y: 3
            radius: 7
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
