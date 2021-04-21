
import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: activationWindow
    width: 300
    height: 150
    flags: Qt.WindowStaysOnTopHint
    title: qsTr("Activation") + _translator.emptyString

    function doActivate(){
        if (_controllerCore.activate(activationTextField.text))
        {
            _controllerCore.setup();
            activationWindow.hide();
            mainWindow.show();
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

            text: qsTr("Please, enter your activation licence: ") + _translator.emptyString
        }

        TextField{
            id: activationTextField
            Layout.alignment: Qt.AlignHCenter
            focus: true
            Layout.preferredWidth: 200

            placeholderText: qsTr("Your activation licence") + _translator.emptyString;
            onAccepted: {
                activationWindow.doActivate();
            }
        }
        Text {
            id: actErrorText

            Layout.alignment: Qt.AlignHCenter

            text: qsTr("Activation not successfull") + _translator.emptyString
            color: "red"

            visible: false
        }

        Row{
            Layout.alignment: Qt.AlignHCenter
            spacing: 5

            Button{
                text: qsTr("OK") + _translator.emptyString
                onClicked: {
                    activationWindow.doActivate();
            }

        }

            Button{
                text: qsTr("Cancel") + _translator.emptyString
                onClicked: {
                    Qt.callLater(Qt.quit);
                }
            }

    }

}

    Component.onCompleted: {
        if (_controllerCore.isActivated())
        {
            _controllerCore.setup();
            activationWindow.hide();
            mainWindow.show();
        }
    }
}
