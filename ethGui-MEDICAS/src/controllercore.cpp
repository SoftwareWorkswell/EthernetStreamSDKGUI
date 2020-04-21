#include "controllercore.h"

ControllerCore::ControllerCore(QObject * parent) : QObject (parent){}

ControllerCore::~ControllerCore()
{
    qDebug() << "desctruct core";
    if(_params._isSetup)
        send(_protocol.prepareMessage(_protocol.set_eth_mode, "FALSE"));
}

bool  ControllerCore::sendCommand(const QString & mes, std::istringstream & iss){
    return send(mes,iss, false);
}

bool  ControllerCore::isSetup(){
    return this->_params._isSetup;
}

void ControllerCore::setEthModeOff()
{
    send(_protocol.prepareMessage(_protocol.set_eth_mode, "FALSE"));
}

QString ControllerCore::getLastIp() const
{
    if(QFile::exists(_vals.lastIpFileName))
    {
        QFile file(_vals.lastIpFileName);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            return file.readLine();
    }
    return "";
}


bool ControllerCore::connect(const QString& ip)
{
    //network client class takes care of TCP layer of application
    //it takes MEDICAS ip and communication port(2240) as constructor parameters
    //from now on it is fairly easy to call method send on network client instance with any command as parameter
    //network client should automatically disconnect from MEDICAS in its destructor
    if(this->_networkClient == nullptr || !*this->_networkClient || !_connected)
        _networkClient = std::shared_ptr<NetworkClient>(new NetworkClient(ip.toStdString(), this->_params._endpointPort.toStdString()));

    if (_networkClient)
    {
        std::istringstream iss;
        //Sending test message
        if(!send(_protocol.prepareMessageWithDefDelim(_protocol.check_connection), iss) || !isPositive(iss.str()))
            return false;

        //Setting delimiter to null character
        send(_protocol.prepareMessageWithDefDelim(_protocol.set_delimiter,  "NULL"));
        _params._endpointIp = ip;

        //Starting Ethernet Mode
        send(_protocol.prepareMessage(_protocol.set_eth_mode, "TRUE"));
        send(_protocol.prepareMessage(_protocol.get_eth_mode));


        //we are now connected, so we can save passed IP as valid application state
        _params._endpointIp = ip;

        //we can even save it to local storage so app could remember last used ip on startup
        setLastIp(_params._endpointIp);

        //checking camery type
        fetchBasicInfo();
        if(_params._cameraType != _vals.medicasType)
            return false;
    }

    _connected = (_networkClient != nullptr);
    emit connectedChanged(_connected, 10);
    return _connected;

}

bool ControllerCore::isActivated(){
    std::istringstream iss;

    if(!send(_protocol.prepareMessage(_protocol.is_activated), iss))
        return false;

    return isPositive(iss.str());
}

bool ControllerCore::activate(const QString & actStr)
{
    if (!isActivated())
    {
        std::istringstream iss;
        if(send(_protocol.activate + actStr, iss) && isPositive(iss.str()))
            return true;
    }
    else return true;

    return false;
}

bool ControllerCore::setup()
{
    //checking whether connect function was called
    if (*_networkClient) {
        if (!this->isActivated())
            return false;

        //setting SSD as capture storage by default
        send(_protocol.prepareMessage(_protocol.set_save_dest, "SSD"));

        //fetching data from camera to sync the app
        fetchAll();

        _params._isSetup = true;

        //initiating streaming if not already in progress
        if (!_streamFlagThermal)
            toggleStreamThermal();
        if (!_streamFlagVisible)
            toggleStreamVisible();

        //initiating helper thread for fetching data from MEDICAS during runtime repeatedly
        //(like temperatures, extremes, GPS location, time and so on)
        if (!_helperTreadFlag)
            toggleHelperThread();

    }
    else {
        qDebug() << "networkclient not connected";
        return false;
    }
    return true;
}



void ControllerCore::close()
{
    if(_networkClient && _networkClient->is_connected())
    {
        stopStream();
        _helperTreadFlag = false;
        _networkClient->disconnect();
    }
}

void ControllerCore::forceCalibration()
{
    send(_protocol.prepareMessage(_protocol.force_calibration));
}

