#ifndef CAMERAPARAMETERS_H
#define CAMERAPARAMETERS_H

#include <QString>
#include <QList>

#include <src/graphic/palettewrapper.h>
#include <src/graphic/palettevaluewrapper.h>

struct Extreme;

struct CameraParameters
{
    // basic
    QString _cameraType{};
    QString _firmwareVersion{};
    QString _serialNumber{};
    QString _articleNumber{};
    QString _thermalUnit{};
    QString _thermalUnitSign{};
    // device
    double _cpuTemp = 0;
    double _camTemp = 0;
    QString _systemDate{};
    QString _systemTime{};
    bool _cooledDown{};
    QString _cooldownTime{};
    // palettes
    QList<PaletteWrapper *> _palettes;
    QList<PaletteValueWrapper *> _paletteValues;
    QString _currentPalette{};
    double _paletteBottom{};
    double _paletteTop{};
    // zooms
    QVector<QString> _zoomsThermal;
    QVector<QString> _zoomsVisible;
    QTypedArrayData<QString>::const_iterator _currentZoomThermal;
    QTypedArrayData<QString>::const_iterator _currentZoomVisible;
    // thermal params
    double _emissivity = 0.95;
    double _atmosphericTemp = 20;
    double _reflectedTemp = 20;
    // modes
    int _temperatureMode{};
    int _rangeMode;
    bool _differentialMode{};
    bool _highSensitivityMode{};
    // shutter
    bool _syncShutter;
    int _shutterPeriodMin;
    // capture and record settings
    bool _radJpg{};
    bool _radTiff{};
    bool _srImg{};
    bool _visJpg{};
    bool _radVid{};
    bool _visVid{};
    QString _ssdDrive, _sdDrive, _flashDrive, _currentStorage = "SSD";
    QString _saveDest{};
    // measure
    QString _spanCenter;
    QString _spanWindow;
    QString _manualRange1;
    QString _manualRange2;
    QString _enviroment1;
    QString _enviroment2;
    QString _enviroment3;
    QString _enviroment4;
    QString _currentEnviroment;
    // alarms
    int     _alarmMode{};
    double  _alarmFrom{};
    double  _alarmTo{};
    // alarm colors
    QString _alarmAboveColor{};
    QString _alarmBelowColor{};
    QString _alarmBetweenColor{};
    // streaming
    QString _mainCamera = "THERMAL";
    std::shared_ptr<Extreme> _maximum = std::make_shared<Extreme>();
    std::shared_ptr<Extreme> _minimum = std::make_shared<Extreme>();
    std::shared_ptr<Extreme> _center = std::make_shared<Extreme>();
    bool _paintVals{};
    bool _paintCenter{};
    bool _interpolation{};
    // logging
    bool _sbus_logging{};
    bool _mavlink_logging{};

    CameraParameters() = default;
    CameraParameters(const CameraParameters & other) = delete;
    CameraParameters & operator=(const CameraParameters & other) = delete;
};

struct Extreme{
    int x = 0;
    int y = 0;
    double val = 0;
    bool show = false;
};

#endif // CAMERAPARAMETERS_H
