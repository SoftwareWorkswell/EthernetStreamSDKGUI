import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import "../"
import "../../"

Dialog {
    id: dateTimeDialog
    title: "Change date and time"
    parent: systemDropDown
    enabled: visible
     focus: false
    implicitWidth: parent.width
    implicitHeight: 150
    modal: true
    background: Rectangle {
        color: "#33333D"
        opacity: 75
    }
    contentItem: ColumnLayout {
        RowLayout {
            Layout.fillWidth: true
            Text {
                text: qsTr("Date: ") + _translator.emptyString
                font.pixelSize: 12
                color: "white"
                font.bold: true
                Layout.fillWidth: true
            }

            RowLayout {
                Rectangle{
                    Layout.preferredHeight: 15
                    Layout.preferredWidth: 50
                    TextInput {
                        anchors.fill: parent
                        id: year
                        maximumLength: 4
                        font.bold: true
                        horizontalAlignment: Qt.AlignHCenter
                        text: "YYYY"
                        color: "gray"
                        MouseArea{
                            anchors.fill: parent
                            id: maYear
                            onClicked:
                            {
                                year.forceActiveFocus()
                                year.text = ""
                                year.color = "black"

                            }
                        }
                        onFocusChanged: if (!activeFocus && text === ""){
                                            year.text = "YYYY"
                                            year.color = "gray"
                                        }
                    }
                }
                Rectangle{
                    Layout.preferredHeight: 15
                    Layout.preferredWidth: 25
                    TextInput {
                        anchors.fill: parent
                        id: month
                        maximumLength: 2
                        font.bold: true
                        horizontalAlignment: Qt.AlignHCenter
                        text: "MM"
                        color: "gray"
                        MouseArea{
                            anchors.fill: parent
                            id: maMonth
                            onClicked:
                            {
                                month.forceActiveFocus()
                                month.text = ""
                                month.color = "black"

                            }
                        }
                        onFocusChanged: if (!activeFocus && text === ""){
                                            month.text = "MM"
                                            month.color = "gray"
                                        }
                    }
                }
                Rectangle{
                    Layout.preferredHeight: 15
                    Layout.preferredWidth: 25
                    TextInput {
                        anchors.fill: parent
                        id: day
                        maximumLength: 2
                        font.bold: true
                        horizontalAlignment: Qt.AlignHCenter
                        text: "DD"
                        color: "gray"
                        MouseArea{
                            anchors.fill: parent
                            id: maDay
                            onClicked:
                            {
                                day.forceActiveFocus()
                                day.text = ""
                                day.color = "black"

                            }
                        }
                        onFocusChanged: if (!activeFocus && text === ""){
                                            day.text = "DD"
                                            day.color = "gray"
                                        }
                    }
                }
            }
        }
    }
    RowLayout {
        Text {
            Layout.fillWidth: true
            text: qsTr("Time: ") + _translator.emptyString
            font.pixelSize: 12
            color: "white"
            font.bold: true
        }
        RowLayout {
            Rectangle{
                Layout.preferredHeight: 15
                Layout.preferredWidth: 25
                TextInput {
                    anchors.fill: parent
                    id: hour
                    maximumLength: 2
                    font.bold: true
                    horizontalAlignment: Qt.AlignHCenter
                    text: "hh"
                    color: "gray"
                    MouseArea{
                        anchors.fill: parent
                        id: maHour
                        onClicked:
                        {
                            hour.forceActiveFocus()
                            hour.text = ""
                            hour.color = "black"
                        }
                    }
                    onFocusChanged: if (!activeFocus && text === ""){
                                        hour.text = "hh"
                                        hour.color = "gray"
                                    }
                }
            }
            Rectangle{
                Layout.preferredHeight: 15
                Layout.preferredWidth: 25
                TextInput {
                    anchors.fill: parent
                    id: minute
                    maximumLength: 2
                    font.bold: true
                    horizontalAlignment: Qt.AlignHCenter
                    text: "mm"
                    color: "gray"
                    MouseArea{
                        anchors.fill: parent
                        id: maMinute
                        onClicked:
                        {
                            minute.forceActiveFocus()
                            minute.text = ""
                            minute.color = "black"

                        }
                    }
                    onFocusChanged: if (!activeFocus && text === ""){
                                        "mm"
                                        minute.color = "gray"
                                    }
                }
            }
            Rectangle{
                Layout.preferredHeight: 15
                Layout.preferredWidth: 25
                TextInput {
                    anchors.fill: parent
                    id: second
                    maximumLength: 2
                    font.bold: true
                    horizontalAlignment: Qt.AlignHCenter
                    text: "ss"
                    color: "gray"
                    MouseArea{
                        anchors.fill: parent
                        id: maSecond
                        onClicked:
                        {
                            second.forceActiveFocus()
                            second.text = ""
                            second.color = "black"

                        }
                    }
                    onFocusChanged: if (!activeFocus && text === ""){
                                        second.text = "ss"
                                        second.color = "gray"
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
            text: qsTr("Cancel") + _translator.emptyString
            Layout.alignment: Qt.AlignLeft
            Layout.leftMargin: 10
            Layout.bottomMargin: 10
            MouseArea {
                id: maCloseLogin
                anchors.fill: parent
                hoverEnabled: true
                onClicked: dateTimeDialog.close()
            }
        }
        Text {
            color: maLoginBtn.containsMouse ? "orange" : "white"
            font.bold: true
            font.pixelSize: 12
            text: qsTr("Apply") + _translator.emptyString
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 10
            Layout.bottomMargin: 10
            MouseArea {
                id: maLoginBtn
                anchors.fill: parent
                hoverEnabled: true
                onClicked:if( _controllerCore.setDateTime(year.text, month.text, day.text, hour.text, minute.text, second.text))
                          {
                              dateTimeDialog.close()
                          }
            }
        }

    }
    Action{
        shortcut: StandardKey.InsertParagraphSeparator
        enabled: dateTimeDialog.opened
        onTriggered:{
            if( _controllerCore.setDateTime(year.text, month.text, day.text, hour.text, minute.text, second.text))
            {
                dateTimeDialog.close()
            }
        }
    }
    Action{
        shortcut: StandardKey.Cancel
        enabled: dateTimeDialog.opened
        onTriggered: dateTimeDialog.close()
    }
}
