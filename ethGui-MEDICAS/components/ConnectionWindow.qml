import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: connectionWindow
    width: 300
    height: 150
    flags: Qt.WindowStaysOnTopHint
    title: "Connect"

    ColumnLayout{

        width: parent.width
        anchors.centerIn: parent

        Text {
            Layout.alignment: Qt.AlignHCenter

            text: qsTr("MEDICAS IP address: ")
        }

        TextField{
            id: ipTextField
            Layout.alignment: Qt.AlignHCenter
            text: _controllerCore.getLastIp();
            Layout.preferredWidth: 200

            placeholderText: "10.0.0.230"
        }
        Text{
            id: connectionErrorText
            color: "red"
            Layout.alignment: Qt.AlignHCenter
            text: "Unable to connect, please check your connection to MEDICAS"
            font.pixelSize: 9
            visible: false
        }


        Row{
            Layout.alignment: Qt.AlignHCenter
            spacing: 5


            Button{
                text: "Connect"
                onClicked: {

                    var ip = ipTextField.text ? ipTextField.text : ipTextField.placeholderText;
                    console.log(ip);
                    if (_controllerCore.connect(ip))
                    {
                        var component = Qt.createComponent("ActivationWindow.qml");
                        var window    = component.createObject(mainWindow);
                        window.show();
                        connectionWindow.hide();
                    }
                    else {
                        connectionErrorText.visible = true;
                    }
                }
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
