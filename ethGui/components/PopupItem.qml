import QtQuick 2.0



Rectangle{
    color: "white"
    width: parent.width
    anchors.topMargin: 4
    property alias itemText: innerText.text
    Rectangle{
        color: "black"
        anchors.fill: parent
        anchors.rightMargin: 2
        anchors.topMargin: 1




        Text{
            id: innerText
            anchors.centerIn: parent
            font.bold: true
            color: "white"
        }

    }
}
