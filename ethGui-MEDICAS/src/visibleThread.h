#ifndef CUSTOMTHREAD_H
#define CUSTOMTHREAD_H

#include <QDebug>
#include <QImage>
#include <QMutex>
#include <QObject>
#include <QString>
#include <QThread>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include <memory>

#include "networkclient.h"
#include "streamthread.h"


extern QImage streamFrameVisible;
extern QMutex *visibleMutex;

class VisibleThread : public StreamThread {
    Q_OBJECT

public:
    VisibleThread(bool *stream, const QString &ssrc) : StreamThread(stream, ssrc) {}


private:
    void detectFaces(cv::Mat &frame);

protected:
    virtual void run() override;
};


#endif // CUSTOMTHREAD_H
