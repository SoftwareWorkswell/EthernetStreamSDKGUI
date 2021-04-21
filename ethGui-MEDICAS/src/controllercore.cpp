#include "controllercore.h"

ControllerCore::ControllerCore(QObject * parent) : QObject (parent)
{
    _saveDest =
            QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
            .at(0) +
            "/" + "Medicas-Snapshots";
    QDir saveDestDir(_saveDest);
    if(!saveDestDir.exists())
    {
        saveDestDir.mkdir(_saveDest);
    }
    QDir cacheDir(QDir::currentPath() + "/cache/");
    if(!cacheDir.exists())
    {
        cacheDir.mkdir(QDir::currentPath() + "/cache/");
    }
}

ControllerCore::~ControllerCore()
{
    if(_params._isSetup)
        send(_protocol.prepareMessage(_protocol.set_eth_mode, "FALSE"));
}

bool  ControllerCore::sendCommand(const QString & mes, std::istringstream & iss){
    return send(mes,iss, false, false);
}

bool ControllerCore::isSetup() const { return this->_params._isSetup; }

void ControllerCore::setEthModeOff()
{
    send(_protocol.prepareMessage(_protocol.set_eth_mode, "FALSE"));
}

QString ControllerCore::getLastIp() {
    if(QFile::exists(_vals.currentIpFileName))
    {
        QFile file(_vals.currentIpFileName);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            return file.readLine();
    }
    if(QFile::exists(_vals.lastIpFileName))
    {
        QFile file(_vals.lastIpFileName);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            return file.readLine();
    }
    return "";
}

bool ControllerCore::checkAddressFormat(const QString& ip)
{
    QRegularExpression regex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    QRegularExpressionMatch match = regex.match(ip);
    return match.hasMatch();
}

bool ControllerCore::connect(const QString& ip)
{
    //we can even save it to local storage so app could remember last used ip on startup
    setLastIp(ip);
    //network client class takes care of TCP layer of application
    //it takes MEDICAS ip and communication port(2240) as constructor parameters
    //from now on it is fairly easy to call method send on network client instance with any command as parameter
    //network client should automatically disconnect from MEDICAS in its destructor
    if (this->_networkClient == nullptr || !_connected) {
        if (_connectionThread)
            delete _connectionThread;
        _connectionThread = new ConnectionThread(_networkClient, ip, _params._endpointPort);
        QObject::connect(_connectionThread, &ConnectionThread::connectionFinished, this, &ControllerCore::afterConnectHandler);
        QObject::connect(_connectionThread, &ConnectionThread::connectionStarted, this, &ControllerCore::connectionStarted);
        //QObject::connect(_connectionThread, SIGNAL(finished()), _connectionThread, SLOT(deleteLater()));
        _connectionThread->start();
        return true;
    }
    return false;
}

void ControllerCore::stopConnect()
{
    if(!_connectionThread || !_connectionThread->isRunning())
        return;
    _connectionThread->terminate();
    _connectionThread->wait();
    _connected = false;
    emit connectedChanged(_connected, 10);
    emit connectionFinished(_connected);
}


bool ControllerCore::afterConnectHandler(const QString& ip)
{
    qDebug() << "after connect handler";
    if(!_connectionThread)
        return false;
    _networkClient = _connectionThread->networkClient();
    if (_networkClient && _networkClient->is_connected())
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


        //checking camery type
        fetchBasicInfo();
        if(_params._cameraType != _vals.medicasType)
            return false;
    }

    _connected = (_networkClient != nullptr) && _networkClient->is_connected();
    emit connectedChanged(_connected, 10);
    emit connectionFinished(_connected);
    qDebug() << "after emit" << _connected;
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
        if(send(_protocol.activate + ' ' + actStr, iss) && isPositive(iss.str()))
            return true;
    }
    else
        return true;
    return false;
}

bool ControllerCore::setup()
{
    //checking whether connect function was called
    if (_networkClient) {
        if (!this->isActivated())
            return false;

        //fetching data from camera to sync the app
        fetchAll();

        _params._isSetup = true;

        //initiating helper thread for fetching data from MEDICAS during runtime repeatedly
        //(like temperatures, extremes, GPS location, time and so on)
        if (!_helperTreadFlag)
            toggleHelperThread();

        //initiating streaming if not already in progress
        if (!_streamFlagThermal)
            toggleStreamThermal();
        if (!_streamFlagVisible)
            toggleStreamVisible();

    } else {
        return false;
    }
    return true;
}



void ControllerCore::close()
{
    if(_networkClient && _networkClient->is_connected())
    {
        qDebug()<< "Closing connection";
        stopThreads();
        setEthModeOff();
        _networkClient->disconnect();
        _networkClient.reset();
        _connected = false;
        qDebug()<< "Closed connection succesfully";
    }
}

void ControllerCore::forceCalibration()
{
    send(_protocol.prepareMessage(_protocol.force_calibration));
}

// reset default settings command - same for all wiris cameras
void ControllerCore::setDefaultSettings(){
    if(_networkClient && _networkClient->is_connected())
    {
        stopThreads();
    }
    send(_protocol.prepareMessage(_protocol.set_default_settings));
    setEthModeOff();
    _networkClient->disconnect();
    _networkClient.reset();
    _connected = false;
    qDebug()<< "Restarting application";
    QCoreApplication::quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

// reboot command - same for all wiris cameras
void ControllerCore::reboot(){
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
        if (send(_protocol.set_save_dest + ' ' + storage, iss) && isPositive(iss.str()))
        {
            _captureParams._currentStorage = storage;
            return true;
        }
    }
    return false;
}

QString ControllerCore::getStorage() { return _captureParams._currentStorage; }

bool ControllerCore::compareFw(const QString &older, const QString &newer) {
    return compareVersion(older, newer);
}

bool ControllerCore::isHeatingUp()
{
    return _params._heatingUp;
}

QQmlListProperty<PaletteWrapper> ControllerCore::palettes() {
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

bool ControllerCore::showCrossUser() const
{
    return this->_params._showCrossUser;
}

bool ControllerCore::showRoiUserNames() const
{
    return this->_params._showUserRoiNames;
}

QString ControllerCore::blackbodyStatus() const
{
    return this->_params._blackbodyStatus;
}

bool ControllerCore::showBlackbodyRoi() const
{
    return this->_params._showBlackbodyRoi;
}

int ControllerCore::showUserRoi1() const
{
    return this->_params._showUserRoi1;
}

int ControllerCore::showUserRoi2() const
{
    return this->_params._showUserRoi2;
}

int ControllerCore::showUserRoi3() const
{
    return this->_params._showUserRoi3;
}

int ControllerCore::showUserRoi4() const
{
    return this->_params._showUserRoi4;
}

int ControllerCore::temperatureMode() const
{
    return this->_params._temperatureMode;
}

int ControllerCore::gamma() const
{
    return this->_params._gamma;
}

int ControllerCore::whiteBalance() const
{
    return this->_params._whiteBalance;
}

bool ControllerCore::backlightCompensation() const {
    return this->_params._backlightCompensation;
}

bool ControllerCore::wideDynamicRangeMode() const {
    return this->_params._backlightCompensation;
}

int ControllerCore::noiseReduction2D() const {
    return this->_params._noiseReduction2D;
}

int ControllerCore::noiseReduction3D() const {
    return this->_params._noiseReduction3D;
}

int ControllerCore::colorGain() const { return this->_params._colorGain; }

int ControllerCore::gain() const { return this->_params._gain; }

int ControllerCore::exposureMode() const { return this->_params._exposureMode; }

int ControllerCore::exposureCompensation() const {
    return this->_params._exposureCompensation;
}

QString ControllerCore::preventiveColor() const {
    return this->_params._preventiveColor;
}

QString ControllerCore::criticalColor() const {
    return this->_params._criticalColor;
}

int ControllerCore::alarmOpacity() const { return this->_params._alarmOpacity; }

int ControllerCore::iris() const { return this->_params._iris; }

QString ControllerCore::units() const { return this->_params._units; }

QString ControllerCore::language() const { return this->_params._language; }

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
    {
        if(send(_protocol.prepareMessage(_protocol.start_recording)))
            _recordingToggled = true;

    }
    else
    {
        if(send(_protocol.prepareMessage(_protocol.finish_recording)))
            _recordingToggled = false;
    }
}

void ControllerCore::toggleStreamThermal(){

    if (!this->_params._isSetup)
        return;

    if (!_streamFlagThermal)
    {
        _streamFlagThermal = true;
        _thermalThread = new ThermalThread(&this->_streamFlagThermal, this->_params._endpointIp, _params._maximum, _params._userCross, _params._blackbody, _params._head,_params._userRoiContainer, &_params._blackbodySize, &_params._blackbodyMaskSize, &_params._cameraType, &_params._units);
        QObject::connect(_thermalThread, &ThermalThread::imageSourceChanged, this, &ControllerCore::onThermalFrameChanged);
        QObject::connect(_thermalThread, SIGNAL(finished()), _thermalThread, SLOT(deleteLater()));
        QObject::connect(_thermalThread, &ThermalThread::disconnected, this, &ControllerCore::onCameraDisconnected);
        _thermalThread->start();
    } else {
        _streamFlagThermal = false;
        _thermalThread->wait();
    }
}

