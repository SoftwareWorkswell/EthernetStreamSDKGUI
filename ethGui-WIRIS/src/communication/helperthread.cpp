#include "helperthread.h"
#include <src/communication/protocol.h>
#include <src/camera/camerautils.h>

void HelperThread::run()
{

    long iters = 0;
    std::istringstream iss;
    this->_ctmr.start();
    CustomTimer latencyTimer;

    while(*_runFlag)
    {
        // check connection every run
        _sendFun(_protocol.GET_ETH_MODE, iss);
        if(iss.str().find("TRUE") == std::string::npos)
        {
            emit connectionStateChanged(false, 0);
            return;
        }
        if(*_extremes && _sendFun(_protocol.GET_THERMAL_EXTREMES, iss))
            loadExtremes(iss);

        loadCaptureRecordState();

        // cooldownstate, detect thermal on/off and cooldown time
        if( _type == _vals.GIS_TYPE)
            loadCooldown();

        if (iters % 3 == 0)
            if (CameraUtils::compareVersion(_vals.MIN_FW_VERSION, *_fwVersion) && *_type != _vals.WIRIS_SECURITY_TYPE)
            {
                if(*_type == _vals.GIS_TYPE)
                    loadGisShutterData();
                else
                    loadShutterData();
            }

        if(iters % 10 == 0)
        {
            loadDateTime();
            loadGps();
            loadMemoryState();
        }

        if (iters % 50 == 0)
        {
            latencyTimer.start();
            bool res = _sendFun(_protocol.CHECK_CONNECTION, iss);
            emit connectionStateChanged(res, latencyTimer.elapsedMilliseconds());
            latencyTimer.stop();
            loadTemps();
        }

        while(_ctmr.elapsedMilliseconds() < 99)
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        iters++;
        _ctmr.start();
    }
}


QString secToMMSS(std::string cache)
{
    QString next;
    int min = std::stoi(cache)/60, sec = std::stoi(cache)%60;
    next = QString::number(min) + ":";
    sec < 10 ? next += "0" + QString::number(sec) : next += QString::number(sec);
    return next;
}


void HelperThread::loadShutterData()
{
    std::istringstream iss;
    std::string cache;

    QString next, last;

    if(_sendFun(_protocol.GET_SECONDS_TO_SHUTTER, iss))
    {
        Protocol::Protocol::getAnyLine(iss, cache);
        if(cache != "N/A")
        {
            next = secToMMSS(cache);
        } else next = QString::fromStdString(cache);
    }

    if(_sendFun(_protocol.GET_SECONDS_FROM_SHUTTER, iss))
    {
        Protocol::getAnyLine(iss, cache);
        if(cache != "N/A")
        {
            last = secToMMSS(cache);
        } else last = QString::fromStdString(cache);
    }

    if (!next.isEmpty() && !last.isEmpty() )
        emit shutterDataChanged(next, last);
}

void HelperThread::loadGisShutterData()
{
    std::istringstream iss;
    std::string cache;

    QString next, last;
    if(_sendFun(_protocol.GET_SECONDS_FROM_SHUTTER, iss))
    {
        Protocol::getAnyLine(iss, cache);
        if(cache != "N/A")
        {
            last = secToMMSS(cache);
        } else last = QString::fromStdString(cache);
    }

    if (!last.isEmpty() )
        emit shutterDataChanged(next, last);
}

void HelperThread::loadTemps()
{
    std::istringstream iss;
    double cpu = 0, cam = 0;
    if (_sendFun("GTCU", iss))
        iss >> cpu;
    if (_sendFun("GTIC", iss))
        iss >> cam;

    if (cpu && cam)
        emit tempsChanged(cpu, cam);
}

void HelperThread::loadGps()
{
    std::istringstream iss;
    std::string cache;
    if (_sendFun(_protocol.GET_GPS_COORDS, iss))
    {
        Protocol::getAnyLine(iss, cache);
        if (cache != "N/A" && cache != "INVALID")
        {
            std::istringstream line(cache);
            std::getline(line, cache, ' ');
            Protocol::getAnyLine(line, cache);
            _gps = "lat: " + QString::fromStdString( cache ) + "\n";

            Protocol::getAnyLine(iss, cache);
            line = std::istringstream(cache);
            std::getline(line, cache, ' ');
            Protocol::getAnyLine(line, cache);
            _gps += "lon: " + QString::fromStdString( cache ) + "\n";

            Protocol::getAnyLine(iss, cache);
            line = std::istringstream(cache);
            std::getline(line, cache, ' ');
            Protocol::getAnyLine(line, cache);
            _gps += "alt: " + QString::fromStdString( cache ) + "\n";

        }
        else {
            _gps = QString::fromStdString(cache);
        }

        emit gpsChanged(_gps);
    }

}

