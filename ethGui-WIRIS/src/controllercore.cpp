#include "controllercore.h"

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
    //it takes wiris ip and communication port(2240) as constructor parameters
    //from now on it is fairly easy to call method send on network client instance with any command as parameter
    //network client should automatically disconnect from WIRIS in its destructor
    if(this->_networkClient == nullptr || !*this->_networkClient || !_connected)
        _networkClient = std::shared_ptr<NetworkClient>(new NetworkClient(ip.toStdString(), this->_endpointPort.toStdString()));

    if (*_networkClient)
    {
        std::istringstream iss;
        //Sending test message
        if(!send("HIWS\n", iss) || !isPositive(iss.str()))
            return false;

        //Setting delimiter to null character
        send("SDLM NULL\n");

        //checking camery type
        fetchBasicInfo();
        if(_cameraType != _vals.wirisProType && _cameraType != _vals.wirisSecurityType)
            return false;

        //we are now connected, so we can save passed IP as valid application state
        _endpointIp = ip;

        //we can even save it to local storage so app could remember last used ip on startup
        setLastIp(_endpointIp);
    }

    _connected = *_networkClient;
    emit connectedChanged(_connected, 10);

    return _connected;

}

bool ControllerCore::isActivated(){
    std::istringstream iss;

    if(!send("IACTV", iss))
        return false;

    return isPositive(iss.str());
}

