import QtQuick 2.12
import QtQuick.Controls 2.12

Button {
    id: control
    text: qsTr("Button")
    font.bold: true

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.down ? "white" : "orange"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        opacity: enabled ? 1 : 0.3
        border.color: control.down ? "white" : "orange"
        border.width: 1
        color: control.down ? "gray" : "black"
        radius: 2
    }
}
