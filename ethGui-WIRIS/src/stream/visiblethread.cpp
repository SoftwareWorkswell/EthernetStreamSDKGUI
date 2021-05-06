#include "visiblethread.h"
#include <QDebug>

void VisibleThread::run()
{
#ifdef _WIN32
    std::string latency = "250";
    std::string addr(("rtspsrc location=rtsp://" + this->_ssrc.toStdString() + ":8554/visible latency=" + latency + " ! rtph264depay ! avdec_h264 ! videoconvert ! appsink"));
    cv::VideoCapture cap(addr, cv::CAP_GSTREAMER);
    if(!cap.isOpened())
    {
        qDebug() << "Visible: Input error";
        *_stream = false;
        return;
    }
    cap.release();
    latency = "250";
    addr = ("rtspsrc location=rtsp://" + this->_ssrc.toStdString() + ":8554/visible latency=" + latency + " ! rtph264depay ! avdec_h264 ! videoconvert ! appsink");
    cap.open(addr, cv::CAP_GSTREAMER);
#else
    const std::string addr("rtspsrc location=rtsp://" + this->_ssrc.toStdString() + ":8554/visible latency=300 caps = \"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\" ! rtph264depay ! decodebin ! videoconvert ! appsink");
    cv::VideoCapture cap(addr, cv::CAP_GSTREAMER);
#endif

    if(!cap.isOpened())
    {
        qDebug() << "visible: Input error";
        *_stream = false;
        return;
    }

    cv::Mat frame;

    while(*_stream)
    {
        cap >> frame;
        if (frame.empty())
        {
            qDebug() << "visible: empty frame occurance, stopping stream!";
            break;
        }
        cv::cvtColor(frame, frame,cv::ColorConversionCodes::COLOR_BGR2RGB);
        static uchar *data = nullptr;
        if (!data) {
            data = new uchar[frame.rows * frame.step];
        }
        memcpy(data, frame.data, frame.rows * frame.step);

        streamFrameVisible = QImage(data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888).copy();
        if(*_stream)
            emit imageSourceChanged();
    }
    cap.release();
    *_stream = false;
}
