#ifndef CAMERASTREAMER_H
#define CAMERASTREAMER_H

#include <QObject>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include <src/stream/visiblethread.h>
#include <src/stream/thermalthread.h>
#include <src/camera/cameraconnection.h>

extern QImage streamFrame;
extern QImage streamFrameVisible;
extern std::shared_ptr<QMutex> thermalMutex;
extern std::shared_ptr<QMutex> visibleMutex;

class CameraStreamer : public QObject
{
    Q_OBJECT
    //threads for recieving, they are started after successful connection and setup
    std::unique_ptr<VisibleThread>      _visibleThread = nullptr;
    std::unique_ptr<ThermalThread>      _thermalThread = nullptr;
    // camera parameters and connection state
    std::shared_ptr<CameraParameters>   _params = nullptr;
    std::shared_ptr<CameraConnection>   _connection = nullptr;
public:
    CameraStreamer(std::shared_ptr<CameraConnection> connection, std::shared_ptr<CameraParameters> params);
    void toggleStreamThermal();
    void toggleStreamVisible();
    void killStreamThermal();
    void stopStream();
    void mainCameraSnapshot();
    void secondaryCameraSnapshot();
signals:
    void streamFrameChanged();
    void streamFrameVisibleChanged();
    void snapshotSaved(QString path);
};

#endif // CAMERASTREAMER_H