// reset default settings command - same for all wiris cameras
void ControllerCore::setDefaultSettings(){
    stopStream();
    _helperTreadFlag = false;
    send(_protocol.prepareMessage(_protocol.set_default_settings));
    close();
    QCoreApplication::quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

// reboot command - same for all wiris cameras
void ControllerCore::reboot(){
    _helperTreadFlag = false;
    stopStream();
    send(_protocol.prepareMessage(_protocol.reboot));
    close();
    QCoreApplication::quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

bool ControllerCore::changeStorage(const QString & storage){
    if (storage == _captureParams._currentStorage)
        return true;

    std::istringstream iss;
    std::string cache;

    if (storage == "SSD" || storage == "SD_CARD" || storage == "FLASH_DRIVE")
    {
        if (send("SILC " + storage, iss) && isPositive(iss.str()))
        {
            _captureParams._currentStorage = storage;
            return true;
        }
    }

    return false;
}

bool ControllerCore::compareFw(const QString& older, const QString& newer) const{
    return compareVersion(older, newer);
}

QQmlListProperty<PaletteWrapper> ControllerCore::palettes()  {
    return QQmlListProperty<PaletteWrapper>(this, _palettes);
}

QQmlListProperty<PaletteValueWrapper> ControllerCore::paletteValues() {
    return QQmlListProperty<PaletteValueWrapper>(this, _paletteValues);
}

int ControllerCore::alarmMode() const
{
    return this->_params._alarmMode;
}

double ControllerCore::alarmCritical() const
{
    return this->_params._alarmCritical;
}

double ControllerCore::alarmPreventive() const
{
    return this->_params._alarmPreventive;
}

bool ControllerCore::showCrossMax() const
{
    return this->_params._showCrossMax;
}

bool ControllerCore::showCrossCenter() const
{
    return this->_params._showCrossCenter;
}

QString ControllerCore::blackbodyStatus() const
{
    return this->_params._blackbodyStatus;
}

bool ControllerCore::showBlackbodyRoi() const
{
    return this->_params._showBlackbodyRoi;
}

int ControllerCore::gamma() const
{
    return this->_params._gamma;
}

int ControllerCore::whiteBalance() const
{
    return this->_params._whiteBalance;
}

bool ControllerCore::backlightCompensation()
{
    return this->_params._backlightCompensation;
}

bool ControllerCore::wideDynamicRangeMode()
{
    return this->_params._backlightCompensation;
}

int ControllerCore::noiseReduction2D()
{
    return this->_params._noiseReduction2D;
}

int ControllerCore::noiseReduction3D()
{
    return this->_params._noiseReduction3D;
}

int ControllerCore::colorGain()
{
    return this->_params._colorGain;
}

int ControllerCore::gain()
{
    return this->_params._gain;
}

int ControllerCore::exposureMode()
{
    return this->_params._exposureMode;
}

int ControllerCore::exposureCompensation()
{
    return this->_params._exposureCompensation;
}

QString ControllerCore::preventiveColor()
{
    return this->_params._preventiveColor;
}

QString ControllerCore::criticalColor()
{
    return this->_params._criticalColor;
}

int ControllerCore::alarmOpacity()
{
    return this->_params._alarmOpacity;
}

int ControllerCore::iris() {
    return this->_params._iris;
}

QString ControllerCore::units()
{
    return this->_params._units;
}

QString ControllerCore::language()
{
    return this->_params._language;
}

void ControllerCore::setVisShutterPeriod(double val)
{
    std::istringstream iss;
    if (val >= 2 && val <= 30)
    {
        if (send(_protocol.prepareMessage(_protocol.set_vis_shutter, QString::number(val)), iss) && isPositive(iss.str())){
            _params._visShutterPeriod = val;
        }
    }
    emit visShutterPeriodChanged(_params._visShutterPeriod);
}



void ControllerCore::triggerCapture(){
    send(_protocol.prepareMessage(_protocol.capture_image));
}

void ControllerCore::toggleRecording(){
    if(!_networkClient || ! _networkClient->is_connected())
        return;
    if(!_captureParams._recording && (_captureParams._visVid || _captureParams._encIRVid))
        send(_protocol.prepareMessage(_protocol.start_recording));

    else send(_protocol.prepareMessage(_protocol.finish_recording));

}

void ControllerCore::toggleStreamThermal(){

    if (!this->_params._isSetup)
        return;

    if (!_streamFlagThermal)
    {
        _streamFlagThermal = true;
        _thermalThread = new ThermalThread(&this->_streamFlagThermal, this->_params._endpointIp, _params._maximum, _params._center, _params._blackbody,  &_params._showCrossMax, &_params._showCrossCenter, &_params._showBlackbodyRoi, &_params._cameraType);
        QObject::connect(_thermalThread, &ThermalThread::imageSourceChanged, this, &ControllerCore::onThermalFrameChanged);        
        _thermalThread->start();
    } else _streamFlagThermal = false;
}

void ControllerCore::toggleStreamVisible(){
    if (!this->_params._isSetup)
        return;

    if (!_streamFlagVisible)
    {
        _streamFlagVisible = true;
        _visibleThread = new VisibleThread(&this->_streamFlagVisible, this->_params._endpointIp);
        QObject::connect(_visibleThread, &VisibleThread::imageSourceChanged, this, &ControllerCore::onVisibleFrameChanged);
        _visibleThread->start();
    }
    else _streamFlagVisible = false;
}

void ControllerCore::stopStream(){
    this->_streamFlagThermal = false;
    this->_streamFlagVisible = false;
}

void ControllerCore::setAlarmColors(const QString& preventive, const QString& critical)
{
    if (_vals.supportedAlarmColors.contains(preventive) && _vals.supportedAlarmColors.contains(critical))
    {
        if(send(_protocol.prepareMessage(_protocol.set_thresholds_colors, preventive + " " + critical)))
        {
            _params._preventiveColor = preventive;
            _params._criticalColor = critical;
            emit preventiveColorChanged(_params._preventiveColor);
            emit criticalColorChanged(_params._criticalColor);
        }
    }
}

void ControllerCore::setCaptureVideoSettings(bool radJpg, bool visJpg, bool encIRVid, bool visVid)
{
    std::istringstream iss;

    if (_captureParams._radJpg != radJpg)
        if (send(_protocol.prepareMessage(_protocol.set_image_radiometric_jpeg, boolToString(radJpg)), iss) && isPositive(iss.str()))
            _captureParams._radJpg = radJpg;
    if (_captureParams._visJpg != visJpg)
        if (send(_protocol.prepareMessage(_protocol.set_image_visible_jpeg, boolToString(visJpg)), iss) && isPositive(iss.str()))
            _captureParams._visJpg = visJpg;
    if (_captureParams._encIRVid != encIRVid)
        if (send(_protocol.prepareMessage(_protocol.set_video_ir_encoded, boolToString(visVid)), iss) && isPositive(iss.str()))
            _captureParams._visVid = visVid;
    if (_captureParams._visVid != visVid)
        if (send(_protocol.prepareMessage(_protocol.set_video_visible, boolToString(visVid)), iss) && isPositive(iss.str()))
            _captureParams._visVid = visVid;
    emit capRecSettingChanged(_captureParams._radJpg,_captureParams._visJpg, _captureParams._encIRVid, _captureParams._visVid);
}

void ControllerCore::setCurrentPaletteByIndex(int i){
    if (_palettes.empty())
        return;
    if(i >= 0 && i < _palettes.size())
    {
        if (send(_protocol.prepareMessage(_protocol.set_palette_by_index, QString::number(i))))
        {
            this->_params._currentPalette = _palettes.at(i)->value();
            fetchPaletteValues();
            emit currentPaletteChanged(_params._currentPalette);
        }
    }
}

void ControllerCore::setCurrentPaletteByName(int i){
    if (_palettes.empty())
        return;
    if(i >= 0 && i < _palettes.size())
    {
        if (send(_protocol.prepareMessage(_protocol.set_palette_by_name, _palettes.at(i)->value())))
        {
            this->_params._currentPalette = _palettes.at(i)->value();
            fetchPaletteValues();
            emit currentPaletteChanged(_params._currentPalette);
        }
    }
}


void ControllerCore::setVisibleCameraDefaults()
{
    if(!send(_protocol.prepareMessage(_protocol.set_visible_default)))
        return;
    fetchGamma();
    fetchGains();
    fetchExposure();
    fetchIris();
    fetchWhiteBalance();
    fetchNoiseReduction();
    fetchDynamicRangeMode();
    fetchBacklightCompensation();
}

bool ControllerCore::send(const QString &message){
    std::istringstream s;
    return send(message, s);
}

bool ControllerCore::send(const QString &message, std::istringstream & answ, bool debugPrint)
{
    QMutexLocker locker(connectionMutex);

    if (_networkClient == nullptr || !*_networkClient)
        return  false;

    std::string answer;
    int length;

    _networkClient->write(message.toStdString());
    length = _networkClient->read_until(answer, '\0', std::chrono::seconds(10));

    QString log = "Command: " + message;
    log += "   Answer: ";
    if (length > 0) {
        log += QString::fromStdString(answer);
    }

    if (debugPrint)
        qDebug() << log;

    answ = std::istringstream(answer);

    return !(QString::fromStdString(answer).contains("ERR") || answer.empty());
}


void ControllerCore::toggleHelperThread()
{
    using namespace std::placeholders;
    auto fp = std::bind(&ControllerCore::sendCommand, this, _1, _2);
    _extremesFlag = true;
    if (!_helperTreadFlag)
    {
        _helperTreadFlag = true;
        _helperThread = new HelperThread(&_helperTreadFlag, &_extremesFlag, fp, _params._maximum, _params._center, _params._blackbody, &_params._firmwareVersion, &_params._cameraType);
        QObject::connect(_helperThread, &HelperThread::extremesChanged, this, &ControllerCore::onHelperExtremesChanged);
        QObject::connect(_helperThread, &HelperThread::tempsChanged, this, &ControllerCore::onHelperTempsChanged);
        QObject::connect(_helperThread, &HelperThread::captureRecordingChanged , this, &ControllerCore::onHelpercaptureRecordingChanged);
        QObject::connect(_helperThread, &HelperThread::driveStateChanged , this, &ControllerCore::onHelperdriveStateChanged);
        QObject::connect(_helperThread, &HelperThread::dateTimeChanged , this, &ControllerCore::onHelperDateTImeChanged);
        QObject::connect(_helperThread, &HelperThread::connectionStateChanged, this, &ControllerCore::onHelperConnectionStateChanged);
        _helperThread->start();
    }
    else _helperTreadFlag = false;
}

void ControllerCore::onHelperExtremesChanged()
{
    emit extremesChanged();
}

void ControllerCore::onHelperTempsChanged(double cpu, double cam){
    _params._cpuTemp = cpu;
    _params._camTemp = cam;
    emit cpuTempChanged(cpu);
    emit camTempChanged(cam);
}

void ControllerCore::onHelpercaptureRecordingChanged(bool capture, bool record)
{
    _captureParams._capturing = capture;
    _captureParams._recording = record;
    emit captureRecordingChanged(capture, record);
}

void ControllerCore::onHelperdriveStateChanged(const QString& ssd, const QString& ssdCap, const QString& ssdFree,
                                               const QString& sd, const QString& sdCap, const QString& sdFree,
                                               const QString& flash, const QString& flashCap, const QString& flashFree)
{
    _captureParams._ssdDrive = ssd;
    _captureParams._sdDrive = sd;
    _captureParams._flashDrive = flash;
    emit driveStateChanged(ssd, ssdCap, ssdFree, sd, sdCap, sdFree, flash, flashCap, flashFree);
}

void ControllerCore::onHelperDateTImeChanged(const QString& date, const QString& time){
    _systemDate = date;
    _systemTime = time;
    emit dateTimeChanged();
}

void ControllerCore::onHelperConnectionStateChanged(bool state, double ping){
    _connected = state;
    emit connectedChanged(state, ping);
}

void ControllerCore::fetchManualRange()
{
    std::istringstream iss;
    std::string cache;
    if(send(_protocol.prepareMessage(_protocol.get_range), iss))
    {
        std::getline(iss, cache, ' ');
        this->_params._manualRange1 = QString::fromStdString(cache);
        getAnyLine(iss, cache);
        this->_params._manualRange2 = QString::fromStdString(cache);
        emit manualRange2Changed(_params._manualRange2);
        emit manualRange1Changed(_params._manualRange1);

        _params._paletteTop = _params._manualRange2.toDouble();
        _params._paletteBottom = _params._manualRange1.toDouble();
        emit paletteTopChanged(_params._paletteTop);
        emit paletteBottomChanged(_params._paletteBottom);

        qDebug() << "fetched manual range 1: " << _params._manualRange1 <<" r2: " << _params._manualRange2;
    }
}

void ControllerCore::fetchMainCamera()
{
    std::istringstream iss;
    std::string cache;

    if(send(_protocol.prepareMessage(_protocol.get_main_camera), iss))
    {
        getAnyLine(iss, cache);
        this->_params._mainCamera = QString::fromStdString(cache);
        emit mainCameraChanged(_params._mainCamera);
    }
}

void ControllerCore::fetchLevelofAcceptance()
{
    std::istringstream iss;
    std::string cache;

    double level;
    if (send(_protocol.prepareMessage(_protocol.get_gain), iss))
    {
        iss >> level;
        _params._levelOfAcception = level;
        emit levelOfAcceptionChanged(_params._levelOfAcception);
    }
    this->_params._firmwareVersion = QString::fromStdString(cache);
}

void ControllerCore::fetchBlacbodyRoi()
{
    std::istringstream iss;
    std::string cache;

    if(send(_protocol.prepareMessage(_protocol.get_show_blackbody_roi), iss))
    {
        getAnyLine(iss, cache);
        this->_params._showBlackbodyRoi = stringToBool(QString::fromStdString(cache));
        emit showBlackbodyRoiChanged(_params._showBlackbodyRoi);
    }

    if (send(_protocol.prepareMessage(_protocol.get_blackbody_roi), iss))        {
        std::getline(iss, cache, ' ');
        qDebug() << QString::fromStdString( cache);

        this->_params._blackbodyRoiX = QString::fromStdString(cache).toInt();
        std::getline(iss, cache, ' ');

        this->_params._blackbodyRoiY = QString::fromStdString(cache).toInt();
        getAnyLine(iss, cache);
        _params._blackbody->val = QString::fromStdString(cache).toDouble();
        _params._blackbody->x = _params._blackbodyRoiX;
        _params._blackbody->y = _params._blackbodyRoiY;
        emit blackbodyRoiXChanged(_params._blackbodyRoiX);
        emit blackbodyRoiYChanged(_params._blackbodyRoiY);
    }
    if (send(_protocol.prepareMessage(_protocol.get_blackbody_status), iss))        {
        std::getline(iss, cache, '\n');
        this->_params._blackbodyStatus = QString::fromStdString(cache);
        emit blackbodyStatusChanged(_params._blackbodyStatus);
    }
}

void  ControllerCore::fetchShowCrosses()
{
    std::string cache;
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.get_show_cross_max), iss))
    {
        getAnyLine(iss, cache);
        this->_params._showCrossMax = stringToBool(QString::fromStdString(cache));
        emit showCrossMaxChanged(_params._showCrossMax);
    }
    if(send(_protocol.prepareMessage(_protocol.get_show_cross_max), iss))
    {
        getAnyLine(iss, cache);
        this->_params._showCrossCenter = stringToBool(QString::fromStdString(cache));
        emit showCrossCenterChanged(_params._showCrossCenter);
    }
}

