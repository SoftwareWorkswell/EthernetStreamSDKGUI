#ifndef CAMERACONSTANTS_H
#define CAMERACONSTANTS_H

#include <QString>
#include <QStringList>

#include <istream>
#include <string>
#include <map>

struct CameraConstants
{
    // camera codes
    const QString WIRIS_PRO_CODE = "WWP";
    const QString WIRIS_SECURITY_CODE = "WWS";
    const QString GIS_CODE = "GIS";
    // full camera names
    const QString WIRIS_PRO_TYPE       = "WIRIS Pro";
    const QString WIRIS_SECURITY_TYPE = "WIRIS Security";
    const QString GIS_TYPE            = "GIS-320";
    const QString MIN_FW_VERSION      = "1.1.1";
    // ranges
    const QString RANGE_MODE_SPAN      = "SPAN";
    const QString RANGE_MODE_AUTOMATIC = "AUTOMATIC";
    const QString RANGE_MODE_MANUAL    = "MANUAL";
    // alarms
    const QString ALARM_MODE_OFF     = "OFF";
    const QString ALARM_MODE_ABOVE   = "ABOVE";
    const QString ALARM_MODE_BELOW   = "BELOW";
    const QString ALARM_MODE_BETWEEN = "BETWEEN";
    const QString ALARM_MODE_OUTSIDE   = "OUTSIDE";
    const QStringList SUPPORTED_ALARM_COLORS = {"RED", "GREEN", "BLUE"};
    const std::map<QString, int> ALARM_COLOR_TO_INDEX = {{"RED", 0}, {"GREEN", 1}, {"BLUE", 2}};
    const int RANGE_MODE_AUTOMATIC_INT = 0;
    const int RANGE_MODE_MANUAL_INT = 1;
    const int RANGE_MODE_SPAN_INT = 2;
    const double MAX_EMISSIVITY = 1;
    const double MIN_EMISSIVITY = 0.5;
    const double MIN_ENV_TEMP = -50;
    const double MAX_ENV_TEMP = 1500;
    const int MIN_SHUTTER_PERIOD = 2;
    const int MAX_SHUTTER_PERIOD = 30;
    // min temperature resolvable value
    const double EPS = 0.01;

    const QString LAST_IP_FILENAME = "lastIpCache";
};

#endif // CAMERACONSTANTS_H