void ControllerCore::toggleStreamVisible(){
    if (!this->_params._isSetup)
        return;

    if (!_streamFlagVisible)
    {
        _streamFlagVisible = true;
        _visibleThread = new VisibleThread(&this->_streamFlagVisible, this->_params._endpointIp);
        QObject::connect(_visibleThread, &VisibleThread::imageSourceChanged, this, &ControllerCore::onVisibleFrameChanged);
        QObject::connect(_visibleThread, SIGNAL(finished()), _visibleThread, SLOT(deleteLater()));
        QObject::connect(_visibleThread, &VisibleThread::disconnected, this, &ControllerCore::onCameraDisconnected);
        _visibleThread->start();
    }
    else {
        _streamFlagVisible = false;
        _visibleThread->wait();
    }
}

void ControllerCore::stopThreads()
{
    qDebug() << "Stopping threads";

    if(_helperTreadFlag){
        _helperTreadFlag = false;
        this->_helperThread->wait();
    }

    if(_streamFlagThermal){
        _streamFlagThermal = false;
        _thermalThread->wait();
    }

    if(_streamFlagVisible){
        _streamFlagVisible = false;
        _visibleThread->wait();
    }
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

bool ControllerCore::getCaptureIRVideoChecked() const {
    return _captureParams._encIRVid;
}

bool ControllerCore::getCaptureVisVideoChecked() const {
    return _captureParams._visVid;
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
        if (send(_protocol.prepareMessage(_protocol.set_video_ir_encoded, boolToString(encIRVid)), iss) && isPositive(iss.str()))
            _captureParams._encIRVid = encIRVid;
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

bool ControllerCore::send(const QString &message, std::istringstream & answ, bool debugPrint, bool cacheAnswer)
{
    QMutexLocker locker(connectionMutex);

    if (_networkClient == nullptr)
        return  false;

    std::string answer;
    int length;

    _networkClient->write(message.toStdString());
    length = _networkClient->read_until(answer, '\0', std::chrono::seconds(1));

    QString log = "Command: " + message;
    log += "   Answer(" + QString::number(length) + "): ";
    if (length > 0)
    {
        log += QString::fromStdString(answer);
    }
    else
    {
        onCameraDisconnected();
    }

    if (debugPrint)
        qDebug() << log;

    answ = std::istringstream(answer);
    if(cacheAnswer)
    {
        _lastAns = QString::fromStdString(answer);
        emit lastAnsChanged();
    }
    bool bbError = QString::fromStdString(answer).contains(_errorCodes.blackbody_error + "\r\n");
    // ignore when setting black body or heating up
    if(!_params._heatingUp && bbError && _params._userRoiContainer->_currentRoi != 6)
        emit blackbodyError();
    return !(QString::fromStdString(answer).contains(_errorCodes.general_error + "\r\n") ||
             bbError || answer.empty());
}


void ControllerCore::toggleHelperThread()
{
    using namespace std::placeholders;
    auto fp = std::bind(&ControllerCore::sendCommand, this, _1, _2);
    _extremesFlag = true;
    if (!_helperTreadFlag)
    {
        _helperTreadFlag = true;
        _helperThread = new HelperThread(
                    &_helperTreadFlag, &_extremesFlag, fp, _params._maximum,
                    _params._userCross, _params._blackbody, _params._head,
                    _params._userRoiContainer, &_params._firmwareVersion,
                    &_params._cameraType, &_params._dioConnected);
        QObject::connect(_helperThread, &HelperThread::extremesChanged, this, &ControllerCore::onHelperExtremesChanged);
        QObject::connect(_helperThread, &HelperThread::tempsChanged, this, &ControllerCore::onHelperTempsChanged);
        QObject::connect(_helperThread, &HelperThread::captureRecordingChanged , this, &ControllerCore::onHelpercaptureRecordingChanged);
        QObject::connect(_helperThread, &HelperThread::driveStateChanged , this, &ControllerCore::onHelperdriveStateChanged);
        QObject::connect(_helperThread, &HelperThread::dateTimeChanged , this, &ControllerCore::onHelperDateTImeChanged);
        QObject::connect(_helperThread, &HelperThread::dioConnectedChanged,
                         this, &ControllerCore::dioConnectedChanged);
        QObject::connect(_helperThread, &HelperThread::connectionStateChanged, this, &ControllerCore::onHelperConnectionStateChanged);
        QObject::connect(_helperThread, SIGNAL(finished()), _helperThread, SLOT(deleteLater()));
        _helperThread->start();
    }
    else
    {
        _helperTreadFlag = false;
        if(!_helperThread->wait(100))
        {
            _helperThread->terminate();
            _helperThread->wait();
        }
    }
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
    if(!state)
    {
        onCameraDisconnected();
    }
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

void ControllerCore::fetchUserCross()
{
    std::istringstream iss;
    std::string cache;
    if (send(_protocol.prepareMessage(_protocol.get_cross_user), iss))        {
        std::getline(iss, cache, ' ');

        this->_params._crossUserX = QString::fromStdString(cache).toInt();
        std::getline(iss, cache, ' ');

        this->_params._crossUserY = QString::fromStdString(cache).toInt();
        getAnyLine(iss, cache);
        _params._userCross->val = QString::fromStdString(cache).toDouble();
        _params._userCross->x = _params._crossUserX;
        _params._userCross->y = _params._crossUserY;
        emit crossUserXChanged(_params._crossUserX);
        emit crossUserYChanged(_params._crossUserY);
    }
}

void ControllerCore::fetchBlackbodyRoi()
{
    std::istringstream iss;
    std::string cache;
    if(send(_protocol.prepareMessage(_protocol.get_show_blackbody_roi), iss))
    {
        getAnyLine(iss, cache);
        this->_params._showBlackbodyRoi = stringToBool(QString::fromStdString(cache));
        this->_params._blackbody->show = stringToBool(QString::fromStdString(cache));
        emit showBlackbodyRoiChanged(_params._showBlackbodyRoi);
    }
    if(send(_protocol.prepareMessage(_protocol.get_blackbody_size), iss))
    {
        std::string resp;
        getAnyLine(iss, resp);
        _params._blackbodySize = _vals.blackbodySizeToIndex.find(QString::fromStdString(resp))->second;
        emit blackbodySizeChanged(_params._blackbodySize);
    }
    if(send(_protocol.prepareMessage(_protocol.get_blackbody_mask_size), iss))
    {
        std::string resp;
        getAnyLine(iss, resp);
        _params._blackbodyMaskSize = _vals.blackbodyMaskSizeToIndex.find(QString::fromStdString(resp))->second;
        emit blackbodyMaskSizeChanged(_params._blackbodyMaskSize);
    }
    if(send(_protocol.prepareMessage(_protocol.get_blackbody_detection), iss))
    {
        std::string resp;
        getAnyLine(iss, resp);
        _params._blackbodyDetection = _vals.blackbodyDetectionToIndex.find(QString::fromStdString(resp))->second;
        emit blackbodyDetectionChanged(_params._blackbodyDetection);
    }

    if (send(_protocol.prepareMessage(_protocol.get_blackbody_roi), iss))        {
        std::getline(iss, cache, ' ');
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
        _params._heatingUp  = (_params._blackbodyStatus.contains(_errorCodes.blackbody_heating_up));
        qDebug() << "heating up" << _params._heatingUp;
        emit blackbodyStatusChanged(_params._blackbodyStatus);
    }
    // get bb scale
    if(send(_protocol.prepareMessage(_protocol.get_blackbody_scale), iss))
    {
        std::getline(iss, cache, '\n');
        this->_params._blackbody->scale = stoi(cache);
        emit blackBodyRoiScaleChanged();
    }
    else
    {
        this->_params._blackbody->scale = 100;
        emit blackBodyRoiScaleChanged();
    }
}


void ControllerCore::fetchBlackbodyStatus()
{
    std::istringstream iss;
    std::string cache;
    if (_connected && send(_protocol.prepareMessage(_protocol.get_blackbody_status), iss, true, false))        {
        std::getline(iss, cache, '\n');
        this->_params._blackbodyStatus = QString::fromStdString(cache);
        _params._heatingUp  = (_params._blackbodyStatus.contains(_errorCodes.blackbody_heating_up));
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
        this->_params._maximum->show = stringToBool(QString::fromStdString(cache));
        emit showCrossMaxChanged(_params._showCrossMax);
    }

    if(send(_protocol.prepareMessage(_protocol.get_show_cross_user), iss))
    {
        getAnyLine(iss, cache);
        this->_params._showCrossUser = stringToBool(QString::fromStdString(cache));
        this->_params._userCross->show = stringToBool(QString::fromStdString(cache));
        emit showCrossUserChanged(_params._showCrossUser);
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
    fetchBlackbodyStatus();
    fetchBasicInfo();
    fetchTemperatureMode();
    fetchManualRange();
    fetchMainCamera();
    fetchBlackbodyRoi();
    fetchShowCrosses();
    fetchPalettes();
    fetchPaletteValues();
    fetchCaptureRecSettings();
    fetchAlarms();
    fetchUserRois();
    fetchUserCross();
    fetchExposure();
    fetchNoiseReduction();
    fetchGains();
    fetchIris();
    fetchGamma();
    fetchWhiteBalance();
    fetchBacklightCompensation();
    fetchDynamicRangeMode();
    fetchSystemSettings();
    fetchDioParams();
    fetchCurrentUser();
    fetchStorage();
}

void ControllerCore::fetchTemperatureMode() {
    std::istringstream iss;
    if (send(_protocol.prepareMessage(_protocol.get_temperature_mode), iss))
    {
        std::string resp;
        getAnyLine(iss, resp);
        _params._temperatureMode = _vals.temperatureModeToIndex.find(QString::fromStdString(resp))->second;
        emit temperatureModeChanged(_params._temperatureMode);
    }
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
        if (qcache.contains(_vals.supportedAlarmModes[1]))
            _params._alarmMode = 1;
        else if (qcache.contains(_vals.supportedAlarmModes[0]))
            _params._alarmMode = 0;
        else if (qcache.contains(_vals.supportedAlarmModes[2]))
            _params._alarmMode = 2;
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
        _params._criticalColor = QString::fromStdString(cache);
        _params._criticalColor = _params._criticalColor.remove(_params._criticalColor.indexOf('\r', 0), _params._criticalColor.length() - _params._criticalColor.indexOf('\r'));
        emit criticalColorChanged(_params._criticalColor);
    }

    int op;
    if (send(_protocol.prepareMessage(_protocol.get_thresholds_opacity), iss))        {
        iss >> op;
        _params._alarmOpacity = op;
        emit alarmOpacityChanged(_params._alarmOpacity);
    }
}

void ControllerCore::fetchUserRois()
{
    std::istringstream iss;
    std::string cache;
    QStringList anstokenlist;

    if(send(_protocol.prepareMessage(_protocol.get_show_user_roi), iss))
    {
        std::getline(iss, cache);
        _params._showUserRoi1 = _vals.roiVisibilityToIndex.find(QString::fromStdString(cache).split(' ').last())->second;
        _params._userRoiContainer->_userRoi1->show = _params._showUserRoi1;
        _params._userRoiContainer->_userRoi1->name = QString::fromStdString(cache).split(' ').first();
        std::getline(iss, cache);
        _params._showUserRoi2 = _vals.roiVisibilityToIndex.find(QString::fromStdString(cache).split(' ').last())->second;
        _params._userRoiContainer->_userRoi2->show = _params._showUserRoi2;
        _params._userRoiContainer->_userRoi2->name = QString::fromStdString(cache).split(' ').first();
        std::getline(iss, cache);
        _params._showUserRoi3 = _vals.roiVisibilityToIndex.find(QString::fromStdString(cache).split(' ').last())->second;
        _params._userRoiContainer->_userRoi3->show = _params._showUserRoi3;
        _params._userRoiContainer->_userRoi3->name = QString::fromStdString(cache).split(' ').first();
        std::getline(iss, cache);
        _params._showUserRoi4 = _vals.roiVisibilityToIndex.find(QString::fromStdString(cache).split(' ').last())->second;
        _params._userRoiContainer->_userRoi4->show = _params._showUserRoi4;
        _params._userRoiContainer->_userRoi4->name = QString::fromStdString(cache).split(' ').first();
        emit showUserRoiChanged();
    }
    if(send(_protocol.prepareMessage(_protocol.get_user_roi), iss))
    {
        // ans format: "0: # START COORD: 50 50 END COORD: 100 100"
        std::getline(iss, cache);
        anstokenlist = QString::fromStdString(cache).split(' ');
        _params._userRoiContainer->_userRoi1->x = anstokenlist.at(4).toInt();
        _params._userRoiContainer->_userRoi1->y = anstokenlist.at(5).toInt();
        _params._userRoiContainer->_userRoi1->w = anstokenlist.at(8).toInt() - anstokenlist.at(4).toInt();
        _params._userRoiContainer->_userRoi1->h = anstokenlist.at(9).toInt() - anstokenlist.at(5).toInt();
        _params._userRoiX[0] = _params._userRoiContainer->_userRoi1->x;
        _params._userRoiY[0] =_params._userRoiContainer->_userRoi1->y;
        _params._userRoiW[0] =_params._userRoiContainer->_userRoi1->w;
        _params._userRoiH[0] =_params._userRoiContainer->_userRoi1->h;
        std::getline(iss, cache);
        anstokenlist = QString::fromStdString(cache).split(' ');
        _params._userRoiContainer->_userRoi2->x = anstokenlist.at(4).toInt();
        _params._userRoiContainer->_userRoi2->y = anstokenlist.at(5).toInt();
        _params._userRoiContainer->_userRoi2->w = anstokenlist.at(8).toInt() - anstokenlist.at(4).toInt();
        _params._userRoiContainer->_userRoi2->h = anstokenlist.at(9).toInt() - anstokenlist.at(5).toInt();
        _params._userRoiX[1] = _params._userRoiContainer->_userRoi2->x;
        _params._userRoiY[1] =_params._userRoiContainer->_userRoi2->y;
        _params._userRoiW[1] =_params._userRoiContainer->_userRoi2->w;
        _params._userRoiH[1] =_params._userRoiContainer->_userRoi2->h;
        std::getline(iss, cache);
        anstokenlist = QString::fromStdString(cache).split(' ');
        _params._userRoiContainer->_userRoi3->x = anstokenlist.at(4).toInt();
        _params._userRoiContainer->_userRoi3->y = anstokenlist.at(5).toInt();
        _params._userRoiContainer->_userRoi3->w = anstokenlist.at(8).toInt() - anstokenlist.at(4).toInt();
        _params._userRoiContainer->_userRoi3->h = anstokenlist.at(9).toInt() - anstokenlist.at(5).toInt();
        _params._userRoiX[2] =_params._userRoiContainer->_userRoi3->x;
        _params._userRoiY[2] =_params._userRoiContainer->_userRoi3->y;
        _params._userRoiW[2] =_params._userRoiContainer->_userRoi3->w;
        _params._userRoiH[2] =_params._userRoiContainer->_userRoi3->h;
        std::getline(iss, cache);
        anstokenlist = QString::fromStdString(cache).split(' ');
        _params._userRoiContainer->_userRoi4->x = anstokenlist.at(4).toInt();
        _params._userRoiContainer->_userRoi4->y = anstokenlist.at(5).toInt();
        _params._userRoiContainer->_userRoi4->w = anstokenlist.at(8).toInt() - anstokenlist.at(4).toInt();
        _params._userRoiContainer->_userRoi4->h = anstokenlist.at(9).toInt() - anstokenlist.at(5).toInt();
        _params._userRoiX[3] = _params._userRoiContainer->_userRoi4->x;
        _params._userRoiY[3] =_params._userRoiContainer->_userRoi4->y;
        _params._userRoiW[3] =_params._userRoiContainer->_userRoi4->w;
        _params._userRoiH[3] =_params._userRoiContainer->_userRoi4->h;
    }
    if(send(_protocol.prepareMessage(_protocol.get_show_user_roi_name), iss))
    {
        getAnyLine(iss, cache);
        this->_params._showUserRoiNames = stringToBool(QString::fromStdString(cache));
        _params._userRoiContainer->showNames = this->_params._showUserRoiNames;
        emit showRoiUserNamesChanged(_params._showUserRoiNames);
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
        _params._gamma = _vals.gammaModeToIndex.find(QString::fromStdString(resp))->second;
        emit gammaChanged(_params._gamma);
    }
}

void ControllerCore::fetchWhiteBalance() {
    std::istringstream iss;
    if (send(_protocol.prepareMessage(_protocol.get_white_balance), iss))
    {
        std::string resp;
        getAnyLine(iss, resp);
        _params._whiteBalance = _vals.whiteBalanceModeToIndex.find(QString::fromStdString(resp))->second;
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
        QString temp = QString::fromStdString(cache);
        if(temp == "CELSIUS")
        {
            this->_params._units = " °C";
        }
        else if(temp == "FAHRENHEIT" )
        {
            this->_params._units = " °F";
        }
        emit unitsChanged(_params._units);
    }
    if(send(_protocol.prepareMessage(_protocol.get_language), iss))
    {
        getAnyLine(iss, cache);
        this->_params._language = QString::fromStdString(cache);
        emit languageChanged(_params._language);
    }
}

void ControllerCore::fetchStorage()
{
    std::istringstream iss;
    std::string cache;
    if(send(_protocol.prepareMessage(_protocol.get_save_dest), iss))
    {
        getAnyLine(iss, cache);
        this->_captureParams._currentStorage = QString::fromStdString(cache);
        emit selectedStorageChanged(_captureParams._currentStorage);
    }
}

QString ControllerCore::getLastAns() const
{
    return _lastAns;
}

bool ControllerCore::getRecordingToggled() const
{
    return _recordingToggled;
}

double ControllerCore::visShutterPeriod() const {
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

int ControllerCore::blackbodyRoiX() const
{
    return this->_params._blackbody->x;
}

int ControllerCore::blackbodyRoiY() const
{
    return this->_params._blackbody->y;
}

int ControllerCore::crossUserX() const
{
    return this->_params._userCross->x;
}

int ControllerCore::crossUserY() const
{
    return this->_params._userCross->y;
}
// corner  ["TOP-LEFT", "TOP-RIGHT", "BOTTOM-LEFT", "BOTTOM-RIGHT"]
int ControllerCore::getUserRoiX(int idx, int corner)
{
    UserRoi * currRoi = nullptr;
    switch(idx)
    {
    case 0:
        currRoi = _params._userRoiContainer->_userRoi1;
        break;
    case 1:
        currRoi = _params._userRoiContainer->_userRoi2;
        break;
    case 2:
        currRoi = _params._userRoiContainer->_userRoi3;
        break;
    case 3:
        currRoi = _params._userRoiContainer->_userRoi4;
        break;
    default:
        return 0;
    }
    if(!currRoi)
        return 0;
    switch (corner) {
    case 0:
    case 2:
        return currRoi->x;
    case 1:
    case 3:
        return currRoi->x + currRoi->w;
    default:
        return 0;
    }
    return 0;
}

int ControllerCore::getUserRoiY(int idx, int corner)
{
    UserRoi * currRoi = nullptr;
    switch(idx)
    {
    case 0:
        currRoi = _params._userRoiContainer->_userRoi1;
        break;
    case 1:
        currRoi = _params._userRoiContainer->_userRoi2;
        break;
    case 2:
        currRoi = _params._userRoiContainer->_userRoi3;
        break;
    case 3:
        currRoi = _params._userRoiContainer->_userRoi4;
        break;
    default:
        return 0;
    }
    if(!currRoi)
        return 0;
    switch (corner) {
    case 0:
    case 1:
        return currRoi->y;
    case 2:
    case 3:
        return currRoi->y + currRoi->h;
    default:
        return 0;
    }
    return 0;
}

int ControllerCore::blackbodySize() const
{
    return this->_params._blackbodySize;
}

int ControllerCore::blackbodyMaskSize() const
{
    return this->_params._blackbodyMaskSize;
}

int ControllerCore::blackbodyDetection() const
{
    return this->_params._blackbodyDetection;
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
        if(val.toDouble() < getManualMinLimit() || _params._manualRange2.toDouble() - val.toDouble() < getManualDiffLimit())
        {
            emit rangeError();
            emit manualRange1Changed(_params._manualRange1);
            return;
        }
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
        if(val.toDouble() > getManualMaxLimit() || val.toDouble() - _params._manualRange1.toDouble() < getManualDiffLimit())
        {
            emit rangeError();
            emit manualRange2Changed(_params._manualRange2);
            return;
        }
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

void ControllerCore::setBlackbodyRoiX(int val)
{
    if(val < 0)
        return;
    _params._blackbody->x = val;
    emit blackbodyRoiXChanged(_params._blackbodyRoiX);
}

void ControllerCore::setBlackbodyRoiY(int val)
{
    if(val < 0)
        return;
    _params._blackbody->y = val;
    emit blackbodyRoiYChanged(_params._blackbodyRoiY);
}

void ControllerCore::setCrossUserX(int val)
{
    //qDebug() << " setting cross user x" << val;
    if(val < 0)
        return;
    _params._userCross->x = val;
    emit crossUserXChanged(_params._userCross->x);
}

void ControllerCore::setCrossUserY(int val)
{
    if(val < 0)
        return;
    _params._userCross->y = val;
    emit crossUserYChanged(_params._userCross->y);
}

void ControllerCore::setUserRoiSelectedCorner(int idx, int corner)
{
    UserRoi * currRoi = nullptr;
    switch(idx)
    {
    case 0:
        currRoi = _params._userRoiContainer->_userRoi1;
        break;
    case 1:
        currRoi = _params._userRoiContainer->_userRoi2;
        break;
    case 2:
        currRoi = _params._userRoiContainer->_userRoi3;
        break;
    case 3:
        currRoi = _params._userRoiContainer->_userRoi4;
        break;
    default:
        return;
    }
    if(!currRoi)
        return;
    currRoi->selectedCorner = corner;
}

void ControllerCore::setUserRoiX(int idx, int corner, int value)
{
    UserRoi * currRoi = nullptr;
    switch(idx)
    {
    case 0:
        currRoi = _params._userRoiContainer->_userRoi1;
        break;
    case 1:
        currRoi = _params._userRoiContainer->_userRoi2;
        break;
    case 2:
        currRoi = _params._userRoiContainer->_userRoi3;
        break;
    case 3:
        currRoi = _params._userRoiContainer->_userRoi4;
        break;
    default:
        return;
    }
    if(!currRoi)
        return;
    int origX;
    currRoi->selectedCorner = corner;
    switch (corner) {
    case 0:
    case 2:
        origX = currRoi->x;
        currRoi->x = value;
        // do not allow too small roi
        if(currRoi->w + origX - value < 3)
        {
            currRoi->x = origX;
            return;
        }
        currRoi->w = currRoi->w + origX - value;
        break;
    case 1:
    case 3:
         // do not allow too small roi
        if(value - currRoi->x < 3)
            return;
        currRoi->w = value - currRoi->x;
        break;
    default:
        return;
    }
}

void ControllerCore::setUserRoiY(int idx, int corner, int value)
{
    UserRoi * currRoi = nullptr;
    switch(idx)
    {
    case 0:
        currRoi = _params._userRoiContainer->_userRoi1;
        break;
    case 1:
        currRoi = _params._userRoiContainer->_userRoi2;
        break;
    case 2:
        currRoi = _params._userRoiContainer->_userRoi3;
        break;
    case 3:
        currRoi = _params._userRoiContainer->_userRoi4;
        break;
    default:
        return;
    }
    if(!currRoi)
        return;
    int origY;
    switch (corner) {
    case 0:
    case 1:
        origY = currRoi->y;
        currRoi->y = value;
        // do not allow too small roi
        if(currRoi->h + origY - value < 3)
        {
            currRoi->y = origY;
            return;
        }
        currRoi->h = currRoi->h + origY - value;
        break;
    case 2:
    case 3:
        if(value - currRoi->y < 3)
            return;
        currRoi->h = value - currRoi->y;
        break;
    default:
        return;
    }
}

void ControllerCore::sendBlackbodyCoordinates()
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_blackbody_roi, QString::number(_params._blackbody->x) + " " + QString::number(_params._blackbody->y)), iss) && isPositive(iss.str()))
    {
        _params._blackbodyRoiX = _params._blackbody->x;
        _params._blackbodyRoiY = _params._blackbody->y;
    }
    fetchBlackbodyStatus();
}

void ControllerCore::sendOriginalBlackbodyCoordinates()
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_blackbody_roi, QString::number(_params._blackbodyRoiX) + " " + QString::number(_params._blackbodyRoiY)), iss) && isPositive(iss.str()))
    {
        _params._blackbody->x = _params._blackbodyRoiX;
        _params._blackbody->y =_params._blackbodyRoiY;
    }
    fetchBlackbodyStatus();
}

