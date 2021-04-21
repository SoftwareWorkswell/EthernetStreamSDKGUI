import QtQuick 2.3
import QtQuick.Controls 1.2


ApplicationWindow {
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
