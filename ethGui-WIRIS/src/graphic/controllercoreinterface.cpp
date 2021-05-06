#include "controllercoreinterface.h"
#include <src/camera/camerautils.h>
#include <QDebug>
ControllerCoreInterface::ControllerCoreInterface(QObject * parent)
    :QObject(parent)
{
    _params = std::make_shared<CameraParameters>();
    _connection = std::make_shared<CameraConnection>();
#ifdef __APPLE__
    _streamer = make_unique<CameraStreamer>(_connection, _params);
#else
    _streamer = std::make_unique<CameraStreamer>(_connection, _params);
#endif
    QObject::connect(_streamer.get(), &CameraStreamer::streamFrameChanged, this, &ControllerCoreInterface::streamFrameChanged);
    QObject::connect(_streamer.get(), &CameraStreamer::streamFrameVisibleChanged, this, &ControllerCoreInterface::streamFrameVisibleChanged);
    QObject::connect(_streamer.get(), &CameraStreamer::snapshotSaved, this, &ControllerCoreInterface::afterCameraSnapshot);
}

bool ControllerCoreInterface::connect(const QString &ip)
{
    _connection->_endpointIp = ip;
    if(_core == nullptr || ! _connection->_connected)
    {
        if (_connectionThread)
            _connectionThread.reset();
#ifdef __APPLE__
        _connectionThread = make_unique<ConnectionThread>(_connection->_endpointIp, _connection->_endpointPort);
#else
        _connectionThread = std::make_unique<ConnectionThread>(_connection->_endpointIp, _connection->_endpointPort);
#endif
        QObject::connect(_connectionThread.get(), &ConnectionThread::connectionFinished, this, &ControllerCoreInterface::connectionFinishedHandler);
        QObject::connect(_connectionThread.get(), &ConnectionThread::connectionStarted, this, &ControllerCoreInterface::connectionStarted);
        _connectionThread->start();
        return true;
    }
    return false;
}

void ControllerCoreInterface::stopConnect()
{
    if(!_connectionThread || !_connectionThread->isRunning())
        return;
    _connectionThread->terminate();
    _connectionThread->wait();
    _connection->_connected = false;
    emit connectedChanged(_connection->_connected, 10);
    emit connectionFinished(_connection->_connected);
}

bool ControllerCoreInterface::connectionFinishedHandler(const QString& ip)
{
    if(!_connectionThread)
        return false;
    _connection->_endpointIp = ip;
#ifdef __APPLE__
    _core = make_unique<ControllerCore>(_connection, _params, _connectionThread->networkClient());
#else
    _core = std::make_unique<ControllerCore>(_connection, _params, _connectionThread->networkClient());
#endif
    if(_connection->_connected)
    {
        std::istringstream iss;
        //Sending test message
        if(! _core->send(_protocol.prepareMessageWithDefDelim(_protocol.CHECK_CONNECTION), iss) || ! _protocol.isPositive(iss.str()))
            return false;
        //Setting delimiter to null character
        _core->send(_protocol.prepareMessageWithDefDelim(_protocol.SET_DELIMITER,  "NULL"));
        //checking camery type
        _core->fetchBasicInfo();
        if(_params->_cameraType != _vals.WIRIS_PRO_TYPE && _params->_cameraType != _vals.WIRIS_SECURITY_TYPE && _params->_cameraType != _vals.GIS_TYPE)
            return false;
        // just gis has cooling system
        if(_params->_cameraType != _vals.GIS_TYPE)
            _params->_cooledDown = true;
        //we can even save it to local storage so app could remember last used ip on startup
        setLastIp(_connection->_endpointIp);
    }
    emit connectedChanged(_connection->_connected, 10);
    emit connectionFinished(_connection->_connected);
    return _connection->_connected;
}

void ControllerCoreInterface::close()
{
    if(_core && _connection->_connected)
    {
        _streamer->stopStream();
        _connection->_helperTreadFlag = false;
        _helperThread->wait();
        _core->send(Protocol::prepareMessage(_protocol.SET_ETH_MODE, Protocol::boolToString(false)));
        _core->disconnect();
        _core.reset();
    }
}

