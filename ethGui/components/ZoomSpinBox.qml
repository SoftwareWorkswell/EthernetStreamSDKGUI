import QtQuick 2.12
import QtQuick.Controls 2.12

SpinBox {
    id: control
    value: 1
    editable: true
    width: parent.width
    font.bold: true
    property string zoomValue: ""

    contentItem: TextInput {
        z: 2
        text: control.zoomValue;

        font: control.font
        color: "black"
        selectionColor: "orange"
        selectedTextColor: "orange"
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter

        readOnly: !control.editable
    }

    up.indicator: Rectangle {
        x: control.mirrored ? 0 : parent.width - width
        height: parent.height
        implicitWidth: 40
        anchors.rightMargin: 2
        implicitHeight: 30
        color: control.up.pressed ? "gray" : "black"
        border.color: "orange"
        enabled: true


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
        border.color: "orange"
        enabled: true


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