void ControllerCore::fetchPalettes(){
    _palettes.clear();
    std::string cache;
    std::istringstream iss;
    if (send(_protocol.prepareMessage(_protocol.get_palette_name_index), iss))
    {
        getAnyLine(iss, cache);
        this->_params._currentPalette = QString::fromStdString(cache);
        this->_params._currentPalette = this->_params._currentPalette.split(" ")[1];
        qDebug() << QString::fromStdString(cache) << _params._currentPalette;
        send(_protocol.prepareMessage(_protocol.set_palette_by_name, _params._currentPalette));
    }

    if (send(_protocol.prepareMessage(_protocol.get_all_palettes), iss))
    {
        while(getAnyLine(iss, cache))
        {
            if(!cache.empty())
            {
                this->_palettes.push_back(new PaletteWrapper(QString::fromStdString(cache)));
            }
        }
        emit palettesChanged();
    }
}

void ControllerCore::fetchPaletteValues()
{
    std::istringstream iss, line;
    std::string cache, val;
    QString r, g, b;

    _paletteValues.clear();

    if (send(_protocol.get_palette_values, iss))
    {
        while (getAnyLine(iss, cache))
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

                _paletteValues.push_back(new PaletteValueWrapper(r, g, b));
            }
        }
        emit paletteValuesChanged();
    }
}