bool ControllerCore::sendCrossUserCoordinates()
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_cross_user, QString::number(_params._userCross->x) + " " + QString::number(_params._userCross->y)), iss) && isPositive(iss.str()))
    {
        _params._crossUserX = _params._userCross->x;
        _params._crossUserY = _params._userCross->y;
        return true;
    }
    _params._userCross->x = _params._crossUserX;
    _params._userCross->y = _params._crossUserY;
    emit crossUserXChanged(_params._userCross->x);
    emit crossUserYChanged(_params._userCross->y);
    return false;
}

void ControllerCore::sendOriginalUserRoiCoordinates(int idx)
{
    UserRoi * currRoi = nullptr;
    switch(idx)
    {
    case 0:
        currRoi = _params._userRoiContainer->_userRoi1;
        break;
    case 1:
        currRoi = _params._userRoiContainer->_userRoi2;
        break;
    case 2:
        currRoi = _params._userRoiContainer->_userRoi3;
        break;
    case 3:
        currRoi = _params._userRoiContainer->_userRoi4;
        break;
    default:
        return;
    }
    // copy old values
    currRoi->x = _params._userRoiX[idx];
    currRoi->y = _params._userRoiY[idx];
    currRoi->h = _params._userRoiW[idx];
    currRoi->w = _params._userRoiH[idx];
    QString params = QString::number(idx) + ' ' + QString::number(currRoi->x) + " " + QString::number(currRoi->y) + ' ' +
            QString::number(currRoi->x + currRoi->w) + ' ' + QString::number(currRoi->y + currRoi->h);
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_user_roi, params), iss) && isPositive(iss.str()))
    {
        emit userRoiXChanged(idx, currRoi->x);
        emit userRoiYChanged(idx, currRoi->y);
    }
}

