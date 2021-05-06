import QtQuick 2.0

Rectangle{
    id: infoFrame

    property variant infoMess: "--"
    property bool stateVisible: false

    color: "#343437"
    border.color: "#606060"
    border.width: 3

    Image {
        id: infoIcon
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 15
        anchors.leftMargin: 15
        height: 50
        fillMode: Image.PreserveAspectFit
        source: "../img/png/info.png"
    }

    Item{
        anchors.fill:parent
        anchors.leftMargin: 80

        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: 400
            text: infoMess
            color: "white"
            font.family: "Helvetica"
            font.pointSize: 9
            font.bold: true
            elide: Text.ElideMiddle
            wrapMode: Text.Wrap
        }
    }

    onStateVisibleChanged: {
        //TODO: handle if the info is currently visible
        if(stateVisible == true)
            hideInfoTimer.running = true;
    }

    states: [
        State { when: infoFrame.stateVisible;
            PropertyChanges {   target: infoFrame; opacity: 0.8    }},
        State { when: !infoFrame.stateVisible;
            PropertyChanges {   target: infoFrame; opacity: 0.0    }}
    ]
    transitions: [ Transition { NumberAnimation { property: "opacity"; duration: 500}} ]

    Timer {
        id: hideInfoTimer
        interval: 2500; running: false; repeat: false
        onTriggered: {

            stateVisible = false
        }
    }
}