void ControllerCore::fetchAll()
{
    fetchBasicInfo();
    fetchManualRange();
    fetchMainCamera();
    fetchLevelofAcceptance();
    fetchBlacbodyRoi();
    fetchShowCrosses();
    fetchPalettes();
    fetchPaletteValues();
    fetchCaptureRecSettings();
    fetchAlarms();
    fetchExposure();
    fetchNoiseReduction();
    fetchGains();
    fetchIris();
    fetchGamma();
    fetchWhiteBalance();
    fetchBacklightCompensation();
    fetchDynamicRangeMode();
    fetchSystemSettings();
}

void ControllerCore::fetchBasicInfo()
{
    std::istringstream iss;
    std::string cache;

    send(_protocol.prepareMessage(_protocol.get_fw_version), iss);
    getAnyLine(iss, cache);
    this->_params._firmwareVersion = QString::fromStdString(cache);

    send(_protocol.prepareMessage(_protocol.get_serial_number), iss);
    getAnyLine(iss, cache);
    this->_params._serialNumber = QString::fromStdString(cache);

    send(_protocol.prepareMessage(_protocol.get_article_number), iss);
    getAnyLine(iss, cache);
    this->_params._articleNumber = QString::fromStdString(cache);

    this->_params._cameraType = _vals.medicasType;

    emit typeChanged(_params._cameraType);
    emit serialNumberChanged(_params._serialNumber);
    emit articleNumberChanged(_params._articleNumber);
    emit firmwareVersionChanged(_params._firmwareVersion);
    emit ipAdressChanged(_params._endpointIp);
}


