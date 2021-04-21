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

#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QImage>
#include <QMutex>
#include <QMutexLocker>
#include <QList>
#include <QUrl>
#include <QQmlListProperty>
#include <QArrayData>
#include <QCoreApplication>
#include <QProcess>
#include <QDesktopServices>
#include <QRegularExpression>
#include <QDateTime>

#include <regex>
#include <string>
#include <memory>
#include <iostream>
#include <istream>
#include <math.h>
#include <functional>
#include <algorithm>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include "networkclient.h"
#include "customtools.h"
#include "visiblethread.h"
#include "thermalthread.h"
#include "connectionthread.h"
#include "helperthread.h"
#include "palettewrapper.h"
#include "palettevaluewrapper.h"


extern QImage streamFrame;
extern QImage streamFrameVisible;
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
    //this(Q_PROPERTY) is purely QT feture, most of MEDICAS states (each obtained and set by commands) has its property,
    //which consits of getter, setter and signal that indicates that state has been changed
    // basic
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(bool isSetup READ isSetup CONSTANT)
    Q_PROPERTY(QString ipAddress READ ipAddress NOTIFY ipAdressChanged)
    Q_PROPERTY(QString type READ type NOTIFY typeChanged)
    Q_PROPERTY(QString firmwareVersion READ firmwareVersion NOTIFY firmwareVersionChanged)
    Q_PROPERTY(QString serialNumber READ serialNumber NOTIFY serialNumberChanged)
    Q_PROPERTY(QString articleNumber READ articleNumber NOTIFY articleNumberChanged)
    Q_PROPERTY(double cpuTemp READ cpuTemp NOTIFY cpuTempChanged)
    Q_PROPERTY(double camTemp READ camTemp NOTIFY camTempChanged)
    Q_PROPERTY(double deviceTemp READ deviceTemp NOTIFY deviceTempChanged)
    // temp mode
    Q_PROPERTY(int temperatureMode READ temperatureMode WRITE setTemperatureMode NOTIFY temperatureModeChanged)

    // range
    Q_PROPERTY(QString manualRange1 READ manualRange1 WRITE setManualRange1 NOTIFY manualRange1Changed)
    Q_PROPERTY(QString manualRange2 READ manualRange2 WRITE setManualRange2 NOTIFY manualRange2Changed)
    // functions
    Q_PROPERTY(QString mainCamera READ mainCamera WRITE setMainCamera NOTIFY mainCameraChanged)
    // blackbody coordinates in an app, use the confirm button to set coordinates to camera and refresh calibration
    // after position is synced with camera by sendCoordinates method new coordinates are saved to camera params struct
    Q_PROPERTY(int blackbodyRoiX READ blackbodyRoiX WRITE setBlackbodyRoiX NOTIFY blackbodyRoiXChanged)
    Q_PROPERTY(int blackbodyRoiY READ blackbodyRoiY WRITE setBlackbodyRoiY NOTIFY blackbodyRoiYChanged)
    Q_PROPERTY(int blackbodySize READ blackbodySize NOTIFY blackbodySizeChanged)
    Q_PROPERTY(int blackbodyMaskSize READ blackbodyMaskSize  NOTIFY blackbodyMaskSizeChanged)
    Q_PROPERTY(int blackbodyDetection READ blackbodyDetection WRITE setBlackbodyDetection NOTIFY blackbodyDetectionChanged)
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
    Q_PROPERTY(bool showBlackbodyRoi READ showBlackbodyRoi WRITE setShowBlackbodyRoi NOTIFY showBlackbodyRoiChanged)
    // cross user coordinates in an app, use the confirm button to set coordinates to camera
    // after position is synced with camera by sendCoordinates method new coordinates are saved to camera params struct
    Q_PROPERTY(int crossUserX READ crossUserX WRITE setCrossUserX NOTIFY crossUserXChanged)
    Q_PROPERTY(int crossUserY READ crossUserY WRITE setCrossUserY NOTIFY crossUserYChanged)
    Q_PROPERTY(bool showCrossUser READ showCrossUser WRITE setShowCrossUser NOTIFY showCrossUserChanged)
    Q_PROPERTY(bool showRoiUserNames READ showRoiUserNames WRITE setShowRoiUserNames NOTIFY showRoiUserNamesChanged)
    Q_PROPERTY(int showUserRoi1 READ showUserRoi1 WRITE setShowUserRoi1 NOTIFY showUserRoiChanged)
    Q_PROPERTY(int showUserRoi2 READ showUserRoi2 WRITE setShowUserRoi2 NOTIFY showUserRoiChanged)
    Q_PROPERTY(int showUserRoi3 READ showUserRoi3 WRITE setShowUserRoi3 NOTIFY showUserRoiChanged)
    Q_PROPERTY(int showUserRoi4 READ showUserRoi4 WRITE setShowUserRoi4 NOTIFY showUserRoiChanged)
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
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(QString lastAns READ getLastAns CONSTANT)

    // DIO
    Q_PROPERTY(bool dioConnected READ dioConnected NOTIFY dioConnectedChanged)
    Q_PROPERTY(QString dioTriggerMode READ dioTriggerMode WRITE setDioTriggerMode NOTIFY dioTriggerModeChanged)
    Q_PROPERTY(bool dioPictureOnTrigger READ dioPictureOnTrigger WRITE setdioPictureOnTrigger NOTIFY dioPictureOnTriggerChanged)
    Q_PROPERTY(QString dioMinimumPulseWidth READ dioMinimumPulseWidth WRITE setDioMinimumPulseWidth NOTIFY dioMinimumPulseWidthChanged)
    Q_PROPERTY(QString dioSignal READ dioSignal WRITE setDioSignal NOTIFY dioSignalChanged)
    Q_PROPERTY(QString dioTriggerDelay READ dioTriggerDelay WRITE setDioTriggerDelay NOTIFY dioTriggerDelayChanged)
    Q_PROPERTY(QString dioMinDetectionTime READ dioMinDetectionTime WRITE setDioMinDetectionTime NOTIFY dioMinDetectionTimeChanged)
    Q_PROPERTY(QString dioTempHysteresis READ dioTempHysteresis WRITE setDioTempHysteresis NOTIFY dioTempHysteresisChanged)
    Q_PROPERTY(QString dioMinAlarmTime READ dioMinAlarmTime WRITE setDioMinAlarmTime NOTIFY dioMinAlarmTimeChanged)
    Q_PROPERTY(QString dioLogic READ dioLogic WRITE setDioLogic NOTIFY dioLogicChanged)
    Q_PROPERTY(QString dioAlarm READ dioAlarm WRITE setDioAlarm NOTIFY dioAlarmChanged)
    Q_PROPERTY(QString dioGateOpenTime READ dioGateOpenTime WRITE setDioGateOpenTime NOTIFY dioGateOpenTimeChanged)
    Q_PROPERTY(bool dioShowStatus READ dioShowStatus WRITE setDioShowStatus NOTIFY dioShowStatusChanged)

    // USER
    Q_PROPERTY(QString currentUser READ currentUser NOTIFY currentUserChanged)

    // ROI AREA
    Q_PROPERTY(int roiAreaMaxX READ roiAreaMaxX NOTIFY roiAreaMaxXChanged)
    Q_PROPERTY(int roiAreaMinX READ roiAreaMinX NOTIFY roiAreaMinXChanged)
    Q_PROPERTY(int roiAreaMaxY READ roiAreaMaxY NOTIFY roiAreaMaxYChanged)
    Q_PROPERTY(int roiAreaMinY READ roiAreaMinY NOTIFY roiAreaMinYChanged)
    Q_PROPERTY(int userCrossMaxX READ userCrossMaxX NOTIFY userCrossMaxXChanged)
    Q_PROPERTY(int userCrossMinX READ userCrossMinX NOTIFY userCrossMinXChanged)
    Q_PROPERTY(int userCrossMaxY READ userCrossMaxY NOTIFY userCrossMaxYChanged)
    Q_PROPERTY(int userCrossMinY READ userCrossMinY NOTIFY userCrossMinYChanged)
    Q_PROPERTY(int blackBodyMaxX READ blackBodyMaxX NOTIFY blackBodyMaxXChanged)
    Q_PROPERTY(int blackBodyMinX READ blackBodyMinX NOTIFY blackBodyMinXChanged)
    Q_PROPERTY(int blackBodyMaxY READ blackBodyMaxY NOTIFY blackBodyMaxYChanged)
    Q_PROPERTY(int blackBodyMinY READ blackBodyMinY NOTIFY blackBodyMinYChanged)
    Q_PROPERTY(int blackBodyRoiScale READ blackBodyRoiScale WRITE setBlackBodyRoiScale NOTIFY blackBodyRoiScaleChanged)

