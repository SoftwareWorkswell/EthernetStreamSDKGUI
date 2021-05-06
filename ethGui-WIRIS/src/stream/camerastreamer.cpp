#include "camerastreamer.h"
#include <QDir>
#include <QStandardPaths>
#include <QDateTime>

CameraStreamer::CameraStreamer(std::shared_ptr<CameraConnection> connection, std::shared_ptr<CameraParameters> params)
    : QObject(nullptr)
{
    _params = params;
    _connection = connection;
    _params->_saveDest =
            QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
            .at(0) +
            "/" + "Wiris&GIS-Snapshots";
    QDir saveDestDir(_params->_saveDest);
    if(!saveDestDir.exists())
        saveDestDir.mkdir(_params->_saveDest);
    QDir cacheDir(QDir::currentPath() + "/cache/");
    if(!cacheDir.exists())
        cacheDir.mkdir(QDir::currentPath() + "/cache/");
}

// STREAM HANDLE
void CameraStreamer::toggleStreamThermal()
{
    if (!_connection->_isSetup)
        return;

    if (!_connection->_streamFlagThermal)
    {
        _connection->_streamFlagThermal = true;
        QTypedArrayData<QString>::const_iterator* zoom = nullptr;
        if (_params->_currentZoomThermal >= _params->_zoomsThermal.begin() && _params->_currentZoomThermal < _params->_zoomsThermal.end())
            zoom = &_params->_currentZoomThermal;
#ifdef __APPLE__
        _thermalThread = make_unique<ThermalThread>(&_connection->_streamFlagThermal, &_params->_cooledDown, _connection->_endpointIp, _params->_maximum, _params->_minimum, _params->_center, &_params->_paintVals, &_params->_paintCenter, zoom, &_params->_cameraType,  &_params->_thermalUnitSign, &_params->_cooldownTime);
#else
        _thermalThread = std::make_unique<ThermalThread>(&_connection->_streamFlagThermal, &_params->_cooledDown, _connection->_endpointIp, _params->_maximum, _params->_minimum, _params->_center, &_params->_paintVals, &_params->_paintCenter, zoom, &_params->_cameraType, &_params->_thermalUnitSign, &_params->_cooldownTime);
#endif
        QObject::connect(_thermalThread.get(), &ThermalThread::imageSourceChanged, this, &CameraStreamer::streamFrameChanged);
        _thermalThread->start();
    }
    else
    {
        _connection->_streamFlagThermal = false;
        _thermalThread->wait();
    }

}

void CameraStreamer::killStreamThermal()
{
    if (!_connection->_isSetup)
        return;
    _connection->_streamFlagThermal = false;
    _thermalThread->terminate();
    _thermalThread->wait();
}

void CameraStreamer::toggleStreamVisible(){
    if(!_connection->_isSetup)
        return;

    if (!_connection->_streamFlagVisible)
    {
        _connection->_streamFlagVisible = true;
#ifdef __APPLE__
        _visibleThread = make_unique<VisibleThread>(&_connection->_streamFlagVisible, _connection->_endpointIp);
#else
        _visibleThread = std::make_unique<VisibleThread>(&_connection->_streamFlagVisible, _connection->_endpointIp);
#endif
        QObject::connect(_visibleThread.get(), &VisibleThread::imageSourceChanged, this, &CameraStreamer::streamFrameVisibleChanged);
        _visibleThread->start();
    }
    else
    {
        _connection->_streamFlagVisible = false;
        _visibleThread->wait();
    }
}

void CameraStreamer::stopStream()
{
    _connection->_streamFlagThermal = false;
    _connection->_streamFlagVisible = false;
    _thermalThread->wait();
    _visibleThread->wait();
}

void CameraStreamer::mainCameraSnapshot()
{
    if(_params->_mainCamera == "THERMAL")
    {
        if (!streamFrame.isNull())
        {
            if(streamFrame.save(_params->_saveDest + "/" + QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss-zzz") + "_thermal.jpg"))
                emit snapshotSaved(_params->_saveDest + "/" + QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss-zzz") + "_thermal.jpg");
        }

    }
    else
    {
        if (!streamFrameVisible.isNull())
        {
            if(streamFrameVisible.save(_params->_saveDest + "/" + QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss-zzz") + "_visible.jpg"))
                emit snapshotSaved(_params->_saveDest + "/" + QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss-zzz") + "_visible.jpg");
        }
    }
}

void CameraStreamer::secondaryCameraSnapshot()
{
    if(_params->_mainCamera == "VISIBLE")
    {
        if (!streamFrame.isNull())
        {
            if(streamFrame.save(_params->_saveDest + "/" + QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss-zzz") + "_thermal.jpg"))
                emit snapshotSaved(_params->_saveDest + "/" + QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss-zzz") + "_thermal.jpg");
        }
    }
    else
    {
        if (!streamFrameVisible.isNull())
        {
            if(streamFrameVisible.save(_params->_saveDest + "/" + QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss-zzz") + "_visible.jpg"))
                emit snapshotSaved(_params->_saveDest + "/" + QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss-zzz") + "_visible.jpg");
        }
    }
}