void ControllerCore::fetchCaptureRecSettings(){
    std::istringstream iss;

    send(_protocol.prepareMessage(_protocol.get_image_radiometric_jpeg), iss);
    _captureParams._radJpg = isPositive(iss.str());

    send(_protocol.prepareMessage(_protocol.get_image_visible_jpeg), iss);
    _captureParams._visJpg = isPositive(iss.str());

    send(_protocol.prepareMessage(_protocol.get_video_ir_encoded), iss);
    _captureParams._encIRVid = isPositive(iss.str());

    send(_protocol.prepareMessage(_protocol.get_video_visible), iss);
    _captureParams._visVid = isPositive(iss.str());
    emit capRecSettingChanged(_captureParams._radJpg, _captureParams._visJpg, _captureParams._encIRVid, _captureParams._visVid);

    qDebug() <<"fetched settings: " <<_captureParams._radJpg << _captureParams._visJpg<< _captureParams._encIRVid << _captureParams._visVid;

}

void ControllerCore::fetchAlarms()
{
    std::istringstream iss;
    std::string cache;
    QString qcache;
    if (send(_protocol.prepareMessage(_protocol.get_alarm_mode), iss))
    {
        getAnyLine(iss, cache);
        qcache = QString::fromStdString(cache);
        if (qcache == _vals.supportedAlarmModes[1])
            _params._alarmMode = 1;
        else if (qcache == _vals.supportedAlarmModes[0])
            _params._alarmMode = 0;
        emit alarmModeChanged(_params._alarmMode);
    }
    // fetch preventive and critical
    if (send(_protocol.prepareMessage(_protocol.get_thresholds), iss))
    {
        std::getline(iss, cache, ' ');
        _params._alarmPreventive = std::stod(cache);
        getAnyLine(iss, cache);
        _params._alarmCritical = std::stod(cache);
        emit alarmPreventiveChanged(_params._alarmPreventive);
        emit alarmCriticalChanged(_params._alarmCritical);
    }
    if(send(_protocol.prepareMessage(_protocol.get_thresholds_colors), iss))
    {
        std::getline(iss, cache,' ');
        _params._preventiveColor = QString::fromStdString(cache);
        emit preventiveColorChanged(_params._preventiveColor);
        std::getline(iss, cache,' ');
        qDebug() << "cache" << QString::fromStdString(cache);

        _params._criticalColor = QString::fromStdString(cache);
        emit criticalColorChanged(_params._criticalColor);
    }

    int op;
    if (send(_protocol.prepareMessage(_protocol.get_thresholds_opacity), iss))        {
        iss >> op;
        _params._alarmOpacity = op;
        emit alarmOpacityChanged(_params._alarmOpacity);
    }
}

void ControllerCore::fetchExposure() {
    std::istringstream iss;
    if (send(_protocol.prepareMessage(_protocol.get_exposure_mode), iss))
    {
        std::string resp;
        getAnyLine(iss, resp);
        this->_params._exposureMode = _vals.exposureModeToIndex.find(QString::fromStdString(resp))->second;
        emit exposureModeChanged(_params._exposureMode);
    }
    int compensation;
    if (send(_protocol.prepareMessage(_protocol.get_exposure_compensation), iss))
    {
        iss >> compensation;
        _params._exposureCompensation = compensation;
        emit exposureCompensationChanged(_params._exposureCompensation);
    }

}

void ControllerCore::fetchNoiseReduction() {
    std::istringstream iss;
    int val;
    if (send(_protocol.prepareMessage(_protocol.get_noise_reduction2d), iss))
    {
        iss >> val;
        _params._noiseReduction2D = val;
        emit noiseReduction2DChanged(_params._noiseReduction2D);
    }
    if (send(_protocol.prepareMessage(_protocol.get_noise_reduction3d), iss))
    {
        iss >> val;
        _params._noiseReduction3D = val;
        emit noiseReduction3DChanged(_params._noiseReduction3D);
    }
}

void ControllerCore::fetchGains() {
    std::istringstream iss;
    int gain;
    if (send(_protocol.prepareMessage(_protocol.get_gain), iss))
    {
        iss >> gain;
        _params._gain = gain;
        emit colorGainChanged(_params._gain);
    }
    if (send(_protocol.prepareMessage(_protocol.get_color_gain), iss))
    {
        iss >> gain;
        _params._colorGain = gain;
        emit colorGainChanged(_params._colorGain);
    }
}

