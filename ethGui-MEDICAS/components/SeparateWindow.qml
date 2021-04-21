import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "./"
import "./popups"
import "./panels"

ApplicationWindow {
    id: streamWindow
    title: qsTr("MEDICAS Ethernet Stream SDK GUI")
    visible: false
    minimumWidth: 640
    minimumHeight: 480
    onHeightChanged: onResizedSeparateWindow(streamWindow.width, streamWindow.height)
    onWidthChanged: onResizedSeparateWindow(streamWindow.width, streamWindow.height)
    property var streamSource: ""
    Image {
        id: separateView
        source: streamSource
        anchors.centerIn: parent
    }
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
}
/*
    Window {
        id: streamWindow
        title: qsTr("MEDICAS Ethernet Stream SDK GUI")
        transientParent: null
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
   */