void HelperThread::loadCooldown()
{
    std::istringstream iss;
    std::string time;
    if(_sendFun(_protocol.GET_COOLDOWN_TIME, iss))
    {
        std::getline(iss, time, ' ');
        int sec = std::stoi(time);
        emit cooldownTimeChanged(QString::fromStdString(time));
        if(sec == 0)
            *_cooledDown = true;
        else if(*(_cooledDown)) // was cooled, not is not - switch on camera turned off
        {
            emit gisThermalSwitchOff();
            *_cooledDown = false;
        }
    }
}

void HelperThread::loadDateTime()
{
    std::istringstream iss;
    std::string date, time;
    if(_sendFun(_protocol.GET_CURRENT_DATETIME, iss))
    {
        std::getline(iss, date, '-');
        Protocol::getAnyLine(iss, time);
        emit dateTimeChanged(QString::fromStdString(date), QString::fromStdString(time));
    }

}


void HelperThread::loadMemoryState()
{
    std::istringstream iss, line;
    std::string cache;

    if(_sendFun(_protocol.GET_MEMORY_STATUS, iss))
    {
        std::getline(iss, cache, ' ');
        Protocol::getAnyLine(iss, cache);
        _ssdDrive = QString::fromStdString(cache);

        std::getline(iss, cache, ' ');
        Protocol::getAnyLine(iss, cache);
        _flashDrive = QString::fromStdString(cache);

        std::getline(iss, cache, ' ');
        Protocol::getAnyLine(iss, cache);
        _sdDrive = QString::fromStdString(cache);


    }

    if (_sendFun(_protocol.GET_MEMORY_SIZE, iss))
    {
        std::getline(iss, cache, ' ');
        Protocol::getAnyLine(iss, cache);
        _ssdDriveCap = QString::fromStdString(cache);

        std::getline(iss, cache, ' ');
        Protocol::getAnyLine(iss, cache);
        _flashDriveCap = QString::fromStdString(cache);

        std::getline(iss, cache, ' ');
        Protocol::getAnyLine(iss, cache);
        _sdDriveCap = QString::fromStdString(cache);

    }

    if (_sendFun(_protocol.GET_MEMORY_FREE, iss))
    {
        std::getline(iss, cache, ' ');
        Protocol::getAnyLine(iss, cache);
        _ssdDriveFree = QString::fromStdString(cache);

        std::getline(iss, cache, ' ');
        Protocol::getAnyLine(iss, cache);
        _flashDriveFree = QString::fromStdString(cache);

        std::getline(iss, cache, ' ');
        Protocol::getAnyLine(iss, cache);
        _sdDriveFree = QString::fromStdString(cache);


    }

    emit driveStateChanged(_ssdDrive, _ssdDriveCap, _ssdDriveFree, _sdDrive, _sdDriveCap, _sdDriveFree, _flashDrive, _flashDriveCap, _flashDriveFree);
}

void HelperThread::loadCaptureRecordState()
{
     std::istringstream iss;
     std::string cache;
     std::string positive = "TRUE";

     if(_sendFun(_protocol.CHECK_IS_RECORDING, iss))
     {
         Protocol::getAnyLine(iss, cache);
         _recording = cache == positive;
     }


     if(_sendFun(_protocol.CHECK_CAPTURE, iss))
     {
         Protocol::getAnyLine(iss, cache);
         _capturing = cache == positive;
     }

     emit captureRecordingChanged(_capturing, _recording);
}

void HelperThread::loadExtremes(std::istringstream& iss)
{
    std::string cache;
    Protocol::getAnyLine(iss, cache);
    std::istringstream line(cache);
    std::getline(line, cache, ' ');
    std::getline(line, cache, ' ');
    _maximum->x = stoi(cache);
    std::getline(line, cache, ' ');
    _maximum->y = stoi(cache);
    std::getline(line, cache);
    _maximum->val = stod(cache);

    Protocol::getAnyLine(iss, cache);
    line = std::istringstream(cache);
    std::getline(line, cache, ' ');
    std::getline(line, cache, ' ');
    _minimum->x = stoi(cache);
    std::getline(line, cache, ' ');
    _minimum->y = stoi(cache);
    std::getline(line, cache);
    _minimum->val = stod(cache);

    Protocol::getAnyLine(iss, cache);
    line = std::istringstream(cache);
    std::getline(line, cache, ' ');
    std::getline(line, cache, ' ');
    _center->x = stoi(cache);
    std::getline(line, cache, ' ');
    _center->y = stoi(cache);
    std::getline(line, cache);
    _center->val = stod(cache);

    emit extremesChanged();
}