void ControllerCore::fetchIris() {
    std::istringstream iss;
    int val;
    if(send(_protocol.prepareMessage(_protocol.get_iris), iss))
    {
        iss >> val;
        _params._iris = val;
        emit irisChanged(_params._iris);
    }
}

void ControllerCore::fetchGamma() {
    std::istringstream iss;
    if (send(_protocol.prepareMessage(_protocol.get_gamma), iss))
    {
        std::string resp;
        getAnyLine(iss, resp);
        _params._iris = _vals.gammaModeToIndex.find(QString::fromStdString(resp))->second;
        emit gammaChanged(_params._gamma);
    }
}

void ControllerCore::fetchWhiteBalance() {
    std::istringstream iss;
    if (send(_protocol.prepareMessage(_protocol.get_white_balance), iss))
    {
        std::string resp;
        getAnyLine(iss, resp);
        _params._iris = _vals.whiteBalanceModeToIndex.find(QString::fromStdString(resp))->second;
        emit whiteBalanceChanged(_params._whiteBalance);
    }
}

void ControllerCore::fetchBacklightCompensation() {
    std::istringstream iss;
    std::string cache;

    if(send(_protocol.prepareMessage(_protocol.get_backlight_compensation), iss))
    {
        getAnyLine(iss, cache);
        this->_params._backlightCompensation = stringToBool(QString::fromStdString(cache));
        emit backlightCompensationChanged(_params._backlightCompensation);
    }
}

void ControllerCore::fetchDynamicRangeMode()
{
    std::istringstream iss;
    std::string cache;

    send(_protocol.prepareMessage(_protocol.get_wide_range_mode), iss);
    getAnyLine(iss, cache);
    this->_params._dynamicRangeMode = (cache == "ON" ? true : false);
}

void ControllerCore::fetchSystemSettings()
{
    std::istringstream iss;
    std::string cache;
    if(send(_protocol.prepareMessage(_protocol.get_units), iss))
    {
        getAnyLine(iss, cache);
        this->_params._units = QString::fromStdString(cache);
        emit unitsChanged(_params._units);
    }
    if(send(_protocol.prepareMessage(_protocol.get_language), iss))
    {
        getAnyLine(iss, cache);
        this->_params._language = QString::fromStdString(cache);
        emit languageChanged(_params._language);
    }
}

double ControllerCore::visShutterPeriod(){
    return  _params._visShutterPeriod;
}

double ControllerCore::ControllerCore::cpuTemp() const{
    return _params._cpuTemp;
}
double ControllerCore::camTemp() const{
    return _params._camTemp;
}

double ControllerCore::ControllerCore::deviceTemp() const{
    return _params._deviceTemp;
}

QString ControllerCore::type() const
{
    return this->_params._cameraType;
}

double ControllerCore::paletteBottom() const{
    return this->_params._paletteBottom;
}

double ControllerCore::paletteTop() const{
    return this->_params._paletteTop;
}

QString ControllerCore::currentPalette() const{
    return this->_params._currentPalette;
}

QString ControllerCore::ipAddress() const{
    return  this->_params._endpointIp;
}

QString ControllerCore::firmwareVersion() const
{
    return this->_params._firmwareVersion;
}

QString ControllerCore::serialNumber() const
{
    return this->_params._serialNumber;
}

QString ControllerCore::articleNumber() const
{
    return this->_params._articleNumber;
}

QString ControllerCore::manualRange1() const{
    return this->_params._manualRange1;
}

QString ControllerCore::manualRange2() const{
    return this->_params._manualRange2;
}

QString ControllerCore::mainCamera() const
{
    return this->_params._mainCamera;
}

double ControllerCore::levelOfAcception() const
{
    return this->_params._levelOfAcception;
}

int ControllerCore::blackbodyRoiX() const
{
    return this->_params._blackbodyRoiX;
}

int ControllerCore::blackbodyRoiY() const
{
    return this->_params._blackbodyRoiY;
}

void ControllerCore::setManualRange1(const QString& val)
{
    if(val == _params._manualRange1 )
        return;

    if( val.toDouble() < _params._manualRange2.toDouble())
    {
        std::istringstream iss;
        std::string cache;
        std::ostringstream oss;
        oss << _protocol.set_range.toStdString() << " " << val.toDouble() << " " << _params._manualRange2.toDouble();

        if(send(QString::fromStdString(oss.str()), iss))
        {
            if (isOk(iss.str()))
            {
                _params._manualRange1 = val;
                emit manualRange1Changed(_params._manualRange1);
                _params._paletteBottom = _params._manualRange1.toDouble();
                emit paletteBottomChanged(_params._paletteBottom);

            }
        }
    }
}

void ControllerCore::setManualRange2(const QString& val)
{
    if (val == _params._manualRange2)
        return;

    if( val.toDouble() > _params._manualRange1.toDouble())
    {
        std::istringstream iss;
        std::string cache;
        std::ostringstream oss;

        oss <<_protocol.set_range.toStdString() << " " << _params._manualRange1.toDouble() << " " << val.toDouble();

        if(send(QString::fromStdString(oss.str()), iss))
        {
            if (isOk(iss.str()))
            {
                _params._manualRange2 = val;
                emit manualRange2Changed(_params._manualRange2);
                _params._paletteTop = _params._manualRange2.toDouble();
                emit paletteTopChanged(_params._paletteTop);
            }
        }
    }
}
void ControllerCore::setMainCamera(const QString& val)
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_main_camera, val), iss))
    {
        if (isOk(iss.str()))
        {
            _params._mainCamera = val;
            emit mainCameraChanged(_params._mainCamera);
        }
    }
}

