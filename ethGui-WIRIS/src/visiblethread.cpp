#include "visiblethread.h"



void VisibleThread::run()
{
#ifdef _WIN32
    std::string latency = "3000";
    std::string addr(("rtspsrc location=rtsp://" + this->_ssrc.toStdString() + ":8554/visible latency=" + latency + " ! rtph264depay ! avdec_h264 ! videoconvert ! appsink"));
    cv::VideoCapture cap(addr, cv::CAP_GSTREAMER);
    cap.release();
    latency = "250";
    addr = ("rtspsrc location=rtsp://" + this->_ssrc.toStdString() + ":8554/visible latency=" + latency + " ! rtph264depay ! avdec_h264 ! videoconvert ! appsink");
    cap.open(addr, cv::CAP_GSTREAMER);

#else
    cv::VideoCapture cap("rtspsrc location=rtsp://" + this->_ssrc.toStdString() + ":8554/visible latency=3000 caps = \"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\" ! rtph264depay ! decodebin ! videoconvert ! appsink", cv::CAP_GSTREAMER);
    cap.release();
    cap.open("rtspsrc location=rtsp://" + this->_ssrc.toStdString() + ":8554/visible latency=0 caps = \"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\" ! rtph264depay ! decodebin ! videoconvert ! appsink", cv::CAP_GSTREAMER);
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
        streamFrameVisible = new QImage((uchar*) frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        emit imageSourceChanged();
    }
    cap.release();
    *_stream = false;
}
