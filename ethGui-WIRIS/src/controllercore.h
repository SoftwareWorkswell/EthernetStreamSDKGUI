
/* ----------------------------------------------------------
 * Please, refer to the WIRIS Ethernet Stream SDK manual for
 * full understanding
 *
** ControllerCore class provides WIRIS communication interface
** Before accessing any other command methods, it is required to call
** following initialization methods in this order:
**
** 1) connect, with IP address of WIRIS as passed argument
**
** 2) activate - if isActivated method returns false(SDK was never
** activated previously), it is required to activate it by calling
** activate method, with your activation code as passed argument
**
** 3) setup - at last, setup function needs to be called to enable
** ethernet mode, which causes WIRIS to start RTSP server
** and to stop transmitting to standard HDMI output
** (note that this is purely application constraint, commands
**  would work properly even with ethernet mode off, but it
** is not recommended)
**
** All these three init functions are detaily described step by step
** in their bodies
**
** Other command methods are mostly just validating the
** input and sending concrete commands - which can be found
** in manual
** ---------------------------------------------------------*/

#ifndef CONTROLLERCORE_H
#define CONTROLLERCORE_H

#include <qobject.h>
#include <QString>
#include <QFile>
#include <qtextstream.h>
#include <QDebug>
#include <qimage.h>
#include <QMutex>
#include <QMutexLocker>
#include <QList>
#include <QQmlListProperty>
#include <qarraydata.h>
#include <qcoreapplication.h>
#include <QProcess>

#include <regex>
#include <string>
#include <memory>
#include <iostream>
#include <istream>
#include <math.h>
#include <functional>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include "networkclient.h"
#include "customtools.h"
#include "visiblethread.h"
#include "thermalthread.h"
#include "helperthread.h"
#include "palettewrapper.h"
#include "palettevaluewrapper.h"

extern QImage *streamFrame;
extern QImage *streamFrameVisible;
extern QMutex *thermalMutex;
extern QMutex *visibleMutex;
extern QMutex *connectionMutex;


class ControllerCore : public QObject
{
    Q_OBJECT

    //this(Q_PROPERTY) is purely QT feture, every wiris state(each obtained and set by commands) has its property,
    //which consits of getter, setter and signal that indicates that state has been changed

    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(bool isSetup READ isSetup)
    Q_PROPERTY(QString ipAddress READ ipAddress NOTIFY ipAdressChanged)
    Q_PROPERTY(QString type READ type NOTIFY typeChanged)
    Q_PROPERTY(QString firmwareVersion READ firmwareVersion NOTIFY firmwareVersionChanged)
    Q_PROPERTY(QString serialNumber READ serialNumber NOTIFY serialNumberChanged)
    Q_PROPERTY(QString articleNumber READ articleNumber NOTIFY articleNumberChanged)

    Q_PROPERTY(double cpuTemp READ cpuTemp NOTIFY cpuTempChanged)
    Q_PROPERTY(double camTemp READ camTemp NOTIFY camTempChanged)

    Q_PROPERTY(QString systemTime MEMBER _systemTime NOTIFY dateTimeChanged )
    Q_PROPERTY(QString systemDate MEMBER _systemDate NOTIFY dateTimeChanged )

    Q_PROPERTY(QString currentZoomVisible READ currentZoomVisible NOTIFY currentZoomVisibleChanged)
    Q_PROPERTY(QString currentZoomThermal READ currentZoomThermal NOTIFY currentZoomThermalChanged)

    Q_PROPERTY(double paletteBottom READ paletteBottom NOTIFY paletteBottomChanged)
    Q_PROPERTY(double paletteTop READ paletteTop NOTIFY paletteTopChanged)
    Q_PROPERTY(QQmlListProperty<PaletteWrapper> palettes READ palettes NOTIFY palettesChanged)
    Q_PROPERTY(QQmlListProperty<PaletteValueWrapper> paletteValues READ paletteValues NOTIFY paletteValuesChanged)
    Q_PROPERTY(QString currentPalette READ currentPalette NOTIFY currentPaletteChanged)