void ControllerCore::setLevelOfAcception(double val)
{
    if(val < 0 || val > 3)
        return;
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_level_of_acception, QString::number(val)), iss) && isPositive(iss.str()))
    {
        _params._alarmPreventive = val;
        emit alarmPreventiveChanged(_params._alarmPreventive);
    }
}


void ControllerCore::setBlackbodyRoiX(int val)
{
    if(val < 0)
        return;
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_blackbody_roi, QString::number(val) + " " + QString::number(_params._blackbodyRoiY)), iss) && isPositive(iss.str()))
    {
        _params._blackbodyRoiX = val;
        _params._blackbody->x = _params._blackbodyRoiX;
        emit blackbodyRoiXChanged(_params._blackbodyRoiX);
    }
}

void ControllerCore::setBlackbodyRoiY(int val)
{
    if(val < 0)
        return;
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_blackbody_roi, QString::number(_params._blackbodyRoiX) + " " + QString::number(val)), iss) && isPositive(iss.str()))
    {
        _params._blackbodyRoiY = val;
        _params._blackbody->y = _params._blackbodyRoiY;
        emit blackbodyRoiYChanged(_params._blackbodyRoiY);
    }
}

// in medicas alarm mode is just ON/OFF
void ControllerCore::setAlarmMode(int val)
{
    if (val < 0 || val > 4)
        return;
    QString req = _protocol.set_alarm_mode;
    req += " " + _vals.supportedAlarmModes[val];
    std::istringstream iss;

    if (send(req, iss) && isPositive(iss.str()))
        _params._alarmMode = val;
    emit alarmModeChanged(val);

}

void ControllerCore::setAlarmPreventive(double val)
{
    if (val <= _params._alarmCritical)
    {
        std::istringstream iss;
        if(send(_protocol.prepareMessage(_protocol.set_thresholds, QString::number(val) + " " + QString::number(_params._alarmCritical)), iss) && isPositive(iss.str()))
        {
            _params._alarmPreventive = val;
            emit alarmPreventiveChanged(_params._alarmPreventive);
        }
    }
}

void ControllerCore::setShowCrossMax(bool val)
{
    QString strval = boolToString(val);
    std::istringstream iss;
    if (send(_protocol.prepareMessage(_protocol.set_show_cross_max, strval), iss) && isPositive(iss.str())){
        _params._showCrossMax = val;
        _params._maximum->show = val;
        emit showCrossMaxChanged(_params._showCrossMax);
    }
}

void ControllerCore::setShowCrossCenter(bool val)
{
    QString strval = boolToString(val);
    std::istringstream iss;
    if (send(_protocol.prepareMessage(_protocol.set_show_cross_center, strval), iss) && isPositive(iss.str())){
        _params._showCrossCenter = val;
        _params._center->show = val;
    }
    emit showCrossCenterChanged(_params._showCrossCenter);
}

void ControllerCore::setShowBlackbodyRoi(bool val)
{
    QString strval = boolToString(val);
    std::istringstream iss;
    if (send(_protocol.prepareMessage(_protocol.set_show_blackbody_roi, strval), iss) && isPositive(iss.str())){
        _params._showBlackbodyRoi = val;
        _params._blackbody->show = val;
    }
    emit showBlackbodyRoiChanged(_params._showBlackbodyRoi);
}

void ControllerCore::setGamma(int val)
{
    std::istringstream iss;
    QString param = _vals.supportedGammaModes[val];
    if(send(_protocol.prepareMessage(_protocol.set_gamma,param), iss) && isPositive(iss.str()))
    {
        _params._gamma = val;
        emit gammaChanged(_params._gamma);
    }
}

void ControllerCore::setWhiteBalance(int val)
{
    std::istringstream iss;
    QString param = _vals.supportedWhiteBalanceModes[val];
    if(send(_protocol.prepareMessage(_protocol.set_white_balance, param), iss) && isPositive(iss.str()))
    {
        _params._whiteBalance = val;
        emit whiteBalanceChanged(_params._whiteBalance);
    }
}

void ControllerCore::setBacklightCompensation(bool value)
{
    QString strval = boolToString(value);
    std::istringstream iss;
    if (send(_protocol.prepareMessage(_protocol.set_backlight_compensation, strval), iss) && isPositive(iss.str())){
        _params._backlightCompensation = value;
    }
    emit backlightCompensationChanged(_params._backlightCompensation);
}

void ControllerCore::setWideDynamicRangeMode(bool value)
{
    QString strval = boolToString(value);
    std::istringstream iss;
    if (send(_protocol.prepareMessage(_protocol.set_wide_range_mode, strval), iss) && isPositive(iss.str())){
        _params._dynamicRangeMode = value;
    }
    emit wideDynamicRangeModeChanged(_params._dynamicRangeMode);
}

void ControllerCore::setNoiseReduction2D(int val)
{
    // [0  - 5]
    if(val < 0 || val > 5)
        return;
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_noise_reduction2d, QString::number(val)), iss) && isPositive(iss.str()))
    {
        _params._noiseReduction2D = val;
        emit noiseReduction2DChanged(_params._noiseReduction2D);
    }
}

