import QtQuick 2.3
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: activationWindow
    width: 300
    height: 150
    flags: Qt.WindowStaysOnTopHint
    title: "Activation"
    function doActivate(){
        if (_controllerCore.activate(activationTextField.text))
        {
            if(_controllerCore.setup())
            {
                activationWindow.hide();
                mainWindow.show();
            }
        }
        else
            actErrorText.visible = true;
    }
    Action{
        shortcut: StandardKey.Cancel
        onTriggered: Qt.callLater(Qt.quit);
    }
    ColumnLayout{

        width: parent.width
        anchors.centerIn: parent

        Text {
            Layout.alignment: Qt.AlignHCenter

            text: qsTr("Please, enter your activation licence: ")
        }

        TextField{
            id: activationTextField
            Layout.alignment: Qt.AlignHCenter

            Layout.preferredWidth: 200

            placeholderText: "Your activation licence"
            onAccepted: {
                activationWindow.doActivate();
            }
        }
        Text {
            id: actErrorText

            Layout.alignment: Qt.AlignHCenter

            text: "Activation not successfull"
            color: "red"

            visible: false
        }

        Row{
            Layout.alignment: Qt.AlignHCenter
            spacing: 5

            Button{
                text: "OK"
                onClicked: {
                    activationWindow.doActivate();
                }

            }

            Button{
                text: "Cancel"
                onClicked: {
                    Qt.callLater(Qt.quit);
                }
            }

        }

    }

    Component.onCompleted: {
        if (_controllerCore.isActivated())
        {
            if(_controllerCore.setup())
            {
                activationWindow.hide();
                mainWindow.show();
            }
        }
    }
}
