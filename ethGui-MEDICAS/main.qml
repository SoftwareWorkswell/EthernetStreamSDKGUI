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
    property bool testGUI:false
    visible: true
    minimumWidth: 960
    minimumHeight: 540
    title: qsTr("MEDICAS Ethernet Stream SDK GUI 0.0.1")
    onClosing: {
        _controllerCore.setEthModeOff()
        _controllerCore.close()
    }
    function showConnectionWindow() {
        var component = Qt.createComponent("components/ConnectionWindow.qml");
        var window    = component.createObject(mainWindow);
        window.show();
        mainWindow.hide();
    }

    Rectangle{
        anchors.fill: parent
        Row{
            id: mainRow
            anchors.fill: parent
            Rectangle{
                anchors.left: parent.left;
                anchors.top: parent.top;
                width: parent.width *7/10
                height: parent.height
                color: "red"
                Row{
                    anchors.fill: parent
                    Rectangle{
                        anchors.top: parent.top
                        anchors.left: parent.left
                        width: parent.width
                        height: parent.height - 40
                        Rectangle{
                            anchors.fill: parent
                            ControlPanel {
                                id: controlPanel
                                anchors.top: parent.top
                                anchors.left: parent.left
                                height: parent.height
                                width: 70
                            }
                            MainviewPanel{
                                id: mainviewParent
                                height: parent.height
                                anchors.left: controlPanel.right
                                anchors.right: palettePanel.left
                            }
                            Component
                            {
                                id:stopComponent
                                GradientStop {}
                            }
                            PalettePanel{
                                id: palettePanel
                                anchors.right: parent.right
                                width: 60
                                height: parent.height
                                color: "#444444"
                            }
                        }
                    }
                    QuickMenuPanel{
                        id: quickMenuPanel
                        color: "#16151D"
                        width: parent.width
                        anchors.bottom: parent.bottom
                        height: 40
                    }
                }
            }
            RightPanel{
                id: rightPanel
                anchors.right: parent.right;
                anchors.top: parent.top;
                width: parent.width *3/10
                height: parent.height
                color: "black"
            }
        }
    }
    Component.onCompleted: {
        if(mainWindow.testGUI)
            return
        showConnectionWindow()
    }
    ApplicationWindow {
        id: streamWindow
        title: qsTr("MEDICAS Ethernet Stream SDK GUI")
        minimumWidth: 640
        minimumHeight: 480
        onHeightChanged: separateView.onResizedSeparateWindow(streamWindow.width, streamWindow.height)
        onWidthChanged: separateView.onResizedSeparateWindow(streamWindow.width, streamWindow.height)
        Image {
            id: separateView
            function onResizedSeparateWindow(parentWidth, parentHeight)
            {
                var cwidth = _controllerCore.mainCamera === "THERMAL" ? 16 : 5;
                var cheight = _controllerCore.mainCamera === "THERMAL" ? 9 : 4;
                if(parentWidth/cwidth*cheight < parentHeight)
                {
                    separateView.width = parentWidth;
                    separateView.height = separateView.width/cwidth*cheight;
                }
                else{
                    separateView.height = parentHeight;
                    separateView.width = separateView.height/cheight*cwidth;
                }
            }
            anchors.centerIn: parent
        }
    }
}
