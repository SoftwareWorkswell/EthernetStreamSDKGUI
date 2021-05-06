#ifndef THERMALTHREAD_H
#define THERMALTHREAD_H

#include <qobject.h>
#include <qthread.h>
#include <QString>
#include <qimage.h>
#include <QMutex>
#include <QMutexLocker>
#include <QPainter>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include <memory>
#include <src/stream/streamthread.h>
#include <src/communication/networkclient.h>
#include <src/camera/cameraparameters.h>
#include <src/communication/protocol.h>
#include <src/camera/cameraconstants.h>

extern QImage streamFrame;
extern std::shared_ptr<QMutex> thermalMutex;

class ThermalThread : public StreamThread
{
    Q_OBJECT
public:
    ThermalThread(bool* stream,bool *cooledDown, const QString & ssrc, std::shared_ptr<Extreme> max, std::shared_ptr<Extreme> min, std::shared_ptr<Extreme> center, bool* showVals, bool* showCenter, QTypedArrayData<QString>::const_iterator* zoom, const QString* type, const QString* unitSign, const QString* cooldownTime)
       : StreamThread (stream, ssrc), _cooledDown(cooledDown), _maximum(max), _minimum(min), _center(center), _showVals(showVals), _showCenter(showCenter), _zoom(zoom), _type(type), _unitSign(unitSign), _cooldownTime(cooldownTime)
    {}
protected:
    virtual void run() override;

private:
    Protocol _protocol;

    void zoomImage(cv::Mat& mat);
    void drawWirisExtremes();
    void drawGisExtremes();
    int _skipX = 0, _skipY = 0;
    bool *_cooledDown;
    std::shared_ptr<Extreme> _maximum;
    std::shared_ptr<Extreme> _minimum;
    std::shared_ptr<Extreme> _center;
    bool* _showVals;
    bool* _showCenter;
    QTypedArrayData<QString>::const_iterator* _zoom;
    const QString* _type;
    const QString* _unitSign;
    const QString* _cooldownTime;
    const CameraConstants _vals;
};

#endif // THERMALTHREAD_H
