#include "helperthread.h"

void HelperThread::run()
{

    long iters = 0;
    std::istringstream iss0;
    std::istringstream iss1;
    std::istringstream iss2;
    this->_ctmr.start();
    CustomTimer latencyTimer;

    while(*_runFlag)
    {
        if(*_extremes && _sendFun(_protocol.get_cross_max, iss0) && _sendFun(_protocol.get_cross_center, iss1) && _sendFun(_protocol.get_blackbody_roi, iss2))
            loadExtremes(iss0, iss1, iss2);

        loadCaptureRecordState();

        if (iters % 3 == 0)
            if (compareVersion("1.1.1", *_fwVersion))
                loadShutterData();

        if(iters % 10 == 0)
        {
            loadDateTime();
            loadMemoryState();
        }

        if (iters % 50 == 0)
        {
            latencyTimer.start();
            bool res = _sendFun(_protocol.check_connection, iss0);
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

    if(_sendFun("GTSN", iss))
    {
        getAnyLine(iss, cache);
        if(cache != "N/A")
        {
            next = secToMMSS(cache);
        } else next = QString::fromStdString(cache);
    }

    if(_sendFun("GTSL", iss))
    {
        getAnyLine(iss, cache);
        if(cache != "N/A")
        {
            last = secToMMSS(cache);
        } else last = QString::fromStdString(cache);
    }




    if (!next.isEmpty() && !last.isEmpty() )
        emit shutterDataChanged(next, last);
}


void HelperThread::loadTemps()
{
    std::istringstream iss;
    double cpu = 0, cam = 0;
    if (_sendFun(_protocol.get_cpu_temp, iss))
        iss >> cpu;
    if (_sendFun(_protocol.get_camera_core_temp, iss))
        iss >> cam;

    if (cpu && cam)
        emit tempsChanged(cpu, cam);
}


void HelperThread::loadDateTime()
{
    std::istringstream iss;
    std::string date, time;
    if(_sendFun("GDTI", iss))
    {
        std::getline(iss, date, '-');
        getAnyLine(iss, time);
        emit dateTimeChanged(QString::fromStdString(date), QString::fromStdString(time));
    }

}


void HelperThread::loadMemoryState()
{
    std::istringstream iss, line;
    std::string cache;

    if(_sendFun(_protocol.get_memory_status, iss))
    {
        std::getline(iss, cache, ' ');
        getAnyLine(iss, cache);
        _ssdDrive = QString::fromStdString(cache);

        std::getline(iss, cache, ' ');
        getAnyLine(iss, cache);
        _flashDrive = QString::fromStdString(cache);

        std::getline(iss, cache, ' ');
        getAnyLine(iss, cache);
        _sdDrive = QString::fromStdString(cache);


    }

    if (_sendFun(_protocol.get_memory_size, iss))
    {
        std::getline(iss, cache, ' ');
        getAnyLine(iss, cache);
        _ssdDriveCap = QString::fromStdString(cache);

        std::getline(iss, cache, ' ');
        getAnyLine(iss, cache);
        _flashDriveCap = QString::fromStdString(cache);

        std::getline(iss, cache, ' ');
        getAnyLine(iss, cache);
        _sdDriveCap = QString::fromStdString(cache);

    }

    if (_sendFun(_protocol.get_memory_free, iss))
    {
        std::getline(iss, cache, ' ');
        getAnyLine(iss, cache);
        _ssdDriveFree = QString::fromStdString(cache);

        std::getline(iss, cache, ' ');
        getAnyLine(iss, cache);
        _flashDriveFree = QString::fromStdString(cache);

        std::getline(iss, cache, ' ');
        getAnyLine(iss, cache);
        _sdDriveFree = QString::fromStdString(cache);


    }

    emit driveStateChanged(_ssdDrive, _ssdDriveCap, _ssdDriveFree, _sdDrive, _sdDriveCap, _sdDriveFree, _flashDrive, _flashDriveCap, _flashDriveFree);
}

void HelperThread::loadCaptureRecordState()
{
     std::istringstream iss;
     std::string cache;
     std::string positive = "TRUE";

     if(_sendFun(_protocol.prepareMessage(_protocol.check_is_recording), iss))
     {
         getAnyLine(iss, cache);
         _recording = cache == positive;
     }


     if(_sendFun(_protocol.prepareMessage(_protocol.check_capture), iss))
     {
         getAnyLine(iss, cache);
         _capturing = cache == positive;
     }

     emit captureRecordingChanged(_capturing, _recording);
}

void HelperThread::loadExtremes(std::istringstream& max_iss, std::istringstream& center_iss,  std::istringstream& blackbody_iss)
{
    std::string cache;
    getAnyLine(max_iss, cache);
    std::istringstream line(cache);
    std::getline(line, cache, ' ');
    std::getline(line, cache, ' ');
    _maximum->x = stoi(cache);
    std::getline(line, cache, ' ');
    _maximum->y = stoi(cache);
    std::getline(line, cache);
    _maximum->val = stod(cache);

    getAnyLine(center_iss, cache);
    line = std::istringstream(cache);
    std::getline(line, cache, ' ');
    std::getline(line, cache, ' ');
    _center->x = stoi(cache);
    std::getline(line, cache, ' ');
    _center->y = stoi(cache);
    std::getline(line, cache);
    _center->val = stod(cache);

    std::getline(blackbody_iss, cache, ' ');
    _blackbody->x = QString::fromStdString(cache).toInt();
    std::getline(blackbody_iss, cache, ' ');
    _blackbody->y = QString::fromStdString(cache).toInt();
    getAnyLine(blackbody_iss, cache);
    _blackbody->val = QString::fromStdString(cache).toDouble();
    emit extremesChanged();
}
