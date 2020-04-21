#ifndef HELPERTHREAD_H
#define HELPERTHREAD_H

#include <qobject.h>
#include <qthread.h>
#include <QString>
#include <qdebug.h>
#include <qimage.h>
#include <QMutex>

#include <chrono>
#include <thread>
#include <memory>
#include "customtimer.h"

#include "customtools.h"
#include "streamthread.h"
#include "networkclient.h"

#include "protocol.h"

class HelperThread: public QThread
{
    Q_OBJECT

public:
    HelperThread(bool* runFlag, bool* extremes, std::function<bool(const QString &, std::istringstream&)> sendFun, Extreme* max, Extreme* center, Extreme* blackbody, const QString* fw, const QString* type)
        :   _runFlag(runFlag), _sendFun(sendFun), _extremes(extremes), _maximum(max), _center(center), _blackbody(blackbody), _fwVersion(fw), _type(type)
    {}

signals:
    void extremesChanged();
    void tempsChanged(double cpu, double cam);
    void captureRecordingChanged(bool capturing, bool recording);
    void driveStateChanged(const QString& ssd, const QString& ssdCap, const QString& ssdFree,
                           const QString& sd, const QString& sdCap, const QString& sdFree,
                           const QString& flash, const QString& flashCap, const QString& flashFree);
    void dateTimeChanged(const QString& date, const QString& time);

    void connectionStateChanged(bool connected, double ping);
    void shutterDataChanged(const QString& nextShutter, const QString& lastShutter);

protected:
    virtual void run() override;
private:
    void loadExtremes(std::istringstream& max_iss, std::istringstream& center_iss,  std::istringstream& blackbody_iss);
    void loadCaptureRecordState();
    void loadMemoryState();
    void loadDateTime();
    void loadTemps();
    void loadShutterData();

    CustomTimer _ctmr;
    std::function<bool(const QString &, std::istringstream&)> _sendFun;
    Protocol _protocol;
    bool* _extremes, _recording, _capturing;
    bool* _runFlag;

    bool _connectionState = true;

    Extreme* _maximum;
    Extreme* _center;
    Extreme* _blackbody;
    QString _ssdDrive, _sdDrive, _flashDrive;
    QString _ssdDriveCap, _flashDriveCap, _sdDriveCap;
    QString _ssdDriveFree, _flashDriveFree, _sdDriveFree;
    QString _gps;
    const QString* _fwVersion;
    const QString* _type;
    const Values _vals;


};


#endif // HELPERTHREAD_H