bool ControllerCore::sendUserRoiCoordinates(int idx)
{
    UserRoi * currRoi = nullptr;
    switch(idx)
    {
    case 0:
        currRoi = _params._userRoiContainer->_userRoi1;
        break;
    case 1:
        currRoi = _params._userRoiContainer->_userRoi2;
        break;
    case 2:
        currRoi = _params._userRoiContainer->_userRoi3;
        break;
    case 3:
        currRoi = _params._userRoiContainer->_userRoi4;
        break;
    default:
        return false;
    }
    if(!currRoi)
        return false;
    QString params = QString::number(idx + 1) + ' ' + QString::number(currRoi->x) + " " + QString::number(currRoi->y) + ' ' +
            QString::number(currRoi->x + currRoi->w) + ' ' + QString::number(currRoi->y + currRoi->h);
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_user_roi, params), iss) && isPositive(iss.str()))
    {
        _params._userRoiX[idx] = currRoi->x;
        _params._userRoiY[idx] = currRoi->y;
        _params._userRoiW[idx] = currRoi->w;
        _params._userRoiH[idx] = currRoi->h;
        return true;
    }
    currRoi->x = _params._userRoiX[idx];
    currRoi->y = _params._userRoiY[idx];
    currRoi->h = _params._userRoiW[idx];
    currRoi->w = _params._userRoiH[idx];
    emit userRoiXChanged(idx, currRoi->x);
    emit userRoiYChanged(idx, currRoi->y);
    return false;
}

void ControllerCore::fetchBlackbodyCoordinates()
{
    fetchBlackbodyRoi();
}

// in medicas alarm mode is just ON/OFF/ROIS
void ControllerCore::setAlarmMode(int val)
{
    if (val < 0 || val > 4)
        return;
    QString req = _protocol.set_alarm_mode;
    req += " " + _vals.supportedAlarmModes[val];
    std::istringstream iss;

    if (send(req, iss) && isPositive(iss.str()))
        _params._alarmMode = val;
    emit alarmModeChanged(_params._alarmMode);
}