bool ControllerCoreInterface::isActivated(){
    std::istringstream iss;

    if(!_core->send(Protocol::prepareMessage(_protocol.IS_ACTIVATED), iss))
        return false;

    return Protocol::isPositive(iss.str());
}

bool ControllerCoreInterface::activate(const QString & actStr)
{
    if (!isActivated())
    {
        std::istringstream iss;
        if(_core->send(Protocol::prepareMessage(_protocol.ACTIVATE, actStr), iss) && Protocol::isPositive(iss.str()))
            return true;
    }
    else
        return true;

    return false;
}

bool ControllerCoreInterface::setup()
{
    //checking whether connect function was called
    if (_connection->_connected)
    {
        if (!isActivated())
            return false;
        //Starting Ethernet Mode
        if(!_core->send(Protocol::prepareMessage(_protocol.SET_ETH_MODE, Protocol::boolToString(true))))
            return false;

        //setting SSD as capture storage by default
        _core->send(Protocol::prepareMessage(_protocol.SET_IMAGE_SAVE_DEST , QString("SSD")));
        _core->send(Protocol::prepareMessage(_protocol.SET_VIDEO_SAVE_DEST , QString("SSD")));

        //fetching data from camera to sync the app
        if(_core->fetchAll())
            notifyAll();

        _connection->_isSetup = true;
        //initiating helper thread for fetching data from WIRIS during runtime repeatedly
        //(like temperatures, extremes, GPS location, time and so on)
        if(!_connection->_helperTreadFlag)
            toggleHelperThread();
        //initiating streaming if not already in progress
        if(!_connection->_streamFlagThermal)
            _streamer->toggleStreamThermal();
        if(!_connection->_streamFlagVisible)
            _streamer->toggleStreamVisible();
    }
    else
        return false;
    return true;
}

void ControllerCoreInterface::notifyAll()
{
    if(_params->_cameraType != _vals.WIRIS_SECURITY_TYPE)
    {
        emit enviroment1Changed();
        emit enviroment2Changed();
        emit enviroment3Changed();
        emit enviroment4Changed();
        emit currentEnviromentChanged();
        emit rangeModeChanged(_params->_rangeMode);
        emit emissivityChanged(_params->_emissivity);
        emit reflectedTempChanged(_params->_reflectedTemp);
        emit atmosphericTempChanged(_params->_atmosphericTemp);
        emit alarmModeChanged(_params->_alarmMode);
        emit alarmFromChanged(_params->_alarmFrom);
        emit alarmToChanged(_params->_alarmTo);
        emit alarmColorsChanged(_vals.ALARM_COLOR_TO_INDEX.at(_params->_alarmAboveColor), _vals.ALARM_COLOR_TO_INDEX.at(_params->_alarmBetweenColor), _vals.ALARM_COLOR_TO_INDEX.at(_params->_alarmBelowColor));
        emit spanCenterChanged(QString::number(round(_params->_spanCenter.toDouble()*100)/100));
        emit spanWindowChanged(QString::number(round(_params->_spanWindow.toDouble()*100)/100));
        emit thermalUnitChanged();
    }
    if(_params->_cameraType != _vals.GIS_TYPE)
    {
        emit syncShutterChanged(_params->_syncShutter);
        emit shutterPeriodChanged(_params->_shutterPeriodMin);
    }
    else
    {
        emit differentialModeChanged(_params->_differentialMode);
        emit highSensitivityModeChanged(_params->_highSensitivityMode);
    }
    // common
    emit typeChanged(_params->_cameraType);
    emit serialNumberChanged(_params->_serialNumber);
    emit articleNumberChanged(_params->_articleNumber);
    emit firmwareVersionChanged(_params->_firmwareVersion);
    emit ipAdressChanged(_connection->_endpointIp);
    emit paletteTopChanged(_params->_paletteTop);
    emit paletteBottomChanged(_params->_paletteBottom);
    emit paletteTopChanged(_params->_paletteTop);
    emit paletteBottomChanged(_params->_paletteBottom);
    emit paletteTopChanged(_params->_paletteTop);
    emit paletteBottomChanged(_params->_paletteBottom);
    emit palettesChanged();
    emit paletteValuesChanged();
    emit currentZoomThermalChanged(*_params->_currentZoomThermal);
    emit currentZoomVisibleChanged(*_params->_currentZoomVisible);
    emit capRecSettingChanged(_params->_radJpg, _params->_radTiff, _params->_srImg, _params->_visJpg, _params->_radVid, _params->_visVid);
    emit manualRange2Changed(_params->_manualRange2);
    emit manualRange1Changed(_params->_manualRange1);
    emit currentPaletteChanged(_params->_currentPalette);
}

