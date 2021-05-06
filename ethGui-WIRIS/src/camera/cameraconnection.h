#ifndef CAMERACONNECTION_H
#define CAMERACONNECTION_H

#include <QString>

struct CameraConnection
{
    // endpoint
    const QString   _endpointPort = "2240";
    QString         _endpointIp{};
    // connection state
    bool _isSetup{};
    bool _connected{};
    bool _recording{};
    bool _capturing{};
    bool _streamFlagVisible{};
    bool _streamFlagThermal{};
    bool _extremesFlag{};
    bool _helperTreadFlag{};
};

#endif // CAMERACONNECTION_H