void ControllerCore::setAlarmPreventive(double val)
{
    if (val <= _params._alarmCritical && val >= getAlarmMinLimit())
    {

        std::istringstream iss;
        if(send(_protocol.prepareMessage(_protocol.set_thresholds, QString::number(val) + " " + QString::number(_params._alarmCritical)), iss) && isPositive(iss.str()))
        {
            _params._alarmPreventive = val;
        }
        emit alarmPreventiveChanged(_params._alarmPreventive);
    }
    else{
        emit alarmError();
        emit alarmPreventiveChanged(_params._alarmPreventive);
    }
}

void ControllerCore::setShowCrossMax(bool val)
{
    QString strval = boolToString(val);
    std::istringstream iss;
    if (send(_protocol.prepareMessage(_protocol.set_show_cross_max, strval), iss) && isPositive(iss.str())){
        _params._showCrossMax = val;
        _params._maximum->show = val;
    }
    emit showCrossMaxChanged(_params._showCrossMax);
}

void ControllerCore::setShowCrossUser(bool val)
{
    QString strval = boolToString(val);
    std::istringstream iss;
    if (send(_protocol.prepareMessage(_protocol.set_show_cross_user, strval), iss) && isPositive(iss.str())){
        _params._showCrossUser = val;
        _params._userCross->show = val;
    }
    emit showCrossUserChanged(_params._showCrossUser);
}

void ControllerCore::setShowRoiUserNames(bool val)
{
    QString strval = boolToString(val);
    std::istringstream iss;
    if (send(_protocol.prepareMessage(_protocol.set_show_user_roi_name, strval), iss) && isPositive(iss.str())){
        _params._showUserRoiNames = val;
        _params._userRoiContainer->showNames = val;
        emit showRoiUserNamesChanged(_params._showUserRoiNames);
    }
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

void ControllerCore::setShowUserRoi1(int val)
{
    if(setShowUserRoi(1, val))
    {
        _params._showUserRoi1 = val;
        _params._userRoiContainer->_userRoi1->show = val;
    }
    emit showUserRoiChanged();
}

void ControllerCore::setShowUserRoi2(int val)
{
    if(setShowUserRoi(2, val))
    {
        _params._showUserRoi2 = val;
        _params._userRoiContainer->_userRoi2->show = val;
    }
    emit showUserRoiChanged();
}

void ControllerCore::setShowUserRoi3(int val)
{
    if(setShowUserRoi(3, val))
    {
        _params._showUserRoi3 = val;
        _params._userRoiContainer->_userRoi3->show = val;
    }
    emit showUserRoiChanged();
}

void ControllerCore::setShowUserRoi4(int val)
{
    if(setShowUserRoi(4, val))
    {
        _params._showUserRoi4 = val;
        _params._userRoiContainer->_userRoi4->show = val;
    }
    emit showUserRoiChanged();
}

bool ControllerCore::setShowUserRoi(int roiNumber, int val)
{
    QString strval = QString::number(roiNumber) + " " + _vals.supportedRoiVisibilityModes.at(val);
    std::istringstream iss;
    return send(_protocol.prepareMessage(_protocol.set_show_user_roi, strval), iss) && isPositive(iss.str());
    switch(roiNumber)
    {
    case 1:
        setShowUserRoi1(val);
        break;
    case 2:
        setShowUserRoi2(val);
        break;
    case 3:
        setShowUserRoi3(val);
        break;
    case 4:
        setShowUserRoi4(val);
        break;
    }
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

void ControllerCore::setTemperatureMode(int val)
{
    std::istringstream iss;
    QString param = _vals.supportedTemperatureModes[val];
    if(send(_protocol.prepareMessage(_protocol.set_temperature_mode,param), iss) && isPositive(iss.str()))
    {
        _params._temperatureMode = val;
        emit temperatureModeChanged(_params._temperatureMode);
    }
    fetchBlackbodyRoi();
}


void ControllerCore::setBlackbodyDetection(int val)
{
    std::istringstream iss;
    QString param = _vals.supportedBlackbodyDetections[val];
    if(send(_protocol.prepareMessage(_protocol.set_blackbody_detection,param), iss) && isPositive(iss.str()))
    {
        _params._blackbodyDetection = val;
        emit blackbodyDetectionChanged(_params._blackbodyDetection);
    }
}

void ControllerCore::setBlackbodyArea()
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_blackbody_area), iss) && isPositive(iss.str()))
    {
        fetchBlackbodyRoi();
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
    if(send(_protocol.prepareMessage(_protocol.set_units, val), iss) && isOk(iss.str()))
    {
        if(val == "CELSIUS")
        {
            _params._units = " °C";
        }
        else if(val == "FAHRENHEIT")
        {
            _params._units = " °F";
        }
        emit unitsChanged(val);
        fetchAll();
    }
}

void ControllerCore::setAlarmCritical(double val)
{
    if (val >= _params._alarmPreventive && val <= getAlarmMaxLimit())
    {
        std::istringstream iss;
        if(send(_protocol.prepareMessage(_protocol.set_thresholds, QString::number(_params._alarmPreventive) + " " + QString::number(val)), iss) && isPositive(iss.str()))
        {
            _params._alarmCritical = val;
        }
    }
    else{
        emit alarmError();
    }
    emit alarmCriticalChanged(_params._alarmCritical);
}

void ControllerCore::setLastIp(const QString& ip)
{
    // save to general ip cache and also to current ip cache
    QFile file(_vals.currentIpFileName);
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << ip;
        file.close();
    }
    QFile file0(_vals.lastIpFileName);
    if ( file0.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file0 );
        stream << ip;
        file0.close();
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
    if(!_streamFlagThermal)
        return;
    emit streamFrameChanged();
}

void ControllerCore::onVisibleFrameChanged(){
    if(!_streamFlagVisible)
        return;
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
    return val.contains("TRUE", Qt::CaseInsensitive);
}


void ControllerCore::fetchDioParams()
{
    std::istringstream answ;
    if(send(_protocol.prepareMessage(_protocol.get_dio_connected), answ))
    {
        _params._dioPictureOnTrigger = stringToBool(QString::fromStdString(answ.str()));
        emit dioConnectedChanged();
    }
    if(send(_protocol.prepareMessage(_protocol.get_trigger_mode), answ))
    {
        if(answ.str().find("RISING EDGE TRIGGER") != std::string::npos)
        {
            _params._dioTriggerMode = "RISING-EDGE";
            emit dioTriggerModeChanged();
        }
        else if(answ.str().find("PERMANENT NON-TRIGGER") != std::string::npos)
        {
            _params._dioTriggerMode = "NON-TRIGGERED";
            emit dioTriggerModeChanged();
        }
    }
    if(send(_protocol.prepareMessage(_protocol.get_picture_on_trigger), answ))
    {
        _params._dioPictureOnTrigger = stringToBool(QString::fromStdString(answ.str()));
        emit dioPictureOnTriggerChanged();
    }
    if(send(_protocol.prepareMessage(_protocol.get_min_pulse_width), answ))
    {
        _params._dioMinimumPulseWidth = QString::fromStdString(answ.str());
        emit dioMinimumPulseWidthChanged();
    }
    if(send(_protocol.prepareMessage(_protocol.get_dio_signal), answ))
    {
        if(answ.str().find("INTERNAL") != std::string::npos)
        {
            _params._dioSignal = "INTERNAL";
            emit dioSignalChanged();
        }
        else if(answ.str().find("EXTERNAL") != std::string::npos)
        {
            _params._dioSignal = "EXTERNAL";
            emit dioSignalChanged();
        }
    }
    if(send(_protocol.prepareMessage(_protocol.get_trigger_delay), answ))
    {
        _params._dioTriggerDelay = QString::fromStdString(answ.str());
        emit dioTriggerDelayChanged();
    }
    if(send(_protocol.prepareMessage(_protocol.get_min_detection_time), answ))
    {
        _params._dioMinDetectionTime = QString::fromStdString(answ.str());
        emit dioMinDetectionTimeChanged();
    }
    if(send(_protocol.prepareMessage(_protocol.get_temperature_hysteris), answ))
    {
        _params._dioTempHysteresis =  QString::fromStdString(answ.str());
        emit dioTempHysteresisChanged();
    }
    if(send(_protocol.prepareMessage(_protocol.get_min_alarm_time), answ))
    {
        _params._dioMinAlarmTime =  QString::fromStdString(answ.str());
        emit dioMinAlarmTimeChanged();
    }
    if(send(_protocol.prepareMessage(_protocol.get_dio_logic), answ))
    {
        if(answ.str().find("OPEN") != std::string::npos)
        {
            _params._dioLogic =  "OPEN";
            emit dioLogicChanged();
        }
        else if(answ.str().find("CLOSED") != std::string::npos)
        {
            _params._dioLogic =  "CLOSED";
            emit dioLogicChanged();
        }
    }
    // dio alarm level missing
    if(send(_protocol.prepareMessage(_protocol.get_gate_open_time), answ))
    {
        _params._dioGateOpenTime =  QString::fromStdString(answ.str());
        emit dioGateOpenTimeChanged();
    }
    if(send(_protocol.prepareMessage(_protocol.get_show_dio_status), answ))
    {
        _params._dioShowStatus = stringToBool(QString::fromStdString(answ.str()));
        emit dioPictureOnTriggerChanged();
    }
}

bool ControllerCore::dioConnected()
{
    return _params._dioConnected;
}

QString ControllerCore::dioTriggerMode()
{
    return _params._dioTriggerMode;
}

void ControllerCore::setDioTriggerMode(QString dioTriggerMode)
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_trigger_mode, dioTriggerMode), iss) && isOk(iss.str()))
    {
        _params._dioTriggerMode = dioTriggerMode;
        emit dioTriggerModeChanged();
    }
}

