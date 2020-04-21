#ifndef THERMALTHREAD_H
#define THERMALTHREAD_H

#include <qobject.h>
#include <qthread.h>
#include <QString>
#include <qdebug.h>
#include <qimage.h>
#include <QMutex>
#include <QMutexLocker>
#include <QPainter>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include <memory>
#include "customtools.h"
#include "streamthread.h"
#include "networkclient.h"

extern QImage *streamFrame;
extern QMutex *thermalMutex;

class ThermalThread : public StreamThread
{
    Q_OBJECT


public:
    ThermalThread(bool* stream, const QString & ssrc, Extreme* max, Extreme* center, Extreme* blackbody, bool* showMax, bool* showCenter, bool * showBlackbody, const QString* type)
       : StreamThread (stream, ssrc), _maximum(max), _center(center),_blackbody(blackbody), _showMax(showMax), _showCenter(showCenter), _showBlackbody(showBlackbody), _type(type)
    {}


protected:
    virtual void run() override;

private:
    void zoomImage(cv::Mat& mat);
    void drawExtremes();
    Extreme* _maximum;
    Extreme* _center;
    Extreme* _blackbody;
    bool* _showMax;
    bool* _showCenter;
    bool* _showBlackbody;
    int _skipX = 0, _skipY = 0;
    const QString* _type;
    const Values _vals;
};

#endif // THERMALTHREAD_H