bool ControllerCoreInterface::compareFw(const QString& currentV, const QString& updateV) const
{
    return CameraUtils::compareFw(currentV, updateV);
}

void ControllerCoreInterface::setEmissivity(double val)
{
    if(_core && _connection->_connected)
        _core->setEmissivity(val);
    emit emissivityChanged(_params->_emissivity);
}

void ControllerCoreInterface::setReflectedTemp(double val)
{
    if(_core && _connection->_connected)
        _core->setReflectedTemp(val);
    emit reflectedTempChanged(_params->_reflectedTemp);
}

void ControllerCoreInterface::setAtmosphericTemp(double val)
{
    if(_core && _connection->_connected)
        _core->setAtmosphericTemp(val);
    emit atmosphericTempChanged(_params->_atmosphericTemp);
}

void ControllerCoreInterface::setAlarmColors(const QString& above, const QString& between, const QString& below)
{
    if(_core && _connection->_connected)
        _core->setAlarmColors(above, between, below);
}

void ControllerCoreInterface::setCurrentPaletteByIndex(int i){
    if(_core && _connection->_connected)
        _core->setCurrentPaletteByIndex(i);
    emit paletteValuesChanged();
    emit currentPaletteChanged(_params->_currentPalette);
}

void ControllerCoreInterface::setCaptureVideoSettings(bool radJpg, bool radTiff, bool srImg, bool visJpg, bool radVid, bool visVid)
{
    if(_core && _connection->_connected)
        _core->setCaptureVideoSettings(radJpg, radTiff, srImg, visJpg, radVid, visVid);
    emit capRecSettingChanged(_params->_radJpg, _params->_radTiff,_params-> _srImg, _params->_visJpg, _params->_radVid, _params->_visVid);
}

bool ControllerCoreInterface::changeStorage(const QString & storage)
{
    if(!_core || ! _connection->_connected)
        return false;
    if (storage == _params->_currentStorage)
        return true;
    return _core->changeStorage(storage);
}

void ControllerCoreInterface::setManualRange1(const QString& val)
{
    if(_core && _connection->_connected)
        _core->setManualRange1(val);
    emit manualRange1Changed(_params->_manualRange1);
    emit paletteBottomChanged(_params->_paletteBottom);
}

void ControllerCoreInterface::setManualRange2(const QString& val)
{
    if(_core && _connection->_connected)
        _core->setManualRange2(val);
    emit manualRange2Changed(_params->_manualRange2);
    emit paletteTopChanged(_params->_paletteTop);
}

void ControllerCoreInterface::setSpanCenter(const QString& val)
{
    if(_core && _connection->_connected)
        _core->setSpanCenter(val);
    emit spanCenterChanged(_params->_spanCenter);
    emit paletteTopChanged(_params->_paletteTop);
    emit paletteBottomChanged(_params->_paletteBottom);
}

void ControllerCoreInterface::setSpanWindow(const QString& val)
{
    if(_core && _connection->_connected)
        _core->setSpanWindow(val);
    emit spanWindowChanged(_params->_spanWindow);
    emit paletteTopChanged(_params->_paletteTop);
    emit paletteBottomChanged(_params->_paletteBottom);
}

