#include "controllercore.h"
#include <src/camera/camerautils.h>

#include <QDebug>

ControllerCore::ControllerCore(std::shared_ptr<CameraConnection> connection, std::shared_ptr<CameraParameters> params, std::shared_ptr<NetworkClient> networkClient)
{
    _params = params;
    _connection = connection;
    _networkClient = networkClient;
    _connection->_connected =_networkClient && _networkClient->is_connected();
}

bool ControllerCore::send(const QString &message)
{
    std::istringstream s;
    return send(message, s);
}

void ControllerCore::disconnect()
{
    _networkClient->disconnect();
}

bool ControllerCore::fetchAll()
{
    try
    {
        fetchEnviroments();
        fetchRangeModes();
        fetchManualRange();
        fetchSpanRange();
        fetchZooms();
        fetchCurrentZooms();
        fetchPalettes();
        fetchBasicInfo();
        fetchPaletteValues();
        fetchThermalParams();
        fetchShutter();
        fetchCaptureRecSettings();
        fetchAlarms();
        fetchTempModes();
    }
    catch(...)
    {
        return false;
    }
    return true;
}

bool ControllerCore::send(const QString &message, std::istringstream & answ, bool debugPrint)
{
    QMutexLocker locker(connectionMutex.get());
    if (_networkClient.get() == nullptr)
        return  false;
    // Write and read using network client
    std::string answer;
    int length;
    _networkClient->write(message.toStdString());
    length = _networkClient->read_until(answer, '\0', std::chrono::seconds(10));
    // communication logging
    QString log = "Command: " + message;
    log += "   Answer: ";
    if (length > 0)
        log += QString::fromStdString(answer);
    if (debugPrint)
        qDebug() << log;
    answ = std::istringstream(answer);
    return !(QString::fromStdString(answer).contains("ERR") || answer.empty());
}

void ControllerCore::fetchBasicInfo()
{
    std::istringstream iss;
    std::string cache;

    send(_protocol.prepareMessage(_protocol.GET_FW_VERSION), iss);
    Protocol::getAnyLine(iss, cache);
    _params->_firmwareVersion = QString::fromStdString(cache);

    send(_protocol.prepareMessage(_protocol.GET_SERIAL_NUMBER), iss);
    Protocol::getAnyLine(iss, cache);
    _params->_serialNumber = QString::fromStdString(cache);

    send(_protocol.prepareMessage(_protocol.GET_ARTICLE_NUMBER), iss);
    Protocol::getAnyLine(iss, cache);
    _params->_articleNumber = QString::fromStdString(cache);

    if (_params->_serialNumber.contains(_vals.WIRIS_SECURITY_CODE))
        _params->_cameraType = _vals.WIRIS_SECURITY_TYPE;
    else if(_params->_serialNumber.contains(_vals.WIRIS_PRO_CODE))
        _params->_cameraType = _vals.WIRIS_PRO_TYPE;
    else if(_params->_serialNumber.contains(_vals.GIS_CODE))
        _params->_cameraType = _vals.GIS_TYPE;
    if(_params->_cameraType != _vals.WIRIS_SECURITY_CODE)
    {
        send(_protocol.prepareMessage(_protocol.GET_THERMAL_UNITS), iss);
        Protocol::getAnyLine(iss, cache);
        _params->_thermalUnit =  CameraUtils::thermalUnitFromCode(QString::fromStdString(cache));
        _params->_thermalUnitSign = CameraUtils::thermalUnitToSign(_params->_thermalUnit);
    }
}

