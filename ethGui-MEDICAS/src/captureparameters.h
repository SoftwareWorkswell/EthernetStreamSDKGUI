#ifndef CAPTUREPARAMS_H
#define CAPTUREPARAMS_H

#include <QString>

struct CaptureParameters
{
    bool _radJpg{};
    bool _visJpg{};
    bool _encIRVid{};
    bool _visVid{};
    bool _recording{};
    bool _capturing{};
    QString _ssdDrive{};
    QString _sdDrive{};
    QString _flashDrive;
    QString _currentStorage = "SSD";
};

#endif // CAPTUREPARAMS_H
