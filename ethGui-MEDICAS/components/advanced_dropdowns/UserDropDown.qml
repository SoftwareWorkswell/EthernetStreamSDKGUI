import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import "../"
import "./user_settings_dialogs"

ColumnLayout {
    Connections
    {
        target: _controllerCore
        onCurrentUserChanged:{
            currentUserText.text = _controllerCore.currentUser
        }
    }

    RowLayout{
        Text {
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            color: "white"
            font.pixelSize: 12
            Layout.fillWidth: true
            text: qsTr("Current user:") + _translator.emptyString
        }

        Text {
            id: currentUserText
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            color: "white"
            font.pixelSize: 12
            font.bold: true
            Layout.alignment: Qt.AlignRight
            text: _controllerCore.currentUser
        }
    }
    Text {
        id: loginText
        color: maLogin.containsMouse ? "orange" : "white"
        Layout.topMargin: 5
        Layout.leftMargin: 5
        Layout.rightMargin: 5
        font.bold: true
        font.pixelSize: 12
        text: qsTr("Login") + _translator.emptyString
        MouseArea {
            id: maLogin
            anchors.fill: parent
            hoverEnabled: true
            onClicked: loginDialog.open()
        }
    }
    Text {
        id: logoutText
        color: maLogout.containsMouse ? "orange" : "white"
        Layout.leftMargin: 5
        Layout.rightMargin: 5
        font.bold: true
        font.pixelSize: 12
        text: qsTr("Logout") + _translator.emptyString
        MouseArea {
            id: maLogout
            anchors.fill: parent
            hoverEnabled: true
            onClicked: _controllerCore.logoutUser()
        }
    }

    Text {
        id: changePasswordText
        color: _controllerCore.currentUser === "ADMIN" ?  (machangePassw.containsMouse ? "orange" : "white") : "gray"
        Layout.leftMargin: 5
        Layout.rightMargin: 5
        font.bold: true
        font.pixelSize: 12
        text: qsTr("Change passwords") + _translator.emptyString
        MouseArea {
            id: machangePassw
            anchors.fill: parent
            hoverEnabled: true
            enabled: (_controllerCore.currentUser === "ADMIN")
            onClicked: passwordDialog.open()
        }
    }

    Text {
        id: userSettingsText
        color:  _controllerCore.currentUser === "ADMIN" ? (maUserSettingsText.containsMouse ? "orange" : "white") : "gray"
        font.bold: true
        font.pixelSize: 12
        text: qsTr("Change user settings") + _translator.emptyString
        Layout.leftMargin: 5
        Layout.rightMargin: 5
        MouseArea {
            id: maUserSettingsText
            anchors.fill: parent
            hoverEnabled: true
            onClicked: settingsDialog.open()
            enabled: (_controllerCore.currentUser === "ADMIN")
        }
    }

    UserLoginDialog{
        id: loginDialog
    }

    ChangePasswordDialog{
        id: passwordDialog
    }

    ChangeSettingsDialog{
        id: settingsDialog
    }
}

