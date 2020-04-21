#ifndef CUSTOMTOOLS_H
#define CUSTOMTOOLS_H

#include <istream>
#include <string>
#include <map>
#include <QString>
#include <qstringlist.h>

struct Values{
    const QString wirisProType = "WIRIS Pro";
    const QString wirisSecurityType = "WIRIS Security";
    const QString rangeModeSpan = "SPAN";
    const QString rangeModeAutomatic = "AUTOMATIC";
    const QString rangeModeManual = "MANUAL";

    const QString alarmModeOFF = "OFF";
    const QString alarmModeABOVE = "ABOVE";
    const QString alarmModeBELOW = "BELOW";
    const QString alarmModeBETWEEN = "BETWEEN";
    const QString alarmModeOUTSIDE = "OUTSIDE";
    const QStringList supportedAlarmColors = {"RED", "GREEN", "BLUE"};
    const std::map<QString, int> alarmColorToIndex = {{"RED", 0}, {"GREEN", 1}, {"BLUE", 2}};
    const int rangeModeAutomaticInt = 0;
    const int rangeModeManualInt = 1;
    const int rangeModeSpanInt = 2;

    const QString lastIpFileName = "lastIpCache";
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
