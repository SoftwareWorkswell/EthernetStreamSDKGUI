import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3

import "./components"
import "./components/popups"
import "./components/panels"

Window {
    id: mainWindow
    // set to true to just show gui without camera connection
    property bool testGUI: false
    property var separateWindow: Qt.createComponent(
                                     "./components/SeparateWindow.qml").createObject(
                                     "streamWindow")
    property bool disconnectedWindowShown: false
    property bool connectionWindowShown: false
    visible: true
    minimumWidth: 960
    minimumHeight: 540
    title: Qt.application.name + " " + Qt.application.version
    onClosing: _controllerCore.close()
    function showConnectionWindow() {
        var component = Qt.createComponent("components/ConnectionWindow.qml")
        var window = component.createObject(mainWindow)
        window.show()
        mainWindow.hide()
        disconnectedWindowShown = false
        connectionWindowShown = true
    }

    function showDisconnectedWindow() {
        if (disconnectedWindowShown == false
                && connectionWindowShown == false) {
            var component = Qt.createComponent(
                        "components/CameraDisconnectedMessage.qml")
            var window = component.createObject(mainWindow)
            window.show()
            mainWindow.disconnectedWindowShown = true
        }
    }

    function onAutoReconnected() {
        disconnectedWindowShown = false
        connectionWindowShown = false
    }

    Rectangle {
        anchors.fill: parent
        Item {
            id: mainRow
            anchors.fill: parent
            Rectangle {
                anchors.left: parent.left
                anchors.top: parent.top
                width: parent.width * 7 / 10
                height: parent.height
                color: "red"
                Item {
                    anchors.fill: parent
                    Rectangle {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        width: parent.width
                        height: parent.height - 40
                        Rectangle {
                            anchors.fill: parent
                            ControlPanel {
                                id: controlPanel
                                anchors.top: parent.top
                                anchors.left: parent.left
                                height: parent.height
                                width: 70
                            }
                            MainviewPanel {
                                id: mainviewParent
                                height: parent.height
                                anchors.left: controlPanel.right
                                anchors.right: palettePanel.left
                            }
                            Component {
                                id: stopComponent
                                GradientStop {}
                            }
                            PalettePanel {
                                id: palettePanel
                                anchors.right: parent.right
                                width: 60
                                height: parent.height
                                color: "#444444"
                            }
                        }
                    }
                    QuickMenuPanel {
                        id: quickMenuPanel
                        color: "#16151D"
                        width: parent.width
                        anchors.bottom: parent.bottom
                        height: 40
                    }
                }
            }
            RightPanel {
                id: rightPanel
                anchors.right: parent.right
                anchors.top: parent.top
                width: parent.width * 3 / 10
                height: parent.height
                color: "black"
            }
        }
    }
    Component.onCompleted: {
        if (mainWindow.testGUI)
            return
        showConnectionWindow()
    }

    Connections {
        target: _controllerCore
        function onCameraDisconnected() {
            showDisconnectedWindow()
        }
    }
}
