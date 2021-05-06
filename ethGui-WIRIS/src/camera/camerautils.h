#ifndef CAMERAUTILS_H
#define CAMERAUTILS_H

#include <QString>

#include <string>

class CameraUtils
{
public:
    static bool isValidSpan(const QString& window, const QString & center, const QString& env);
    static bool isValidIpv4Addr(const std::string &addr);
    static bool compareFw(const QString& currentV, const QString& updateV);
    static bool compareVersion(const QString& currentV, const QString& updateV);
    static QString thermalUnitToSign(const QString & unit);
    static QString thermalUnitFromCode(const QString & code);
};

#endif // CAMERAUTILS_H