void ControllerCoreInterface::setRangeMode(int value)
{
    if(_core && _connection->_connected)
        _core->setRangeMode(value);
    emit rangeModeChanged(_params->_rangeMode);
    if(_params->_rangeMode == 1)
    {
        _core->fetchManualRange();
        emit manualRange2Changed(_params->_manualRange2);
        emit manualRange1Changed(_params->_manualRange1);
        emit paletteTopChanged(_params->_paletteTop);
        emit paletteBottomChanged(_params->_paletteBottom);
    }
    else if (_params->_rangeMode == 2)
    {
        _core->fetchSpanRange();
        emit spanCenterChanged(QString::number(round(_params->_spanCenter.toDouble()*100)/100));
        emit spanWindowChanged(QString::number(round(_params->_spanWindow.toDouble()*100)/100));
        emit paletteTopChanged(_params->_paletteTop);
        emit paletteBottomChanged(_params->_paletteBottom);
    }
}

void ControllerCoreInterface::setAlarmMode(int val)
{
    if(_core && _connection->_connected)
        _core->setAlarmMode(val);
    emit alarmModeChanged(_params->_alarmMode);
}

void ControllerCoreInterface::setAlarmFrom(double val)
{
    if(_core && _connection->_connected)
        _core->setAlarmFrom(val);
    emit alarmFromChanged(_params->_alarmFrom);
}

void ControllerCoreInterface::setAlarmTo(double val)
{
    if(_core && _connection->_connected)
        _core->setAlarmTo(val);
    emit alarmToChanged(_params->_alarmTo);
}

void ControllerCoreInterface::setMainCamera(const QString& val)
{
    _params->_mainCamera = val;
}

int ControllerCoreInterface::currentPaletteIndex()
{
    for(int i = 0; i < _params->_palettes.size(); ++i)
        if(_params->_palettes.at(i)->value().contains(_params->_currentPalette))
            return i;
    return -1;
}

void ControllerCoreInterface::setSyncShutter(bool val)
{
    if(_core && _connection->_connected)
        _core->setSyncShutter(val);
    emit syncShutterChanged(_params->_syncShutter);
}

void ControllerCoreInterface::setInterpolation(bool val)
{
    if(_core && _connection->_connected)
        _core->setInterpolation(val);
    emit interpolationChanged(_params->_interpolation);
}

void ControllerCoreInterface::setMavlinkLogging(bool val)
{
    if(_core && _connection->_connected)
        _core->setMavlinkLogging(val);
    emit mavlinkLoggingChanged(_params->_mavlink_logging);
}

void ControllerCoreInterface::setSbusLogging(bool val)
{
    if(_core && _connection->_connected)
        _core->setSBUSLogging(val);
    emit sbusLoggingChanged(_params->_sbus_logging);
}

void ControllerCoreInterface::setThermalUnit(const QString & unit)
{
    if(_core && _connection->_connected)
        _core->setThermalUnit(QString(unit.at(0)));
    notifyAll();
}

void ControllerCoreInterface::setShutterPeriod(int val)
{
    if(_core && _connection->_connected)
        _core->setShutterPeriod(val);
    emit shutterPeriodChanged(_params->_shutterPeriodMin);
}

void ControllerCoreInterface::setEnviroment1(const QString & env)
{
    QThread *thread = QThread::create([&]{
        emit envChangeStarted();
        if(_core && _connection->_connected)
            _core->setEnviroment1(env);
        emit manualRange1Changed(_params->_manualRange1);
        emit paletteBottomChanged(_params->_paletteBottom);
        emit manualRange2Changed(_params->_manualRange2);
        emit paletteTopChanged(_params->_paletteTop);
        QThread::msleep(1000); // wait until camera finishes settings
        emit currentEnviromentChanged();
    });
    thread->start();
}

void ControllerCoreInterface::setEnviroment2(const QString & env)
{
    QThread *thread = QThread::create([&]{
        emit envChangeStarted();
        if(_core && _connection->_connected)
            _core->setEnviroment2(env);
        emit manualRange1Changed(_params->_manualRange1);
        emit paletteBottomChanged(_params->_paletteBottom);
        emit manualRange2Changed(_params->_manualRange2);
        emit paletteTopChanged(_params->_paletteTop);
        QThread::msleep(1000); // wait until camera finishes settings
        emit currentEnviromentChanged();
    });
    thread->start();
}

