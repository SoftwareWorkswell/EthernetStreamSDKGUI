
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

#ifndef CONTROLLERCOREINTERFACE_H
#define CONTROLLERCOREINTERFACE_H

#include <QObject>
#include <QString>
#include <QTextStream>
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
#include <QMutex>
#include <QDir>

#include <src/communication/networkclient.h>
#include <src/camera/cameraconstants.h>
#include <src/communication/connectionthread.h>
#include <src/communication/helperthread.h>
#include <src/graphic/palettewrapper.h>
#include <src/graphic/palettevaluewrapper.h>
#include <src/camera/cameraparameters.h>
#include <src/camera/cameraconnection.h>
#include <src/stream/camerastreamer.h>
#include <src/communication/controllercore.h>
#include <src/communication/protocol.h>

extern std::shared_ptr<QMutex> connectionMutex;

class ControllerCoreInterface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(bool isSetup READ isSetup)
    Q_PROPERTY(QString ipAddress READ ipAddress NOTIFY ipAdressChanged)
    Q_PROPERTY(QString type READ type NOTIFY typeChanged)
    Q_PROPERTY(QString firmwareVersion READ firmwareVersion NOTIFY firmwareVersionChanged)
    Q_PROPERTY(QString serialNumber READ serialNumber NOTIFY serialNumberChanged)
    Q_PROPERTY(QString articleNumber READ articleNumber NOTIFY articleNumberChanged)
    Q_PROPERTY(double cpuTemp READ cpuTemp NOTIFY cpuTempChanged)
    Q_PROPERTY(double camTemp READ camTemp NOTIFY camTempChanged)
    Q_PROPERTY(QString thermalUnit READ thermalUnit WRITE setThermalUnit NOTIFY thermalUnitChanged)
    Q_PROPERTY(QString thermalUnitSign READ thermalUnitSign NOTIFY thermalUnitChanged)
    Q_PROPERTY(QString systemTime READ systemTime NOTIFY dateTimeChanged )
    Q_PROPERTY(QString systemDate READ systemDate NOTIFY dateTimeChanged )
    Q_PROPERTY(QString currentZoomVisible READ currentZoomVisible NOTIFY currentZoomVisibleChanged)
    Q_PROPERTY(QString currentZoomThermal READ currentZoomThermal NOTIFY currentZoomThermalChanged)
    Q_PROPERTY(double paletteBottom READ paletteBottom NOTIFY paletteBottomChanged)
    Q_PROPERTY(double paletteTop READ paletteTop NOTIFY paletteTopChanged)
    Q_PROPERTY(QQmlListProperty<PaletteWrapper> palettes READ palettes NOTIFY palettesChanged)
    Q_PROPERTY(QQmlListProperty<PaletteValueWrapper> paletteValues READ paletteValues NOTIFY paletteValuesChanged)
    Q_PROPERTY(QString currentPalette READ currentPalette NOTIFY currentPaletteChanged)
    Q_PROPERTY(double emissivity READ emissivity WRITE setEmissivity NOTIFY emissivityChanged)
    Q_PROPERTY(double reflectedTemp READ reflectedTemp WRITE setReflectedTemp NOTIFY reflectedTempChanged)
    Q_PROPERTY(double atmosphericTemp READ atmosphericTemp WRITE setAtmosphericTemp NOTIFY atmosphericTempChanged)
    Q_PROPERTY(bool syncShutter READ syncShutter WRITE setSyncShutter NOTIFY syncShutterChanged)
    Q_PROPERTY(bool interpolation READ interpolation WRITE setInterpolation NOTIFY interpolationChanged)
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
    Q_PROPERTY(QString enviroment3 READ enviroment3 WRITE setEnviroment4 NOTIFY enviroment3Changed)
    Q_PROPERTY(QString enviroment4 READ enviroment4 WRITE setEnviroment3 NOTIFY enviroment4Changed)
    Q_PROPERTY(bool sbusLogging READ sbusLogging WRITE setSbusLogging NOTIFY sbusLoggingChanged)
    Q_PROPERTY(bool mavlinkLogging READ mavlinkLogging WRITE setMavlinkLogging NOTIFY mavlinkLoggingChanged)

