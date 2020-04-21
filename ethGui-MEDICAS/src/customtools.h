#ifndef CUSTOMTOOLS_H
#define CUSTOMTOOLS_H

#include <istream>
#include <string>
#include <map>
#include <QString>
#include <qstringlist.h>

// possible values that are used in communication
struct Values{
    const QString lastIpFileName = "lastIpCache";
    const QString medicasType = "MEDICAS";
    const QString rangeModeSpan = "SPAN";
    const QString rangeModeAutomatic = "AUTOMATIC";
    const QString rangeModeManual = "MANUAL";

    const QStringList supportedAlarmModes = {"ON", "OFF"};
    const std::map<QString, int> alarmModeToIndex = {{"ON", 0}, {"OFF", 1}};

    const QStringList supportedAlarmColors = {"RED", "GREEN", "BLUE", "LIME", "YELLOW", "CYAN", "MAGENTA", "MAROON", "OLIVE", "PURPLE", "TEAL", "NAVY", "BROWN"};
    const std::map<QString, int> alarmColorToIndex = {{"RED", 0}, {"GREEN", 1}, {"BLUE", 2}, {"LIME", 3}, {"YELLOW", 4}, {"CYAN", 5}, {"MAGENTA", 6}, {"MAROON", 7}, {"OLIVE", 8}, {"PURPLE", 9}, {"TEAL", 10},{"NAVY", 11}, {"BROWN", 12}};

    const QStringList supportedGammaModes = {"STANDARD", "STREIGHT", "NARROW", "WIDE"};
    const std::map<QString, int> gammaModeToIndex = {{"STANDARD", 0}, {"STREIGHT", 1}, {"NARROW", 2}, {"WIDE", 3}};

    const QStringList supportedWhiteBalanceModes = {"ATW1", "INDOOR", "OUTDOOR", "ATW2", "ONE_PUSH_TRIGGER"};
    const std::map<QString, int> whiteBalanceModeToIndex = {{"ATW1", 0}, {"INDOOR", 1}, {"OUTDOOR", 2}, {"ATW2", 3}, {"ONE_PUSH_TRIGGER", 4}};

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