void ControllerCore::setNoiseReduction3D(int val)
{
    // [0  - 5]
    if(val < 0 || val > 5)
        return;
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_noise_reduction3d, QString::number(val)), iss) && isPositive(iss.str()))
    {
        _params._noiseReduction3D = val;
        emit noiseReduction3DChanged(_params._noiseReduction3D);
    }
}

void ControllerCore::setColorGain(int val)
{
    // [0  - 60]
    if(val < 0 || val > 60)
        return;
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_color_gain, QString::number(val)), iss) && isPositive(iss.str()))
    {
        _params._colorGain = val;
        emit colorGainChanged(_params._colorGain);
    }
}

void ControllerCore::setGain(int val)
{
    // [0  - 34]
    if(val < 0 || val > 34)
        return;
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_gain, QString::number(val)), iss) && isPositive(iss.str()))
    {
        _params._gain = val;
        emit gainChanged(_params._gain);
    }
}

void ControllerCore::setExposureMode(int val)
{
    std::istringstream iss;
    QString param = _vals.supportedExposureModes[val];
    if(send(_protocol.prepareMessage(_protocol.set_exposure_mode,param), iss) && isPositive(iss.str()))
    {
        _params._exposureMode = val;
        emit exposureModeChanged(_params._exposureMode);
    }
}

void ControllerCore::setExposureCompensation(int val)
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_exposure_compensation, QString::number(val)), iss) && isPositive(iss.str()))
    {
        _params._exposureCompensation = val;
        emit exposureCompensationChanged(_params._exposureCompensation);
    }
}
void ControllerCore::setPreventiveColor(const QString & val)
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_thresholds_colors, val + " " + _params._criticalColor), iss))
    {
        if (isOk(iss.str()))
            _params._preventiveColor = val;
        emit preventiveColorChanged(val);
    }
}

void ControllerCore::setCriticalColor(const QString & val)
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_thresholds_colors, _params._preventiveColor + " " + val), iss))
    {
        if (isOk(iss.str()))
            _params._criticalColor = val;
        emit criticalColorChanged(val);
    }
}

void ControllerCore::setAlarmOpacity(int val)
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_thresholds_opacity, QString::number(val)), iss) && isPositive(iss.str()))
    {
        _params._alarmOpacity = val;
        emit alarmOpacityChanged(_params._alarmOpacity);
    }
}

void ControllerCore::setIris(int val)
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_iris, QString::number(val)), iss))
    {
        if (isOk(iss.str()))
            _params._iris = val;
        emit irisChanged(val);
    }
}

void ControllerCore::setUnits(const QString & val)
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_units, val), iss))
    {
        if (isOk(iss.str()))
            _params._units = val;
        emit unitsChanged(val);
    }
}

void ControllerCore::setAlarmCritical(double val)
{
    if (val >= _params._alarmPreventive)
    {
        std::istringstream iss;
        if(send(_protocol.prepareMessage(_protocol.set_thresholds, QString::number(_params._alarmPreventive) + " " + QString::number(val)), iss) && isPositive(iss.str()))
        {
            _params._alarmCritical = val;
            emit alarmCriticalChanged(_params._alarmCritical);
        }
    }
}

void ControllerCore::setLastIp(const QString& ip)
{
    QFile file(_vals.lastIpFileName);
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << ip;
        file.close();
    }
}

bool ControllerCore::isValidSpan(const QString& window, const QString & center, const QString& env)
{
    std::string cache;
    std::istringstream iss(env.toStdString());
    std::getline(iss, cache, ' ');
    QString env1 = QString::fromStdString(cache);
    std::getline(iss, cache);
    QString env2 = QString::fromStdString(cache);
    bool ok1, ok2;
    double dval = center.toDouble(&ok1);
    double halfwin = window.toDouble(&ok2)/2;

    if(!ok1 || !ok2)
        return false;

    if (dval - halfwin >= env1.toDouble() && dval + halfwin <= env2.toDouble() && halfwin >=  0.1)
        return true;

    return false;
}


bool ControllerCore::connected() const{
    return _connected;
}

bool ControllerCore::isValidIpv4Addr(const std::string &addr)
{
    boost::system::error_code ec;
    boost::asio::ip::address::from_string( addr, ec );
    return !ec.failed();
}

bool ControllerCore::isOk(const std::string & response){
    std::istringstream iss(response);
    std::string cache;
    getAnyLine(iss, cache);

    return cache == "OK";
}
bool ControllerCore::isTrue(const std::string & response){
    std::istringstream iss(response);
    std::string cache;
    getAnyLine(iss, cache);


    return cache == "TRUE";
}

void ControllerCore::onThermalFrameChanged(){
    emit streamFrameChanged();
}

void ControllerCore::onVisibleFrameChanged(){
    emit streamFrameVisibleChanged();
}

bool ControllerCore::isOk(const QString & response){
    return ControllerCore::isOk(response.toStdString());
}

bool ControllerCore::isTrue(const QString & response){
    return ControllerCore::isTrue(response.toStdString());
}

bool ControllerCore::isPositive(const QString & response){
    return ControllerCore::isOk(response) || ControllerCore::isTrue(response);
}


bool ControllerCore::isPositive(const std::string & response){
    return ControllerCore::isOk(response) || ControllerCore::isTrue(response);
}

QString ControllerCore::boolToString(bool val){
    return val ? "TRUE" : "FALSE";
}

bool ControllerCore::stringToBool(const QString & val)
{
    return val.compare("TRUE", Qt::CaseInsensitive);
}

