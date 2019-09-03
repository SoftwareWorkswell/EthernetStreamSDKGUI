import QtQuick 2.0
import QtQuick.Controls 2.5

Button {
    id: control
    text: qsTr("Button")
    width: 255
    font.pixelSize: 12

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.highlighted ? "orange" : "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
    //  implicitWidth: parent.width
        implicitHeight: 10
        opacity: enabled ? 1 : 0.3
        border.color: control.highlighted ? "orange" : "black"
        border.width: 1
        color: control.down ? "gray" : "#44000000"
        radius: 2
    }
}
