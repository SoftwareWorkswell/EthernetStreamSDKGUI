#ifndef THERMALTHREAD_H
#define THERMALTHREAD_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QDebug>
#include <QImage>
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

extern QImage streamFrame;
extern QMutex *thermalMutex;

class ThermalThread : public StreamThread
{
    Q_OBJECT

public:
    ThermalThread(bool *stream, const QString &ssrc, Extreme *max, Extreme *crossUser, Extreme *blackbody, HeadRoi *head, UserRoiContainer *userRoiContainer,
                  int *blackbodySize, int *blackbodyMaskSize, const QString *type, const QString *units)
        : StreamThread(stream, ssrc), _maximum(max), _crossUser(crossUser), _blackbody(blackbody), _head(head), _userRoiContainer(userRoiContainer),
          _blackbodySize(blackbodySize), _blackbodyMaskSize(blackbodyMaskSize), _units(units), _type(type) {}


protected:
    virtual void run() override;

private:
    void zoomImage(cv::Mat& mat);
    void drawExtremes();
    void drawRoiExtremes(QPainter & paint);
    void drawRois();
    void drawRoiBorders(int x, int y, int w, int h, QPainter & paint);
    void drawRoiCorners(int x, int y, int w, int h, QPainter & paint);
    void drawSelectedRoiCorner(int x, int y, int w, int h, int corner, QPainter & paint);
    void drawRoiArea();
    void drawRoiName(int x, int y, QString &text, QPainter & paint);
    void drawHead();

    Extreme *_maximum{};
    Extreme *_crossUser{};
    Extreme *_blackbody{};
    HeadRoi *_head{};
    UserRoiContainer *_userRoiContainer{};
    int *_blackbodySize{};
    int *_blackbodyMaskSize{};

    const QString *_units{};

    int _skipX = 0, _skipY = 0;
    const QString *_type;
    const Values _vals;
};

#endif // THERMALTHREAD_H
