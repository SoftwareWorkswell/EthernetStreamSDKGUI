import QtQuick 2.0
import QtQuick.Layouts 1.3
import "../"
import "./user_settings_dialogs"

ColumnLayout{
    spacing: 10
    RowLayout {
        id: userCrossRow
        Layout.preferredHeight: 35
        Layout.alignment: Qt.AlignHCenter
        PopupButton{
            id: showUserCrossButton
            Layout.preferredWidth: 150
            //Layout.preferredHeight: userCrossRow.height * 0.85
            Layout.alignment: Qt.AlignCenter | Qt.AlignHCenter
            text: qsTr("Set User Cross Position") + _translator.emptyString
            onClicked:{
                _controllerCore.showCrossUser = true
                mainviewParent.hideAllQuickRects()
                mainviewParent.setUserCrossRectVisible(true)
                controlPanel.hideAllPopups()
            }
        }
    }
    RowLayout {
        Layout.preferredHeight: userCrossRow.height
        Layout.preferredWidth: userCrossRow.width
        Layout.alignment: Qt.AlignHCenter
        PopupButton{
            Layout.preferredWidth: showUserCrossButton.width
            Layout.preferredHeight: showUserCrossButton.height
            Layout.alignment: Qt.AlignCenter | Qt.AlignHCenter
            text: qsTr("Set User ROI1 Position") + _translator.emptyString
            onClicked:{
                _controllerCore.setCurrentRoiIdx(0)
                _controllerCore.showUserRoi1 = 1
                mainviewParent.hideAllQuickRects()
                mainviewParent.setUserROIRectVisible(true)
                mainviewParent.initUserROIRect(0)
                controlPanel.hideAllPopups()
            }
        }
    }
    RowLayout {
        Layout.preferredHeight: userCrossRow.height
        Layout.preferredWidth: userCrossRow.width
        Layout.alignment: Qt.AlignHCenter
        PopupButton{
            Layout.preferredWidth: showUserCrossButton.width
            Layout.preferredHeight: showUserCrossButton.height
            Layout.alignment: Qt.AlignCenter | Qt.AlignHCenter
            text: qsTr("Set User ROI2 Position") + _translator.emptyString
            onClicked:{
               _controllerCore.setCurrentRoiIdx(1)
                _controllerCore.showUserRoi2 = 1
                mainviewParent.hideAllQuickRects()
                mainviewParent.setUserROIRectVisible(true)
                mainviewParent.initUserROIRect(1)
                controlPanel.hideAllPopups()
            }
        }
    }
    RowLayout {
        Layout.preferredHeight: userCrossRow.height
        Layout.preferredWidth: userCrossRow.width
        Layout.alignment: Qt.AlignHCenter
        PopupButton{
            Layout.preferredWidth: showUserCrossButton.width
            Layout.preferredHeight: showUserCrossButton.height
            Layout.alignment: Qt.AlignCenter | Qt.AlignHCenter
            text: qsTr("Set User ROI3 Position") + _translator.emptyString
            onClicked:{
                _controllerCore.setCurrentRoiIdx(2)
                _controllerCore.showUserRoi3 = 1
                mainviewParent.hideAllQuickRects()
                mainviewParent.setUserROIRectVisible(true)
                mainviewParent.initUserROIRect(2)
                controlPanel.hideAllPopups()
            }
        }
    }
    RowLayout {
        Layout.preferredHeight: userCrossRow.height
        Layout.preferredWidth: userCrossRow.width
        Layout.alignment: Qt.AlignHCenter
        PopupButton{
            Layout.preferredWidth: showUserCrossButton.width
            Layout.preferredHeight: showUserCrossButton.height
            Layout.alignment: Qt.AlignCenter | Qt.AlignHCenter
            text: qsTr("Set User ROI4 Position") + _translator.emptyString
            onClicked:{
                _controllerCore.setCurrentRoiIdx(3)
                _controllerCore.showUserRoi4 = 1
                mainviewParent.hideAllQuickRects()
                mainviewParent.setUserROIRectVisible(true)
                mainviewParent.initUserROIRect(3)
                controlPanel.hideAllPopups()
            }
        }
    }
    RowLayout {
        Layout.preferredHeight: userCrossRow.height
        Layout.preferredWidth: userCrossRow.width
        Layout.alignment: Qt.AlignHCenter
        PopupButton{
            Layout.preferredWidth: showUserCrossButton.width
            Layout.preferredHeight: showUserCrossButton.height
            Layout.alignment: Qt.AlignCenter | Qt.AlignHCenter
            text: qsTr("Set User ROI Names") + _translator.emptyString
            onClicked:{
                onClicked: roiNamesDialog.open()
            }
        }
    }
    PopupCheckBox{
        text: qsTr("Show User Roi Names") + _translator.emptyString
        checked: false
        visible: true
        enabled: true
        id: showRoiNamesCheck
        onClicked: {
            _controllerCore.showRoiUserNames = checked
        }
        Connections{
            target: _controllerCore
            function onShowRoiUserNamesChanged(val){
                showRoiNamesCheck.checked = val
            }
        }
    }
    SetRoiNamesDialog{
        id: roiNamesDialog
    }
}