public:
    ControllerCoreInterface(QObject * parent = nullptr);
    // connection & license setup
    Q_INVOKABLE bool connect(const QString& ip);
    Q_INVOKABLE void stopConnect();
    Q_INVOKABLE bool setup();
    Q_INVOKABLE bool isActivated();
    Q_INVOKABLE bool activate(const QString & actStr);
    Q_INVOKABLE void close();
    // ACTIONS
    Q_INVOKABLE void toggleDifferentialMode(bool val);
    Q_INVOKABLE void toggleHighSensitivityMode(bool val);
    Q_INVOKABLE void setAlarmColors(const QString& above, const QString& below, const QString& between);
    Q_INVOKABLE void setCaptureVideoSettings(bool radJpg, bool radTiff, bool srImg, bool visJpg, bool radVid, bool visVid);
    Q_INVOKABLE bool changeStorage(const QString & storage);
    Q_INVOKABLE void setDefaultSettings();
    Q_INVOKABLE void reboot();
    Q_INVOKABLE void triggerShutter();
    Q_INVOKABLE void triggerCapture();
    Q_INVOKABLE void toggleRecording();
    Q_INVOKABLE void mainCameraSnapshot();
    Q_INVOKABLE void secondaryCameraSnapshot();
    Q_INVOKABLE void paintMax(bool paint);
    Q_INVOKABLE void paintMin(bool paint);
    Q_INVOKABLE void paintVals(bool paint);
    Q_INVOKABLE void paintCenter(bool paint);
    Q_INVOKABLE void setCurrentPaletteByIndex(int i);
    Q_INVOKABLE void changeZoomVisible(bool direction);
    Q_INVOKABLE void changeZoomThermal(bool direction);
    Q_INVOKABLE void setEnviroment1(const QString & env);
    Q_INVOKABLE void setEnviroment2(const QString & env);
    Q_INVOKABLE void setEnviroment3(const QString & env);
    Q_INVOKABLE void setEnviroment4(const QString & env);
    Q_INVOKABLE void autoFocus();
    Q_INVOKABLE void infiniteFocus();
    Q_INVOKABLE void toggleStreamThermal();
    Q_INVOKABLE void toggleStreamVisible();
    Q_INVOKABLE void setMainCamera(const QString& val);
    Q_INVOKABLE int currentPaletteIndex();
    // helpers and utils
    Q_INVOKABLE QString getLastIp() const;
    Q_INVOKABLE void setLastIp(const QString& ip);
    Q_INVOKABLE bool compareFw(const QString& currentV, const QString& updateV) const;
    Q_INVOKABLE bool checkAddressFormat(const QString& ip) const;
    // invokable getters
    Q_INVOKABLE QString enviromentLowerThreshold() const;
    Q_INVOKABLE QString enviromentUpperThreshold() const;
    Q_INVOKABLE bool isCurrentEnviroment(const QString & env) const;
    Q_INVOKABLE bool streamingThermal() const;
    Q_INVOKABLE bool streamingVisible() const;
    bool sendCommand(const QString & mes, std::istringstream & iss);
signals:
    void connectionStarted();
    void connectedChanged(bool connected, double ping);
    void connectionFinished(bool connected);
    void enviroment1Changed();
    void enviroment2Changed();
    void enviroment3Changed();
    void enviroment4Changed();
    void envChangeStarted();
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
    void reflectedTempChanged(double val);
    void atmosphericTempChanged(double val);
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
    void streamFrameChanged();
    void streamFrameVisibleChanged();
    void highSensitivityModeChanged(bool val);
    void differentialModeChanged(bool val);
    void interpolationChanged(bool val);
    void sbusLoggingChanged(bool val);
    void mavlinkLoggingChanged(bool val);
    void thermalUnitChanged();
    void showInfoMessage(QString message);
    void cameraDisconnected();
