import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "../"
import "../popups/"
import "../positioners"

Rectangle {
    function toggleControlPanel() {
        if (controlPanel.visible) {
            controlPanel.visible = false
            controlPanel.width = 0
            hideControlManelImg.rotation = 270
        } else {
            controlPanel.width = 70
            controlPanel.visible = true
            hideControlManelImg.rotation = 90
        }
        mainView.onResized(mainviewParent.width, mainviewParent.height)
    }
    function isPopupVisible(id) {
        for (var i = 0; i < controlPanelPopup.children.length; ++i)
            if (controlPanelPopup.children[i].objectName === id)
                return controlPanelPopup.children[i].visible
        return false
    }

    function setPopupVisible(id, visible) {
        for (var i = 0; i < controlPanelPopup.children.length; ++i)
            if (controlPanelPopup.children[i].objectName === id)
                return controlPanelPopup.children[i].visible = visible
    }

    function getPopup(id) {
        for (var i = 0; i < controlPanelPopup.children.length; ++i)
            if (controlPanelPopup.children[i].objectName === id)
                return controlPanelPopup.children[i]
        return []
    }

    function setMainViewSource(source) {
        mainView.source = source
    }

    function hideAllQuickRects() {
        quickRangeSliderRect.visible = false
        quickAlarmsRect.visible = false
        if(quickBlackbodyRect.visible == true)
        {
            _controllerCore.setCurrentRoiIdx(-1)
            _controllerCore.sendOriginalBlackbodyCoordinates();
        }
        quickBlackbodyRect.visible = false
        quickUserRoiRect.visible = false
        quickUserCrossRect.visible = false

        quickMenuPanel.disableHighlights()
    }

    function setManRangeSliderVisible(visible) {
        quickRangeSliderRect.visible = visible
    }

    function setAlarmsSliderVisible(visible) {
        quickAlarmsRect.visible = visible
    }

    function setBlackbodyRectVisible(visible) {
        quickBlackbodyRect.visible = visible
    }

    function setUserROIRectVisible(visible) {
        quickUserRoiRect.visible = visible
    }

    function setUserROIRectIdx(idx) {
        quickUserRoiRect.roiIdx = idx
    }

    function setUserCrossRectVisible(visible) {
        quickUserCrossRect.visible = visible
    }

    function initUserROIRect(roiIdx) {
        quickUserRoiRect.roiIdx = roiIdx
        quickUserRoiForm.roiIdx = roiIdx
    }

    function isPeriodicCaptureChecked() {
        return capturePopup.isPeriodicCaptureChecked()
    }

    function isPeriodicCaptureRunning() {
        return capturePopup.isPeriodicCaptureRunning()
    }

    function setPeriodicCaptureRunning(running) {
        capturePopup.setPeriodicCaptureRunning(running)
    }

    function setPauseIcon(src) {
        quickPauseImg.source = src
    }

    function showHeatingUpMessage()
    {
        measurePopup.showHeatingUpMessage()
    }

    Image {
        id: mainView
        function onResized(parentWidth, parentHeight) {
            var cwidth = _controllerCore.mainCamera === "THERMAL" ? 5 : 16
            var cheight = _controllerCore.mainCamera === "THERMAL" ? 4 : 9
            if (parentWidth / cwidth * cheight < parentHeight) {
                mainView.width = parentWidth
                mainView.height = mainView.width / cwidth * cheight
            } else {
                mainView.height = parentHeight
                mainView.width = mainView.height / cheight * cwidth
            }
        }
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectFit
        height: 400
        width: 500
        source: "/img/screen/init.png"
        Connections {
            target: _controllerCore
            function onStreamFrameChanged() {
                if (_controllerCore.mainCamera === "THERMAL")
                    mainView.source = "image://streamFramesProvider/500/500/?seed=" + Math.random()
                else {
                    if (mainWindow.separateWindow.visible)
                        mainWindow.separateWindow.streamSource
                                = "image://streamFramesProvider/500/500/?seed=" + Math.random()
                    else
                        rightPanel.setSecondarySource(
                                    "image://streamFramesProvider/500/500/?seed=" + Math.random(
                                        ))
                }
            }
        }
        Connections {
            target: mainviewParent
            function onHeightChanged() {
                mainView.onResized(mainviewParent.width, mainviewParent.height)
            }
        }
        Connections {
            target: mainviewParent
            function onWidthChanged() {
                mainView.onResized(mainviewParent.width, mainviewParent.height)
            }
        }
    }
    MouseArea {
        width: 20
        hoverEnabled: true
        height: 20
        opacity: containsMouse ? 1.0 : 0.5
        x: 10
        y: 10
        Image {
            id: hideControlManelImg
            source: "/img/png/ico_arrow_down.png"
            height: parent.height
            fillMode: Image.PreserveAspectFit
            rotation: 90
        }
        onClicked: {
            mainviewParent.toggleControlPanel()
        }
    }
    MouseArea {
        width: 30
        hoverEnabled: true
        height: 30
        opacity: containsMouse ? 1.0 : 0.5
        x: 10
        y: parent.height - 35
        Image {
            id: quickPauseImg
            source: "/img/png/button-pause.png"
            height: parent.height
            fillMode: Image.PreserveAspectFit
        }
        onClicked: {
            if (!(_controllerCore.mainCamera === "THERMAL")) {
                _controllerCore.toggleStreamVisible()
                if (_controllerCore.streamingVisible())
                    quickPauseImg.source = "/img/png/button-pause.png"
                else
                    quickPauseImg.source = "/img/png/button-play.png"
            } else {
                _controllerCore.toggleStreamThermal()
                if (_controllerCore.streamingThermal())
                    quickPauseImg.source = "/img/png/button-pause.png"
                else
                    quickPauseImg.source = "/img/png/button-play.png"
            }
        }
    }
    MouseArea {
        width: 20
        hoverEnabled: true
        height: 20
        opacity: containsMouse ? 1.0 : 0.5
        x: 40
        y: parent.height - 30
        Image {
            id: snapImg
            source: "/img/png/button_functions_selected.png"
            height: parent.height
            fillMode: Image.PreserveAspectFit
        }
        onClicked: {
            _controllerCore.mainCameraSnapshot()
        }
    }
    Rectangle {
        height: 30
        width: 120
        visible: false
        x: parent.width - 130
        y: 20
        id: nucPopup
        color: "#88000000"
        Text {
            anchors.centerIn: parent
            color: "red"
            text: qsTr("NUC in progress...") + _translator.emptyString
        }
    }
    ColumnLayout {
        id: controlPanelPopup
        width: 280
        height: parent.height

        RangePopup {
            id: rangePopup
            width: parent.width
            objectName: "rangePopup"
        }
        FunctionsPopup {
            id: functionsPopup
            width: parent.width
            objectName: "functionsPopup"
        }
        PalettePopup {
            id: palettePopup
            width: parent.width
            objectName: "palettePopup"
        }
        CapturePopup {
            id: capturePopup
            width: parent.width
            objectName: "capturePopup"
            onCaptureToggled: quickMenuPanel.toggleCaptureClick()
        }
        MeasurePopup {
            id: measurePopup
            width: parent.width
            objectName: "measurePopup"
        }
        AdvancedPopup {
            id: advancedPopup
            width: parent.width
            objectName: "advancedPopup"
        }
    }
    Rectangle {
        id: quickRangeSliderRect
        visible: false
        height: 35
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.left: mainView.left
        anchors.right: mainView.right
        color: "#DD444444"
        ManualRangeSlider {
            id: quickManSlider
            anchors.fill: parent
        }
    }
    Rectangle {
        id: quickAlarmsRect
        visible: false
        height: 35
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.left: mainView.left
        anchors.right: mainView.right
        color: "#DD444444"
        AlarmsSliderQuick {
            id: quickAlarmSlider
            from: _controllerCore.getAlarmMinLimit()
            to: _controllerCore.getAlarmMaxLimit()
            anchors.fill: parent
        }
    }
    Rectangle {
        id: quickBlackbodyRect
        visible: false
        height: 42
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.left: mainView.left
        anchors.right: mainView.right
        color: "#DD444444"
        BlackbodyPositionForm {
            anchors.fill: parent
            onClosingForm: {
                mainviewParent.setBlackbodyRectVisible(false)
            }
        }
    }
    Rectangle {
        id: quickUserRoiRect
        visible: false
        height: 42
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.left: mainView.left
        anchors.right: mainView.right
        color: "#DD444444"
        property int roiIdx: -1
        UserRoiPositionForm {
            id: quickUserRoiForm
            anchors.fill: parent
            onClosingForm: {
                mainviewParent.setUserROIRectVisible(false)
            }
        }
    }
    Rectangle {
        id: quickUserCrossRect
        visible: false
        height: 42
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.left: mainView.left
        anchors.right: mainView.right
        color: "#DD444444"
        property int roiIdx: -1
        UserCrossPositionForm {
            anchors.fill: parent
            onClosingForm: {
                mainviewParent.setUserCrossRectVisible(false)
            }
        }
    }
}
