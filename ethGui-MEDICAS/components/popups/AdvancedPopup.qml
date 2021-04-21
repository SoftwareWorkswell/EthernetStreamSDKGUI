import QtQuick 2.0
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "../"
import "../advanced_dropdowns"

ControlPopup {
    ScrollView {
        anchors.fill: parent
        clip: true
        contentHeight: helpDrop.y + helpDrop.height + 30
        contentWidth: width
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.width: 10
        contentChildren: [
            PopupDropDown {
                implicitHeight: 30
                anchors.top: parent.top
                anchors.margins: 5
                id: userDropDown
                dropdownText: qsTr("User") + _translator.emptyString
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            UserDropDown {
                implicitHeight: height
                id: userDrop
                anchors.top: userDropDown.bottom
                visible: userDropDown.activated
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            Rectangle {
                id: userRec
                anchors.top: userDropDown.activated ? userDrop.bottom : userDropDown.bottom
                implicitHeight: 1
                color: "black"
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            PopupDropDown {
                implicitHeight: 30
                anchors.top: userRec.bottom
                id: visibleDropDown
                dropdownText: qsTr("Visible Camera") + _translator.emptyString
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            VisibleDropDown {
                id: visibleDrop
                implicitHeight: height
                anchors.top: visibleDropDown.bottom
                visible: visibleDropDown.activated
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            Rectangle {
                id: visibleRec
                anchors.top: visibleDropDown.activated ? visibleDrop.bottom : visibleDropDown.bottom
                implicitHeight: 1
                color: "black"
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            PopupDropDown {
                implicitHeight: 30
                id: imageVidDropDown
                anchors.top: visibleRec.bottom
                dropdownText: qsTr("Images & Video") + _translator.emptyString
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            ImgVidDropDown {
                id: imageDrop
                implicitHeight: height
                anchors.top: imageVidDropDown.bottom
                visible: imageVidDropDown.activated
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            Rectangle {
                id: imageRec
                anchors.top: imageVidDropDown.activated ? imageDrop.bottom : imageVidDropDown.bottom
                implicitHeight: 1
                color: "black"
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            PopupDropDown {
                implicitHeight: 30
                id: alarmsDropDown
                anchors.top: imageRec.bottom
                dropdownText: qsTr("Alarms") + _translator.emptyString
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            AlarmsDropDown {
                id: alarmsDrop
                implicitHeight: height
                anchors.top: alarmsDropDown.bottom
                visible: alarmsDropDown.activated
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            Rectangle {
                id: alarmsRec
                anchors.top: alarmsDropDown.activated ? alarmsDrop.bottom : alarmsDropDown.bottom
                implicitHeight: 1
                color: "black"
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },

            PopupDropDown {
                implicitHeight: 30
                id: roiDropDown
                anchors.top: alarmsRec.bottom
                dropdownText: qsTr("ROI") + _translator.emptyString
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            ROIDropDown {
                id: roiDrop
                implicitHeight: height
                anchors.top: roiDropDown.bottom
                visible: roiDropDown.activated
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            Rectangle {
                id: roiRec
                anchors.top: roiDropDown.activated ? roiDrop.bottom : roiDropDown.bottom
                implicitHeight: 1
                color: "black"
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },

            PopupDropDown {
                implicitHeight: 30
                id: dioDropDown
                anchors.top: roiRec.bottom
                dropdownText: qsTr("Digital I/O Unit") + _translator.emptyString
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            DIODropDown {
                id: dioDrop
                implicitHeight: height
                anchors.top: dioDropDown.bottom
                visible: dioDropDown.activated
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            Rectangle {
                id: dioRec
                anchors.top: dioDropDown.activated ? dioDrop.bottom : dioDropDown.bottom
                implicitHeight: 1
                color: "black"
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },

            PopupDropDown {
                implicitHeight: 30
                id: memoryDropDown
                anchors.top: dioRec.bottom
                dropdownText: qsTr("Memory") + _translator.emptyString
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            MemoryDropDown {
                id: memoryDrop
                implicitHeight: height
                anchors.top: memoryDropDown.bottom
                visible: memoryDropDown.activated
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            Rectangle {
                id: memoryRec
                anchors.top: memoryDropDown.activated ? memoryDrop.bottom : memoryDropDown.bottom
                implicitHeight: 1
                color: "black"
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },

            PopupDropDown {
                implicitHeight: 30
                id: systemDropDown
                anchors.top: memoryRec.bottom
                dropdownText: qsTr("System") + _translator.emptyString
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            SystemDropDown {
                id: systemDrop
                implicitHeight: height
                anchors.top: systemDropDown.bottom
                visible: systemDropDown.activated
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            Rectangle {
                id: systemRec
                anchors.top: systemDropDown.activated ? systemDrop.bottom : systemDropDown.bottom
                implicitHeight: 1
                color: "black"
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },

            PopupDropDown {
                implicitHeight: 30
                id: infoDropdown
                anchors.top: systemRec.bottom
                dropdownText: qsTr("Info") + _translator.emptyString
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            InfoDropDown {
                id: infoDrop
                implicitHeight: height
                anchors.top: infoDropdown.bottom
                visible: infoDropdown.activated
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            Rectangle {
                id: infoRec
                anchors.top: infoDropdown.activated ? infoDrop.bottom : infoDropdown.bottom
                implicitHeight: 1
                color: "black"
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            PopupDropDown {
                implicitHeight: 30
                id: helpDropdown
                anchors.top: infoRec.bottom
                dropdownText: qsTr("Help") + _translator.emptyString
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
            },
            ColumnLayout {
                id: helpDrop
                implicitHeight: height
                anchors.top: helpDropdown.bottom
                visible: helpDropdown.activated
                anchors.margins: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10

                RowLayout {
                    id: helpRow
                    Layout.leftMargin: 5
                    Layout.rightMargin: 5
                    Text {
                        Layout.leftMargin: 5
                        Layout.rightMargin: 5
                        color: "white"
                        font.pixelSize: 12
                        Layout.fillWidth: true
                        text: qsTr("Medicas SDK Manual:") + _translator.emptyString
                    }
                    PopupButton {
                        Layout.preferredHeight: 20
                        Layout.preferredWidth: 60
                        text: qsTr("Open") + _translator.emptyString
                        onClicked: {
                            _controllerCore.openManual()
                        }
                    }
                }
            }
        ]
    }
}
