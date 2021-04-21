#ifndef CUSTOMTOOLS_H
#define CUSTOMTOOLS_H

#include <QString>
#include <QList>
#include <QStringList>
#include <QDir>
#include <QDateTime>

#include <istream>
#include <string>
#include <map>


// possible values that are used in communication
struct Values{
    // generate unique last ip cache so they are not rewritten when running more app instances with different cams
    const QString lastIpFileName = QDir::currentPath() +"/cache/lastIpCache";
    const QString currentIpFileName = QDir::currentPath() +"/cache/lastIpCache_" + QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss-zzz");
    const QString medicasType = "MEDICAS";
    const QString rangeModeSpan = "SPAN";
    const QString rangeModeAutomatic = "AUTOMATIC";
    const QString rangeModeManual = "MANUAL";

    const QStringList supportedTemperatureModes = {"CBT", "SST", "ISO", "AXI", "LONG-SST", "LONG-CBT", "LONG-AXI"};
    const std::map<QString, int> temperatureModeToIndex = {{"CBT", 0}, {"SST", 1}, {"ISO", 2}, {"AXI", 3}, {"LONG-SST",4}, {"LONG-CBT", 5}, {"LONG-AXI", 6}};
    const std::map<QString, double> temperatureModeToOffset = {{"CBT", 2.2}, {"SST", 0}, {"ISO", 1.5}, {"AXI", 1.9}, {"LONG-SST", -0.3}, {"LONG-CBT", 2.5}, {"LONG-AXI", 2.2}};

    const QStringList supportedBlackbodyDetections = {"STANDARD", "SENSITIVE", "OFF"};
    const std::map<QString, int> blackbodyDetectionToIndex = {{"STANDARD", 0}, {"SENSITIVE", 1}, {"OFF", 2}};

    const QStringList supportedBlackbodySizes = {"SHORT" ,"NORMAL", "BIG", "LARGE"};
    const QList<double> supportedBlackbodySizeCoeffs {2.5,5, 7.5, 12.5};
    const std::map<QString, int> blackbodySizeToIndex = {{"SHORT", 0}, {"NORMAL", 1}, {"BIG", 2}, {"LARGE", 3}};

    const QStringList supportedBlackbodyMaskSizes = {"SMALL", "MIDDLE", "LARGE"};
    const QList<double> supportedBlackbodyMaskSizeCoeffs {0.45, 0.75, 1.5};
    const std::map<QString, int> blackbodyMaskSizeToIndex = {{"SMALL", 0},{"MIDDLE", 1}, {"LARGE", 2}};

    const QStringList supportedAlarmModes = {"OFF", "ON", "ROIS"};
    const std::map<QString, int> alarmModeToIndex = {{"OFF", 0}, {"ON", 1}, {"ROIS", 2}};

    const QStringList supportedAlarmColors = {"RED", "GREEN", "BLUE", "LIME", "YELLOW", "CYAN", "MAGENTA", "MAROON", "OLIVE", "PURPLE", "TEAL", "NAVY", "BROWN"};
    const std::map<QString, int> alarmColorToIndex = {{"RED", 0}, {"GREEN", 1}, {"BLUE", 2}, {"LIME", 3}, {"YELLOW", 4}, {"CYAN", 5}, {"MAGENTA", 6}, {"MAROON", 7}, {"OLIVE", 8}, {"PURPLE", 9}, {"TEAL", 10},{"NAVY", 11}, {"BROWN", 12}};

    const QStringList supportedRoiVisibilityModes = {"NONE", "ALL", "CORNERS"};
    const std::map<QString, int> roiVisibilityToIndex = {{"NONE", 0},  {"ALL", 1}, {"CORNERS",2}};

    const QStringList supportedGammaModes = {"STANDART", "STRAIGHT", "NARROW", "WIDE"};
    const std::map<QString, int> gammaModeToIndex = {{"STANDART", 0}, {"STRAIGHT", 1}, {"NARROW", 2}, {"WIDE", 3}};

    const QStringList supportedWhiteBalanceModes = {"NARROW", "INDOOR", "OUTDOOR", "WIDE", "ONE_PUSH_TRIGGER"};
    const std::map<QString, int> whiteBalanceModeToIndex = {{"NARROW", 0}, {"INDOOR", 1}, {"OUTDOOR", 2}, {"WIDE", 3}, {"ONE_PUSH_TRIGGER", 4}};

    const QStringList supportedExposureModes = {"AUTO", "SHUTTER_PRIORITY", "IRIS_PRIORITY", "MANUAL"};
    const std::map<QString, int> exposureModeToIndex = {{"AUTO", 0}, {"SHUTTER_PRIORITY", 1}, {"IRIS_PRIORITY", 2}, {"MANUAL", 3}};
    // iris modes have ex codes used as parameters in communication
    const std::map<QString, int> supportedIrisModes { {"F22", 0x03}, {"F16", 0x04}, {"F14", 0x05}, {"F11", 0x06}, {"F9.6", 0x07},
                                                      {"F8", 0x08}, {"F6.8", 0x09}, {"F5.6", 0x0A}, {"F4.8", 0x0B}, {"F4", 0x0C},
                                                      {"F3.4", 0x0D}, {"F2.8",  0x0E}, {"F2.4", 0x0F}, {"F2", 0x10}, {"F1.8", 0x11}};
};


struct Extreme{
    int x;
    int y;
    double val;
    bool show = false;
    int scale = 100; // scale for BB
};

struct UserRoi{
    int x;
    int y;
    int w;
    int h;
    // 0 - off, 1 - border. 2-corners
    int show = 0;
    int selectedCorner = 0;
    QString name = "*";
    Extreme max;
};

struct HeadRoi{
    int y;
    const int x = 320; // head can be only in middle
    Extreme max;
    double isoMax;
    bool show = false;
};

struct UserRoiContainer
{
    UserRoi * _userRoi1 = new UserRoi();
    UserRoi * _userRoi2 = new UserRoi();
    UserRoi * _userRoi3 = new UserRoi();
    UserRoi * _userRoi4 = new UserRoi();
    // holds index of currently chosen roi for positioning/name change
    int _currentRoi = -1;

    int roiAreaMaxX = 613;
    int roiAreaMinX = 26;
    int roiAreaMaxY = 511;
    int roiAreaMinY = 490;
    bool showNames = false;

    UserRoiContainer() = default;
    ~UserRoiContainer() {
        delete _userRoi1;
        delete _userRoi2;
        delete _userRoi3;
        delete _userRoi4;
    }
    UserRoiContainer(const UserRoiContainer & other) = delete;
    UserRoiContainer& operator=(const UserRoiContainer & other) = delete;
};

static std::istream& getAnyLine(std::istream& is, std::string& t)
{
    t.clear();

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        case 0:
           is.setstate(std::ios::eofbit);
           return is;

        default:
            t += (char)c;
        }
    }
}

static bool compareVersion(const QString& currentV, const QString& updateV)
{

    auto current = currentV.split(".");
    auto update = updateV.split(".");

    if (current.count() != 3 || update.count() != 3)
        return false;

    if (update[0].toInt() > current[0].toInt()) {
        return true;
    } else if (update[0].toInt() == current[0].toInt() && update[1].toInt() > current[1].toInt()) {
        return true;
    } else if (update[0].toInt() == current[0].toInt() && update[1].toInt() == current[1].toInt() && update[2].toInt() > current[2].toInt()) {
        return true;
    }

    return false;
}
#endif // CUSTOMTOOLS_H
