import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "../"

ControlPopup{
    ColumnLayout{
        width: parent.width
        height: parent.height

        PopupItem{

            width: parent.width
            itemText: "Palettes"
            Layout.preferredHeight: 25
        }


        Component{
            id: paletteDelegate
            PaletteButton {
                id: delegateButton
                text: value
                highlighted: _controllerCore.currentPalette == value
                onClicked: _controllerCore.setCurrentPaletteByIndex(index)
            }
        }

        ListView {
            id: palettesListView
            Layout.fillHeight: true
            Layout.fillWidth: true

            model: _controllerCore.palettes
            delegate: paletteDelegate
            Connections{
                target: _controllerCore
                function onPalettesChanged() {
                    palettesListView.model = _controllerCore.palettes;
                }
            }
        }
    }
}
