import QtQuick 2.3
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3


ApplicationWindow {
    id: disconnectedWindow
    title: qsTr("Disconnected")
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
        interval: 10000
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
        property int secsRemaining: 10
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
            if (connected && _controllerCore.setup())
            {
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
            text: qsTr("Connection to the camera lost!")
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 11
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillHeight: true
        }
        Text{
            id: reconnectText
            function refreshText()
            {
                 text = qsTr("Reconnecting in... ") + watchTimer.secsRemaining
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
                text: qsTr("Reconnect")
                onClicked: {
                    mainWindow.showConnectionWindow()
                    reconnectTimer.stop()
                    watchTimer.stop()
                    disconnectedWindow.hide()
                }
            }

            Button{
                Layout.fillWidth: true
                text: qsTr("Exit")
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