    Q_PROPERTY(double emissivity READ emissivity WRITE setEmissivity NOTIFY emissivityChanged)
    Q_PROPERTY(bool syncShutter READ syncShutter WRITE setSyncShutter NOTIFY syncShutterChanged)
    Q_PROPERTY(int shutterPeriod READ shutterPeriod WRITE setShutterPeriod NOTIFY shutterPeriodChanged)

    Q_PROPERTY(int rangeMode READ rangeMode WRITE setRangeMode NOTIFY rangeModeChanged)
    Q_PROPERTY(QString manualRange1 READ manualRange1 WRITE setManualRange1 NOTIFY manualRange1Changed)
    Q_PROPERTY(QString manualRange2 READ manualRange2 WRITE setManualRange2 NOTIFY manualRange2Changed)

    Q_PROPERTY(int alarmMode READ alarmMode WRITE setAlarmMode NOTIFY alarmModeChanged)
    Q_PROPERTY(double alarmFrom READ alarmFrom WRITE setAlarmFrom NOTIFY alarmFromChanged)
    Q_PROPERTY(double alarmTo READ alarmTo WRITE setAlarmTo NOTIFY alarmToChanged)

    Q_PROPERTY(QString spanCenter READ spanCenter WRITE setSpanCenter NOTIFY spanCenterChanged)
    Q_PROPERTY(QString spanWindow READ spanWindow WRITE setSpanWindow NOTIFY spanWindowChanged)

    Q_PROPERTY(QString enviroment1 READ enviroment1 WRITE setEnviroment1 NOTIFY enviroment1Changed)
    Q_PROPERTY(QString enviroment2 READ enviroment2 WRITE setEnviroment2 NOTIFY enviroment2Changed)

signals:
    //signals indicating which WIRIS states has just changed
    void connectedChanged(bool connected, double ping);
    void streamFrameChanged();
    void streamFrameVisibleChanged();
    void enviroment1Changed();
    void enviroment2Changed();
    void alarmModeChanged(int val);
    void alarmFromChanged(double val);
    void alarmToChanged(double val);
    void gpsChanged(const QString& val);
    void typeChanged(const QString& type);
    void firmwareVersionChanged(const QString& val);
    void serialNumberChanged(const QString& val);
    void articleNumberChanged(const QString& val);
    void cpuTempChanged(double val);
    void camTempChanged(double val);
    void emissivityChanged(double val);
    void dateTimeChanged();
    void capRecSettingChanged(bool radJpg, bool radTiff, bool srImg, bool visJpg, bool radVid, bool visVid);
    void captureRecordingChanged(bool capturing, bool recording);
    void driveStateChanged(const QString& ssd, const QString& ssdCap, const QString& ssdFree,
                           const QString& sd, const QString& sdCap, const QString& sdFree,
                           const QString& flash, const QString& flashCap, const QString& flashFree);
    void alarmColorsChanged(int above, int between, int below);
    void palettesChanged();
    void paletteValuesChanged();
    void currentPaletteChanged(const QString& val);
    void paletteBottomChanged(double val);
    void paletteTopChanged(double val);
    void extremesChanged();
    void syncShutterChanged(bool val);
    void shutterPeriodChanged(int val);
    void manualRange1Changed(const QString& val);
    void manualRange2Changed(const QString& val);
    void spanCenterChanged(const QString& val);
    void spanWindowChanged(const QString& val);
    void rangeModeChanged(int mode); //0 = automatic, 1 = manual, 2 = span
    void currentEnviromentChanged();
    void currentZoomThermalChanged(const QString& val);
    void currentZoomVisibleChanged(const QString& val);
    void ipAdressChanged(const QString& ipAddress);
    void shutterDataChanged(const QString& next, const QString& last);

public slots:
    //slots, methods which are possible to be invoked from frontend
    QString getLastIp() const;
    bool connect(const QString& ip);
    bool isActivated();
    bool activate(const QString & actStr);
    bool setup();
    void close();
    void setDefaultSettings();
    void reboot();

    void triggerShutter(){ send("STCP " + QString::number(_shutterPeriodMin*60)); }
    void triggerCapture();
    void toggleRecording();

