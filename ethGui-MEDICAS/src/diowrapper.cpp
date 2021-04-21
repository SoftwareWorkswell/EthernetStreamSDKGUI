/*
#include "diowrapper.h"

DIOWrapper::DIOWrapper()
{

}

DIOWrapper::DIOWrapper(Protocol *protocol) : QObject(nullptr)
{
    _protocol = protocol;
}


bool DIOWrapper::dioConnected()
{
    return m_dioConnected;
}

QString DIOWrapper::dioTriggerMode()
{
    return m_dioTriggerMode;
}

void DIOWrapper::setDioTriggerMode(QString dioTriggerMode)
{
    std::istringstream iss;
    if(send(_protocol->prepareMessage(_protocol->set_trigger_mode, dioTriggerMode), iss) && isOk(iss.str()))
    {
        m_dioTriggerMode = dioTriggerMode;
        emit dioTriggerModeChanged();
    }

}

bool DIOWrapper::dioPictureOnTrigger()
{
    return m_dioPictureOnTrigger;
}

void DIOWrapper::setdioPictureOnTrigger(bool val)
{
    std::istringstream iss;
    if(send(_protocol->prepareMessage(_protocol->set_picture_on_trigger,
                                     boolToString(val)), iss) && isOk(iss.str()))
    {
        m_dioPictureOnTrigger = val;
        emit dioPictureOnTriggerChanged();
    }

}

QString DIOWrapper::dioMinimumPulseWidth()
{
    return m_dioMinimumPulseWidth;
}

void DIOWrapper::setDioMinimumPulseWidth(QString minPulse)
{
    std::istringstream iss;
    if(minPulse.toInt() > 1000)
    {
        minPulse = "1000";
    }
    if(minPulse.toInt() < 100)
    {
        minPulse = "100";
    }
    if(send(_protocol->prepareMessage(_protocol->set_min_pulse_width, minPulse), iss) && isOk(iss.str()))
    {
        m_dioMinimumPulseWidth = minPulse;
        emit dioMinimumPulseWidthChanged();
    }

}

QString DIOWrapper::dioSignal()
{
    return m_dioSignal;
}

void DIOWrapper::setDioSignal(QString signal)
{
    std::istringstream iss;
    if(send(_protocol->prepareMessage(_protocol->set_dio_signal, signal), iss) && isOk(iss.str()))
    {
        m_dioSignal = signal;
        emit dioSignalChanged();
    }

}

QString DIOWrapper::dioTriggerDelay()
{
    return m_dioTriggerDelay;
}

void DIOWrapper::setDioTriggerDelay(QString delay)
{
    std::istringstream iss;
    if(send(_protocol->prepareMessage(_protocol->set_trigger_dalay, delay), iss) && isOk(iss.str()))
    {
        m_dioTriggerDelay = delay;
        emit dioTriggerDelayChanged();
    }

}

QString DIOWrapper::dioMinDetectionTime()
{
    return m_dioMinDetectionTime;
}

void DIOWrapper::setDioMinDetectionTime(QString detectionTime)
{
    std::istringstream iss;
    if(detectionTime.toInt() > 1000)
    {
        detectionTime = "1000";
    }
    if(detectionTime.toInt() < 0)
    {
        detectionTime = "0";
    }
    if(send(_protocol->prepareMessage(_protocol->set_min_detection_time, detectionTime), iss) && isOk(iss.str()))
    {
        m_dioMinDetectionTime = detectionTime;
        emit dioMinDetectionTimeChanged();
    }

}

QString DIOWrapper::dioTempHysteresis()
{
    return m_dioTempHysteresis;
}

void DIOWrapper::setDioTempHysteresis(QString hyst)
{
    std::istringstream iss;
    if(hyst.toDouble() > 2.0)
    {
        hyst = "2";
    }
    if(hyst.toDouble() < 0.0)
    {
        hyst = "0";
    }
    if(send(_protocol->prepareMessage(_protocol->set_temperature_hysteris, hyst), iss) && isOk(iss.str()))
    {
        m_dioTempHysteresis = hyst;
        emit dioTempHysteresisChanged();
    }

}

QString DIOWrapper::dioMinAlarmTime()
{
    return m_dioMinAlarmTime;
}

void DIOWrapper::setDioMinAlarmTime(QString alarmTime)
{
    std::istringstream iss;
    if(alarmTime.toInt() > 10)
    {
        alarmTime = "10";
    }
    if(alarmTime.toInt() < 0)
    {
        alarmTime = "0";
    }
    if(send(_protocol->prepareMessage(_protocol->set_min_alarm_time, alarmTime), iss) && isOk(iss.str()))
    {
        m_dioMinAlarmTime = alarmTime;
        emit dioMinAlarmTimeChanged();
    }

}

QString DIOWrapper::dioLogic()
{
    return m_dioLogic;
}

void DIOWrapper::setDioLogic(QString logic)
{
    std::istringstream iss;
    if(send(_protocol->prepareMessage(_protocol->set_dio_logic, logic), iss) && isOk(iss.str()))
    {
        m_dioLogic = logic;
        emit dioLogicChanged();
    }

}

QString DIOWrapper::dioAlarm()
{
    return m_dioAlarm;
}

void DIOWrapper::setDioAlarm(QString alarm)
{
    std::istringstream iss;
    if(send(_protocol->prepareMessage(_protocol->set_dio_alarm_level), iss) && isOk(iss.str()))
    {
        m_dioAlarm = alarm;
        emit dioAlarmChanged();
    }

}
QString DIOWrapper::dioGateOpenTime()
{
    return m_dioGateOpenTime;
}

void DIOWrapper::setDioGateOpenTime(QString gateTime)
{
    std::istringstream iss;
    if(send(_protocol->prepareMessage(_protocol->set_gate_open_time, gateTime), iss) && isOk(iss.str()))
    {
        m_dioGateOpenTime = gateTime;
        emit dioGateOpenTimeChanged();
    }

}

bool DIOWrapper::dioShowStatus()
{
    return m_dioShowStatus;
}

void DIOWrapper::setDioShowStatus(bool val)
{
    std::istringstream iss;
    if(send(_protocol->prepareMessage(_protocol->set_show_dio_status, boolToString(val)), iss) && isOk(iss.str()))
    {
        m_dioShowStatus = val;
        emit dioShowStatusChanged();
    }

}
*/