public:
    ControllerCore(QObject * parent = nullptr);
    virtual ~ControllerCore();
public slots:
    bool sendCommand(const QString &mes, std::istringstream &iss);
    // getters
    bool getCaptureIRVideoChecked() const;
    bool getCaptureVisVideoChecked() const;
    bool getRecordingToggled() const;
    int temperatureMode() const;
    bool connected() const;
    bool isSetup() const;
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
    int blackbodyRoiX() const;
    int blackbodyRoiY() const;
    int crossUserX() const;
    int crossUserY() const;
    int blackbodySize() const;
    int blackbodyMaskSize() const;
    int blackbodyDetection() const;
    double paletteBottom() const;
    double paletteTop() const;
    QString currentPalette() const;
    int alarmMode() const;
    double alarmCritical() const;
    double alarmPreventive() const;
    bool showCrossMax() const;
    bool showCrossUser() const;
    bool showRoiUserNames() const;
    bool showBlackbodyRoi() const;
    int showUserRoi1() const;
    int showUserRoi2() const;
    int showUserRoi3() const;
    int showUserRoi4() const;
    QString blackbodyStatus() const;
    int gamma() const;
    int whiteBalance() const;
    double visShutterPeriod() const;
    bool backlightCompensation() const;
    bool wideDynamicRangeMode() const;
    int noiseReduction2D() const;
    int noiseReduction3D() const;
    int colorGain() const;
    int gain() const;
    int exposureMode() const;
    int exposureCompensation() const;
    QString preventiveColor() const;
    QString criticalColor() const;
    int alarmOpacity() const;
    int iris() const;
    QString units() const;
    QString language() const;
    QString getLastAns() const;

    void setEthModeOff();
    //slots, methods which are possible to be invoked from frontend
    bool checkAddressFormat(const QString& ip);
    // connect toggles connect thread
    // after thread finished it triggers signal that is connected to afterConnectHandler
    // this way GUI can remain updated during connection and wont freeze
    bool connect(const QString& ip);
    void stopConnect();
    bool afterConnectHandler(const QString& ip);
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
    void stopThreads();
    bool streamingThermal(){return _streamFlagThermal;}
    bool streamingVisible(){return _streamFlagVisible;}
    bool changeStorage(const QString & storage);
    bool compareFw(const QString &older, const QString &newer);
    void setAlarmColors(const QString& preventive, const QString& critical);
    void setCaptureVideoSettings(bool radJpg, bool visJpg, bool encIRVid, bool visVid);
    void setCurrentPaletteByIndex(int i);
    void setCurrentPaletteByName(int i);
    void setVisibleCameraDefaults();
    void setBlackbodyArea();
    // sync blackbody position to camera
    void sendBlackbodyCoordinates();
    void sendOriginalBlackbodyCoordinates();

    bool sendCrossUserCoordinates();
    void sendOriginalUserRoiCoordinates(int idx);
    bool sendUserRoiCoordinates(int idx);
    void fetchBlackbodyCoordinates(); 
    void fetchBlackbodyStatus();
    // corner  ["TOP-LEFT", "TOP-RIGHT", "BOTTOM-LEFT", "BOTTOM-RIGHT"]
    int getUserRoiX(int idx, int corner);
    int getUserRoiY(int idx, int corner);
    // return value of range limits according to actual temperature units
    double getManualMinLimit();
    double getManualMaxLimit();
    double getManualDiffLimit();
    double getAlarmMinLimit();
    double getAlarmMaxLimit();
    double getCriticalUpperBoundary();
    void setUserRoiX(int idx, int corner, int value);
    void setUserRoiY(int idx, int corner, int value);
    void setUserRoiSelectedCorner(int idx, int corner);
    void dioReset();
    void fetchUser(QString user);
    bool loginUser(QString user, QString password);
    void logoutUser();
    void changeUserPassword(QString user, QString newPassword);
    void setCurrentUserSettings( QString user,
                                 bool rangeCheck,
                                 bool functionsCheck,
                                 bool captureCheck,
                                 bool paletteCheck,
                                 bool measureCheck,
                                 bool  advancedVisibleCheck,
                                 bool  advancedImageVisibleCheck,
                                 bool advancedAlarmsCheck,
                                 bool advancedMemoryCheck,
                                 bool advancedSystemCheck,
                                 bool advancedRoiCheck,
                                 bool advancedDioCheck,
                                 bool advancedLayoutCheck);

    bool setDateTime(QString year, QString month, QString day, QString hour, QString minute, QString second);
    void setCurrentRoiIdx(int idx);
    void updateRoiAreaLimits();
    void updateUserCrossLimits();
    void updateBlackBodyLimits();
    bool setRoiNames(QString roi1Name, QString roi2Name, QString roi3Name, QString roi4Name);
    void openManual();
    void setSpecialPheripheralOutput(bool val);
    void mainCameraSnapshot();
    void secondaryCameraSnapshot();
    void openPage();

    QString getLastIp();
    QString getStorage();
    bool    isHeatingUp();