    void toggleStreamThermal();
    void toggleStreamVisible();
    void stopStream(){
        this->_streamFlagThermal = false;
        this->_streamFlagVisible = false;
    }
    bool streamingThermal(){return _streamFlagThermal;}
    bool streamingVisible(){return _streamFlagVisible;}

    bool changeStorage(const QString & storage);

    bool compareFw(const QString& older, const QString& newer) const{
        return compareVersion(older, newer);
    }

    QQmlListProperty<PaletteWrapper> palettes()  {
        return QQmlListProperty<PaletteWrapper>(this, _palettes);
    }

    QQmlListProperty<PaletteValueWrapper> paletteValues() {
        return QQmlListProperty<PaletteValueWrapper>(this, _paletteValues);
    }

    QString type() const{ return this->_cameraType; }
    QString firmwareVersion() const{ return this->_firmwareVersion; }
    QString serialNumber() const{ return this->_serialNumber; }
    QString articleNumber() const{ return this->_articleNumber; }

    void paintMax(bool paint);
    void paintMin(bool paint);

    void paintVals(bool paint){
        _paintVals = paint;
    }
    void paintCenter(bool paint){
        _paintCenter = paint;
    }

    void setAlarmColors(const QString& above, const QString& below, const QString& between);

    void setCaptureVideoSettings(bool radJpg, bool radTiff, bool srImg, bool visJpg, bool radVid, bool visVid);

    void setCurrentPaletteByIndex(int i);

    void changeZoomThermal(bool direction);
    void changeZoomVisible(bool direction);


    int rangeMode() const{ return this->_rangeMode; }

    void setRangeMode(int value);

    QString enviroment1() const{
        return this->_enviroment1;
    }
    void setEnviroment1(const QString & env);

    QString enviroment2() const  {
        return this->_enviroment2;
    }
    void setEnviroment2(const QString & env);

    bool isCurrentEnviroment(const QString & env){
        return env == this->_currentEnviroment;
    }

    QString enviromentLowerThreshold() const;

    QString enviromentUpperThreshold() const;

public:
    ControllerCore(QObject * parent = nullptr) : QObject (parent){}

    bool sendCommand(const QString & mes, std::istringstream & iss){
        return send(mes,iss, false);
    }

    bool isSetup(){
        return this->_isSetup;
    }


private:
    const Values _vals;
    const QString _endpointPort = "2240";
    QString _endpointIp;

    //application states that are used to cache values from WIRIS responses, some of them can be get/set from runtime/thermal thread
    QString _cameraType;
    QString _firmwareVersion, _serialNumber, _articleNumber;
    bool _paintVals = false, _paintCenter = false;
    bool _isSetup = false;
    bool _streamFlagVisible = false, _streamFlagThermal = false, _extremesFlag = false, _helperTreadFlag = false;
    bool _connected = false;
    QString _systemDate, _systemTime;

    QList<PaletteWrapper *> _palettes;
    QList<PaletteValueWrapper *> _paletteValues;
    QVector<QString> _zoomsThermal;
    QVector<QString> _zoomsVisible;
    QTypedArrayData<QString>::const_iterator _currentZoomThermal;
    QTypedArrayData<QString>::const_iterator _currentZoomVisible;
    double _cpuTemp = 0, _camTemp = 0;
    double _emissivity = 0.95;
    bool _syncShutter;
    int _shutterPeriodMin;
    bool _radJpg, _radTiff, _srImg, _visJpg, _radVid, _visVid;
    bool _recording, _capturing;
    QString _ssdDrive, _sdDrive, _flashDrive, _currentStorage = "SSD";
    QString _currentPalette;
    double _paletteBottom, _paletteTop;
    QString _spanCenter, _spanWindow;
    int _rangeMode;
    QString _manualRange1, _manualRange2;
    QString _enviroment1, _enviroment2, _currentEnviroment;
    int _alarmMode;
    double _alarmFrom, _alarmTo;

    //threads for recieving streams and runtime information, they are started after successful connection and setup
    HelperThread* _helperThread = nullptr;
    VisibleThread* _visibleThread = nullptr;
    ThermalThread* _thermalThread = nullptr;

    Extreme* _maximum = new Extreme();
    Extreme* _minimum = new Extreme();

