import QtQuick 2.0
import QtQuick.Layouts 1.3


Rectangle{
    color: "#000000FF"
    Layout.fillHeight: true
    Layout.fillWidth: true
    visible: false
    Rectangle{
        color: "#DD444444"
        anchors.fill: parent
        width: parent.width
        anchors.rightMargin: 2

    }
    Rectangle{
        anchors.top: parent.top
        anchors.right: parent.right
        height: parent.height
        width: 2
        color: "#ff8e00"

    }

}
