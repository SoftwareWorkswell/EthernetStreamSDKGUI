import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3


ApplicationWindow {
    id: disconnectedWindow
    title: qsTr("Disconnected") + _translator.emptyString
    width: 300
    height: 150
    flags: Qt.WindowStaysOnTopHint
    Component.onCompleted: {
        reconnectTimer.start()
        watchTimer.start()
    }

    Timer
    {
        id: reconnectTimer
        interval: 5000
        repeat: false
        onTriggered: {
            _controllerCore.connect(_controllerCore.getLastIp())
        }
    }
    Timer
    {
        id: watchTimer
        interval: 1000
        repeat: true
        property int secsRemaining: 5
        triggeredOnStart: true
        onTriggered: {
            reconnectText.refreshText()
            if(secsRemaining == 0)
                watchTimer.stop()
            --secsRemaining
        }
    }
    Connections{
        target: _controllerCore
        function onConnectionFinished(connected)
        {
            if (connected)
            {
               _controllerCore.setup();
                mainWindow.onAutoReconnected()
                disconnectedWindow.hide()
                reconnectText.text = "Reconnection success"
            }
            else
                reconnectText.text = "Unable to Auto Reconnect"

        }
        function onConnectionStarted(){
            reconnectText.text = "Reconnecting..."
        }
    }
    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        anchors.margins: 10

        Text{
            text: qsTr("Connection to MEDICAS lost") + _translator.emptyString
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 11
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillHeight: true
        }
        Text{
            id: reconnectText
            function refreshText()
            {
                 text = qsTr("Reconnecting in...") + _translator.emptyString + watchTimer.secsRemaining
            }
            property bool on: false
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 7
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillHeight: true
        }
        RowLayout{
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom | Qt.AlignVCenter

            Button{
                Layout.fillWidth: true
                text: qsTr("Reconnect") + _translator.emptyString
                onClicked: {
                    mainWindow.showConnectionWindow()
                    disconnectedWindow.hide()
                }
            }

            Button{
                Layout.fillWidth: true
                text: qsTr("Exit") + _translator.emptyString
                onClicked: Qt.callLater(Qt.quit);
            }
        }
    }

    Action{
        shortcut: StandardKey.InsertParagraphSeparator
        enabled: disconnectedWindow.visible
        onTriggered:{
            mainWindow.showConnectionWindow()
            disconnectedWindow.hide()
        }
    }

    Action{
        shortcut: StandardKey.Cancel
        enabled: disconnectedWindow.visible
        onTriggered: Qt.callLater(Qt.quit);
    }
}