    std::shared_ptr<NetworkClient> _networkClient = nullptr;

    bool connected() const {return _connected;}
    bool isValidIpv4Addr(const std::string& addr); //ipv6 ?
    bool send(const QString &message);
    bool send(const QString &message, std::istringstream & answ, bool debugPrint = true);

    bool changeEnviroment(const QString & env);

    void toggleHelperThread();
    void onHelperExtremesChanged();
    void onHelperTempsChanged(double cpu, double cam);

    void onHelpercaptureRecordingChanged(bool capture, bool record);
    void onHelperdriveStateChanged(const QString& ssd, const QString& ssdCap, const QString& ssdFree,
                                   const QString& sd, const QString& sdCap, const QString& sdFree,
                                   const QString& flash, const QString& flashCap, const QString& flashFree);

    void onHelperDateTImeChanged(const QString& date, const QString& time);
    void onHelperConnectionStateChanged(bool state, double ping);
    void onHelperGpsChanged(const QString& gps);

    void fetchEnviroments();
    void fetchRangeModes();
    void fetchManualRange();
    void fetchSpanRange();
    void fetchZooms();
    void fetchCurrentZooms();
    void fetchPalettes();
    void fetchBasicInfo();
    void fetchPaletteValues();
    void fetchEmissivity();
    void fetchShutter();
    void fetchCaptureRecSettings();
    void fetchAlarms();

    bool syncShutter()const{
        return _syncShutter;
    }
    void setSyncShutter(bool val);

    int shutterPeriod(){
        return  _shutterPeriodMin;
    }
    void setShutterPeriod(int val);

    double cpuTemp() const{
        return _cpuTemp;
    }
    double camTemp() const{
        return _camTemp;
    }

    double paletteBottom() const{
        return this->_paletteBottom;
    }

    double paletteTop() const{
        return this->_paletteTop;
    }

    QString currentPalette() const{
        return this->_currentPalette;
    }

    QString currentZoomThermal() const{
        if (_currentZoomThermal >= _zoomsThermal.begin() && _currentZoomThermal < _zoomsThermal.end())
            return *_currentZoomThermal;
        return "";
    }
    QString currentZoomVisible() const{
        if (_currentZoomVisible >= _zoomsVisible.begin() && _currentZoomVisible < _zoomsVisible.end())
            return *_currentZoomVisible;
        return "";
    }

    QString ipAddress() const{
        return  this->_endpointIp;
    }


    double emissivity() const{
        return _emissivity;
    }

    void setEmissivity(double val);


    QString manualRange1() const{
        return this->_manualRange1;
    }
    void setManualRange1(const QString& val);

    QString manualRange2() const{
        return this->_manualRange2;
    }

    void setManualRange2(const QString& val);

    QString spanCenter() const{
        return this->_spanCenter;
    }

    void setSpanCenter(const QString& val);

    QString spanWindow() const{
        return this->_spanWindow;
    }

    void setSpanWindow(const QString& val);

    int alarmMode() const {return this->_alarmMode; }
    void setAlarmMode(int val);

    double alarmTo() const {return this->_alarmTo; }
    void setAlarmTo(double val);

    double alarmFrom() const {return this->_alarmFrom; }
    void setAlarmFrom(double val);

    void onThermalFrameChanged(){
        emit streamFrameChanged();
    }

    void onVisibleFrameChanged(){
        emit streamFrameVisibleChanged();
    }

    void setLastIp(const QString& ip);

    static bool isValidSpan(const QString& window, const QString & center, const QString& env);

    static bool isOk(const QString & response){
        return ControllerCore::isOk(response.toStdString());
    }
    static bool isTrue(const QString & response){
        return ControllerCore::isTrue(response.toStdString());
    }
    static bool isPositive(const QString & response){
        return ControllerCore::isOk(response) || ControllerCore::isTrue(response);
    }

    static bool isOk(const std::string & response);
    static bool isTrue(const std::string & response);
    static bool isPositive(const std::string & response){
        return ControllerCore::isOk(response) || ControllerCore::isTrue(response);
    }

    static QString boolToString(bool val){
        return val ? "TRUE" : "FALSE";
    }

};

#endif // CONTROLLERCORE_H
