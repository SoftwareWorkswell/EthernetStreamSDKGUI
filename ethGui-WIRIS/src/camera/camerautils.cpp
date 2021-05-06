#include "camerautils.h"

#include <QStringList>
#include <sstream>
#include <boost/asio.hpp>

bool CameraUtils::isValidSpan(const QString& window, const QString & center, const QString& env)
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

bool CameraUtils::isValidIpv4Addr(const std::string &addr)
{
    boost::system::error_code ec;
    boost::asio::ip::address::from_string( addr, ec );
    return !ec.failed();
}

bool CameraUtils::compareFw(const QString& currentV, const QString& updateV)
{
    return CameraUtils::compareVersion(currentV, updateV);
}

bool CameraUtils::compareVersion(const QString& currentV, const QString& updateV)
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

QString CameraUtils::thermalUnitToSign(const QString & unit)
{
    if(unit.startsWith("C"))
        return "°C";
    if(unit.startsWith("F"))
        return "°F";
    if(unit.startsWith("K"))
        return "°K";
}

QString CameraUtils::thermalUnitFromCode(const QString & code)
{
    if(code == "K")
        return QString("KELVIN");
    else if(code == "F")
        return "FAHRENHEIT";
    else return QString("CELSIUS");
}