signals:
    //signals indicating which MEDICAS states has just changed
    void temperatureModeChanged(int val);
    void connectedChanged(bool connected, double ping);
    void connectionStarted();
    void connectionFinished(bool connected);
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
    void blackbodyRoiXChanged(int val);
    void blackbodyRoiYChanged(int val);
    void crossUserXChanged(int val);
    void crossUserYChanged(int val);
    void blackbodySizeChanged(int val);
    void blackbodyMaskSizeChanged(int val);
    void blackbodyDetectionChanged(int val);
    void showCrossMaxChanged(bool val);
    void showCrossUserChanged(bool val);
    void userRoiXChanged(int idx, int value);
    void userRoiYChanged(int idx, int value);
    void showBlackbodyRoiChanged(bool val);
    void showRoiUserNamesChanged(bool val);
    void showUserRoiChanged();
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
    void lastAnsChanged();
    void blackbodyError();
    void selectedStorageChanged(QString storage);
    void rangeError();
    void alarmError();
    //dio
    void dioConnectedChanged();
    void dioTriggerModeChanged();
    void dioPictureOnTriggerChanged();
    void dioMinimumPulseWidthChanged();
    void dioSignalChanged();
    void dioTriggerDelayChanged();
    void dioMinDetectionTimeChanged();
    void dioTempHysteresisChanged();
    void dioMinAlarmTimeChanged();
    void dioLogicChanged();
    void dioAlarmChanged();
    void dioGateOpenTimeChanged();
    void dioShowStatusChanged();
    //user
    void currentUserChanged();
    void currentUserSettingsChanged();
    // roi area
    void roiAreaMaxXChanged();
    void roiAreaMinXChanged();
    void roiAreaMaxYChanged();
    void roiAreaMinYChanged();
    void userCrossMaxXChanged();
    void userCrossMinXChanged();
    void userCrossMaxYChanged();
    void userCrossMinYChanged();
    void blackBodyMaxXChanged();
    void blackBodyMinXChanged();
    void blackBodyMaxYChanged();
    void blackBodyMinYChanged();
    void blackBodyRoiScaleChanged();
    void cameraDisconnected();