bool ControllerCore::dioPictureOnTrigger()
{
    return _params._dioPictureOnTrigger;
}

void ControllerCore::setdioPictureOnTrigger(bool val)
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_picture_on_trigger,
                                     boolToString(val)), iss) && isOk(iss.str()))
    {
        _params._dioPictureOnTrigger = val;
        emit dioPictureOnTriggerChanged();
    }
}

QString ControllerCore::dioMinimumPulseWidth()
{
    return _params._dioMinimumPulseWidth;
}

void ControllerCore::setDioMinimumPulseWidth(QString minPulse)
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
    if(send(_protocol.prepareMessage(_protocol.set_min_pulse_width, minPulse), iss) && isOk(iss.str()))
    {
        _params._dioMinimumPulseWidth = minPulse;
        emit dioMinimumPulseWidthChanged();
    }
}

QString ControllerCore::dioSignal()
{
    return _params._dioSignal;
}

void ControllerCore::setDioSignal(QString signal)
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_dio_signal, signal), iss) && isOk(iss.str()))
    {
        _params._dioSignal = signal;
        emit dioSignalChanged();
    }
}

QString ControllerCore::dioTriggerDelay()
{
    return _params._dioTriggerDelay;
}

void ControllerCore::setDioTriggerDelay(QString delay)
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_trigger_dalay, delay), iss) && isOk(iss.str()))
    {
        _params._dioTriggerDelay = delay;
        emit dioTriggerDelayChanged();
    }
}

QString ControllerCore::dioMinDetectionTime()
{
    return _params._dioMinDetectionTime;
}

void ControllerCore::setDioMinDetectionTime(QString detectionTime)
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
    if(send(_protocol.prepareMessage(_protocol.set_min_detection_time, detectionTime), iss) && isOk(iss.str()))
    {
        _params._dioMinDetectionTime = detectionTime;
        emit dioMinDetectionTimeChanged();
    }
}

QString ControllerCore::dioTempHysteresis()
{
    return QString::number(_params._dioTempHysteresis.toDouble(), 'f', 2);
}

void ControllerCore::setDioTempHysteresis(QString hyst)
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
    if(send(_protocol.prepareMessage(_protocol.set_temperature_hysteris, hyst), iss) && isOk(iss.str()))
    {
        _params._dioTempHysteresis = hyst;
        emit dioTempHysteresisChanged();
    }
}

QString ControllerCore::dioMinAlarmTime()
{
    return _params._dioMinAlarmTime;
}

void ControllerCore::setDioMinAlarmTime(QString alarmTime)
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
    if(send(_protocol.prepareMessage(_protocol.set_min_alarm_time, alarmTime), iss) && isOk(iss.str()))
    {
        _params._dioMinAlarmTime = alarmTime;
        emit dioMinAlarmTimeChanged();
    }
}

QString ControllerCore::dioLogic()
{
    return _params._dioLogic;
}

void ControllerCore::setDioLogic(QString logic)
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_dio_logic, logic), iss) && isOk(iss.str()))
    {
        _params._dioLogic = logic;
        emit dioLogicChanged();
    }
}

QString ControllerCore::dioAlarm()
{
    return _params._dioAlarm;
}

void ControllerCore::setDioAlarm(QString alarm)
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_dio_alarm_level), iss) && isOk(iss.str()))
    {
        _params._dioAlarm = alarm;
        emit dioAlarmChanged();
    }
}
QString ControllerCore::dioGateOpenTime()
{
    return _params._dioGateOpenTime;
}

void ControllerCore::setDioGateOpenTime(QString gateTime)
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_gate_open_time, gateTime), iss) && isOk(iss.str()))
    {
        _params._dioGateOpenTime = gateTime;
        emit dioGateOpenTimeChanged();
    }
}

bool ControllerCore::dioShowStatus()
{
    return _params._dioShowStatus;
}

void ControllerCore::setDioShowStatus(bool val)
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_show_dio_status, boolToString(val)), iss) && isOk(iss.str()))
    {
        _params._dioShowStatus = val;
        emit dioShowStatusChanged();
    }
}

void ControllerCore::dioReset()
{
    std::istringstream iss;
    if (send(_protocol.prepareMessage(_protocol.restart_dio), iss) && isOk(iss.str()))
    {
        fetchDioParams();
    }
}

void ControllerCore::fetchCurrentUser()
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.get_current_user), iss))
    {
        if(iss.str().find("USER1") != std::string::npos)
        {
            _params._currentUser = "USER1";
            emit currentUserChanged();
        }
        else if(iss.str().find("USER2") != std::string::npos)
        {
            _params._currentUser = "USER2";
            emit currentUserChanged();
        }
        else if(iss.str().find("USER3") != std::string::npos)
        {
            _params._currentUser = "USER3";
            emit currentUserChanged();
        }
        else if(iss.str().find("USER4") != std::string::npos)
        {
            _params._currentUser = "USER4";
            emit currentUserChanged();
        }
        else if(iss.str().find("ADMIN") != std::string::npos)
        {
            _params._currentUser = "ADMIN";
            emit currentUserChanged();
        }
        else
        {
            _params._currentUser = "DEFAULT-USER";
            emit currentUserChanged();
        }
    }
}

bool ControllerCore::loginUser(QString user, QString password)
{
    if(password.isEmpty())
    {
        password = "-";
    }
    QString arg = QString(user + " " + password);
    std::istringstream iss;
    if (send(_protocol.prepareMessage(_protocol.login, arg), iss) && isOk(iss.str()))
    {
        _params._currentUser = user;
        emit currentUserChanged();
    }
    return isOk(iss.str());
}

void ControllerCore::logoutUser()
{
    std::istringstream iss;
    if (send(_protocol.prepareMessage(_protocol.logout), iss) && isOk(iss.str()))
    {
        if(send(_protocol.prepareMessage(_protocol.get_current_user), iss))
        {
            if(iss.str().find("DEFAULT_USER") != std::string::npos)
            {
                _params._currentUser = "DEFAULT-USER";
                emit currentUserChanged();
            }
            else if(iss.str().find("USER1") != std::string::npos)
            {
                _params._currentUser = "USER1";
                emit currentUserChanged();
            }
            else if(iss.str().find("USER2") != std::string::npos)
            {
                _params._currentUser = "USER2";
                emit currentUserChanged();
            }
            else if(iss.str().find("USER3") != std::string::npos)
            {
                _params._currentUser = "USER3";
                emit currentUserChanged();
            }
            else if(iss.str().find("USER4") != std::string::npos)
            {
                _params._currentUser = "USER4";
                emit currentUserChanged();
            }
            else if(iss.str().find("ADMIN") != std::string::npos)
            {
                _params._currentUser = "ADMIN";
                emit currentUserChanged();
            }
        }
    }
}

void ControllerCore::changeUserPassword(QString user, QString newPassword)
{
    if(newPassword.isEmpty())
    {
        newPassword = "-";
    }
    QString arg = QString(user.toLower() + " " + newPassword);
    std::istringstream iss;
    send(_protocol.prepareMessage(_protocol.change_user_password, arg), iss);
}

QString ControllerCore::currentUser()
{
    return _params._currentUser;
}


void ControllerCore::fetchUser(QString user)
{
    std::istringstream iss;
    int userVal = getUserValue(user);
    if(userVal != -1 && send(_protocol.prepareMessage(_protocol.get_all_user_settings), iss))
    {
        QString str = QString::fromStdString(iss.str());
        QVector<QString> lines = str.split("\n").toVector();
        if(lines.size() > 12)
        {
            // range
            _params._currentUserSettings.setRange(isTrueForUser(lines[0].toInt(), userVal));
            // functions
            _params._currentUserSettings.setFunctions(isTrueForUser(lines[1].toInt(), userVal));
            // capture
            _params._currentUserSettings.setCapture(isTrueForUser(lines[2].toInt(), userVal));
            // palette
            _params._currentUserSettings.setPalette ( isTrueForUser(lines[3].toInt(), userVal));
            // measure
            _params._currentUserSettings.setMeasure (isTrueForUser(lines[4].toInt(), userVal));
            // advanced visible
            _params._currentUserSettings.setAdvancedVisible (isTrueForUser(lines[5].toInt(), userVal));
            // advanced image visible
            _params._currentUserSettings.setAdvancedImageVisible (isTrueForUser(lines[6].toInt(), userVal));
            // advanced alarms
            _params._currentUserSettings.setAdvancedAlarms (isTrueForUser(lines[7].toInt(), userVal));
            // advanced memory
            _params._currentUserSettings.setAdvancedMemory (isTrueForUser(lines[8].toInt(), userVal));
            // advanced system
            _params._currentUserSettings.setAdvancedSystem (isTrueForUser(lines[9].toInt(), userVal));
            // advanced roi
            _params._currentUserSettings.setAdvancedRoi (isTrueForUser(lines[10].toInt(), userVal));
            // advanced dio
            _params._currentUserSettings.setAdvancedDio (isTrueForUser(lines[11].toInt(), userVal));
            // advanced layout
            _params._currentUserSettings.setAdvancedLayout (isTrueForUser(lines[12].toInt(), userVal));
            emit currentUserSettingsChanged();
        }
    }
}

int ControllerCore::getUserValue(QString user)
{
    int value = -1;
    if(user == "USER1") {
        value = 1;
    }
    else if(user == "USER2")
    {
        value = 2;
    }
    else if(user == "USER3")
    {
        value = 4;
    }
    else if(user == "USER4")
    {
        value = 8;
    }
    return value;
}

