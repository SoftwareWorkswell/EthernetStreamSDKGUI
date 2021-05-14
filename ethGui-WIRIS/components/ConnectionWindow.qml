import QtQuick 2.3
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: connectionWindow
    width: 350
    height: 175
    flags: Qt.WindowStaysOnTopHint
    title: "WIRIS & GIS Ethernet Stream SDK GUI 1.0.0 - Connect"

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
                    connectionWindow.hide();
                }
                else if(_controllerCore.setup())
                {
                    connectionWindow.hide();
                    mainWindow.show()
                    mainWindow.connectionWindowShown = false;
                }
                else
                    showError()
            }
            else {
                showError()
            }
        }
        function onConnectionStarted(){
            progressRect.on = true
        }
        function showError()
        {
            if(!progressRect.terminated)
                connectionErrorText.visible = true;
            progressRect.terminated = false
        }
    }


    Rectangle{
        id: progressRect
        visible: progressRect.on
        anchors.centerIn: parent
        anchors.fill: parent
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
                text: "Connecting..."
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
                text: "Stop"
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
            text: "WIRIS or GIS IP address: "
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
            text: "Unable to connect, please check your connection to camera"
            visible: false
        }
        Text{
            id: ipErrorText
            color: "red"
            Layout.alignment: Qt.AlignHCenter
            text: "Wrong IP address format, please use valid IPv4 address"
            font.pixelSize: 9
            visible: false
        }
        Row{
            Layout.alignment: Qt.AlignHCenter
            spacing: 5
            Button{
                text: "Connect"
                onClicked: connectionWindow.tryConnect()
            }

            Button{
                text: "Exit"
                onClicked: {
                    Qt.callLater(Qt.quit);
                }
            }
        }
    }
}
