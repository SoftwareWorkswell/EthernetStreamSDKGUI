import QtQuick 2.0
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "../"

ColumnLayout {
    id: imageVidColumn
    property bool changedProgramatically: false
    function setImageVid() {
        if (imageVidColumn.changedProgramatically)
            return
        _controllerCore.setCaptureVideoSettings(radJpgCheck.checked,
                                                visJpgCheck.checked,
                                                radVidCheck.checked,
                                                visVidCheck.checked)
    }
    PopupCheckBox {
        id: radJpgCheck
        text: _controllerCore.type === mainWindow.securityType ? (qsTr("Image IR JPEG") + _translator.emptyString) : (qsTr("Image Radiometric JPEG") + _translator.emptyString)
        onClicked: imageVidColumn.setImageVid()
        onCheckedChanged: quickMenuPanel.captureEnabled = (radJpgCheck.checked
                                                           || visJpgCheck.checked)
    }
    PopupCheckBox {
        id: visJpgCheck
        text: qsTr("Image Visible JPEG") + _translator.emptyString
        onClicked: imageVidColumn.setImageVid()
        onCheckedChanged: quickMenuPanel.captureEnabled = (radJpgCheck.checked
                                                           || visJpgCheck.checked)
    }
    RowLayout {
        Text {
            text: qsTr("Save Images to: ") + _translator.emptyString
            font.pixelSize: 12
            color: "white"
            font.bold: true
        }
        CustomComboBox {
            id: selectedStorage
            onActivated: {
                _controllerCore.changeStorage(selectedStorage.currentText)
            }
            Connections {
                target: _controllerCore
                function onSelectedStorageChanged(storage) {
                    if (typeof selectedStorage.model !== 'undefined')
                        selectedStorage.currentIndex = selectedStorage.model.indexOf(
                                    storage)
                }
            }
        }
    }
    Connections {
        target: _controllerCore
        function onDriveStateChanged(ssd, ssdCap, ssdFree, sd, sdCap, sdFree, flash, flashCap, flashFree) {
            var gig = 1024 * 1024 * 1024
            var arr = []
            if (ssd === "READY") {
                arr.push("SSD")
                rightPanel.refreshSSDState("SSD Free: " + Math.round(
                                               ssdFree) + "%")
            }
            if (sd === "READY") {
                arr.push("SD_CARD")
            }
            if (flash === "READY") {
                arr.push("FLASH_DRIVE")
            }
            selectedStorage.model = arr
            selectedStorage.update()
            selectedStorage.currentIndex = selectedStorage.model.indexOf(
                        _controllerCore.getStorage())
            //selectedStorage.customBackground = _controllerCore.changeStorage(selectedStorage.currentText) ? "white" : "red";
        }
    }
    Connections {
        target: _controllerCore
        function onCapRecSettingChanged(radJpg, visJpg, encIRVid, visVid) {
            imageVidColumn.changedProgramatically = true
            radJpgCheck.checked = radJpg
            visJpgCheck.checked = visJpg
            imageVidColumn.changedProgramatically = false
            quickMenuPanel.captureEnabled = (radJpgCheck.checked
                                             || visJpgCheck.checked)
            quickMenuPanel.recordingEnabled = (radVidCheck.checked
                                               || visVidCheck.checked)
        }
    }
}
