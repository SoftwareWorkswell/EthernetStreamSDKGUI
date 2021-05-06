#ifndef HELPERTHREAD_H
#define HELPERTHREAD_H

#include <QString>
#include <QImage>
#include <QMutex>

#include <chrono>
#include <thread>
#include <memory>

#include <src/graphic/customtimer.h>
#include <src/camera/cameraparameters.h>
#include <src/camera/cameraconstants.h>
#include <src/stream/streamthread.h>
#include <src/communication/protocol.h>
#include <src/communication/networkclient.h>

class HelperThread: public QThread
{
    Q_OBJECT
public:
    HelperThread(bool* runFlag, bool *cooledDown, bool* extremes, std::function<bool(const QString &, std::istringstream&)> sendFun, std::shared_ptr<Extreme> max, std::shared_ptr<Extreme> min, std::shared_ptr<Extreme> center, const QString* fw, const QString* type)
        :   _sendFun(sendFun), _extremes(extremes), _cooledDown(cooledDown), _runFlag(runFlag),_maximum(max), _minimum(min), _center(center), _fwVersion(fw), _type(type)
    {}
signals:
    void extremesChanged();
    void tempsChanged(double cpu, double cam);
    void captureRecordingChanged(bool capturing, bool recording);
    void driveStateChanged(const QString& ssd, const QString& ssdCap, const QString& ssdFree,
                           const QString& sd, const QString& sdCap, const QString& sdFree,
                           const QString& flash, const QString& flashCap, const QString& flashFree);
    void cooldownTimeChanged(const QString& time);
    void dateTimeChanged(const QString& date, const QString& time);
    void connectionStateChanged(bool connected, double ping);
    void gpsChanged(const QString& gps);
    void shutterDataChanged(const QString& nextShutter, const QString& lastShutter);
    void gisThermalSwitchOff();
protected:
    virtual void run() override;
private:
    void loadExtremes(std::istringstream& iss);
    void loadCaptureRecordState();
    void loadMemoryState();
    void loadCooldown();
    void loadDateTime();
    void loadTemps();
    void loadGps();
    void loadShutterData();
    void loadGisShutterData();

    CustomTimer _ctmr;
    Protocol _protocol;
    std::function<bool(const QString &, std::istringstream&)> _sendFun;
    bool* _extremes, * _cooledDown;
    bool _recording, _capturing;
    bool* _runFlag;
    bool _connectionState = true;

    std::shared_ptr<Extreme> _maximum;
    std::shared_ptr<Extreme> _minimum;
    std::shared_ptr<Extreme> _center;
    QString _ssdDrive, _sdDrive, _flashDrive;
    QString _ssdDriveCap, _flashDriveCap, _sdDriveCap;
    QString _ssdDriveFree, _flashDriveFree, _sdDriveFree;
    QString _gps;
    const QString* _fwVersion;
    const QString* _type;
    const CameraConstants _vals;
};


#endif // HELPERTHREAD_H
