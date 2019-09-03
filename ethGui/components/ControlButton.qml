import QtQuick 2.12
import QtQuick.Controls 2.12

Button {
    id: control
    text: qsTr("Button")
    font.bold: true
    font.pixelSize: 12

    property alias textColor: controlText.color
    property alias backgroundColor: controlRect.color

    contentItem: Text {
        id: controlText
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.highlighted ? "orange" : "gray"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignBottom
        //elide: Text.ElideRight
    }

    background: Rectangle {
        id: controlRect
        implicitWidth: 100
        implicitHeight: 40
        opacity: enabled ? 1 : 0.3
        border.color: "#33333D"
        border.width: 1
        color: control.highlighted ? "black" : "#33333D"
        radius: 0
    }
}
