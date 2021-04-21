/*
#ifndef DIOWRAPPER_H
#define DIOWRAPPER_H

#include <QObject>
#include <regex>
#include <string>
#include <memory>
#include <iostream>
#include <istream>
#include <math.h>
#include <functional>

#include "protocol.h"

class DIOWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool dioConnected READ dioConnected NOTIFY dioConnectedChanged)
    Q_PROPERTY(QString dioTriggerMode READ dioTriggerMode WRITE setDioTriggerMode NOTIFY dioTriggerModeChanged)
    Q_PROPERTY(bool dioPictureOnTrigger READ dioPictureOnTrigger WRITE setdioPictureOnTrigger NOTIFY dioPictureOnTriggerChanged)
    Q_PROPERTY(QString dioMinimumPulseWidth READ dioMinimumPulseWidth WRITE setDioMinimumPulseWidth NOTIFY dioMinimumPulseWidthChanged)
    Q_PROPERTY(QString dioSignal READ dioSignal WRITE setDioSignal NOTIFY dioSignalChanged)
    Q_PROPERTY(QString dioTriggerDelay READ dioTriggerDelay WRITE setDioTriggerDelay NOTIFY dioTriggerDelayChanged)
    Q_PROPERTY(QString dioMinDetectionTime READ dioMinDetectionTime WRITE setDioMinDetectionTime NOTIFY dioMinDetectionTimeChanged)
    Q_PROPERTY(QString dioTempHysteresis READ dioTempHysteresis WRITE setDioTempHysteresis NOTIFY dioTempHysteresisChanged)
    Q_PROPERTY(QString dioMinAlarmTime READ dioMinAlarmTime WRITE setDioMinAlarmTime NOTIFY dioMinAlarmTimeChanged)
    Q_PROPERTY(QString dioLogic READ dioLogic WRITE setDioLogic NOTIFY dioLogicChanged)
    Q_PROPERTY(QString dioAlarm READ dioAlarm WRITE setDioAlarm NOTIFY dioAlarmChanged)
    Q_PROPERTY(QString dioGateOpenTime READ dioGateOpenTime WRITE setDioGateOpenTime NOTIFY dioGateOpenTimeChanged)
    Q_PROPERTY(bool dioShowStatus READ dioShowStatus WRITE setDioShowStatus NOTIFY dioShowStatusChanged)
public:
    DIOWrapper();
    DIOWrapper(Protocol *protocol);

signals:
    void dioConnectedChanged();
    void dioTriggerModeChanged();
    void dioPictureOnTriggerChanged();
    void dioMinimumPulseWidthChanged();
    void dioSignalChanged();
    void dioTriggerDelayChanged();
    void dioMinDetectionTimeChanged();
    void dioTempHysteresisChanged();
    void dioMinAlarmTimeChanged();
    void dioLogicChanged();
    void dioAlarmChanged();
    void dioGateOpenTimeChanged();
    void dioShowStatusChanged();

public slots:
    bool dioConnected();
    QString dioTriggerMode();
    void setDioTriggerMode(QString dioTriggerMode);
    bool dioPictureOnTrigger();
    void setdioPictureOnTrigger(bool val);
    QString dioMinimumPulseWidth();
    void setDioMinimumPulseWidth(QString minPulse);
    QString dioSignal();
    void setDioSignal(QString signal);
    QString dioTriggerDelay();
    void setDioTriggerDelay(QString delay);
    QString dioMinDetectionTime();
    void setDioMinDetectionTime(QString detectionTime);
    QString dioTempHysteresis();
    void setDioTempHysteresis(QString hyst);
    QString dioMinAlarmTime();
    void setDioMinAlarmTime(QString alarmTime);
    QString dioLogic();
    void setDioLogic(QString logic);
    QString dioAlarm();
    void setDioAlarm(QString alarm);
    QString dioGateOpenTime();
    void setDioGateOpenTime(QString gateTime);
    bool dioShowStatus();
    void setDioShowStatus(bool val);

private:
    bool m_dioConnected = false;
    QString m_dioTriggerMode;
    bool m_dioPictureOnTrigger = false;
    QString m_dioMinimumPulseWidth;
    QString m_dioSignal;
    QString m_dioTriggerDelay;
    QString m_dioMinDetectionTime;
    QString m_dioTempHysteresis;
    QString m_dioMinAlarmTime;
    QString m_dioLogic;
    QString m_dioAlarm;
    QString m_dioGateOpenTime;
    bool m_dioShowStatus = false;

    Protocol *_protocol;
};

#endif // DIOWRAPPER_H
*/
