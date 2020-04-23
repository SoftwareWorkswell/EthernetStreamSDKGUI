
/* ----------------------------------------------------------
 * Please, refer to the MEDICAS Ethernet Stream SDK manual for
 * full understanding
 *
** ControllerCore class provides MEDICAS communication interface
** Before accessing any other command methods, it is required to call
** following initialization methods in this order:
**
** 1) connect, with IP address of MEDICAS as passed argument
**
** 2) activate - if isActivated method returns false(SDK was never
** activated previously), it is required to activate it by calling
** activate method, with your activation code as passed argument
**
** 3) setup - at last, setup function needs to be called to enable
** ethernet mode, which causes MEDICAS to start RTSP server
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

// commands definitions
#include <src/protocol.h>
// camera params
#include <src/cameraparameters.h>
#include <src/captureparameters.h>

class ControllerCore : public QObject
{
    Q_OBJECT
    //this(Q_PROPERTY) is purely QT feture, every MEDICAS state(each obtained and set by commands) has its property,
    //which consits of getter, setter and signal that indicates that state has been changed
    // basic
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(bool isSetup READ isSetup)
    Q_PROPERTY(QString ipAddress READ ipAddress NOTIFY ipAdressChanged)
    Q_PROPERTY(QString type READ type NOTIFY typeChanged)
    Q_PROPERTY(QString firmwareVersion READ firmwareVersion NOTIFY firmwareVersionChanged)
    Q_PROPERTY(QString serialNumber READ serialNumber NOTIFY serialNumberChanged)
    Q_PROPERTY(QString articleNumber READ articleNumber NOTIFY articleNumberChanged)
    Q_PROPERTY(double cpuTemp READ cpuTemp NOTIFY cpuTempChanged)
    Q_PROPERTY(double camTemp READ camTemp NOTIFY camTempChanged)
    Q_PROPERTY(double deviceTemp READ deviceTemp NOTIFY deviceTempChanged)
    // range
    Q_PROPERTY(QString manualRange1 READ manualRange1 WRITE setManualRange1 NOTIFY manualRange1Changed)
    Q_PROPERTY(QString manualRange2 READ manualRange2 WRITE setManualRange2 NOTIFY manualRange2Changed)
    // functions
    Q_PROPERTY(QString mainCamera READ mainCamera WRITE setMainCamera NOTIFY mainCameraChanged)
    Q_PROPERTY(double levelOfAcception READ levelOfAcception WRITE setLevelOfAcception NOTIFY levelOfAcceptionChanged)
    Q_PROPERTY(int blackbodyRoiX READ blackbodyRoiX WRITE setBlackbodyRoiX NOTIFY blackbodyRoiXChanged)
    // blackbody coordinates in an app, use the confirm button to set coordinates to camera and refresh calibration
    Q_PROPERTY(int blackbodyRoiY READ blackbodyRoiY WRITE setBlackbodyRoiY NOTIFY blackbodyRoiYChanged)
    // palettes
    Q_PROPERTY(double paletteBottom READ paletteBottom NOTIFY paletteBottomChanged)
    Q_PROPERTY(double paletteTop READ paletteTop NOTIFY paletteTopChanged)
    Q_PROPERTY(QQmlListProperty<PaletteWrapper> palettes READ palettes NOTIFY palettesChanged)
    Q_PROPERTY(QQmlListProperty<PaletteValueWrapper> paletteValues READ paletteValues NOTIFY paletteValuesChanged)
    Q_PROPERTY(QString currentPalette READ currentPalette NOTIFY currentPaletteChanged)
    // measure
    Q_PROPERTY(int alarmMode READ alarmMode WRITE setAlarmMode NOTIFY alarmModeChanged)
    Q_PROPERTY(double alarmPreventive READ alarmPreventive WRITE setAlarmPreventive NOTIFY alarmPreventiveChanged)
    Q_PROPERTY(double alarmCritical READ alarmCritical WRITE setAlarmCritical NOTIFY alarmCriticalChanged)
    Q_PROPERTY(bool showCrossMax READ showCrossMax WRITE setShowCrossMax NOTIFY showCrossMaxChanged)
    Q_PROPERTY(bool showCrossCenter READ showCrossCenter WRITE setShowCrossCenter NOTIFY showCrossCenterChanged)
    Q_PROPERTY(bool showBlackbodyRoi READ showBlackbodyRoi WRITE setShowBlackbodyRoi NOTIFY showBlackbodyRoiChanged)
    Q_PROPERTY(QString blackbodyStatus READ blackbodyStatus NOTIFY blackbodyStatusChanged)
    // visible camera params
    Q_PROPERTY(double gamma READ gamma WRITE setGamma NOTIFY gammaChanged)
    Q_PROPERTY(double whiteBalance READ whiteBalance WRITE setWhiteBalance NOTIFY whiteBalanceChanged)
    Q_PROPERTY(double visShutterPeriod READ visShutterPeriod WRITE setVisShutterPeriod NOTIFY visShutterPeriodChanged)
    Q_PROPERTY(bool backlightCompensation READ backlightCompensation WRITE setBacklightCompensation NOTIFY backlightCompensationChanged)
    Q_PROPERTY(bool wideDynamicRangeMode READ wideDynamicRangeMode WRITE setWideDynamicRangeMode NOTIFY wideDynamicRangeModeChanged)
    Q_PROPERTY(int noiseReduction2D READ noiseReduction2D WRITE setNoiseReduction2D NOTIFY noiseReduction2DChanged)
    Q_PROPERTY(int noiseReduction3D READ noiseReduction3D WRITE setNoiseReduction3D NOTIFY noiseReduction3DChanged)
    Q_PROPERTY(int colorGain READ colorGain WRITE setColorGain NOTIFY colorGainChanged)
    Q_PROPERTY(int gain READ gain WRITE setGain NOTIFY gainChanged)
    Q_PROPERTY(int exposureMode READ exposureMode WRITE setExposureMode NOTIFY exposureModeChanged)
    Q_PROPERTY(int exposureCompensation READ exposureCompensation WRITE setExposureCompensation NOTIFY exposureCompensationChanged)
    Q_PROPERTY(int iris READ iris WRITE setIris NOTIFY irisChanged)
    Q_PROPERTY(QString preventiveColor READ preventiveColor WRITE setPreventiveColor NOTIFY preventiveColorChanged)
    Q_PROPERTY(QString criticalColor READ criticalColor WRITE setCriticalColor NOTIFY criticalColorChanged)
    Q_PROPERTY(int alarmOpacity READ alarmOpacity WRITE setAlarmOpacity NOTIFY alarmOpacityChanged)
    // system
    Q_PROPERTY(QString systemTime MEMBER _systemTime NOTIFY dateTimeChanged)
    Q_PROPERTY(QString systemDate MEMBER _systemDate NOTIFY dateTimeChanged)
    Q_PROPERTY(QString units READ units WRITE setUnits NOTIFY unitsChanged)
    Q_PROPERTY(QString language READ language NOTIFY languageChanged)
public:
    ControllerCore(QObject * parent = nullptr);
    virtual ~ControllerCore();
    bool sendCommand(const QString & mes, std::istringstream & iss);
public slots:
    void setEthModeOff();
    //slots, methods which are possible to be invoked from frontend
    QString getLastIp() const;
    bool connect(const QString& ip);
    bool isActivated();
    bool activate(const QString & actStr);
    bool setup();
    void close();
    void forceCalibration();
    void setDefaultSettings();
    void reboot();
    void triggerCapture();
    void toggleRecording();
    void toggleStreamThermal();
    void toggleStreamVisible();
    void stopStream();
    bool streamingThermal(){return _streamFlagThermal;}
    bool streamingVisible(){return _streamFlagVisible;}
    bool changeStorage(const QString & storage);
    bool compareFw(const QString& older, const QString& newer) const;
    void setAlarmColors(const QString& preventive, const QString& critical);
    void setCaptureVideoSettings(bool radJpg, bool visJpg, bool encIRVid, bool visVid);
    void setCurrentPaletteByIndex(int i);
    void setCurrentPaletteByName(int i);
    void setVisibleCameraDefaults();
    // sync blackbody position to camera
    void sendBlackbodyCoordinates();
    void fetchBlackbodyCoordinates();

signals:
    //signals indicating which MEDICAS states has just changed
    void connectedChanged(bool connected, double ping);
    void streamFrameChanged();
    void streamFrameVisibleChanged();
    void alarmModeChanged(int val);
    void alarmPreventiveChanged(double val);
    void alarmCriticalChanged(double val);
    void typeChanged(const QString& type);
    void ipAdressChanged(const QString& ipAddress);
    void firmwareVersionChanged(const QString& val);
    void serialNumberChanged(const QString& val);
    void articleNumberChanged(const QString& val);
    void cpuTempChanged(double val);
    void camTempChanged(double val);
    void deviceTempChanged(double val);
    void dateTimeChanged();
    void manualRange1Changed(const QString& val);
    void manualRange2Changed(const QString& val);
    void mainCameraChanged(const QString & val);
    void levelOfAcceptionChanged(double val);
    void blackbodyRoiXChanged(double val);
    void blackbodyRoiYChanged(double val);
    void showCrossMaxChanged(bool val);
    void showCrossCenterChanged(bool val);
    void showBlackbodyRoiChanged(bool val);
    void blackbodyStatusChanged(const QString & val);
    void capRecSettingChanged(bool radJpg, bool visJpg, bool encIRVid, bool visVid);
    void captureRecordingChanged(bool capturing, bool recording);
    void driveStateChanged(const QString& ssd, const QString& ssdCap, const QString& ssdFree,
                           const QString& sd, const QString& sdCap, const QString& sdFree,
                           const QString& flash, const QString& flashCap, const QString& flashFree);
    void palettesChanged();
    void paletteValuesChanged();
    void currentPaletteChanged(const QString& val);
    void paletteBottomChanged(double val);
    void paletteTopChanged(double val);
    void extremesChanged();
    void gammaChanged(int val);
    void whiteBalanceChanged(int val);
    void visShutterPeriodChanged(double val);
    void backlightCompensationChanged(bool val);
    void wideDynamicRangeModeChanged(bool val);
    void noiseReduction2DChanged(int val);
    void noiseReduction3DChanged(int val);
    void colorGainChanged(int val);
    void gainChanged(int val);
    void exposureModeChanged(int val);
    void exposureCompensationChanged(int val);
    void irisChanged(int val);
    void languageChanged(const QString & val);
    void unitsChanged(const QString & val);
    void preventiveColorChanged(const QString & val);
    void criticalColorChanged(const QString & val);
    void alarmOpacityChanged(int val);

private slots:
    // getters
    bool connected() const;
    bool isSetup();
    QString ipAddress() const;
    QString firmwareVersion() const;
    QString serialNumber() const;
    QString articleNumber() const;
    double cpuTemp() const;
    double camTemp() const;
    double deviceTemp() const;
    QString type() const;
    QString manualRange1() const;
    QString manualRange2() const;
    QString mainCamera() const;
    double levelOfAcception() const;
    int blackbodyRoiX() const;
    int blackbodyRoiY() const;
    double paletteBottom() const;
    double paletteTop() const;
    QString currentPalette() const;
    QQmlListProperty<PaletteWrapper> palettes();
    QQmlListProperty<PaletteValueWrapper> paletteValues();
    int alarmMode() const;
    double alarmCritical() const;
    double alarmPreventive() const;
    bool showCrossMax() const;
    bool showCrossCenter() const;
    bool showBlackbodyRoi() const;
    QString blackbodyStatus() const;
    int gamma() const;
    int whiteBalance() const;
    double visShutterPeriod();
    bool backlightCompensation();
    bool wideDynamicRangeMode();
    int noiseReduction2D();
    int noiseReduction3D();
    int colorGain();
    int gain();
    int exposureMode();
    int exposureCompensation();
    QString preventiveColor();
    QString criticalColor();
    int alarmOpacity();
    int iris();
    QString units();
    QString language();
    // setters
    void setManualRange1(const QString& val);
    void setManualRange2(const QString& val);
    void setMainCamera(const QString& val);
    void setLevelOfAcception(double val);
    void setBlackbodyRoiX(int val);
    void setBlackbodyRoiY(int val);
    void setAlarmMode(int val);
    void setAlarmCritical(double val);
    void setAlarmPreventive(double val);
    void setShowCrossMax(bool val);
    void setShowCrossCenter(bool val);
    void setShowBlackbodyRoi(bool val);
    void setGamma(int val);
    void setWhiteBalance(int val);
    void setVisShutterPeriod(double val);
    void setBacklightCompensation(bool value);
    void setWideDynamicRangeMode(bool value);
    void setNoiseReduction2D(int val);
    void setNoiseReduction3D(int val);
    void setColorGain(int val);
    void setGain(int val);
    void setExposureMode(int val);
    void setExposureCompensation(int val);
    void setPreventiveColor(const QString & val);
    void setCriticalColor(const QString & val);
    void setAlarmOpacity(int val);
    void setIris(int val);
    void setUnits(const QString & val);
    // fetch params from camera
    void fetchAll();
    void fetchBasicInfo();
    void fetchManualRange();
    void fetchMainCamera();
    void fetchLevelofAcceptance();
    void fetchBlacbodyRoi();
    void fetchShowCrosses();
    void fetchPalettes();
    void fetchPaletteValues();
    void fetchCaptureRecSettings();
    void fetchAlarms();
    void fetchExposure();
    void fetchNoiseReduction();
    void fetchGains();
    void fetchIris();
    void fetchGamma();
    void fetchWhiteBalance();
    void fetchBacklightCompensation();
    void fetchDynamicRangeMode();
    void fetchSystemSettings();
private:
    const Values _vals;
    CameraParameters _params;
    CaptureParameters _captureParams;
    std::shared_ptr<NetworkClient> _networkClient = nullptr;
    Protocol _protocol;
    QList<PaletteWrapper *> _palettes;
    QList<PaletteValueWrapper *> _paletteValues;
    // control flags
    bool _streamFlagVisible = false;
    bool _streamFlagThermal = false;
    bool _extremesFlag = false;
    bool _helperTreadFlag = false;
    bool _connected = false;
    QString _systemDate{};
    QString _systemTime{};
    //threads for recieving streams and runtime information, they are started after successful connection and setup
    HelperThread* _helperThread = nullptr;
    VisibleThread* _visibleThread = nullptr;
    ThermalThread* _thermalThread = nullptr;
    // control helper functions
    bool send(const QString &message);
    bool send(const QString &message, std::istringstream & answ, bool debugPrint = true);
    void toggleHelperThread();
    void onHelperExtremesChanged();
    void onHelperTempsChanged(double cpu, double cam);
    void onHelpercaptureRecordingChanged(bool capture, bool record);
    void onHelperdriveStateChanged(const QString& ssd, const QString& ssdCap, const QString& ssdFree,
                                   const QString& sd, const QString& sdCap, const QString& sdFree,
                                   const QString& flash, const QString& flashCap, const QString& flashFree);

    void onHelperDateTImeChanged(const QString& date, const QString& time);
    void onHelperConnectionStateChanged(bool state, double ping);
    void onThermalFrameChanged();
    void onVisibleFrameChanged();
    // helper functions
    bool isValidIpv4Addr(const std::string& addr); //ipv6 ?
    void setLastIp(const QString& ip);
    static bool isValidSpan(const QString& window, const QString & center, const QString& env);
    static bool isOk(const QString & response);
    static bool isTrue(const QString & response);
    static bool isPositive(const QString & response);
    static bool isOk(const std::string & response);
    static bool isTrue(const std::string & response);
    static bool isPositive(const std::string & response);
    static QString boolToString(bool val);
    static bool stringToBool(const QString & val);
};

#endif // CONTROLLERCORE_H
