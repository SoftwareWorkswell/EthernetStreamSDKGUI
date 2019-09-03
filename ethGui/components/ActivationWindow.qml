import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: activationWindow
    width: 300
    height: 150
    flags: Qt.WindowStaysOnTopHint
    title: "Activation"

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
            textColor: "black"
            Layout.preferredWidth: 200

            placeholderText: "Your activation licence"
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

                    if (_controllerCore.activate(activationTextField.text))
                    {
                        _controllerCore.setup();
                        activationWindow.hide();
                        mainWindow.show();
                    }
                    else actErrorText.visible = true;
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
            _controllerCore.setup();
            activationWindow.hide();
            mainWindow.show();
        }
    }
}
