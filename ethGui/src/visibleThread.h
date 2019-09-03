#ifndef CUSTOMTHREAD_H
#define CUSTOMTHREAD_H

#include <qobject.h>
#include <qthread.h>
#include <QString>
#include <qdebug.h>
#include <qimage.h>
#include <qmutex.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include <memory>
#include "streamthread.h"
#include "networkclient.h"


extern QImage *streamFrameVisible;
extern QMutex *visibleMutex;

class VisibleThread: public StreamThread
{
    Q_OBJECT

public:
    VisibleThread(bool* stream, const QString & ssrc) :  StreamThread (stream, ssrc)
    {}

protected:
    virtual void run() override;


};


#endif // CUSTOMTHREAD_H