private slots:
    // getters
    QQmlListProperty<PaletteWrapper> palettes();
    QQmlListProperty<PaletteValueWrapper> paletteValues();

    // setters
    void setTemperatureMode(int val);
    void setManualRange1(const QString& val);
    void setManualRange2(const QString& val);
    void setMainCamera(const QString& val);
    void setBlackbodyRoiX(int val);
    void setBlackbodyRoiY(int val);
    void setCrossUserX(int val);
    void setCrossUserY(int val);
    void setBlackbodyDetection(int val);
    void setAlarmMode(int val);
    void setAlarmCritical(double val);
    void setAlarmPreventive(double val);
    void setShowCrossMax(bool val);
    void setShowCrossUser(bool val);
    void setShowRoiUserNames(bool val);
    void setShowBlackbodyRoi(bool val);
    void setShowUserRoi1(int val);
    void setShowUserRoi2(int val);
    void setShowUserRoi3(int val);
    void setShowUserRoi4(int val);
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
    void setLanguage(QString language);
    // fetch params from camera
    void fetchAll();
    void fetchTemperatureMode();
    void fetchBasicInfo();
    void fetchManualRange();
    void fetchMainCamera();
    void fetchBlackbodyRoi();
    void fetchUserCross();
    void fetchShowCrosses();
    void fetchPalettes();
    void fetchPaletteValues();
    void fetchCaptureRecSettings();
    void fetchAlarms();
    void fetchUserRois();
    void fetchExposure();
    void fetchNoiseReduction();
    void fetchGains();
    void fetchIris();
    void fetchGamma();
    void fetchWhiteBalance();
    void fetchBacklightCompensation();
    void fetchDynamicRangeMode();
    void fetchSystemSettings();
    void fetchStorage();
    //dio
    void fetchDioParams();
    //user
    void fetchCurrentUser();


    // dio
    bool dioConnected();
    QString dioTriggerMode();
    void setDioTriggerMode(QString dioTriggerMode);
    bool dioPictureOnTrigger();
    void setdioPictureOnTrigger(bool val);
    QString dioMinimumPulseWidth();
    void setDioMinimumPulseWidth(QString minPulse);
    QString dioSignal();
    void setDioSignal(QString signal);
    QString dioTriggerDelay();
    void setDioTriggerDelay(QString delay);
    QString dioMinDetectionTime();
    void setDioMinDetectionTime(QString detectionTime);
    QString dioTempHysteresis();
    void setDioTempHysteresis(QString hyst);
    QString dioMinAlarmTime();
    void setDioMinAlarmTime(QString alarmTime);
    QString dioLogic();
    void setDioLogic(QString logic);
    QString dioAlarm();
    void setDioAlarm(QString alarm);
    QString dioGateOpenTime();
    void setDioGateOpenTime(QString gateTime);
    bool dioShowStatus();
    void setDioShowStatus(bool val);

    //user
    QString currentUser();
    UserSettings* currentUserSettings();


    int roiAreaMaxX();
    int roiAreaMinX();
    int roiAreaMaxY();
    int roiAreaMinY();

    int userCrossMaxX();
    int userCrossMinX();
    int userCrossMaxY();
    int userCrossMinY();

    int blackBodyMaxX();
    int blackBodyMinX();
    int blackBodyMaxY();
    int blackBodyMinY();

    int blackBodyRoiScale();
    void setBlackBodyRoiScale(int val);

    void onCameraDisconnected();

