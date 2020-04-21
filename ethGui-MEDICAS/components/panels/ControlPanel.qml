import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "../"

Rectangle{
    function hideAllPopups()
    {
        mainviewParent.getPopup("rangePopup").visible = false;
        rangeImg.source = "/img/png/button_range_c.png";
        rangeButton.highlighted = false;

        mainviewParent.getPopup("functionsPopup").visible = false;
        functionsImg.source = "/img/png/button_functions.png";
        functionsButton.highlighted = false;

        mainviewParent.getPopup("palettePopup").visible = false;
        paletteImg.source = "/img/png/button_palette.png";
        paletteButton.highlighted = false;

        mainviewParent.getPopup("capturePopup").visible = false;
        captureImg.source = "/img/png/button_capture.png";
        captureButton.highlighted = false;

        mainviewParent.getPopup("measurePopup").visible = false;
        measureImg.source = "/img/png/button_measure.png";
        measureButton.highlighted = false;

        mainviewParent.getPopup("advancedPopup").visible = false;
        advancedImg.source = "/img/png/button_settings.png";
        advancedButton.highlighted = false;
    }
    Rectangle{
        anchors.fill: parent
        color: "#33333D"
        ColumnLayout{
            width: parent.width
            height: parent.height
            ControlButton {
                id: rangeButton
                text: "Range"
                visible: _controllerCore.type != mainWindow.securityType
                Layout.fillWidth: true
                Layout.preferredHeight: 54
                Image{
                    id: rangeImg
                    source: "/img/png/button_range_c.png"
                    fillMode: Image.PreserveAspectFit
                    height: 25
                    anchors.horizontalCenter: parent.horizontalCenter
                    y: 7
                }
                onClicked: {
                    if (!mainviewParent.isPopupVisible("rangePopup"))
                    {
                        controlPanel.hideAllPopups();
                        mainviewParent.setPopupVisible("rangePopup", true)
                        rangeButton.highlighted = true;
                        rangeImg.source = "/img/png/button_range_c_selected.png"
                    }
                    else{
                        controlPanel.hideAllPopups();
                    }
                }
            }
            ControlButton {
                id: functionsButton
                text: "Functions"
                Layout.fillWidth: true
                Layout.preferredHeight: 54
                Image{
                    id: functionsImg
                    source: "/img/png/button_functions.png"
                    fillMode: Image.PreserveAspectFit
                    height: 25
                    anchors.horizontalCenter: parent.horizontalCenter
                    y: 7
                }
                onClicked: {
                    if (!mainviewParent.isPopupVisible("functionsPopup"))
                    {
                        controlPanel.hideAllPopups();
                        mainviewParent.setPopupVisible("functionsPopup", true)
                        functionsButton.highlighted = true;
                        functionsImg.source = "/img/png/button_functions_selected.png"
                    }
                    else{
                        controlPanel.hideAllPopups();
                    }
                }
            }
            ControlButton {
                id: paletteButton
                text: "Palette"
                Layout.fillWidth: true
                Layout.preferredHeight: 54
                Image{
                    id: paletteImg
                    source: "/img/png/button_palette.png"
                    fillMode: Image.PreserveAspectFit
                    height: 25
                    anchors.horizontalCenter: parent.horizontalCenter
                    y: 7
                }
                onClicked: {
                    if (!mainviewParent.isPopupVisible("palettePopup"))
                    {
                        controlPanel.hideAllPopups();
                        mainviewParent.setPopupVisible("palettePopup", true)
                        paletteButton.highlighted = true;
                        paletteImg.source = "/img/png/button_palette_selected.png"
                    }
                    else{
                        controlPanel.hideAllPopups();
                    }
                }
            }
            ControlButton {
                id: captureButton
                text: "Capture"
                Layout.fillWidth: true
                Layout.preferredHeight: 54
                Image{
                    id: captureImg
                    source: "/img/png/button_capture.png"
                    fillMode: Image.PreserveAspectFit
                    height: 25
                    anchors.horizontalCenter: parent.horizontalCenter
                    y: 7
                }
                onClicked: {
                    if (!mainviewParent.isPopupVisible("capturePopup"))
                    {
                        controlPanel.hideAllPopups();
                        mainviewParent.setPopupVisible("capturePopup", true)
                        captureButton.highlighted = true;
                        captureImg.source = "/img/png/button_capture_selected.png"
                    }
                    else{
                        controlPanel.hideAllPopups();
                    }
                }
            }
            ControlButton {
                id: measureButton
                text: "Measure"
                Layout.fillWidth: true
                Layout.preferredHeight: 54
                Image{
                    id: measureImg
                    source: "/img/png/button_measure.png"
                    fillMode: Image.PreserveAspectFit
                    height: 25
                    anchors.horizontalCenter: parent.horizontalCenter
                    y: 7
                }
                onClicked: {
                    if (!mainviewParent.isPopupVisible("measurePopup"))
                    {
                        controlPanel.hideAllPopups();
                        mainviewParent.setPopupVisible("measurePopup", true)
                        measureButton.highlighted = true;
                        measureImg.source = "/img/png/button_measure_selected.png"
                    }
                    else{
                        controlPanel.hideAllPopups();
                    }
                }
            }
            ControlButton {
                id: advancedButton
                text: "Advanced"
                Layout.fillWidth: true
                Layout.preferredHeight: 54
                Image{
                    id: advancedImg
                    source: "/img/png/button_settings.png"
                    fillMode: Image.PreserveAspectFit
                    height: 25
                    anchors.horizontalCenter: parent.horizontalCenter
                    y: 7
                }
                onClicked: {
                    if (!mainviewParent.isPopupVisible("advancedPopup"))
                    {
                        controlPanel.hideAllPopups();
                        mainviewParent.setPopupVisible("advancedPopup", true)
                        advancedButton.highlighted = true;
                        advancedImg.source = "/img/png/button_settings_selected.png"
                    }
                    else{
                        controlPanel.hideAllPopups();
                    }
                }
            }
        }
    }
}