bool ControllerCore::activate(const QString & actStr)
{
    if (!isActivated())
    {
        std::istringstream iss;
        if(send("ACTV " + actStr, iss) && isPositive(iss.str()))
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

        //Starting Ethernet Mode
        send("SETH TRUE");
        send("GETH");

        //setting SSD as capture storage by default
        send("SILC SSD");

        //fetching data from camera to sync the app
        fetchBasicInfo();
        fetchEnviroments();
        fetchRangeModes();
        fetchManualRange();
        fetchSpanRange();
        fetchZooms();
        fetchCurrentZooms();
        fetchEmissivity();
        fetchPalettes();
        fetchPaletteValues();
        fetchShutter();
        fetchCaptureRecSettings();
        fetchAlarms();

        _isSetup = true;

        //initiating streaming if not already in progress
        if (!_streamFlagThermal)
            toggleStreamThermal();
        if (!_streamFlagVisible)
            toggleStreamVisible();

        //initiating helper thread for fetching data from WIRIS during runtime repeatedly
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
    stopStream();
    _helperTreadFlag = false;
    _networkClient->disconnect();
}

void ControllerCore::setDefaultSettings(){
    stopStream();
    _helperTreadFlag = false;
    send("SDST");
    close();
    QCoreApplication::quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void ControllerCore::reboot(){
    if (!compareFw("1.1.1", _firmwareVersion))
        return;

    _helperTreadFlag = false;
    stopStream();
    send("REBT");
    close();
    QCoreApplication::quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

bool ControllerCore::changeStorage(const QString & storage){
    if (storage == _currentStorage)
        return true;

    std::istringstream iss;
    std::string cache;

    if (storage == "SSD" || storage == "SD_CARD" || storage == "FLASH_DRIVE")
    {
        if (send("SILC " + storage, iss) && isPositive(iss.str()))
        {
            _currentStorage = storage;
            return true;
        }
    }

    return false;
}


void ControllerCore::paintMax(bool paint){
    _maximum->show = paint;
}

void ControllerCore::paintMin(bool paint){
    _minimum->show = paint;
}


void ControllerCore::setSyncShutter(bool val)
{
    if (_cameraType == _vals.wirisSecurityType)
        return;

    QString strval = val ? "TRUE" : "FALSE";
    std::istringstream iss;
    if (send("STSC " + strval, iss) && isPositive(iss.str())){
        _syncShutter = val;
    }
    emit syncShutterChanged(_syncShutter);
}

void ControllerCore::setShutterPeriod(int val)
{
    if (_cameraType == _vals.wirisSecurityType)
        return;

    std::istringstream iss;
    if (val >= 2 && val <= 30)
    {
        if (send("STCP " + QString::number(val*60), iss) && isPositive(iss.str())){
            _shutterPeriodMin = val;
        }
    }
    emit shutterPeriodChanged(_shutterPeriodMin);
}


void ControllerCore::triggerCapture(){
    send("CPTR");
}

void ControllerCore::toggleRecording(){
    if(!_recording && (_visVid || _radVid))
        send("RCRS");

    else send("RCRF");
}

void ControllerCore::toggleStreamThermal(){

    if (!this->_isSetup)
        return;

    if (!_streamFlagThermal)
    {
        _streamFlagThermal = true;
        QTypedArrayData<QString>::const_iterator* zoom = nullptr;
        if (_currentZoomThermal >= _zoomsThermal.begin() && _currentZoomThermal < _zoomsThermal.end())
            zoom = &_currentZoomThermal;

        _thermalThread = new ThermalThread(&this->_streamFlagThermal, this->_endpointIp, _maximum, _minimum, &_paintVals, &_paintCenter, zoom, &_cameraType);
        QObject::connect(_thermalThread, &ThermalThread::imageSourceChanged, this, &ControllerCore::onThermalFrameChanged);
        _thermalThread->start();
    } else _streamFlagThermal = false;

}

void ControllerCore::toggleStreamVisible(){
    if (!this->_isSetup)
        return;

    if (!_streamFlagVisible)
    {
        _streamFlagVisible = true;
        _visibleThread = new VisibleThread(&this->_streamFlagVisible, this->_endpointIp);
        QObject::connect(_visibleThread, &VisibleThread::imageSourceChanged, this, &ControllerCore::onVisibleFrameChanged);
        _visibleThread->start();
    }
    else _streamFlagVisible = false;
}

void ControllerCore::setAlarmColors(const QString& above, const QString& between, const QString& below)
{
    if (!compareFw("1.1.1", _firmwareVersion) || _cameraType == _vals.wirisSecurityType)
        return;

    if (_vals.supportedAlarmColors.contains(above) && _vals.supportedAlarmColors.contains(below) && _vals.supportedAlarmColors.contains(between))
    {
        send("SALC " + above + " " + between + " " + below);
    }
}


void ControllerCore::setCaptureVideoSettings(bool radJpg, bool radTiff, bool srImg, bool visJpg, bool radVid, bool visVid)
{
    std::istringstream iss;

    if (_radJpg != radJpg)
    {
        if (send("SIRJ " + boolToString(radJpg), iss) && isPositive(iss.str()))
            _radJpg = radJpg;
    }

    if (_cameraType != _vals.wirisSecurityType)
    {
        if (_radTiff != radTiff)
        {
            if (send("SIRT " + boolToString(radTiff), iss) && isPositive(iss.str()))
                _radTiff = radTiff;
        }

        if (_srImg != srImg)
        {
            if (send("SISR " + boolToString(srImg), iss) && isPositive(iss.str()))
                _srImg = srImg;
        }
    }

    if (_visJpg != visJpg)
    {
        if (send("SIVI " + boolToString(visJpg), iss) && isPositive(iss.str()))
            _visJpg = visJpg;
    }

    if (_radVid != radVid)
    {
        if (send("SVTH " + boolToString(radVid), iss) && isPositive(iss.str()))
            _radVid= radVid;
    }

    if (_visVid != visVid)
    {
        if (send("SVVI " + boolToString(visVid), iss) && isPositive(iss.str()))
            _visVid = visVid;
    }

    emit capRecSettingChanged(_radJpg, _radTiff, _srImg, _visJpg, _radVid, _visVid);
}

void ControllerCore::setCurrentPaletteByIndex(int i){
    if (_palettes.empty())
        return;
    if(i >= 0 && i < _palettes.size())
    {
        if (send("SPTE " + _palettes.at(i)->value()))
        {
            this->_currentPalette = _palettes.at(i)->value();
            fetchPaletteValues();
            emit currentPaletteChanged(_currentPalette);
        }
    }
}

void ControllerCore::changeZoomThermal(bool direction){

    if (direction)
    {
        if (_currentZoomThermal+1 < _zoomsThermal.end() && send("SZTN " + QString::number(_currentZoomThermal+1 - _zoomsThermal.begin())))
        {
            QMutexLocker lock(thermalMutex);
            _currentZoomThermal++;
            emit currentZoomThermalChanged(*_currentZoomThermal);
        }
    }
    else {
        if (_currentZoomThermal > _zoomsThermal.begin() && send("SZTN " + QString::number(_currentZoomThermal-1 - _zoomsThermal.begin())))
        {
            QMutexLocker lock(thermalMutex);
            _currentZoomThermal--;
            emit currentZoomThermalChanged(*_currentZoomThermal);
        }
    }
}

void ControllerCore::changeZoomVisible(bool direction){
    qDebug() << "zoomdir: "<< direction;
    if (direction)
    {
        if (_currentZoomVisible+1 < _zoomsVisible.end() && send("SZVN " + QString::number(_currentZoomVisible+1 - _zoomsVisible.begin())))
        {
            _currentZoomVisible++;
            emit currentZoomVisibleChanged(*_currentZoomVisible);
        }
    }
    else {
        if (_currentZoomVisible > _zoomsVisible.begin() && send("SZVN " + QString::number(_currentZoomVisible-1 - _zoomsVisible.begin())))
        {
            _currentZoomVisible--;
            emit currentZoomVisibleChanged(*_currentZoomVisible);
        }
    }
}

void ControllerCore::setRangeMode(int value)
{
    if (_cameraType == _vals.wirisSecurityType || value < 0 || value > 2)
        return;

    QString req = "";
    switch (value) {
        case 0:
        req += _vals.rangeModeAutomatic;
        break;
        case 1:
        req += _vals.rangeModeManual;
        break;
        case 2:
        req += _vals.rangeModeSpan;
        break;
        default:
        return;
    }

    std::istringstream resp;
    if (send("SRMD " + req, resp))
    {
        if(isPositive(resp.str()))
        {
            this->_rangeMode = value;
            emit rangeModeChanged(_rangeMode);
        }
    }

    if(_rangeMode == 1)
        fetchManualRange();
    else if (_rangeMode == 2)
        fetchSpanRange();
}

void ControllerCore::setEnviroment1(const QString & env)
{
    if (_cameraType == _vals.wirisSecurityType)
        return;

    if (changeEnviroment(_enviroment1)){
        _currentEnviroment = _enviroment1;
        emit currentEnviromentChanged();
    }
}

void ControllerCore::setEnviroment2(const QString & env)
{
    if (_cameraType == _vals.wirisSecurityType)
        return;

    if (changeEnviroment(_enviroment2)){
        _currentEnviroment = _enviroment2;
        emit currentEnviromentChanged();
    }
}

QString ControllerCore::enviromentLowerThreshold() const
{
    if (!this->_currentEnviroment.isEmpty())
    {
        return _currentEnviroment.mid(0, _currentEnviroment.lastIndexOf(' '));
    }

    return "";
}


QString ControllerCore::enviromentUpperThreshold() const
{
    if (!this->_currentEnviroment.isEmpty())
    {
        return _currentEnviroment.mid(_currentEnviroment.lastIndexOf(' '));
    }

    return "";
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
        _helperThread = new HelperThread(&_helperTreadFlag, &_extremesFlag, fp, _maximum, _minimum, &_firmwareVersion, &_cameraType);
        QObject::connect(_helperThread, &HelperThread::extremesChanged, this, &ControllerCore::onHelperExtremesChanged);
        QObject::connect(_helperThread, &HelperThread::tempsChanged, this, &ControllerCore::onHelperTempsChanged);
        QObject::connect(_helperThread, &HelperThread::captureRecordingChanged , this, &ControllerCore::onHelpercaptureRecordingChanged);
        QObject::connect(_helperThread, &HelperThread::driveStateChanged , this, &ControllerCore::onHelperdriveStateChanged);
        QObject::connect(_helperThread, &HelperThread::dateTimeChanged , this, &ControllerCore::onHelperDateTImeChanged);
        QObject::connect(_helperThread, &HelperThread::connectionStateChanged, this, &ControllerCore::onHelperConnectionStateChanged);
        QObject::connect(_helperThread, &HelperThread::gpsChanged, this, &ControllerCore::onHelperGpsChanged);
        QObject::connect(_helperThread, &HelperThread::shutterDataChanged, this, &ControllerCore::shutterDataChanged);
        _helperThread->start();
    }
    else _helperTreadFlag = false;
}

void ControllerCore::onHelperExtremesChanged()
{
    if(_rangeMode == 0)
    {
        _paletteTop = _maximum->val;
        _paletteBottom = _minimum->val;
        emit paletteTopChanged(_paletteTop);
        emit paletteBottomChanged(_paletteBottom);
    }
    emit extremesChanged();
}

void ControllerCore::onHelperTempsChanged(double cpu, double cam){
    _cpuTemp = cpu;
    _camTemp = cam;
    emit cpuTempChanged(cpu);
    emit camTempChanged(cam);
}

void ControllerCore::onHelpercaptureRecordingChanged(bool capture, bool record)
{
    _capturing = capture;
    _recording = record;
    emit captureRecordingChanged(capture, record);
}

void ControllerCore::onHelperdriveStateChanged(const QString& ssd, const QString& ssdCap, const QString& ssdFree,
                               const QString& sd, const QString& sdCap, const QString& sdFree,
                               const QString& flash, const QString& flashCap, const QString& flashFree)
{
    _ssdDrive = ssd;
    _sdDrive = sd;
    _flashDrive = flash;
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

void ControllerCore::onHelperGpsChanged(const QString& gps){
    emit gpsChanged(gps);
}



bool ControllerCore::changeEnviroment(const QString &env){
    if(env.size() > 7 && env.contains(" "))
    {
        if(env == enviroment1())
        {
            setManualRange1(_enviroment1.mid(0, _currentEnviroment.lastIndexOf(' ')));
            setManualRange2(_enviroment1.mid(_currentEnviroment.lastIndexOf(' ')));
        }
        return this->send("SREN " + env.mid(env.lastIndexOf(' ')));
    }
    return false;

}

void ControllerCore::fetchEnviroments()
{
    if (_cameraType == _vals.wirisSecurityType)
        return;

    std::string cache;
    std::istringstream envList;
    if (send("GREL", envList))
    {
        getAnyLine(envList, cache);
        _enviroment1 = QString::fromStdString(cache);

        getAnyLine(envList, cache);
        _enviroment2 = QString::fromStdString(cache);

        send("GREN", envList);
        getAnyLine(envList, cache);
        this->_currentEnviroment = QString::fromStdString(cache);
        emit enviroment1Changed();
        emit enviroment2Changed();
        emit currentEnviromentChanged();
    }

}

void ControllerCore::fetchRangeModes(){
    if (_cameraType == _vals.wirisSecurityType)
        return;

    std::istringstream iss;
    if (send("GRMD", iss))
    {
        std::string resp;
        getAnyLine(iss, resp);

        if (resp == _vals.rangeModeAutomatic.toStdString())
            this->_rangeMode = 0;
        else if (resp == _vals.rangeModeManual.toStdString())
            this->_rangeMode = 1;
        else if (resp == _vals.rangeModeSpan.toStdString())
            this->_rangeMode = 2;
        emit rangeModeChanged(_rangeMode);
    }

}

void ControllerCore::fetchManualRange()
{
    if (_cameraType == _vals.wirisSecurityType)
        return;

    std::istringstream iss;
    std::string cache;
    if(send("GRMM", iss))
    {
        std::getline(iss, cache, ' ');
        this->_manualRange1 = QString::fromStdString(cache);

        getAnyLine(iss, cache);
        this->_manualRange2 = QString::fromStdString(cache);

        emit manualRange2Changed(_manualRange2);
        emit manualRange1Changed(_manualRange1);

        _paletteTop = _manualRange2.toDouble();
        _paletteBottom = _manualRange1.toDouble();
        emit paletteTopChanged(_paletteTop);
        emit paletteBottomChanged(_paletteBottom);

        qDebug() << "fetched manual range 1: " << _manualRange1 <<" r2: " << _manualRange2;
    }
}

void ControllerCore::fetchSpanRange()
{
    if (_cameraType == _vals.wirisSecurityType)
        return;

    std::istringstream iss;
    std::string cache;
    if(send("GRWC", iss))
    {
        std::getline(iss, cache, ' ');
        this->_spanWindow = QString::fromStdString(cache);

        getAnyLine(iss, cache);
        this->_spanCenter = QString::fromStdString(cache);

        emit spanCenterChanged(QString::number(round(_spanCenter.toDouble()*100)/100));
        emit spanWindowChanged(QString::number(round(_spanWindow.toDouble()*100)/100));

        _paletteTop = _spanCenter.toDouble() + _spanWindow.toDouble()/2;
        _paletteBottom = _spanCenter.toDouble() - _spanWindow.toDouble()/2;
        emit paletteTopChanged(_paletteTop);
        emit paletteBottomChanged(_paletteBottom);
    }
}


void ControllerCore::fetchZooms(){
    std::istringstream iss, line;
    std::string cache, val;
    _zoomsThermal.clear();
    _zoomsVisible.clear();

    if (send("GZTL", iss))
    {
        while(getAnyLine(iss, cache))
        {
            if(!cache.empty())
            {
                line = std::istringstream(cache);
                std::getline(line, val, ' ');
                getAnyLine(line, val);
                _zoomsThermal.push_back(QString::fromStdString(val));
                qDebug() << "thermal loaded: " << QString::fromStdString(val);
            }
        }
    }
    iss.clear();
    if (send("GZVL", iss))
    {
        while(getAnyLine(iss, cache))
        {
            if(!cache.empty())
            {
                line = std::istringstream(cache);
                std::getline(line, val, ' ');
                getAnyLine(line, val);
                _zoomsVisible.push_back(QString::fromStdString(val));
                qDebug() << "visible loaded: " << QString::fromStdString(val);
            }
        }
    }


}

void ControllerCore::fetchPalettes(){
    _palettes.clear();
    std::string cache;
    std::istringstream iss;
    if (send("GPTE", iss))
    {
        getAnyLine(iss, cache);
        this->_currentPalette = QString::fromStdString(cache);
        send("SPTE " + _currentPalette);
    }


    if (send("GPTL", iss))
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

    if (send("GPLV", iss))
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

void ControllerCore::fetchCurrentZooms()
{
    std::istringstream iss;
    std::string cache;

    send("SZVN 0");
    send("SZTN 0");

    _currentZoomThermal = _zoomsThermal.begin();
    _currentZoomVisible = _zoomsVisible.begin();

    if (send("GZTV", iss))
    {
        getAnyLine(iss, cache);
        for(int i = 0; i < _zoomsThermal.size(); i++)
        {
            if (_zoomsThermal.at(i).toStdString() == cache){
                _currentZoomThermal = _zoomsThermal.begin() + i;
            }
        }

        emit currentZoomThermalChanged(currentZoomThermal());
    }
    iss.clear();
    cache.clear();

    if (send ("GZVV", iss))
    {
        getAnyLine(iss, cache);
        for(int i = 0; i < _zoomsVisible.size(); i++)
        {
            if (_zoomsVisible.at(i).toStdString() == cache){
                _currentZoomVisible = _zoomsVisible.begin() + i;
            }
        }

        emit currentZoomVisibleChanged(currentZoomVisible());
    }
}

void ControllerCore::fetchEmissivity(){
    if (_cameraType == _vals.wirisSecurityType)
            return;

    std::istringstream iss;
    if(send("GTEM", iss))
    {
        double val = 0;
        iss >> val;
        if (val > 0.5 && val < 1)
            _emissivity = val;
        else setEmissivity(_emissivity);

        emit emissivityChanged(_emissivity);
    }
}


void ControllerCore::fetchBasicInfo()
{
    std::istringstream iss;
    std::string cache;

    send("GFWV", iss);
    getAnyLine(iss, cache);
    this->_firmwareVersion = QString::fromStdString(cache);

    send("GSRN", iss);
    getAnyLine(iss, cache);
    this->_serialNumber = QString::fromStdString(cache);

    if (serialNumber().contains("WWS"))
        this->_cameraType = _vals.wirisSecurityType;
    else if(serialNumber().contains("WWP"))
        this->_cameraType = _vals.wirisProType;

    send("GATN", iss);
    getAnyLine(iss, cache);
    this->_articleNumber = QString::fromStdString(cache);

    emit typeChanged(_cameraType);
    emit serialNumberChanged(serialNumber());
    emit articleNumberChanged(_articleNumber);
    emit firmwareVersionChanged(firmwareVersion());
    emit ipAdressChanged(ipAddress());
}


void ControllerCore::fetchShutter()
{
    if (_cameraType == _vals.wirisSecurityType)
        return;


    std::istringstream iss;
    double shuttersecs;
    if (send("GTSC", iss))
    {
        _syncShutter = isPositive(iss.str());
    }



    if (send("GTCP", iss))
    {
        iss >> shuttersecs;
        _shutterPeriodMin = round(shuttersecs/60);
    }


    emit syncShutterChanged(_syncShutter);
    emit shutterPeriodChanged(_shutterPeriodMin);
}

void ControllerCore::fetchCaptureRecSettings(){
    std::istringstream iss;

    send("GIRJ", iss);
    _radJpg = isPositive(iss.str());

    if (_cameraType != _vals.wirisSecurityType)
    {
        send("GIRT", iss);
        _radTiff = isPositive(iss.str());

        send("GISR", iss);
        _srImg = isPositive(iss.str());
    }
    send("GIVI", iss);
    _visJpg = isPositive(iss.str());

    send("GVTH", iss);
    _radVid = isPositive(iss.str());

    send("GVVI", iss);
    _visVid = isPositive(iss.str());
    emit capRecSettingChanged(_radJpg, _radTiff, _srImg, _visJpg, _radVid, _visVid);

    qDebug() <<"fetched settings: " <<_radJpg << _radTiff << _srImg << _visJpg<< _radVid<< _visVid;

}

void ControllerCore::fetchAlarms()
{
    if (_cameraType == _vals.wirisSecurityType)
        return;

    std::istringstream iss;
    std::string cache;
    QString qcache;

    send("SALM OFF");

    if (send("GALM", iss))
    {
        getAnyLine(iss, cache);
        qcache = QString::fromStdString(cache);
        if (qcache == _vals.alarmModeOFF)
            _alarmMode = 0;
        else if (qcache == _vals.alarmModeABOVE)
            _alarmMode = 1;
        else if (qcache == _vals.alarmModeBELOW)
            _alarmMode = 2;
        else if (qcache == _vals.alarmModeBETWEEN)
            _alarmMode = 3;
        else if (qcache == _vals.alarmModeOUTSIDE)
            _alarmMode = 4;
    }

    if (send("GALV", iss))
    {
        std::getline(iss, cache, ' ');
        _alarmFrom = std::stod(cache);
        getAnyLine(iss, cache);
        _alarmTo = std::stod(cache);
    }

    emit alarmModeChanged(_alarmMode);
    emit alarmFromChanged(_alarmFrom);
    emit alarmToChanged(_alarmTo);

    //fetch colors if current firmware supports them
    if (compareFw("1.1.1", _firmwareVersion))
    {
        if(send("GALC", iss))
        {
            QString above, between, below;
            std::getline(iss, cache,' ');
            above = QString::fromStdString(cache);
            std::getline(iss, cache,' ');
            between = QString::fromStdString(cache);
            getAnyLine(iss, cache);
            below = QString::fromStdString(cache);
            emit alarmColorsChanged(_vals.alarmColorToIndex.at(above), _vals.alarmColorToIndex.at(between), _vals.alarmColorToIndex.at(below));
        }
    }
}

void ControllerCore::setEmissivity(double val)
{
    if (_cameraType == _vals.wirisSecurityType)
        return;

    std::istringstream iss;
    double resp;
    if (val > 0.5 && val <= 1 && send("STEM " + QString::number(val), iss))
    {
        iss >> resp;
        if (abs(resp - val) < 0.01)
        {
            _emissivity = val;
            emit emissivityChanged(val);
        }
    }
}

void ControllerCore::setManualRange1(const QString& val)
{
    if (enviromentLowerThreshold().isEmpty() || enviromentUpperThreshold().isEmpty() || val == _manualRange1 || _cameraType == _vals.wirisSecurityType)
        return;

    if(val.toDouble() >= enviromentLowerThreshold().toDouble() && val.toDouble() < _manualRange2.toDouble())
    {
        std::istringstream iss;
        std::string cache;
        std::ostringstream oss;
        oss << "SRMM " << val.toDouble() << " " << _manualRange2.toDouble();

        if(send(QString::fromStdString(oss.str()), iss))
        {
            if (isOk(iss.str()))
            {
                _manualRange1 = val;
                emit manualRange1Changed(_manualRange1);
                _paletteBottom = _manualRange1.toDouble();
                emit paletteBottomChanged(_paletteBottom);

            }
        }
    }
}

void ControllerCore::setManualRange2(const QString& val)
{
    if (enviromentLowerThreshold().isEmpty() || enviromentUpperThreshold().isEmpty() || val == _manualRange2 || _cameraType == _vals.wirisSecurityType)
        return;

    if(val.toDouble() <= enviromentUpperThreshold().toDouble() && val.toDouble() > _manualRange1.toDouble())
    {
        std::istringstream iss;
        std::string cache;
        std::ostringstream oss;

        oss << "SRMM " << _manualRange1.toDouble() << " " << val.toDouble();

        if(send(QString::fromStdString(oss.str()), iss))
        {
            if (isOk(iss.str()))
            {
                _manualRange2 = val;
                emit manualRange2Changed(_manualRange2);
                _paletteTop = _manualRange2.toDouble();
                emit paletteTopChanged(_paletteTop);
            }
        }
    }
}

void ControllerCore::setSpanCenter(const QString& val)
{
    if (ControllerCore::isValidSpan(_spanWindow, val, _currentEnviroment) && _cameraType != _vals.wirisSecurityType)
    {
        std::ostringstream oss;
        std::istringstream iss;
        oss << "SRWC " << _spanWindow.toStdString() << " " << val.toStdString();

        if(send(QString::fromStdString(oss.str()), iss) && isPositive(iss.str()))
        {
            _spanCenter = val;
            emit spanCenterChanged(_spanCenter);
            _paletteTop = _spanCenter.toDouble() + _spanWindow.toDouble()/2;
            _paletteBottom = _spanCenter.toDouble() - _spanWindow.toDouble()/2;
            emit paletteTopChanged(_paletteTop);
            emit paletteBottomChanged(_paletteBottom);

        }
    }
}

void ControllerCore::setSpanWindow(const QString& val)
{
    if (ControllerCore::isValidSpan(val, _spanCenter, _currentEnviroment) && _cameraType != _vals.wirisSecurityType)
    {
        std::ostringstream oss;
        std::istringstream iss;
        oss << "SRWC "<< val.toStdString() << " " << _spanCenter.toStdString();

        if(send(QString::fromStdString(oss.str()), iss) && isPositive(iss.str()))
        {
            _spanWindow = val;
            emit spanWindowChanged(_spanWindow);
            _paletteTop = _spanCenter.toDouble() + _spanWindow.toDouble()/2;
            _paletteBottom = _spanCenter.toDouble() - _spanWindow.toDouble()/2;
            emit paletteTopChanged(_paletteTop);
            emit paletteBottomChanged(_paletteBottom);
        }
    }
}


void ControllerCore::setAlarmMode(int val)
{
    if (val < 0 || val > 4 || _cameraType == _vals.wirisSecurityType)
        return;
    QString req = "SALM ";
    switch (val) {
        case 0:
        req += _vals.alarmModeOFF;
        break;
        case 1:
        req += _vals.alarmModeABOVE;
        break;
        case 2:
        req += _vals.alarmModeBELOW;
        break;
        case 3:
        req += _vals.alarmModeBETWEEN;
        break;
        case 4:
        req += _vals.alarmModeOUTSIDE;
        break;
        default:
        return;
    }
    std::istringstream iss;

    if (send(req, iss) && isPositive(iss.str()))
        _alarmMode = val;
    emit alarmModeChanged(val);

}

void ControllerCore::setAlarmFrom(double val)
{
    if (val <= _alarmTo && val >= enviromentLowerThreshold().toDouble() && _cameraType != _vals.wirisSecurityType)
    {
        std::istringstream iss;

        if(send("SALV " + QString::number(val) + " " + QString::number(_alarmTo), iss) && isPositive(iss.str()))
         {
            _alarmFrom = val;
            emit alarmFromChanged(_alarmFrom);
         }
    }
}


void ControllerCore::setAlarmTo(double val)
{
    if (val >= _alarmFrom && val <= enviromentUpperThreshold().toDouble() && _cameraType != _vals.wirisSecurityType)
    {
        std::istringstream iss;
        if(send("SALV " + QString::number(_alarmFrom) + " " + QString::number(val), iss) && isPositive(iss.str()))
        {
           _alarmTo = val;
           emit alarmToChanged(_alarmTo);
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
