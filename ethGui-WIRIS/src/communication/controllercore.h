
#ifndef CONTROLLERCORE2_H
#define CONTROLLERCORE2_H

#include <QString>
#include <QMutexLocker>
#include <QMutex>

#include <memory>

#include <src/communication/networkclient.h>
#include <src/communication/protocol.h>
#include <src/camera/cameraconnection.h>
#include <src/camera/cameraparameters.h>
#include <src/camera/cameraconstants.h>

extern std::shared_ptr<QMutex> thermalMutex;
extern std::shared_ptr<QMutex> connectionMutex;

class ControllerCore
{
public:
    ControllerCore(std::shared_ptr<CameraConnection> connection, std::shared_ptr<CameraParameters> params, std::shared_ptr<NetworkClient> networkClient);
    void disconnect();

    bool send(const QString &message);
    bool send(const QString &message, std::istringstream & answ, bool debugPrint = true);

    bool fetchAll();
    void fetchBasicInfo();
    void fetchEnviroments();
    void fetchRangeModes();
    void fetchManualRange();
    void fetchSpanRange();
    void fetchZooms();
    void fetchCurrentZooms();
    void fetchPalettes();
    void fetchPaletteValues();
    void fetchThermalParams();
    void fetchShutter();
    void fetchCaptureRecSettings();
    void fetchAlarms();
    void fetchTempModes();

    void setRangeMode(int value);
    void setManualRange1(const QString& val);
    void setManualRange2(const QString& val);
    void setManualRanges(const QString& val1, const QString val2);
    void setEnviroment1(const QString & env);
    void setEnviroment2(const QString & env);
    void setEnviroment3(const QString & env);
    void setEnviroment4(const QString & env);
    void setSpanCenter(const QString& val);
    void setSpanWindow(const QString& val);
    void setAlarmMode(int val);
    void setAlarmTo(double val);
    void setAlarmFrom(double val);
    void setEmissivity(double val);
    void setAtmosphericTemp(double val);
    void setReflectedTemp(double val);
    void setSyncShutter(bool val);
    void setShutterPeriod(int val);
    void setCaptureVideoSettings(bool radJpg, bool radTiff, bool srImg, bool visJpg, bool radVid, bool visVid);
    void setAlarmColors(const QString& above, const QString& between, const QString& below);
    void setCurrentPaletteByIndex(int i);
    void setInterpolation(bool val);
    void setSBUSLogging(bool val);
    void setMavlinkLogging(bool val);
    void setThermalUnit(const QString & unit);

    bool changeStorage(const QString & storage);
    void changeZoomVisible(bool direction);
    void changeZoomThermal(bool direction);
    void toggleDifferentialMode(bool val);
    void toggleHighSensitivityMode(bool val);
    void autoFocus();
    void infiniteFocus();

    QString enviromentLowerThreshold() const;
    QString enviromentUpperThreshold() const;
private:
    Protocol _protocol;
    const CameraConstants _vals;

    std::shared_ptr<NetworkClient>      _networkClient = nullptr;
    std::shared_ptr<CameraConnection>   _connection = nullptr;
    std::shared_ptr<CameraParameters>   _params = nullptr;

    bool changeEnviroment(const QString & env);
};

#endif // CONTROLLERCORE_H
