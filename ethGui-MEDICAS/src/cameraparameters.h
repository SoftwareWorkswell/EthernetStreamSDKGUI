#ifndef CAMERAPARAMETERS_H
#define CAMERAPARAMETERS_H

#include <QString>
#include <QList>

#include <src/cameraconstants.h>
#include <src/customtools.h>


struct CameraParameters
{
    const QString _endpointPort = "2240";
    // const values such as limits per each setting
    CameraConstants _cameraConstants;
    // basic
    QString _endpointIp{};
    QString _cameraType{};
    QString _firmwareVersion{};
    QString _serialNumber{};
    QString _articleNumber{};
    double _cpuTemp = 0;
    double _camTemp = 0;
    double _deviceTemp = 0;
    // range, init to limit values
    QString _manualRange1 = QString::number(_cameraConstants._minRangeTemp);
    QString _manualRange2 = QString::number(_cameraConstants._maxRangeTemp);
    // functions
    QString _mainCamera = "THERMAL";
    double _levelOfAcception{};
    int _blackbodyRoiX{};
    int _blackbodyRoiY{};
    QString _blackbodyStatus{};
    // palettes
    QString _currentPalette{};
    double _paletteBottom{};
    double _paletteTop{};
    // measure
    int _alarmMode{};
    double _alarmPreventive{};
    double _alarmCritical{};
    bool _showCrossMax{};
    bool _showCrossCenter{};
    bool _showBlackbodyRoi{};
    Extreme* _maximum = new Extreme();
    Extreme* _center = new Extreme();
    Extreme* _blackbody = new Extreme();
    // visible camera advanced params
    int _gamma{};
    int _whiteBalance{};
    double _visShutterPeriod{};
    bool _backlightCompensation{};
    bool _dynamicRangeMode{};
    int _exposureMode{};
    int _noiseReduction2D{};
    int _noiseReduction3D{};
    int _gain{};
    int _colorGain{};
    int _exposureCompensation{};
    // alarm graphic settins
    QString _preventiveColor{};
    QString _criticalColor{};
    int _alarmOpacity{};
    int _iris{};
    // system
    bool _isSetup = false;
    QString _units{};
    QString _language{};
};

#endif // CAMERAPARAMETERS_H
