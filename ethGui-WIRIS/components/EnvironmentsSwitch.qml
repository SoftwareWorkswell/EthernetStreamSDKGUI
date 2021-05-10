import QtQuick 2.0
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

ColumnLayout {
    Item{
        id: gisEnvs
        property int env1LowerC: -40
        property int env1UpperC: -10
        property int env1LowerF: -40
        property int env1UpperF: 14
        property int env1LowerK: 233
        property int env1UpperK: 263

        property int env2LowerC: -10
        property int env2UpperC: 30
        property int env2LowerF: 14
        property int env2UpperF: 86
        property int env2LowerK: 263
        property int env2UpperK: 303

        property int env3LowerC: 10
        property int env3UpperC: 60
        property int env3LowerF: 50
        property int env3UpperF: 140
        property int env3LowerK: 283
        property int env3UpperK: 333

        property int env4LowerC: 30
        property int env4UpperC: 350
        property int env4LowerF: 86
        property int env4UpperF: 662
        property int env4LowerK: 303
        property int env4UpperK: 623
    }

    function thresholds()
    {
        var res = []
        if(range1.checked)
        {
            res.push(range1.lowerThreshold)
            res.push(range1.upperThreshold)
        }
        if(range2.checked)
        {
            res.push(range2.lowerThreshold)
            res.push(range2.upperThreshold)
        }
        if(range3.checked)
        {
            res.push(range3.lowerThreshold)
            res.push(range3.upperThreshold)
        }
        if(range4.checked)
        {
            res.push(range4.lowerThreshold)
            res.push(range4.upperThreshold)
        }
        return res;
    }
    // F: -40.000000 14.000000\r\n14.000000 86.000000\r\n50.000000 140.000000\r\n86.000000 662.000000\r\r\n\u0000"
    // K:  233.150000 263.150000\r\n263.150000 303.150000\r\n283.150000 333.150000\r\n303.150000 623.150000\r\r\n\u0000"
    RowLayout
    {
        CustomRadioButton1{
            id: range1
            checked: _controllerCore.isCurrentEnviroment(range1.envValue)
            property int upperThreshold: 0
            property int lowerThreshold: 0
            property string envValue

            Connections{
                target: _controllerCore
                function onEnviroment1Changed()
                {
                    range1.envValue = _controllerCore.enviroment1
                    if(_controllerCore.type == mainWindow.gisType)
                        range1.text =  _controllerCore.thermalUnit == "CELSIUS" ? "-40°C ... -10°C" :  _controllerCore.thermalUnit == "KELVIN" ? "233°K ... 263°K" : "-40°F ... 14°F"
                    else
                        range1.text = "-25°C ... 150°C"
                    range1.upperThreshold = (function(){
                       if(_controllerCore.type == mainWindow.gisType)
                           return _controllerCore.thermalUnit == "CELSIUS" ? gisEnvs.env1UpperC : _controllerCore.thermalUnit == "KELVIN" ? gisEnvs.env1UpperK  : gisEnvs.env1UpperF
                       return 150
                    })()
                    range1.lowerThreshold = (function(){
                        if(_controllerCore.type == mainWindow.gisType)
                            return _controllerCore.thermalUnit == "CELSIUS" ? gisEnvs.env1LowerC : _controllerCore.thermalUnit == "KELVIN" ? gisEnvs.env1LowerK  : gisEnvs.env1LowerF
                        return -25
                    })()
                    mainWindow.setSliderRanges()
                }
            }
            onClicked: _controllerCore.setEnviroment1(range1.envValue);
        }

        CustomRadioButton1{
            id: range2
            checked: _controllerCore.isCurrentEnviroment(range2.envValue)
            property int upperThreshold: 0
            property int lowerThreshold:  {
                if(_controllerCore.type == mainWindow.gisType)
                    return _controllerCore.thermalUnit == "CELSIUS" ? gisEnvs.env2LowerC : _controllerCore.thermalUnit == "KELVIN" ? gisEnvs.env2LowerK  : gisEnvs.env2LowerF
                return -40
            }
            property string envValue

            onClicked: _controllerCore.setEnviroment2(range2.envValue);

            Connections{
                target: _controllerCore
                function onEnviroment2Changed()
                {
                    range2.envValue = _controllerCore.enviroment2
                    if(_controllerCore.type == mainWindow.gisType)
                        range2.text =  _controllerCore.thermalUnit == "CELSIUS" ? "-10°C ... 30°C" :  _controllerCore.thermalUnit == "KELVIN" ? "263°K ... 303°K" : "14°F ... 86°F"
                    else
                        range2.text = "-40°C ... 550°C"
                    range2.upperThreshold = (function() {
                        if(_controllerCore.type == mainWindow.gisType)
                            return _controllerCore.thermalUnit == "CELSIUS" ? gisEnvs.env2UpperC : _controllerCore.thermalUnit == "KELVIN" ? gisEnvs.env2UpperK  : gisEnvs.env2UpperF
                        return 550
                    })()
                    range2.lowerThreshold = (function() {
                        if(_controllerCore.type == mainWindow.gisType)
                            return _controllerCore.thermalUnit == "CELSIUS" ? gisEnvs.env2LowerC : _controllerCore.thermalUnit == "KELVIN" ? gisEnvs.env2LowerK  : gisEnvs.env2LowerF
                        return -40
                    })()
                    mainWindow.setSliderRanges()
                }
            }
        }
    }
    RowLayout
    {
        CustomRadioButton1{
            id: range3
            checked: _controllerCore.isCurrentEnviroment(range3.envValue)
            property int upperThreshold: 0
            property int lowerThreshold: 0
            property string envValue
            Connections{
                target: _controllerCore
                function onEnviroment3Changed()
                {
                    range3.envValue = _controllerCore.enviroment3
                    if(_controllerCore.type == mainWindow.gisType)
                        range3.text =  _controllerCore.thermalUnit == "CELSIUS" ? "10°C ... 60°C"  :  _controllerCore.thermalUnit == "KELVIN" ? "283°K ... 333°K" : "50°F ... 140°F"
                    else
                        range3.text ="100°C ... 1000°C"
                    range3.upperThreshold =( function() {
                        if(_controllerCore.type == mainWindow.gisType)
                            return _controllerCore.thermalUnit == "CELSIUS" ? gisEnvs.env3UpperC : _controllerCore.thermalUnit == "KELVIN" ? gisEnvs.env3UpperK  : gisEnvs.env3UpperF
                        return 1000
                    })()
                    range3.lowerThreshold =( function() {
                        if(_controllerCore.type == mainWindow.gisType)
                            return _controllerCore.thermalUnit == "CELSIUS" ? gisEnvs.env3LowerC : _controllerCore.thermalUnit == "KELVIN" ? gisEnvs.env3LowerK  : gisEnvs.env3LowerF
                        return 100
                    })()
                    mainWindow.setSliderRanges()
                }
            }
            onClicked: _controllerCore.setEnviroment3(range3.envValue);
        }

        CustomRadioButton1{
            id: range4
            checked: _controllerCore.isCurrentEnviroment(range4.envValue)
            visible: _controllerCore.type != mainWindow.securityType
            property int upperThreshold: 0
            property int lowerThreshold: 0
            property string envValue

            onClicked: _controllerCore.setEnviroment4(range4.envValue);

            Connections{
                target: _controllerCore
                function onEnviroment4Changed()
                {
                    range4.envValue = _controllerCore.enviroment4
                    if(_controllerCore.type == mainWindow.gisType)
                        range4.text =  _controllerCore.thermalUnit == "CELSIUS" ? "30°C ... 350°C" :  _controllerCore.thermalUnit == "KELVIN" ? "303°K ... 623°K" : "86°F ... 662°F"
                    else
                        range4.text = "400°C ... 1500°C"
                    range4.upperThreshold = (function() {
                        if(_controllerCore.type == mainWindow.gisType)
                            return _controllerCore.thermalUnit == "CELSIUS" ? gisEnvs.env4UpperC : _controllerCore.thermalUnit == "KELVIN" ? gisEnvs.env4UpperK  : gisEnvs.env4UpperF
                        return 1500
                    })()
                    range4.lowerThreshold = (function()  {
                        if(_controllerCore.type == mainWindow.gisType)
                            return _controllerCore.thermalUnit == "CELSIUS" ? gisEnvs.env4LowerC : _controllerCore.thermalUnit == "KELVIN" ? gisEnvs.env4LowerK  : gisEnvs.env4LowerF
                        return 400
                    })()
                    setSliderRanges()
                }
            }
        }
    }

    Connections{
        target: _controllerCore
        function onCurrentEnviromentChanged() {
            range1.checked = _controllerCore.isCurrentEnviroment(range1.envValue);
            range2.checked = _controllerCore.isCurrentEnviroment(range2.envValue);
            range3.checked = _controllerCore.isCurrentEnviroment(range3.envValue);
            range4.checked = _controllerCore.isCurrentEnviroment(range4.envValue);

            if (range1.checked)
            {
                range1.upperThreshold = _controllerCore.enviromentUpperThreshold();
                range1.lowerThreshold = _controllerCore.enviromentLowerThreshold();
            }
            else if (range2.checked)
            {
                range2.upperThreshold = _controllerCore.enviromentUpperThreshold();
                range2.lowerThreshold = _controllerCore.enviromentLowerThreshold();
            }
            else if (range3.checked)
            {
                range3.upperThreshold = _controllerCore.enviromentUpperThreshold();
                range3.lowerThreshold = _controllerCore.enviromentLowerThreshold();
            }
            else if (range4.checked)
            {
                range4.upperThreshold = _controllerCore.enviromentUpperThreshold();
                range4.lowerThreshold = _controllerCore.enviromentLowerThreshold();
            }
            mainWindow.setSliderRanges()
        }
    }
}