void ControllerCore::fetchEnviroments()
{
    if (_params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        return;

    std::string cache;
    std::istringstream envList;
    if(send(Protocol::prepareMessage(_protocol.GET_ENVIRONMENT_LIST), envList))
    {
        Protocol::getAnyLine(envList, cache);
        _params->_enviroment1 = QString::fromStdString(cache);
        Protocol::getAnyLine(envList, cache);
        _params->_enviroment2 = QString::fromStdString(cache);
        Protocol::getAnyLine(envList, cache);
        _params->_enviroment3 = QString::fromStdString(cache);
        Protocol::getAnyLine(envList, cache);
        _params->_enviroment4 = QString::fromStdString(cache);
        send(Protocol::prepareMessage(_protocol.GET_ENVIRONMENT), envList);
        Protocol::getAnyLine(envList, cache);
        _params->_currentEnviroment = QString::fromStdString(cache);
        setManualRanges(_params->_currentEnviroment.mid(0, _params->_currentEnviroment.lastIndexOf(' ')),
                        _params->_currentEnviroment.mid(_params->_currentEnviroment.lastIndexOf(' ')));
    }
}

void ControllerCore::fetchRangeModes()
{
    if (_params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        return;

    std::istringstream iss;
    if(send(Protocol::prepareMessage(_protocol.GET_RANGE_MODE), iss))
    {
        std::string resp;
        Protocol::getAnyLine(iss, resp);
        if (resp == _vals.RANGE_MODE_AUTOMATIC.toStdString())
            _params->_rangeMode = 0;
        else if (resp == _vals.RANGE_MODE_MANUAL.toStdString())
            _params->_rangeMode = 1;
        else if (resp == _vals.RANGE_MODE_SPAN.toStdString())
            _params->_rangeMode = 2;
    }

}

void ControllerCore::fetchManualRange()
{
    if(_params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        return;

    std::istringstream iss;
    std::string cache;
    if(send(Protocol::prepareMessage(_protocol.GET_MANUAL_RANGE), iss))
    {
        std::getline(iss, cache, ' ');
        _params->_manualRange1 = QString::fromStdString(cache);
        Protocol::getAnyLine(iss, cache);
        _params->_manualRange2 = QString::fromStdString(cache);
        _params->_paletteTop = _params->_manualRange2.toDouble();
        _params->_paletteBottom = _params->_manualRange1.toDouble();
    }
}

void ControllerCore::fetchSpanRange()
{
    if(_params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        return;

    std::istringstream iss;
    std::string cache;
    if(send(Protocol::prepareMessage(_protocol.GET_SPAN_RANGE), iss))
    {
        std::getline(iss, cache, ' ');
        _params->_spanWindow = QString::fromStdString(cache);
        Protocol::getAnyLine(iss, cache);
        _params->_spanCenter = QString::fromStdString(cache);
        _params->_paletteTop = _params->_spanCenter.toDouble() + _params->_spanWindow.toDouble()/2;
        _params->_paletteBottom = _params->_spanCenter.toDouble() - _params->_spanWindow.toDouble()/2;
    }
}


void ControllerCore::fetchZooms()
{
    std::istringstream iss, line;
    std::string cache, val;
    _params->_zoomsThermal.clear();
    _params->_zoomsVisible.clear();

    if(send(Protocol::prepareMessage(_protocol.GET_THERMAL_ZOOM_LIST), iss))
    {
        while(Protocol::getAnyLine(iss, cache))
        {
            if(!cache.empty())
            {
                line = std::istringstream(cache);
                std::getline(line, val, ' ');
                Protocol::getAnyLine(line, val);
                _params->_zoomsThermal.push_back(QString::fromStdString(val));
            }
        }
    }
    iss.clear();
    if(send(Protocol::prepareMessage(_protocol.GET_VISIBLE_ZOOM_LIST), iss))
    {
        while(Protocol::getAnyLine(iss, cache))
        {
            if(!cache.empty())
            {
                line = std::istringstream(cache);
                std::getline(line, val, ' ');
                Protocol::getAnyLine(line, val);
                _params->_zoomsVisible.push_back(QString::fromStdString(val));
            }
        }
    }
}

void ControllerCore::fetchPalettes()
{
    _params->_palettes.clear();
    std::string cache;
    std::istringstream iss;
    if(send(Protocol::prepareMessage(_protocol.GET_PALETTE_NAME_INDEX), iss))
    {
        Protocol::getAnyLine(iss, cache);
        _params->_currentPalette = QString::fromStdString(cache);
        auto list = _params->_currentPalette.split(" ");
        if(list.size() == 2)
            _params->_currentPalette = list[1];
        send(Protocol::prepareMessage(_protocol.SET_PALETTE_BY_NAME,_params->_currentPalette));
    }

    if( send(Protocol::prepareMessage(_protocol.GET_PALETTE_LIST), iss))
    {
        while(Protocol::getAnyLine(iss, cache))
            if(!cache.empty())
            {
                QString pal = QString::fromStdString(cache);
                auto list = pal.split(" ");
                if(list.size() == 2)
                    pal = list[1];
                _params->_palettes.push_back(new PaletteWrapper(pal));
            }
    }
}

void ControllerCore::fetchPaletteValues()
{
    std::istringstream iss, line;
    std::string cache, val;
    QString r, g, b;

    _params->_paletteValues.clear();

    if(send(Protocol::prepareMessage(_protocol.GET_PALETTE_VALUES), iss))
    {
        while (Protocol::getAnyLine(iss, cache))
        {
            if (!cache.empty())
            {
                line = std::istringstream(cache);
                std::getline(line, val, ' ');
                r = QString::fromStdString(val);
                std::getline(line, val, ' ');
                g = QString::fromStdString(val);
                std::getline(line, val);
                b = QString::fromStdString(val);

                _params->_paletteValues.push_back(new PaletteValueWrapper(r, g, b));
            }
        }
    }
}

void ControllerCore::fetchCurrentZooms()
{
    std::istringstream iss;
    std::string cache;

    send(Protocol::prepareMessage(_protocol.SET_VISIBLE_ZOOM_INDEX, 0));
    send(Protocol::prepareMessage(_protocol.SET_THERMAL_ZOOM_INDEX, 0));

    _params->_currentZoomThermal = _params->_zoomsThermal.begin();
    _params->_currentZoomVisible = _params->_zoomsVisible.begin();

    if(send(Protocol::prepareMessage(_protocol.GET_THERMAL_ZOOM), iss))
    {
        Protocol::getAnyLine(iss, cache);
        for(int i = 0; i < _params->_zoomsThermal.size(); i++)
        {
            if (_params->_zoomsThermal.at(i).toStdString() == cache){
                _params->_currentZoomThermal = _params->_zoomsThermal.begin() + i;
            }
        }
    }
    iss.clear();
    cache.clear();

    if(send (Protocol::prepareMessage(_protocol.GET_VISIBLE_ZOOM), iss))
    {
        Protocol::getAnyLine(iss, cache);
        for(int i = 0; i < _params->_zoomsVisible.size(); i++)
        {
            if (_params->_zoomsVisible.at(i).toStdString() == cache)
                _params->_currentZoomVisible = _params->_zoomsVisible.begin() + i;
        }
    }
}

void ControllerCore::fetchThermalParams()
{
    if(_params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        return;

    std::istringstream iss;
    if(send(Protocol::prepareMessage(_protocol.GET_EMISSIVITY), iss))
    {
        double val = 0;
        iss >> val;
        if (val > _vals.MIN_EMISSIVITY && val < _vals.MAX_EMISSIVITY)
            _params->_emissivity = val;
        else
            setEmissivity(_params->_emissivity);
    }
    if(send(Protocol::prepareMessage(_protocol.GET_REFLECTED_TEMP), iss))
    {
        double val = 0;
        iss >> val;
        if (val > _vals.MIN_ENV_TEMP && val < _vals.MAX_ENV_TEMP)
            _params->_reflectedTemp = val;
        else
            setReflectedTemp(_params->_reflectedTemp);
    }
    if(send(Protocol::prepareMessage(_protocol.GET_ATMOSPHERIC_TEMP), iss))
    {
        double val = 0;
        iss >> val;
        if (val > _vals.MIN_ENV_TEMP && val < _vals.MAX_ENV_TEMP)
            _params->_atmosphericTemp = val;
        else
            setAtmosphericTemp(_params->_atmosphericTemp);
    }
    if(send(Protocol::prepareMessage(_protocol.GET_INTERPOLATION), iss))
        _params->_interpolation = _protocol.isPositive(iss.str());
}

void ControllerCore::fetchShutter()
{
    if(_params->_cameraType == _vals.WIRIS_SECURITY_TYPE || _params->_cameraType == _vals.GIS_TYPE)
        return;
    std::istringstream iss;
    double shuttersecs;
    if(send(Protocol::prepareMessage(_protocol.GET_SYNC_SHUTTER), iss))
        _params->_syncShutter = _protocol.isPositive(iss.str());

    if (send(Protocol::prepareMessage(_protocol.GET_SHUTTER_PERIOD), iss))
    {
        iss >> shuttersecs;
        _params->_shutterPeriodMin = round(shuttersecs/60);
    }
}

void ControllerCore::fetchTempModes()
{
    if(_params->_cameraType != _vals.GIS_TYPE)
        return;
    std::istringstream iss;
    if(send(Protocol::prepareMessage(_protocol.GET_GIS_DIFFERENTIAL_MODE), iss))
        _params->_differentialMode = _protocol.isPositive(iss.str());
    if(send(Protocol::prepareMessage(_protocol.GET_GIS_HIGH_SENSITIVITY_MODE), iss))
        _params->_highSensitivityMode = _protocol.isPositive(iss.str());
}

void ControllerCore::fetchCaptureRecSettings()
{
    std::istringstream iss;

    send(Protocol::prepareMessage(_protocol.GET_IMAGE_RADIOMETRIC_JPEG), iss);
    _params->_radJpg = _protocol.isPositive(iss.str());

    if(_params->_cameraType != _vals.WIRIS_SECURITY_TYPE)
    {
        send(Protocol::prepareMessage(_protocol.GET_IMAGE_RADIOMETRIC_TIFF), iss);
        _params->_radTiff = _protocol.isPositive(iss.str());
        if(_params->_cameraType != _vals.GIS_TYPE)
        {
            send(Protocol::prepareMessage(_protocol.GET_IMAGE_SUPERRES), iss);
            _params->_srImg = _protocol.isPositive(iss.str());
        }
    }
    send(Protocol::prepareMessage(_protocol.GET_IMAGE_VISIBLE_JPEG), iss);
    _params->_visJpg = _protocol.isPositive(iss.str());

    if(_params->_cameraType != _vals.WIRIS_SECURITY_TYPE)
        send(Protocol::prepareMessage(_protocol.GET_VIDEO_IR), iss);
    else
        send(Protocol::prepareMessage(_protocol.GET_VIDEO_IR_ENCODED), iss);
    _params->_radVid = _protocol.isPositive(iss.str());

    send(Protocol::prepareMessage(_protocol.GET_VIDEO_VISIBLE), iss);
    _params->_visVid = _protocol.isPositive(iss.str());
}

void ControllerCore::fetchAlarms()
{
    if(_params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        return;

    std::istringstream iss;
    std::string cache;
    QString qcache;

    send(Protocol::prepareMessage(_protocol.SET_ALARM_MODE, QString("OFF")));

    if (send(Protocol::prepareMessage(_protocol.GET_ALARM_MODE), iss))
    {
        Protocol::getAnyLine(iss, cache);
        qcache = QString::fromStdString(cache);
        if (qcache == _vals.ALARM_MODE_OFF)
            _params->_alarmMode = 0;
        else if (qcache == _vals.ALARM_MODE_ABOVE)
            _params->_alarmMode = 1;
        else if (qcache == _vals.ALARM_MODE_BELOW)
            _params->_alarmMode = 2;
        else if (qcache == _vals.ALARM_MODE_BETWEEN)
            _params->_alarmMode = 3;
        else if (qcache == _vals.ALARM_MODE_OUTSIDE)
            _params->_alarmMode = 4;
    }
    if (send(Protocol::prepareMessage(_protocol.GET_ALARMS), iss))
    {
        std::getline(iss, cache, ' ');
        _params->_alarmFrom = std::stod(cache);
        Protocol::getAnyLine(iss, cache);
        _params->_alarmTo = std::stod(cache);
    }
    //fetch colors if current firmware supports them
    if (CameraUtils::compareFw(_vals.MIN_FW_VERSION, _params->_firmwareVersion))
    {
        if(send(Protocol::prepareMessage(_protocol.GET_ALARMS_COLORS), iss))
        {
            std::getline(iss, cache,' ');
            _params->_alarmAboveColor = QString::fromStdString(cache);
            std::getline(iss, cache,' ');
            _params->_alarmBetweenColor = QString::fromStdString(cache);
            Protocol::getAnyLine(iss, cache);
            _params->_alarmBelowColor = QString::fromStdString(cache);
        }
    }
}

void ControllerCore::setRangeMode(int value)
{
    if(_params->_cameraType == _vals.WIRIS_SECURITY_TYPE || value < 0 || value > 2)
        return;

    QString req = "";
    switch (value)
    {
    case 0:
        req += _vals.RANGE_MODE_AUTOMATIC;
        break;
    case 1:
        req += _vals.RANGE_MODE_MANUAL;
        break;
    case 2:
        req += _vals.RANGE_MODE_SPAN;
        break;
    default:
        return;
    }
    std::istringstream resp;
    if (send(Protocol::prepareMessage(_protocol.SET_RANGE_MODE, req), resp))
        if(Protocol::isPositive(resp.str()))
            _params->_rangeMode = value;
}

void ControllerCore::setEmissivity(double val)
{
    if(!_params || !_networkClient->is_connected() || _params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        return;

    std::istringstream iss;
    if (val > _vals.MIN_EMISSIVITY && val <= _vals.MAX_EMISSIVITY && send(Protocol::prepareMessage(_protocol.SET_EMISSIVITY, QString::number(val)), iss))
        if(Protocol::isOk(iss.str()))
            _params->_emissivity = val;
}

void ControllerCore::setReflectedTemp(double val)
{
    if(_params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        return;

    std::istringstream iss;
    if (val > _vals.MIN_ENV_TEMP && val <= _vals.MAX_ENV_TEMP && send(Protocol::prepareMessage(_protocol.SET_REFLECTED_TEMP,QString::number(val)), iss))
        if(Protocol::isOk(iss.str()))
            _params->_reflectedTemp= val;
}

void ControllerCore::setAtmosphericTemp(double val)
{
    if(_params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        return;

    std::istringstream iss;
    if (val > _vals.MIN_ENV_TEMP && val <= _vals.MAX_ENV_TEMP && send(Protocol::prepareMessage(_protocol.SET_ATMOSPHERIC_TEMP,QString::number(val)), iss))
        if(Protocol::isOk(iss.str()))
            _params->_atmosphericTemp= val;
}

void ControllerCore::setManualRange1(const QString& val)
{
    if (enviromentLowerThreshold().isEmpty() || enviromentUpperThreshold().isEmpty() || val == _params->_manualRange1 || _params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        return;

    if(val.toDouble() >= enviromentLowerThreshold().toDouble() && val.toDouble() < _params->_manualRange2.toDouble())
    {
        std::istringstream iss;
        QStringList args;
        args << val  << _params->_manualRange2;
        if(send(Protocol::prepareMessage(_protocol.SET_MANUAL_RANGE, args), iss))
        {
            if (Protocol::isOk(iss.str()))
            {
                _params->_manualRange1 = val;
                _params->_paletteBottom = _params->_manualRange1.toDouble();
            }
        }
    }
}

void ControllerCore::setManualRange2(const QString& val)
{
    if (enviromentLowerThreshold().isEmpty() || enviromentUpperThreshold().isEmpty() || val == _params->_manualRange2 || _params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        return;

    if(val.toDouble() <= enviromentUpperThreshold().toDouble() && val.toDouble() > _params->_manualRange1.toDouble())
    {
        std::istringstream iss;
        std::string cache;
        QStringList args;
        args << _params->_manualRange1 << val;
        if(send(Protocol::prepareMessage(_protocol.SET_MANUAL_RANGE, args), iss))
        {
            if (Protocol::isOk(iss.str()))
            {
                _params->_manualRange2 = val;
                _params->_paletteTop = _params->_manualRange2.toDouble();
            }
        }
    }
}

void ControllerCore::setManualRanges(const QString& val1, const QString val2)
{
    if (enviromentLowerThreshold().isEmpty() || enviromentUpperThreshold().isEmpty() || _params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        return;

    if(val1.toDouble() <= enviromentUpperThreshold().toDouble() && val2.toDouble() > val1.toDouble())
    {
        std::istringstream iss;
        std::string cache;
        QStringList args;
        args << val1 << val2;
        if(send(Protocol::prepareMessage(_protocol.SET_MANUAL_RANGE, args), iss))
        {
            if (Protocol::isOk(iss.str()))
            {
                _params->_manualRange1 = val1;
                _params->_manualRange2 = val2;
                _params->_paletteBottom = _params->_manualRange1.toDouble();
                _params->_paletteTop = _params->_manualRange2.toDouble();
            }
        }
    }
}

void ControllerCore::setSpanCenter(const QString& val)
{
    if (CameraUtils::isValidSpan(_params->_spanWindow, val, _params->_currentEnviroment) && _params->_cameraType != _vals.WIRIS_SECURITY_TYPE)
    {
        QStringList args;
        std::istringstream iss;
        args << _params->_spanWindow << val;
        if(send(Protocol::prepareMessage(_protocol.SET_SPAN_RANGE, args), iss) && Protocol::isPositive(iss.str()))
        {
            _params->_spanCenter = val;
            _params->_paletteTop = _params->_spanCenter.toDouble() + _params->_spanWindow.toDouble()/2;
            _params->_paletteBottom = _params->_spanCenter.toDouble() - _params->_spanWindow.toDouble()/2;
        }
    }
}

void ControllerCore::setSpanWindow(const QString& val)
{
    if (CameraUtils::isValidSpan(val, _params->_spanCenter, _params->_currentEnviroment) && _params->_cameraType != _vals.WIRIS_SECURITY_TYPE)
    {
        QStringList args;
        std::istringstream iss;
        args << val << _params->_spanCenter;
        if(send(Protocol::prepareMessage(_protocol.SET_SPAN_RANGE, args), iss) && Protocol::isPositive(iss.str()))
        {
            _params->_spanWindow = val;
            _params->_paletteTop = _params->_spanCenter.toDouble() + _params->_spanWindow.toDouble()/2;
            _params->_paletteBottom = _params->_spanCenter.toDouble() - _params->_spanWindow.toDouble()/2;
        }
    }
}

void ControllerCore::setAlarmMode(int val)
{
    if (val < 0 || val > 4 || _params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        return;
    QString req;
    switch (val) {
    case 0:
        req += _vals.ALARM_MODE_OFF;
        break;
    case 1:
        req += _vals.ALARM_MODE_ABOVE;
        break;
    case 2:
        req += _vals.ALARM_MODE_BELOW;
        break;
    case 3:
        req += _vals.ALARM_MODE_BETWEEN;
        break;
    case 4:
        req += _vals.ALARM_MODE_OUTSIDE;
        break;
    default:
        return;
    }
    std::istringstream iss;
    if (send(Protocol::prepareMessage(_protocol.SET_ALARM_MODE, req), iss) && Protocol::isPositive(iss.str()))
        _params->_alarmMode = val;
}

void ControllerCore::setAlarmFrom(double val)
{
    if(val <= _params->_alarmTo && val >= enviromentLowerThreshold().toDouble() && _params->_cameraType != _vals.WIRIS_SECURITY_TYPE)
    {
        std::istringstream iss;
        QStringList args;
        args << QString::number(val) << QString::number(_params->_alarmTo);
        if(send(Protocol::prepareMessage(_protocol.SET_ALARMS, args), iss) && Protocol::isPositive(iss.str()))
            _params->_alarmFrom = val;
    }
}

void ControllerCore::setAlarmTo(double val)
{
    if (val >= _params->_alarmFrom && val <= enviromentUpperThreshold().toDouble() && _params->_cameraType != _vals.WIRIS_SECURITY_TYPE)
    {
        std::istringstream iss;
        QStringList args;
        args << QString::number(_params->_alarmFrom) << QString::number(val);
        if(send(Protocol::prepareMessage(_protocol.SET_ALARMS, args), iss) && Protocol::isPositive(iss.str()))
            _params->_alarmTo = val;
    }
}

void ControllerCore::setSyncShutter(bool val)
{
    if (_params->_cameraType == _vals.GIS_TYPE)
        return;
    std::istringstream iss;
    if (send(Protocol::prepareMessage(_protocol.SET_SYNC_SHUTTER, val), iss) && Protocol::isPositive(iss.str()))
        _params->_syncShutter = val;
}

void ControllerCore::setShutterPeriod(int val)
{
    if (_params->_cameraType == _vals.GIS_TYPE)
        return;
    std::istringstream iss;
    if (val >= _vals.MIN_SHUTTER_PERIOD && val <= _vals.MAX_SHUTTER_PERIOD)
        if (send(Protocol::prepareMessage(_protocol.SET_SHUTTER_PERIOD, val*60), iss) && Protocol::isPositive(iss.str()))
            _params->_shutterPeriodMin = val;
}

void ControllerCore::setInterpolation(bool val)
{
    if (_params->_cameraType != _vals.GIS_TYPE)
        return;
    std::istringstream iss;
    if (send(Protocol::prepareMessage(_protocol.SET_INTERPOLATION, val), iss) && Protocol::isPositive(iss.str()))
        _params->_interpolation = val;
}

void ControllerCore::setSBUSLogging(bool val)
{
    std::istringstream iss;
    if (send(Protocol::prepareMessage(_protocol.SET_SBUS_LOGGING, val), iss) && Protocol::isPositive(iss.str()))
        _params->_sbus_logging = val;
}

void ControllerCore::setMavlinkLogging(bool val)
{
    std::istringstream iss;
    if (send(Protocol::prepareMessage(_protocol.SET_MAVLINK_LOGGING, val), iss) && Protocol::isPositive(iss.str()))
        _params->_mavlink_logging = val;
}

void ControllerCore::setThermalUnit(const QString & unit)
{
    std::istringstream iss;
    if (send(Protocol::prepareMessage(_protocol.SET_THERMAL_UNITS, unit), iss) && Protocol::isPositive(iss.str()))
    {
        _params->_thermalUnit = CameraUtils::thermalUnitFromCode(unit);
        _params->_thermalUnitSign = CameraUtils::thermalUnitToSign(unit);
        fetchEnviroments();
        fetchThermalParams();
        fetchAlarms();
        fetchRangeModes();
    }
}

void ControllerCore::setEnviroment1(const QString &)
{
    if (_params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        return;
    if (changeEnviroment(_params->_enviroment1))
        _params->_currentEnviroment = _params->_enviroment1;
}

void ControllerCore::setEnviroment2(const QString &)
{
    if (_params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        return;

    if (changeEnviroment(_params->_enviroment2))
        _params->_currentEnviroment = _params->_enviroment2;
}

void ControllerCore::setEnviroment3(const QString & env)
{
    if (_params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        return;
    if (changeEnviroment(_params->_enviroment3))
        _params->_currentEnviroment = _params->_enviroment3;
}

void ControllerCore::setEnviroment4(const QString &)
{
    if (_params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        return;

    if (changeEnviroment(_params->_enviroment4))
        _params->_currentEnviroment = _params->_enviroment4;
}

bool ControllerCore::changeEnviroment(const QString &env)
{
    if(env.size() > 7 && env.contains(" "))
    {
        bool res = send(Protocol::prepareMessage(_protocol.SET_ENVIRONMENT, env.mid(env.lastIndexOf(' '))));
        if(res)
        {
            setManualRanges(_params->_currentEnviroment.mid(0, _params->_currentEnviroment.lastIndexOf(' ')),
                            _params->_currentEnviroment.mid(_params->_currentEnviroment.lastIndexOf(' ')));
        }
        return res;
    }
    return false;
}

QString ControllerCore::enviromentLowerThreshold() const
{
    if(!_params->_currentEnviroment.isEmpty())
        return _params->_currentEnviroment.mid(0, _params->_currentEnviroment.lastIndexOf(' '));
    return "";
}

QString ControllerCore::enviromentUpperThreshold() const
{
    if(!_params->_currentEnviroment.isEmpty())
        return _params->_currentEnviroment.mid(_params->_currentEnviroment.lastIndexOf(' '));
    return "";
}

bool ControllerCore::changeStorage(const QString & storage)
{
    if (storage == _params->_currentStorage)
        return true;
    std::istringstream iss;
    if (storage == "SSD" || storage == "SD_CARD" || storage == "FLASH_DRIVE")
    {
        // for the both images and videos
        if(send(Protocol::prepareMessage(_protocol.SET_IMAGE_SAVE_DEST, storage), iss) && Protocol::isPositive(iss.str())
                && send(Protocol::prepareMessage(_protocol.SET_VIDEO_SAVE_DEST, storage), iss) && Protocol::isPositive(iss.str()))
        {
            _params->_currentStorage = storage;
            return true;
        }
    }
    return false;
}

void ControllerCore::setCaptureVideoSettings(bool radJpg, bool radTiff, bool srImg, bool visJpg, bool radVid, bool visVid)
{
    std::istringstream iss;

    if(_params->_radJpg != radJpg)
        if (send(Protocol::prepareMessage(_protocol.SET_IMAGE_RADIOMETRIC_JPEG, radJpg), iss) && Protocol::isPositive(iss.str()))
            _params->_radJpg = radJpg;

    if(_params->_cameraType != _vals.WIRIS_SECURITY_TYPE)
    {
        if(_params->_radTiff != radTiff)
            if(send(Protocol::prepareMessage(_protocol.SET_IMAGE_RADIOMETRIC_TIFF,radTiff), iss) && Protocol::isPositive(iss.str()))
                _params->_radTiff = radTiff;

        if(_params->_cameraType != _vals.GIS_TYPE && _params->_srImg != srImg)
            if(send(Protocol::prepareMessage(_protocol.SET_IMAGE_SUPERRES, srImg), iss) && Protocol::isPositive(iss.str()))
                _params->_srImg = srImg;
    }

    if(_params->_visJpg != visJpg)
        if(send(Protocol::prepareMessage(_protocol.SET_IMAGE_VISIBLE_JPEG, visJpg), iss) && Protocol::isPositive(iss.str()))
            _params->_visJpg = visJpg;

    if(_params->_radVid != radVid)
    {
        if(_params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        {
            if(send(Protocol::prepareMessage(_protocol.SET_VIDEO_IR_ENCODED ,radVid), iss) && Protocol::isPositive(iss.str()))
                _params->_radVid= radVid;
        }
        else
        {
            if(send(Protocol::prepareMessage(_protocol.SET_VIDEO_IR ,radVid), iss) && Protocol::isPositive(iss.str()))
                _params->_radVid= radVid;
        }
    }
    if(_params->_visVid != visVid)
        if(send(Protocol::prepareMessage(_protocol.SET_VIDEO_VISIBLE ,visVid), iss) && Protocol::isPositive(iss.str()))
            _params->_visVid = visVid;
}

void ControllerCore::setAlarmColors(const QString& above, const QString& between, const QString& below)
{
    if (! CameraUtils::compareFw(_vals.MIN_FW_VERSION, _params->_firmwareVersion) || _params->_cameraType == _vals.WIRIS_SECURITY_TYPE)
        return;
    if (_vals.SUPPORTED_ALARM_COLORS.contains(above) && _vals.SUPPORTED_ALARM_COLORS.contains(below) && _vals.SUPPORTED_ALARM_COLORS.contains(between))
    {
        QStringList args;
        args << above << between << below;
        send(Protocol::prepareMessage(_protocol.SET_ALARMS_COLORS, args));
    }
}

void ControllerCore::setCurrentPaletteByIndex(int i)
{
    if (_params->_palettes.empty())
        return;
    if(i >= 0 && i < _params->_palettes.size())
    {
        if(send(Protocol::prepareMessage(_protocol.SET_PALETTE_BY_INDEX, i)))
        {
            _params->_currentPalette = _params->_palettes.at(i)->value();
            fetchPaletteValues();
        }
    }
}

void ControllerCore::toggleDifferentialMode(bool val)
{
    if(_params->_cameraType != _vals.GIS_TYPE)
        return;
    if(send(Protocol::prepareMessage(_protocol.SET_GIS_DIFFERENTIAL_MODE, val)))
        _params->_differentialMode = val;
}

void ControllerCore::toggleHighSensitivityMode(bool val)
{
    if (_params->_cameraType != _vals.GIS_TYPE)
        return;
    if(send(Protocol::prepareMessage(_protocol.SET_GIS_HIGH_SENSITIVITY_MODE, val)))
        _params->_highSensitivityMode = val;
}

void ControllerCore::autoFocus()
{
    if (_params->_cameraType != _vals.GIS_TYPE)
        return;
    send(Protocol::prepareMessage(_protocol.GIS_AUTO_FOCUS));
}

void ControllerCore::infiniteFocus()
{
    if (_params->_cameraType != _vals.GIS_TYPE)
        return;
    send(Protocol::prepareMessage(_protocol.GIS_INFINITE_FOCUS));
}

void ControllerCore::changeZoomThermal(bool direction)
{
    if (direction)
    {
        if(_params->_currentZoomThermal+1 < _params->_zoomsThermal.end()
                && send(Protocol::prepareMessage(_protocol.SET_THERMAL_ZOOM_INDEX, (int) (_params->_currentZoomThermal+1 - _params->_zoomsThermal.begin()))))
        {
            QMutexLocker lock(thermalMutex.get());
            _params->_currentZoomThermal++;
        }
    }
    else {
        if (_params->_currentZoomThermal > _params->_zoomsThermal.begin() && send(Protocol::prepareMessage(_protocol.SET_THERMAL_ZOOM_INDEX, (int)(_params->_currentZoomThermal-1 - _params->_zoomsThermal.begin()))))
        {

            _params->_currentZoomThermal--;
        }
    }
}

void ControllerCore::changeZoomVisible(bool direction)
{
    if (direction)
    {
        if(_params->_currentZoomVisible+1 < _params->_zoomsVisible.end()
                && send(Protocol::prepareMessage(_protocol.SET_VISIBLE_ZOOM_INDEX, (int) (_params->_currentZoomVisible + 1 - _params->_zoomsVisible.begin()))))
            _params->_currentZoomVisible++;
    }
    else {
        if (_params->_currentZoomVisible > _params->_zoomsVisible.begin()
                && send(Protocol::prepareMessage(_protocol.SET_VISIBLE_ZOOM_INDEX, (int) (_params->_currentZoomVisible - 1 - _params->_zoomsVisible.begin()))))
            _params->_currentZoomVisible--;
    }
}
