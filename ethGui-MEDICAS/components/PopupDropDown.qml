import QtQuick 2.0
import QtQuick.Layouts 1.3

MouseArea{
    id: control
    Layout.fillWidth: true
    Layout.preferredHeight: 25

    property bool activated: false
    property alias dropdownText: controlText.text
    onClicked: activated = !activated

    Image{
        id: controlImg
        height: parent.height - 10
        y: 5
        x: 5
        source: parent.activated ? "../img/png/ico_minus_big.png" : "../img/png/ico_plus_big.png"
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: controlText
        anchors.verticalCenter: parent.verticalCenter
        x: parent.height + 5
        font.bold: true
        font.pixelSize: 13
        color: parent.activated ? "orange" : "white"
    }

}
