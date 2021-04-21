import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import "../"
import "../../"

Dialog {
    id: loginDialog
    title: "Login"
    parent: userDropDown
    enabled: visible
    focus: false
    implicitWidth: parent.width
    implicitHeight: 200
    modal: true
    background: Rectangle {
        color: "#33333D"
        opacity: 75
    }
    contentItem: ColumnLayout {
        Layout.minimumWidth: 400
        RowLayout {
            Layout.fillWidth: true
            Text {
                id: userText
                text: qsTr("User: ") + _translator.emptyString
                font.pixelSize: 12
                Layout.fillWidth: true
                color: "white"
                font.bold: true
            }
            CustomComboBox {
                id: selectedUser
                model: ["USER1", "USER2", "USER3", "USER4", "ADMIN"]
                currentIndex: 0
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Text {
                text: qsTr("Password: ") + _translator.emptyString
                font.pixelSize: 12
                Layout.fillWidth: true
                color: "white"
                font.bold: true
            }

            Rectangle {
                Layout.preferredHeight: selectedUser.height
                Layout.preferredWidth: selectedUser.width
                TextInput {
                    property string password: ""
                    anchors.fill: parent
                    anchors.leftMargin: 3
                    maximumLength: 15
                    id: userPassword
                    font.bold: true
                    verticalAlignment: Qt.AlignVCenter
                    text: "PASSWORD"
                    color: "gray"
                    MouseArea {
                        anchors.fill: parent
                        id: maUserPassword
                        onClicked: {
                            userPassword.forceActiveFocus()
                            userPassword.text = ""
                            userPassword.color = "black"
                            userPassword.password = ""
                        }
                    }
                    // hide pass
                    onTextEdited:
                    {
                        if(showPassCheck.checked)
                        {
                            password = text
                            return
                        }

                        if(text.length == 0)
                        {
                            password = "";
                            return;
                        }
                        if(text.length < password.length)
                        {
                            password = password.substr(0, password.length-1)
                            return
                        }

                        password += text.charAt(text.length-1)
                        text = text.substr(0,text.length-1)
                        text += '*'
                        console.log(password)
                    }

                    onFocusChanged: if (!activeFocus && text === "") {
                                        userPassword.text = "PASSWORD"
                                        userPassword.color = "gray"
                                    }
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Text {
                id: showPassText
                text: qsTr("    Show password: ") + _translator.emptyString
                font.pixelSize: 10
                Layout.fillWidth: true
                color: "white"
                font.bold: true
            }
            PopupCheckBox {
                id: showPassCheck
                text: ""
                checked: false
                onCheckedChanged:
                {
                    if(checked)
                    {
                        userPassword.text = userPassword.password
                    }
                    else
                    {
                        userPassword.password = userPassword.text
                        userPassword.text = ""
                        for(var i = 0; i < userPassword.password.length; ++i)
                            userPassword.text += '*'
                    }
                }
            }
        }
    }
    footer: RowLayout {
        Layout.fillWidth: true
        Text {
            color: maCloseLogin.containsMouse ? "orange" : "white"
            font.bold: true
            font.pixelSize: 12
            text: qsTr("Cancel") + _translator.emptyString
            Layout.alignment: Qt.AlignLeft
            Layout.leftMargin: 10
            Layout.bottomMargin: 10
            MouseArea {
                id: maCloseLogin
                anchors.fill: parent
                hoverEnabled: true
                onClicked:{
                    userPassword.text = userPassword.password = ""
                    loginDialog.close()
                }
            }
        }
        Text {
            color: maLoginBtn.containsMouse ? "orange" : "white"
            font.bold: true
            font.pixelSize: 12
            text: qsTr("Login") + _translator.emptyString
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 10
            Layout.bottomMargin: 10
            MouseArea {
                id: maLoginBtn
                anchors.fill: parent
                hoverEnabled: true
                onClicked: if (_controllerCore.loginUser(
                                       selectedUser.currentText,
                                       userPassword.text === "PASSWORD" ? "" : userPassword.password)) {
                               userPassword.text = userPassword.password = ""
                               loginDialog.close()
                           }
            }
        }
    }
    Action {
        shortcut: StandardKey.InsertParagraphSeparator
        enabled: loginDialog.opened
        onTriggered: {
            if (_controllerCore.loginUser(
                        selectedUser.currentText,
                        userPassword.text === "PASSWORD" ? "" : userPassword.text)) {
                loginDialog.close()
            }
        }
    }
    Action {
        shortcut: StandardKey.Cancel
        enabled: loginDialog.opened
        onTriggered: loginDialog.close()
    }
}