void ControllerCoreInterface::setEnviroment3(const QString & env)
{
    QThread *thread = QThread::create([&]{
        emit envChangeStarted();
        if(_core && _connection->_connected)
            _core->setEnviroment3(env);
        emit manualRange1Changed(_params->_manualRange1);
        emit paletteBottomChanged(_params->_paletteBottom);
        emit manualRange2Changed(_params->_manualRange2);
        emit paletteTopChanged(_params->_paletteTop);
        QThread::msleep(1000); // wait until camera finishes settings
        emit currentEnviromentChanged();
    });
    thread->start();
}

void ControllerCoreInterface::setEnviroment4(const QString & env)
{
    QThread *thread = QThread::create([&]{
        emit envChangeStarted();
        if(_core && _connection->_connected)
            _core->setEnviroment4(env);
        emit manualRange1Changed(_params->_manualRange1);
        emit paletteBottomChanged(_params->_paletteBottom);
        emit manualRange2Changed(_params->_manualRange2);
        emit paletteTopChanged(_params->_paletteTop);
        QThread::msleep(1000); // wait until camera finishes settings
        emit currentEnviromentChanged();
    });
    thread->start();
}

void ControllerCoreInterface::autoFocus()
{
    if(_core && _connection->_connected)
        _core->autoFocus();
}

void ControllerCoreInterface::infiniteFocus()
{
    if(_core && _connection->_connected)
        _core->infiniteFocus();
}

void ControllerCoreInterface::toggleStreamThermal()
{
    if(_streamer)
        _streamer->toggleStreamThermal();
}

void ControllerCoreInterface::toggleStreamVisible()
{
    if(_streamer)
        _streamer->toggleStreamVisible();
}

void ControllerCoreInterface::toggleDifferentialMode(bool val)
{
    if(_core && _connection->_connected)
        _core->toggleDifferentialMode(val);
    emit differentialModeChanged(_params->_differentialMode);
}

void ControllerCoreInterface::toggleHighSensitivityMode(bool val)
{
    if(_core && _connection->_connected)
        _core->toggleHighSensitivityMode(val);
    emit highSensitivityModeChanged(_params->_highSensitivityMode);
}

void ControllerCoreInterface::changeZoomThermal(bool direction)
{
    if(_core && _connection->_connected)
        _core->changeZoomThermal(direction);
    emit currentZoomThermalChanged(*_params->_currentZoomThermal);
}

void ControllerCoreInterface::changeZoomVisible(bool direction)
{
    if(_core && _connection->_connected)
        _core->changeZoomVisible(direction);
    emit currentZoomVisibleChanged(*_params->_currentZoomVisible);
}

void ControllerCoreInterface::triggerShutter()
{
    if(_core && _connection->_connected)
        _core->send(Protocol::prepareMessage(_protocol.PERFORM_SHUTTER));
}

