#ifndef CUSTOMTHREAD_H
#define CUSTOMTHREAD_H

#include <qobject.h>
#include <qthread.h>
#include <QString>
#include <qimage.h>
#include <qmutex.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include <memory>
#include <src/stream/streamthread.h>
#include <src/communication/networkclient.h>

extern QImage streamFrameVisible;
extern std::shared_ptr<QMutex> visibleMutex;

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
