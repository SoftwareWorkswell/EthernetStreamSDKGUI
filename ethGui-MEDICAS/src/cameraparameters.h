#ifndef CAMERAPARAMETERS_H
#define CAMERAPARAMETERS_H

#include <QString>
#include <QList>

#include <src/cameraconstants.h>
#include <src/customtools.h>
#include "usersettings.h"

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
    bool   _heatingUp = 0;
    // range, init to limit values
    QString _manualRange1 = QString::number(_cameraConstants._minRangeTemp);
    QString _manualRange2 = QString::number(_cameraConstants._maxRangeTemp);
    // functions
    QString _mainCamera = "THERMAL";
    int _blackbodyRoiX{};
    int _blackbodyRoiY{};
    int _blackbodyMinX = 26;
    int _blackbodyMaxX = 613;
    int _blackbodyMinY = 21;
    int _blackbodyMaxY = 490;
    int _crossUserX{};
    int _crossUserY{};
    int _userRoiX [4];
    int _userRoiY [4];
    int _userRoiW [4];
    int _userRoiH [4];
    QString _blackbodyStatus{};
    int _blackbodyDetection{};
    int _blackbodyMaskSize{};
    int _blackbodySize{};
    int _temperatureMode{};
    // palettes
    QString _currentPalette{};
    double _paletteBottom{};
    double _paletteTop{};
    // measure
    int _alarmMode{};
    double _alarmPreventive = 36.8;
    double _alarmCritical = 37.0;
    bool _showCrossMax{};
    bool _showCrossUser{};
    bool _showBlackbodyRoi{};
    // off / border / corners
    int _showUserRoi1{};
    int _showUserRoi2{};
    int _showUserRoi3{};
    int _showUserRoi4{};
    bool _showUserRoiNames{};
    Extreme* _maximum = new Extreme();
    Extreme* _userCross = new Extreme();
    int _userCrossMaxX = 613;
    int _userCrossMinX = 26;
    int _userCrossMaxY = 490;
    int _userCrossMinY = 21;
    Extreme* _blackbody = new Extreme();
    HeadRoi* _head = new HeadRoi();
    UserRoiContainer * _userRoiContainer = new UserRoiContainer();
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
    QString _units = " °C";
    QString _language{};

    // dio
    bool _dioConnected = false;
    QString _dioTriggerMode;
    bool _dioPictureOnTrigger = false;
    QString _dioMinimumPulseWidth;
    QString _dioSignal;
    QString _dioTriggerDelay;
    QString _dioMinDetectionTime;
    QString _dioTempHysteresis;
    QString _dioMinAlarmTime;
    QString _dioLogic;
    QString _dioAlarm;
    QString _dioGateOpenTime;
    bool _dioShowStatus = false;

    // user
    QString _currentUser;
    UserSettings _currentUserSettings;

    CameraParameters() = default;
    ~CameraParameters(){
        delete _userRoiContainer;
        delete _maximum;
        delete _userCross;
        delete _blackbody;
    }
    CameraParameters(const CameraParameters & other) = delete;
    CameraParameters & operator=(const CameraParameters & other) = delete;

};

#endif // CAMERAPARAMETERS_H