bool ControllerCore::isTrueForUser(int val, int userVal)
{
    if(val >= userVal)
    {
        return true;
    }
    return false;
}

UserSettings* ControllerCore::currentUserSettings()
{
    return &_params._currentUserSettings;
}


void ControllerCore::setCurrentUserSettings( QString user,
                                             bool range,
                                             bool functions,
                                             bool capture,
                                             bool palette,
                                             bool measure,
                                             bool  advancedVisible,
                                             bool  advancedImageVisible,
                                             bool advancedAlarms,
                                             bool advancedMemory,
                                             bool advancedSystem,
                                             bool advancedRoi,
                                             bool advancedDio,
                                             bool advancedLayout)
{
    std::istringstream iss;
    QString arg = user + " 0 " + boolToString(range);
    if(send(_protocol.prepareMessage(_protocol.set_user_settings, arg), iss) && isOk(iss.str()))
    {
        _params._currentUserSettings.setRange(range);
    }

    arg = user+ " 1 " + boolToString(functions);
    if(send(_protocol.prepareMessage(_protocol.set_user_settings, arg), iss) && isOk(iss.str()))
    {
        _params._currentUserSettings.setFunctions(functions);
    }

    arg = user+  " 2 " + boolToString(capture);
    if(send(_protocol.prepareMessage(_protocol.set_user_settings, arg), iss) && isOk(iss.str()))
    {
        _params._currentUserSettings.setCapture(capture);
    }

    arg = user+ " 3 " + boolToString(palette);
    if(send(_protocol.prepareMessage(_protocol.set_user_settings, arg), iss) && isOk(iss.str()))
    {
        _params._currentUserSettings.setPalette(palette);
    }

    arg = user+ " 4 " + boolToString(measure);
    if(send(_protocol.prepareMessage(_protocol.set_user_settings, arg), iss) && isOk(iss.str()))
    {
        _params._currentUserSettings.setMeasure(measure);
    }

    arg = user+ " 5 " + boolToString(advancedVisible);
    if(send(_protocol.prepareMessage(_protocol.set_user_settings, arg), iss) && isOk(iss.str()))
    {
        _params._currentUserSettings.setAdvancedVisible(advancedVisible);
    }

    arg = user+ " 6 " + boolToString(advancedImageVisible);
    if(send(_protocol.prepareMessage(_protocol.set_user_settings, arg), iss) && isOk(iss.str()))
    {
        _params._currentUserSettings.setAdvancedImageVisible(advancedImageVisible);
    }

    arg = user+  " 7 " + boolToString(advancedAlarms);
    if(send(_protocol.prepareMessage(_protocol.set_user_settings, arg), iss) && isOk(iss.str()))
    {
        _params._currentUserSettings.setAdvancedAlarms(advancedAlarms);
    }

    arg = user+ " 8 " + boolToString(advancedMemory);
    if(send(_protocol.prepareMessage(_protocol.set_user_settings, arg), iss) && isOk(iss.str()))
    {
        _params._currentUserSettings.setAdvancedMemory(advancedMemory);
    }

    arg = user+  " 9 " + boolToString(advancedSystem);
    if(send(_protocol.prepareMessage(_protocol.set_user_settings, arg), iss) && isOk(iss.str()))
    {
        _params._currentUserSettings.setAdvancedSystem(advancedSystem);
    }

    arg = user+ " 10 " + boolToString(advancedRoi);
    if(send(_protocol.prepareMessage(_protocol.set_user_settings, arg), iss) && isOk(iss.str()))
    {
        _params._currentUserSettings.setAdvancedRoi(advancedRoi);
    }

    arg = user+ " 11 " + boolToString(advancedDio);
    if(send(_protocol.prepareMessage(_protocol.set_user_settings, arg), iss) && isOk(iss.str()))
    {
        _params._currentUserSettings.setAdvancedDio(advancedDio);
    }

    arg = user+ " 12 " + boolToString(advancedLayout);
    if(send(_protocol.prepareMessage(_protocol.set_user_settings, arg), iss) && isOk(iss.str()))
    {
        _params._currentUserSettings.setAdvancedLayout(advancedLayout);
    }
    emit currentUserSettingsChanged();
}


bool ControllerCore::setDateTime(QString year, QString month, QString day, QString hour, QString minute, QString second)
{
    int yearNum = year.toInt();
    int monthNum = month.toInt();
    int dayNum = day.toInt();
    int hourNum = hour.toInt();
    int minuteNum = minute.toInt();
    int secondNum = second.toInt();
    if(monthNum < 13 && monthNum > 0 && dayNum > 0 && dayNum < 32 && hourNum >= 0 && hourNum < 25 && minuteNum >= 0 && minuteNum < 61 && secondNum >= 0 && secondNum < 61)
    {
        QString arg;
        arg.sprintf("%4d/%2d/%2d-%2d:%2d:%2d", yearNum, monthNum, dayNum, hourNum, minuteNum, secondNum);
        arg.replace(" ", "0");
        std::istringstream iss;
        if(send(_protocol.prepareMessage(_protocol.set_current_datetime, arg), iss) && isOk(iss.str()))
        {
            _systemDate = year + "/"+  month + "/" + day;
            _systemTime = hour + ":" + minute + ":" +  second;
            emit dateTimeChanged();
            return true;
        }
    }
    return false;
}

void ControllerCore::setLanguage(QString language)
{
    std::istringstream iss;
    if(send(_protocol.prepareMessage(_protocol.set_language, language), iss) && isOk(iss.str()))
    {
        _params._language = language;
        emit languageChanged(language);
    }
}

void ControllerCore::setCurrentRoiIdx(int idx)
{
    thermalMutex->lock();
    _params._userRoiContainer->_currentRoi= idx;
    thermalMutex->unlock();
}

void ControllerCore::updateRoiAreaLimits()
{
    int areaW = 640;
    int areaH = 512;
    int borderW = 26;   // 5% of areaW
    int borderH = 21;   // 5% of areaH
    int radius = 50;    // 50 pixel radius
    int radius2 = radius * radius;
    int maxX = areaW - borderW;
    int minX = borderW;
    int maxY = areaH - borderH;
    int minY = borderH;
    // check current roi's corners
    UserRoi * currRoi = nullptr;
    switch(_params._userRoiContainer->_currentRoi)
    {
    case 0:
        currRoi = _params._userRoiContainer->_userRoi1;
        break;
    case 1:
        currRoi = _params._userRoiContainer->_userRoi2;
        break;
    case 2:
        currRoi = _params._userRoiContainer->_userRoi3;
        break;
    case 3:
        currRoi = _params._userRoiContainer->_userRoi4;
        break;
    default:
        return;
    }
    if(!currRoi)
        return;

    int x = currRoi->x;
    int y = currRoi->y;
    int w = currRoi->w;
    int h = currRoi->h;

    /// * max/min is there so new value will fit to all corners

    // left corners
    if(x <= (borderW + radius))
    {
        minY = std::max((int)(borderH + radius - sqrt(radius2 - (x - (borderW + radius)) * (x - (borderW + radius)))), minY);
        maxY = std::min((int)(areaH - borderH - radius + sqrt(radius2 - (x - (borderW + radius)) * (x - (borderW + radius)))), maxY);
    }

    // right corners
    if((x + w) >= (areaW - borderW - radius))
    {
        minY = std::max((int)(borderH + radius - sqrt(radius2 - ((x+w) - (areaW - borderW - 50)) * ((x+w) - (areaW - borderW - 50)))), minY);
        maxY = std::min((int)(areaH - borderH - radius + sqrt(radius2 - ((x+w)-(areaW - borderW - radius)) * ((x+w)-(areaW - borderW - radius)))), maxY);
    }

    // bottom corners
    if(y <= (borderH + radius))
    {
        minX = std::max((int)(borderW + radius - sqrt(radius2 - (y - (borderH + radius)) * (y - (borderH + radius)))),minX);
        maxX = std::min((int)(areaW - borderW - radius + sqrt(radius2 - (y - (borderH + radius)) * (y - (borderH + radius)))), maxX);
    }

    // top corners
    if((y + h) >= (areaH - borderH - radius))
    {
        minX = std::max((int)(borderW + radius - sqrt(radius2 - ((y+h)-(areaH - borderH - radius)) * ((y+h)-(areaH - borderH - radius)))), minX);
        maxX = std::min((int)(areaW - borderW - radius + sqrt(radius2 - ((y+h) - (areaH - borderH - radius)) * ((y+h) - (areaH - borderH - radius)))), maxX);
    }

    // update in params
    _params._userRoiContainer->roiAreaMaxX = maxX;
    _params._userRoiContainer->roiAreaMinX = minX;
    _params._userRoiContainer->roiAreaMaxY = maxY;
    _params._userRoiContainer->roiAreaMinY = minY;
    emit roiAreaMaxXChanged();
    emit roiAreaMinXChanged();
    emit roiAreaMaxYChanged();
    emit roiAreaMinYChanged();
}

int ControllerCore::roiAreaMaxX()
{
    return _params._userRoiContainer->roiAreaMaxX;
}

int ControllerCore:: roiAreaMinX()
{
    return _params._userRoiContainer->roiAreaMinX;
}

int ControllerCore:: roiAreaMaxY()
{
    return _params._userRoiContainer->roiAreaMaxY;
}

int ControllerCore::roiAreaMinY()
{
    return _params._userRoiContainer->roiAreaMinY;
}

