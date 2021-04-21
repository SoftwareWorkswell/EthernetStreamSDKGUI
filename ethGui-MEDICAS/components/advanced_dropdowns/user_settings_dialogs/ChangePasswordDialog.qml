import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import "../"
import "../../"

Dialog {
    id: changePasswordDialog
    title: "Change passwords"
    parent: userDropDown
    implicitWidth: parent.width
    implicitHeight: 150
    modal: true
    background: Rectangle {
        color: "#33333D"
        opacity: 75
    }
    contentItem: ColumnLayout {
        Layout.minimumWidth: 400
        RowLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 5
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
                model: ["USER1", "USER2", "USER3", "USER4"]
                currentIndex: 0
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 5
            Text {
                text: qsTr("New password: ") + _translator.emptyString
                font.pixelSize: 12
                Layout.fillWidth: true
                color: "white"
                font.bold: true
            }

            Rectangle{
                Layout.preferredHeight: selectedUser.height
                Layout.preferredWidth: selectedUser.width
                TextInput {
                    anchors.fill: parent
                    anchors.leftMargin: 3
                    maximumLength: 15
                    id: userPassword
                    verticalAlignment: Qt.AlignVCenter
                    font.bold: true
                    text: "PASSWORD"
                    color: "gray"
                    MouseArea{
                        anchors.fill: parent
                        id: maYear
                        onClicked:
                        {
                            userPassword.forceActiveFocus()
                            userPassword.text = ""
                            userPassword.color = "black"

                        }
                    }
                    onFocusChanged: if (!activeFocus && text === ""){
                                        userPassword.text = "PASSWORD"
                                        userPassword.color = "gray"
                                    }
                }
            }
        }

    }
    footer: RowLayout{
        Layout.fillWidth: true
        Text {
            color: maCloseLogin.containsMouse ? "orange" : "white"
            font.bold: true
            font.pixelSize: 12
            text: qsTr("Close") + _translator.emptyString
            Layout.alignment: Qt.AlignLeft
            Layout.leftMargin: 10
            Layout.bottomMargin: 10
            MouseArea {
                id: maCloseLogin
                anchors.fill: parent
                hoverEnabled: true
                onClicked: changePasswordDialog.close()
            }
        }
        Text {
            color: maLoginBtn.containsMouse ? "orange" : "white"
            font.bold: true
            font.pixelSize: 12
            text: qsTr("Change") + _translator.emptyString
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 10
            Layout.bottomMargin: 10
            MouseArea {
                id: maLoginBtn
                anchors.fill: parent
                hoverEnabled: true
                onClicked: _controllerCore.changeUserPassword(selectedUser.currentText, userPassword.text)
            }
        }

    }
    Action{
        shortcut: StandardKey.InsertParagraphSeparator
        enabled: changePasswordDialog.opened
        onTriggered:{
            _controllerCore.changeUserPassword(selectedUser.currentText, userPassword.text)
        }
    }
    Action{
        shortcut: StandardKey.Cancel
        enabled: changePasswordDialog.opened
        onTriggered: changePasswordDialog.close()
    }
}