private slots:
    // communication with connection thread
    bool connectionFinishedHandler(const QString & ip);
    // communication with helper thread
    void onHelperExtremesChanged();
    void onHelperTempsChanged(double cpu, double cam);
    void onHelpercaptureRecordingChanged(bool capture, bool record);
    void onHelperdriveStateChanged(const QString& ssd, const QString& ssdCap, const QString& ssdFree,
                                   const QString& sd, const QString& sdCap, const QString& sdFree,
                                   const QString& flash, const QString& flashCap, const QString& flashFree);
    void onHelperDateTimeChanged(const QString& date, const QString& time);
    void onCooldownTimeChanged(const QString& time);
    void onHelperConnectionStateChanged(bool state, double ping);
    void onHelperGpsChanged(const QString& gps);
    void afterCameraSnapshot(QString path);
    void onGisThermalSwitchOff();
private:
    Protocol _protocol;
    const CameraConstants _vals;
    // camera parameters and connection state
    std::shared_ptr<CameraParameters>   _params = nullptr;
    std::shared_ptr<CameraConnection>   _connection = nullptr;
    std::unique_ptr<ControllerCore>     _core = nullptr;
    std::unique_ptr<CameraStreamer>     _streamer = nullptr;
    //threads for recieving streams and runtime information, they are started after successful connection and setup
    std::unique_ptr<HelperThread>       _helperThread = nullptr;
    // connection thread
    std::unique_ptr<ConnectionThread>   _connectionThread = nullptr;
    // notify all qml items after settings were fetched
    void notifyAll();
    // getters invoked by QML
    bool connected() const;
    void onCameraDisconnected();
    QString systemTime() const;
    QString systemDate() const;
    bool isSetup() const;
    QString type() const;
    QString firmwareVersion() const;
    QString serialNumber() const;
    QString articleNumber() const;
    bool syncShutter() const;
    bool interpolation() const;
    int shutterPeriod() const;
    double cpuTemp() const;
    double camTemp() const;
    double paletteBottom() const;
    double paletteTop() const;
    QString currentPalette() const;
    QString currentZoomThermal() const;
    QString currentZoomVisible() const;
    QString ipAddress() const;
    double emissivity() const;
    double reflectedTemp() const;
    double atmosphericTemp() const;
    QString enviroment1() const;
    QString enviroment2() const;
    QString enviroment3() const;
    QString enviroment4() const;
    int rangeMode() const;
    QString manualRange1() const;
    QString manualRange2() const;
    QString spanCenter() const;
    QString spanWindow() const;
    int alarmMode() const;
    double alarmTo() const;
    double alarmFrom() const;
    bool sbusLogging() const;
    bool mavlinkLogging() const;
    QString thermalUnit() const;
    QString thermalUnitSign() const;
    QQmlListProperty<PaletteWrapper> palettes();
    QQmlListProperty<PaletteValueWrapper> paletteValues();
    // setters invoked by QML
    void setRangeMode(int value);
    void setManualRange1(const QString& val);
    void setManualRange2(const QString& val);
    void setSpanCenter(const QString& val);
    void setSpanWindow(const QString& val);
    void setAlarmMode(int val);
    void setAlarmTo(double val);
    void setAlarmFrom(double val);
    void setEmissivity(double val);
    void setAtmosphericTemp(double val);
    void setReflectedTemp(double val);
    void setSyncShutter(bool val);
    void setShutterPeriod(int val);
    void setInterpolation(bool val);
    void setMavlinkLogging(bool val);
    void setSbusLogging(bool val);
    void setThermalUnit(const QString & unit);
    void toggleHelperThread();
};

#endif // CONTROLLERCOREINTERFACE_H
