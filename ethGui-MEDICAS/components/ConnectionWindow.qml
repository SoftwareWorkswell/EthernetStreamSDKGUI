import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: connectionWindow
    width: 300
    height: 150
    flags: Qt.WindowStaysOnTopHint
    title: qsTr("Connect") + _translator.emptyString

    function tryConnect()
    {
        var ip = ipTextField.text ? ipTextField.text : ipTextField.placeholderText;
        if(! _controllerCore.checkAddressFormat(ip))
        {
            connectionErrorText.visible = false;
            ipErrorText.visible = true;
            return
        }

        if (! _controllerCore.connect(ip))
        {
            ipErrorText.visible = false;
            connectionErrorText.visible = true;
            return;
        }
    }

    function stopConnect()
    {
        progressRect.terminated = true
        _controllerCore.stopConnect()
        progressRect.on = false

    }

    Connections{
        target: _controllerCore
        function onConnectionFinished(connected)
        {
            progressRect.on = false
            if (connected)
            {
                if (!_controllerCore.isActivated())
                {
                    var component = Qt.createComponent("ActivationWindow.qml");
                    var window    = component.createObject(mainWindow);
                    window.show();
                    window.connectionWindowShown = false
                    connectionWindow.hide();
                }
                else
                {
                    _controllerCore.setup();
                    connectionWindow.hide();
                    mainWindow.show()
                    mainWindow.connectionWindowShown = false;
                }
            }
            else {
                if(!progressRect.terminated)
                    connectionErrorText.visible = true;
                progressRect.terminated = false
            }
        }
        function onConnectionStarted(){
            progressRect.on = true
        }
    }

    Action{
        shortcut: StandardKey.Cancel
        onTriggered:{

            if(progressRect.on)
                connectionWindow.stopConnect()
            else
                Qt.callLater(Qt.quit);
        }
    }

    Rectangle{
        id: progressRect
        visible: progressRect.on
        anchors.centerIn: parent
        anchors.fill: parent
        border.color: Style.ligterGray
        property bool on: false
        property bool terminated: false

        GridLayout
        {
            anchors.fill: parent
            Text {
                Layout.row: 0
                Layout.column: 1
                color: Style.textColor
                font.family: Style.textFontfamily
                font.pointSize: Style.textFontSize
                Layout.alignment: Qt.AlignLeft
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                text: qsTr("Connecting...") + _translator.emptyString
            }
            Image {
                id: container
                Layout.row: 0
                Layout.column: 0
                Layout.preferredWidth: 35
                Layout.preferredHeight: 35
                Layout.alignment: Qt.AlignRight
                source: "../img/png/busy2.png"; visible: progressRect.on
                fillMode: Image.PreserveAspectFit
                NumberAnimation on rotation { running: progressRect.on; from: 0; to: -360; loops: Animation.Infinite; duration: 2000 }
            }
            Button{
                text: qsTr("Stop") + _translator.emptyString
                Layout.row: 1
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignCenter
                onClicked: connectionWindow.stopConnect()
            }
        }
    }
    ColumnLayout{
        visible: !progressRect.on
        width: parent.width
        anchors.centerIn: parent

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("MEDICAS IP address: ") + _translator.emptyString
        }

        TextField{
            id: ipTextField
            Layout.alignment: Qt.AlignHCenter
            text: _controllerCore.getLastIp();
            Layout.preferredWidth: 200
            focus: true
            placeholderText: "10.0.0.230"
            onAccepted: {
                connectionWindow.tryConnect()
            }
        }
        Text{
            id: connectionErrorText
            color: "red"
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Unable to connect, please check your connection to MEDICAS") + _translator.emptyString
            font.pixelSize: 9
            visible: false
        }
        Text{
            id: ipErrorText
            color: "red"
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Wrong IP address format, please use valid IPv4 address") + _translator.emptyString
            font.pixelSize: 9
            visible: false
        }
        Row{
            Layout.alignment: Qt.AlignHCenter
            spacing: 5
            Button{
                text: qsTr("Connect") + _translator.emptyString
                onClicked: connectionWindow.tryConnect()
            }

            Button{
                text: qsTr("Exit") + _translator.emptyString
                onClicked: {
                    Qt.callLater(Qt.quit);
                }
            }
        }
    }

}