void ControllerCoreInterface::setDefaultSettings()
{
    _streamer->stopStream();
    _connection->_helperTreadFlag = false;
    _core->send(Protocol::prepareMessage(_protocol.SET_DEFAULT_SETTINGS));
    close();
    QCoreApplication::quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void ControllerCoreInterface::reboot()
{
    if (!compareFw(_vals.MIN_FW_VERSION, _params->_firmwareVersion))
        return;

    _connection->_helperTreadFlag = false;
    _streamer->stopStream();
    _core->send(Protocol::prepareMessage(_protocol.REBOOT));
    close();
    QCoreApplication::quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

bool ControllerCoreInterface::connected() const
{
    return _connection->_connected;
}

bool ControllerCoreInterface::isSetup() const
{
    return _connection->_isSetup;
}

QString ControllerCoreInterface::systemTime() const
{
    return _params->_systemTime;
}

QString ControllerCoreInterface::systemDate() const
{
    return _params->_systemDate;
}

QString ControllerCoreInterface::type() const
{
    return _params->_cameraType;
}

QString ControllerCoreInterface::firmwareVersion() const
{
    return _params->_firmwareVersion;
}

QString ControllerCoreInterface::serialNumber() const
{
    return _params->_serialNumber;
}

QString ControllerCoreInterface::articleNumber() const
{
    return _params->_articleNumber;
}

bool ControllerCoreInterface::syncShutter() const
{
    return _params->_syncShutter;
}

bool ControllerCoreInterface::interpolation() const
{
    return  _params->_interpolation;
}

int ControllerCoreInterface::shutterPeriod() const
{
    return _params->_shutterPeriodMin;
}

double ControllerCoreInterface::cpuTemp() const
{
    return _params->_cpuTemp;
}

double ControllerCoreInterface::camTemp() const
{
    return _params->_camTemp;
}

double ControllerCoreInterface::paletteBottom() const
{
    return _params->_paletteBottom;
}

double ControllerCoreInterface::paletteTop() const
{
    return _params->_paletteTop;
}

QString ControllerCoreInterface::currentPalette() const
{
    return _params->_currentPalette;
}

QString ControllerCoreInterface::currentZoomThermal() const
{
    if(_params->_currentZoomThermal >= _params->_zoomsThermal.begin() && _params->_currentZoomThermal < _params->_zoomsThermal.end())
    {
        double zoom = _params->_currentZoomThermal->toDouble();
        return QString::number(zoom);
    }
    return "";
}

QString ControllerCoreInterface::currentZoomVisible() const
{
    if (_params->_currentZoomVisible >= _params->_zoomsVisible.begin() && _params->_currentZoomVisible < _params->_zoomsVisible.end())
    {
        double zoom = _params->_currentZoomVisible->toDouble();
        return QString::number(zoom);
    }
    return "";
}

QString ControllerCoreInterface::ipAddress() const
{
    return _connection->_endpointIp;
}

double ControllerCoreInterface::emissivity() const
{
    return _params->_emissivity;
}

double ControllerCoreInterface::reflectedTemp() const
{
    return _params->_reflectedTemp;
}

double ControllerCoreInterface::atmosphericTemp() const
{
    return _params->_atmosphericTemp;
}

QString ControllerCoreInterface::enviromentLowerThreshold() const
{
    return _core->enviromentLowerThreshold();
}

QString ControllerCoreInterface::enviromentUpperThreshold() const
{
    return _core->enviromentUpperThreshold();
}

bool ControllerCoreInterface::isCurrentEnviroment(const QString & env) const
{
    return env == _params->_currentEnviroment;
}

bool ControllerCoreInterface::sendCommand(const QString & mes, std::istringstream & iss){
    return _core->send(mes,iss, false);
}

QString ControllerCoreInterface::enviroment1() const
{
    return _params->_enviroment1;
}

QString ControllerCoreInterface::enviroment2() const
{
    return _params->_enviroment2;
}

QString ControllerCoreInterface::enviroment3() const
{
    return _params->_enviroment3;
}

QString ControllerCoreInterface::enviroment4() const
{
    return _params->_enviroment4;
}

int ControllerCoreInterface::rangeMode() const
{
    return _params->_rangeMode;
}

bool ControllerCoreInterface::streamingThermal() const
{
    return _connection->_streamFlagThermal;
}
#include <QDebug>
bool ControllerCoreInterface::streamingVisible() const
{
    qDebug() << "vis flag" << _connection->_streamFlagVisible;
    return _connection->_streamFlagVisible;
}

QString  ControllerCoreInterface::manualRange1() const
{
    return _params->_manualRange1;
}

QString  ControllerCoreInterface::manualRange2() const
{
    return _params->_manualRange2;
}

QString  ControllerCoreInterface::spanCenter() const
{
    return _params->_spanCenter;
}

QString  ControllerCoreInterface::spanWindow() const
{
    return _params->_spanWindow;
}

int  ControllerCoreInterface::alarmMode() const
{
    return _params->_alarmMode;
}

double  ControllerCoreInterface::alarmTo() const
{
    return _params->_alarmTo;
}

double  ControllerCoreInterface::alarmFrom() const
{
    return _params->_alarmFrom;
}

bool ControllerCoreInterface::sbusLogging() const
{
    return _params->_sbus_logging;
}

bool ControllerCoreInterface::mavlinkLogging() const
{
    return _params->_mavlink_logging;
}

QString ControllerCoreInterface::thermalUnit() const
{
    return _params->_thermalUnit;
}

QString ControllerCoreInterface::thermalUnitSign() const
{
    return _params->_thermalUnitSign;
}

QQmlListProperty<PaletteWrapper> ControllerCoreInterface::palettes()
{
    return QQmlListProperty<PaletteWrapper>(this, &_params->_palettes);
}

QQmlListProperty<PaletteValueWrapper> ControllerCoreInterface::paletteValues()
{
    return QQmlListProperty<PaletteValueWrapper>(this, &_params->_paletteValues);
}

bool ControllerCoreInterface::checkAddressFormat(const QString& ip) const
{
    QRegularExpression regex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    QRegularExpressionMatch match = regex.match(ip);
    return match.hasMatch();
}

void ControllerCoreInterface::setLastIp(const QString& ip)
{
    QFile file(_vals.LAST_IP_FILENAME);
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << ip;
        file.close();
    }
}

QString ControllerCoreInterface::getLastIp() const
{
    if(QFile::exists(_vals.LAST_IP_FILENAME))
    {
        QFile file(_vals.LAST_IP_FILENAME);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            return file.readLine();
    }
    return "";
}

void ControllerCoreInterface::mainCameraSnapshot()
{
    _streamer->mainCameraSnapshot();
}

void ControllerCoreInterface::secondaryCameraSnapshot()
{
    _streamer->secondaryCameraSnapshot();
}

void ControllerCoreInterface::afterCameraSnapshot(QString path)
{
    emit showInfoMessage("Snapshot saved to: " + path);
}

void ControllerCoreInterface::onGisThermalSwitchOff()
{
    // gis thermal was switched off, restart thermal thread
     _streamer->killStreamThermal();
     streamFrame = QImage(640, 480, QImage::Format_RGB888);
     streamFrame.fill(QColor("black"));
     QPainter p(&streamFrame);
     p.setPen(QColor("White"));
     QFont font  = p.font();
     font.setPixelSize(20);
     p.setFont(font);
     p.drawText(110, 240, "Thermal Camera was turned off.");
     p.drawText(110, 260, "Please switch the camera button to ON position\n");
     p.drawText(110, 280, " and restart the application.");
     emit streamFrameChanged();
}

void ControllerCoreInterface::paintMax(bool paint)
{
    _params->_maximum->show = paint;
}

void ControllerCoreInterface::paintMin(bool paint)
{
    _params->_minimum->show = paint;
}

void ControllerCoreInterface::paintVals(bool paint)
{
    _params->_paintVals = paint;
}
void ControllerCoreInterface::paintCenter(bool paint)
{
    _params->_paintCenter = paint;
}

void ControllerCoreInterface::triggerCapture(){
    _core->send(Protocol::prepareMessage(_protocol.CAPTURE_IMAGE));
}

void ControllerCoreInterface::toggleRecording(){
    if(!_connection->_recording && (_params->_visVid || _params->_radVid))
        _core->send(Protocol::prepareMessage(_protocol.START_RECORDING));
    else
        _core->send(Protocol::prepareMessage(_protocol.FINISH_RECORDING));
}

// HELPER HANDLE
void ControllerCoreInterface::toggleHelperThread()
{
    using namespace std::placeholders;
    auto fp = std::bind(&ControllerCoreInterface::sendCommand, this, _1, _2);
    _connection->_extremesFlag = true;
    if(!_connection->_helperTreadFlag)
    {
        _connection->_helperTreadFlag = true;
#ifdef __APPLE__
        _helperThread = make_unique<HelperThread>(&_connection->_helperTreadFlag, &_params->_cooledDown, &_connection->_extremesFlag, fp, _params->_maximum, _params->_minimum, _params->_center, &_params->_firmwareVersion, &_params->_cameraType);
#else
        _helperThread = std::make_unique<HelperThread>(&_connection->_helperTreadFlag, &_params->_cooledDown, &_connection->_extremesFlag, fp, _params->_maximum, _params->_minimum, _params->_center, &_params->_firmwareVersion, &_params->_cameraType);
#endif
        QObject::connect(_helperThread.get(), &HelperThread::extremesChanged, this, &ControllerCoreInterface::onHelperExtremesChanged);
        QObject::connect(_helperThread.get(), &HelperThread::tempsChanged, this, &ControllerCoreInterface::onHelperTempsChanged);
        QObject::connect(_helperThread.get(), &HelperThread::captureRecordingChanged , this, &ControllerCoreInterface::onHelpercaptureRecordingChanged);
        QObject::connect(_helperThread.get(), &HelperThread::driveStateChanged , this, &ControllerCoreInterface::onHelperdriveStateChanged);
        QObject::connect(_helperThread.get(), &HelperThread::dateTimeChanged , this, &ControllerCoreInterface::onHelperDateTimeChanged);
        QObject::connect(_helperThread.get(), &HelperThread::cooldownTimeChanged , this, &ControllerCoreInterface::onCooldownTimeChanged);
        QObject::connect(_helperThread.get(), &HelperThread::connectionStateChanged, this, &ControllerCoreInterface::onHelperConnectionStateChanged);
        QObject::connect(_helperThread.get(), &HelperThread::gpsChanged, this, &ControllerCoreInterface::onHelperGpsChanged);
        QObject::connect(_helperThread.get(), &HelperThread::shutterDataChanged, this, &ControllerCoreInterface::shutterDataChanged);
        QObject::connect(_helperThread.get(), &HelperThread::gisThermalSwitchOff, this, &ControllerCoreInterface::onGisThermalSwitchOff);

        _helperThread->start();
    }
    else _connection->_helperTreadFlag = false;
}

void ControllerCoreInterface::onHelperExtremesChanged()
{
    if(_params->_rangeMode == 0)
    {
        _params->_paletteTop = _params->_maximum->val;
        _params->_paletteBottom = _params->_minimum->val;
        emit paletteTopChanged(_params->_paletteTop);
        emit paletteBottomChanged(_params->_paletteBottom);
    }
    emit extremesChanged();
}

void ControllerCoreInterface::onHelperTempsChanged(double cpu, double cam){
    _params->_cpuTemp = cpu;
    _params->_camTemp = cam;
    emit cpuTempChanged(cpu);
    emit camTempChanged(cam);
}

void ControllerCoreInterface::onHelpercaptureRecordingChanged(bool capture, bool record)
{
    _connection->_capturing = capture;
    _connection->_recording = record;
    emit captureRecordingChanged(capture, record);
}

void ControllerCoreInterface::onHelperdriveStateChanged(const QString& ssd, const QString& ssdCap, const QString& ssdFree,
                                                        const QString& sd, const QString& sdCap, const QString& sdFree,
                                                        const QString& flash, const QString& flashCap, const QString& flashFree)
{
    _params->_ssdDrive = ssd;
    _params->_sdDrive = sd;
    _params->_flashDrive = flash;
    emit driveStateChanged(ssd, ssdCap, ssdFree, sd, sdCap, sdFree, flash, flashCap, flashFree);
}

void ControllerCoreInterface::onHelperDateTimeChanged(const QString& date, const QString& time)
{
    _params->_systemDate = date;
    _params->_systemTime = time;
    emit dateTimeChanged();
}

void ControllerCoreInterface::onCooldownTimeChanged(const QString& time)
{
    _params->_cooldownTime = time;
}

void ControllerCoreInterface::onHelperConnectionStateChanged(bool state, double ping)
{
    _connection->_connected = state;
    emit connectedChanged(state, ping);
    if(!state)
    {
        onCameraDisconnected();
    }
}

void ControllerCoreInterface::onCameraDisconnected()
{
    qCritical() << "Camera disconnected!";
    emit cameraDisconnected();
    _connection->_helperTreadFlag = false;
    _helperThread->wait();
    _streamer->stopStream();
}

void ControllerCoreInterface::onHelperGpsChanged(const QString& gps)
{
    emit gpsChanged(gps);
}
