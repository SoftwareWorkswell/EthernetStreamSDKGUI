#ifndef HELPERTHREAD_H
#define HELPERTHREAD_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QImage>
#include <QDebug>
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
    HelperThread(
        bool *runFlag, bool *extremes,
        std::function<bool(const QString &, std::istringstream &)> sendFun,
        Extreme *max, Extreme *user_cross, Extreme *blackbody, HeadRoi *head,
        UserRoiContainer *userRois, const QString *fw, const QString *type,
        bool *dioConnected)
        : _runFlag(runFlag), _extremes(extremes), _sendFun(sendFun),
          _maximum(max), _userCross(user_cross), _blackbody(blackbody),
          _head(head), _dioConnected(dioConnected), _userRois(userRois),
          _fwVersion(fw), _type(type) {
    }

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
    void dioConnectedChanged();

protected:
    virtual void run() override;
private:
    void loadExtremes(std::istringstream& max_iss, std::istringstream& user_iss, std::istringstream& blackbody_iss);
    void loadCaptureRecordState();
    void loadMemoryState();
    void loadDateTime();
    void loadTemps();
    void loadShutterData();
    void loadUserRois(std::istringstream& iss);
    void loadHead(std::istringstream& head_iss);

    CustomTimer _ctmr;
    Protocol _protocol;
    bool *_runFlag{};
    bool *_extremes{}, _recording{}, _capturing{};
    std::function<bool(const QString &, std::istringstream&)> _sendFun;

    bool _connectionState = true;

    Extreme *_maximum{};
    Extreme *_userCross{};
    Extreme *_blackbody{};
    HeadRoi *_head{};
    bool *_dioConnected{};
    UserRoiContainer *_userRois{};
    QString _ssdDrive{}, _sdDrive{}, _flashDrive{};
    QString _ssdDriveCap{}, _flashDriveCap{}, _sdDriveCap{};
    QString _ssdDriveFree{}, _flashDriveFree{}, _sdDriveFree{};
    QString _gps{};
    const QString* _fwVersion;
    const QString* _type;
    const Values _vals;


};


#endif // HELPERTHREAD_H