private:
    const Values _vals;
    CameraParameters _params;
    CaptureParameters _captureParams;
    std::shared_ptr<NetworkClient> _networkClient = nullptr;
    Protocol _protocol;
    ErrorCodes _errorCodes;
    QList<PaletteWrapper *> _palettes;
    QList<PaletteValueWrapper *> _paletteValues;
    // control flags
    bool _streamFlagVisible = false;
    bool _streamFlagThermal = false;
    bool _extremesFlag = false;
    bool _helperTreadFlag = false;
    bool _connected = false;
    bool _recordingToggled = false;
    QString _systemDate{};
    QString _systemTime{};
    QString _lastAns{};
    QString _saveDest{};
    //threads for recieving streams and runtime information, they are started after successful connection and setup
    HelperThread* _helperThread = nullptr;
    VisibleThread* _visibleThread = nullptr;
    ThermalThread* _thermalThread = nullptr;
    ConnectionThread* _connectionThread = nullptr;

    // control helper functions
    bool send(const QString &message);
    bool send(const QString &message, std::istringstream & answ, bool debugPrint = true, bool cacheAnswer = true);
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
    bool setShowUserRoi(int roiNumber, int val);
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

    int getUserValue(QString user);
    bool isTrueForUser(int val, int userVal);
};

#endif // CONTROLLERCORE_H