void ControllerCore::updateUserCrossLimits()
{
    int areaW = 640;
    int areaH = 512;
    int borderW = 26;   // 5% of areaW
    int borderH = 21;   // 5% of areaH
    int radius = 50;    // 50 pixel radius
    int radius2 = radius * radius;
    int maxX = areaW - borderW;
    int minX = borderW;
    int maxY = areaH - borderH;
    int minY = borderH;

    int x = _params._userCross->x;
    int y = _params._userCross->y;


    // left corners
    if(x <= (borderW + radius))
    {
        minY = (int)(borderH + radius - sqrt(radius2 - (x - (borderW + radius)) * (x - (borderW + radius))));
        maxY = (int)(areaH - borderH - radius + sqrt(radius2 - (x - (borderW + radius)) * (x - (borderW + radius))));
    }

    // right corners
    else if (x >= (areaW - borderW - radius)) {
        minY = (int)(borderH + radius - sqrt(radius2 - (x - (areaW - borderW - 50)) * (x - (areaW - borderW - 50))));
        maxY = (int)(areaH - borderH - radius + sqrt(radius2 - (x-(areaW - borderW - radius)) * (x -(areaW - borderW - radius))));
    }

    // bottom corners
    if(y <= (borderH + radius))
    {
        minX = (int)(borderW + radius - sqrt(radius2 - (y - (borderH + radius)) * (y - (borderH + radius))));
        maxX = (int)(areaW - borderW - radius + sqrt(radius2 - (y - (borderH + radius)) * (y - (borderH + radius))));
    }

    // top corners
    else if (y >= (areaH - borderH - radius)) {
        minX = (int)(borderW + radius - sqrt(radius2 - (y - (areaH - borderH - radius)) * (y-(areaH - borderH - radius))));
        maxX = (int)(areaW - borderW - radius + sqrt(radius2 - (y - (areaH - borderH - radius)) * (y - (areaH - borderH - radius))));
    }

    // update in params
    _params._userCrossMaxX = maxX;
    _params._userCrossMinX = minX;
    _params._userCrossMaxY = maxY;
    _params._userCrossMinY = minY;
    emit userCrossMaxXChanged();
    emit userCrossMinXChanged();
    emit userCrossMaxYChanged();
    emit userCrossMinYChanged();
}

int ControllerCore::userCrossMaxX()
{
    return _params._userCrossMaxX;
}

int ControllerCore::userCrossMinX()
{
    return _params._userCrossMinX;
}

int ControllerCore::userCrossMaxY()
{
    return _params._userCrossMaxY;
}

int ControllerCore::userCrossMinY()
{
    return _params._userCrossMinY;
}


void ControllerCore::updateBlackBodyLimits()
{
    int areaW = 640;
    int areaH = 512;
    int borderW = 26;   // 5% of areaW
    int borderH = 21;   // 5% of areaH
    int radius = 50;    // 50 pixel radius
    int radius2 = radius * radius;
    int maxX = areaW - borderW;
    int minX = borderW;
    int maxY = areaH - borderH;
    int minY = borderH;

    int x = _params._blackbodyRoiX;
    int y = _params._blackbodyRoiY;


    // left corners
    if(x <= (borderW + radius))
    {
        minY = (int)(borderH + radius - sqrt(radius2 - (x - (borderW + radius)) * (x - (borderW + radius))));
        maxY = (int)(areaH - borderH - radius + sqrt(radius2 - (x - (borderW + radius)) * (x - (borderW + radius))));
    }

    // right corners
    else if (x >= (areaW - borderW - radius)) {
        minY = (int)(borderH + radius - sqrt(radius2 - (x - (areaW - borderW - 50)) * (x - (areaW - borderW - 50))));
        maxY = (int)(areaH - borderH - radius + sqrt(radius2 - (x-(areaW - borderW - radius)) * (x -(areaW - borderW - radius))));
    }

    // bottom corners
    if(y <= (borderH + radius))
    {
        minX = (int)(borderW + radius - sqrt(radius2 - (y - (borderH + radius)) * (y - (borderH + radius))));
        maxX = (int)(areaW - borderW - radius + sqrt(radius2 - (y - (borderH + radius)) * (y - (borderH + radius))));
    }

    // top corners
    else if (y >= (areaH - borderH - radius)) {
        minX = (int)(borderW + radius - sqrt(radius2 - (y - (areaH - borderH - radius)) * (y-(areaH - borderH - radius))));
        maxX = (int)(areaW - borderW - radius + sqrt(radius2 - (y - (areaH - borderH - radius)) * (y - (areaH - borderH - radius))));
    }

    // update in params
    _params._blackbodyMaxX = maxX;
    _params._blackbodyMinX = minX;
    _params._blackbodyMaxY = maxY;
    _params._blackbodyMinY = minY;
    emit blackBodyMaxXChanged();
    emit blackBodyMinXChanged();
    emit blackBodyMaxYChanged();
    emit blackBodyMinYChanged();
}

int ControllerCore::blackBodyMaxX()
{
    return _params._blackbodyMaxX;
}

int ControllerCore::blackBodyMinX()
{
    return _params._blackbodyMinX;
}

int ControllerCore::blackBodyMaxY()
{
    return _params._blackbodyMaxY;
}

int ControllerCore::blackBodyMinY()
{
    return _params._blackbodyMinY;
}

void ControllerCore::openManual()
{
    QDesktopServices::openUrl(QUrl("http://software.workswell.cz/subdom/software/medicas-ethernet-stream-sdk/windows/medicas-ethernet-stream-sdk.pdf"));
}

void ControllerCore::mainCameraSnapshot()
{
    if(_params._mainCamera == "THERMAL")
    {
        if (!streamFrame.isNull())
            streamFrame.save(_saveDest + "/" + QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss-zzz") + "_thermal.jpg");
    }
    else
    {
        if (!streamFrameVisible.isNull())
            streamFrameVisible.save(_saveDest + "/" + QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss-zzz") + "_visible.jpg");
    }
}

void ControllerCore::secondaryCameraSnapshot()
{
    if(_params._mainCamera == "VISIBLE")
    {
        if (!streamFrame.isNull())
            streamFrame.save(_saveDest + "/" + QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss-zzz") + "_thermal.jpg");
    }
    else
    {
        if (!streamFrameVisible.isNull())
            streamFrameVisible.save(_saveDest + "/" + QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss-zzz") + "_visible.jpg");
    }
}

bool ControllerCore::setRoiNames(QString roi1Name, QString roi2Name, QString roi3Name, QString roi4Name)
{
    std::istringstream iss;
    QString arg = "1 " + roi1Name;
    bool ret = true;
    if(send(_protocol.prepareMessage(_protocol.set_user_roi_name, arg), iss) && isOk(iss.str()))
    {
        _params._userRoiContainer->_userRoi1->name = roi1Name;
    }
    else ret = false;
    arg = "2 " + roi2Name;
    if(send(_protocol.prepareMessage(_protocol.set_user_roi_name, arg), iss) && isOk(iss.str()))
    {
        _params._userRoiContainer->_userRoi2->name = roi2Name;
    }
    else ret = false;
    arg = "3 " + roi2Name;
    if(send(_protocol.prepareMessage(_protocol.set_user_roi_name, arg), iss) && isOk(iss.str()))
    {
        _params._userRoiContainer->_userRoi3->name = roi3Name;
    }
    else ret = false;
    arg = "4 " + roi2Name;
    if(send(_protocol.prepareMessage(_protocol.set_user_roi_name, arg), iss) && isOk(iss.str()))
    {
        _params._userRoiContainer->_userRoi4->name = roi4Name;
    }
    else ret = false;
    return ret;
}

int ControllerCore::blackBodyRoiScale()
{
    return _params._blackbody->scale;
}

void ControllerCore::setBlackBodyRoiScale(int val)
{
    std::istringstream iss;
    if(val < 70)
        val = 70;
    if(val > 130)
        val = 130;
    if(send(_protocol.prepareMessage(_protocol.set_blackbody_scale, QString::number(val)), iss) && isOk(iss.str()))
    {
        _params._blackbody->scale = val;
        emit blackBodyRoiScaleChanged();
    }
}

void ControllerCore::onCameraDisconnected()
{
    qCritical() << "Camera disconnected!";
    emit cameraDisconnected();
    _helperTreadFlag = false;
    _streamFlagThermal = false;
    _streamFlagVisible = false;
    _connected = false;
}

double ControllerCore::getManualMinLimit()
{
    if(_params._units == " °C")
        return 0.0;
    else
        return 32.0;
}

double ControllerCore::getManualMaxLimit()
{
    if(_params._units == " °C")
        return 50.0;
    else
        return 122.0;
}

double ControllerCore::getManualDiffLimit()
{
    if(_params._units == " °C")
        return 10.0;
    else
        return 18.0;
}

double ControllerCore::getAlarmMinLimit()
{
    if(_params._units == " °C")
        return 35.0;
    else
        return 95.0;
}

double ControllerCore::getAlarmMaxLimit()
{
    if(_params._units == " °C")
        return 42.0;
    else
        return 107.6;
}

double ControllerCore::getCriticalUpperBoundary()
{
    double tempInC = 42.0;
    double offset = _vals.temperatureModeToOffset.find(_vals.supportedTemperatureModes[_params._temperatureMode])->second;

    if(_params._units == " °C")
        return tempInC + offset;
    else
        return 107.6;
}

void ControllerCore::setSpecialPheripheralOutput(bool val)
{
    send(_protocol.prepareMessage(_protocol.set_special_pheripheral_output,
                                  boolToString(val)));
}


void ControllerCore::openPage() {
    QDesktopServices::openUrl(QUrl("https://www.workswell.eu/"));
}
