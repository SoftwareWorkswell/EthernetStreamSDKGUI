#include "thermalthread.h"

void ThermalThread::run()
{
    // qDebug() << "toggle stream thermal";
#ifdef _WIN32
    const std::string addr(("rtspsrc location=rtsp://" + this->_ssrc.toStdString() + ":8554/thermal latency=250 ! rtph264depay ! avdec_h264 ! videoconvert ! appsink"));
#else
    const std::string addr("rtspsrc location=rtsp://" + this->_ssrc.toStdString() + ":8554/thermal latency=300 caps = \"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\" ! rtph264depay ! decodebin ! videoconvert ! appsink");
#endif
    cv::VideoCapture cap(addr, cv::CAP_GSTREAMER);
    if(!cap.isOpened())
    {
        qDebug() << "thermal: Input error";
        *_stream = false;
        return;
    }

    cv::Mat frame;
    while(*_stream)
    {
        cap >> frame;
        if (frame.empty())
        {
            qDebug() << "thermal: empty frame occurance, stopping stream!";
            break;
        }
        cv::cvtColor(frame, frame,cv::ColorConversionCodes::COLOR_BGR2RGB);
        streamFrame = new QImage((uchar*) frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        drawExtremes();
        emit imageSourceChanged();
    }
    cap.release();
    *_stream = false;
}

void ThermalThread::zoomImage(cv::Mat& mat)
{
    QMutexLocker lock(thermalMutex);
    QString cache = "1";

    if (cache.toDouble() > 1)
    {
        int newRows = mat.rows/cache.toDouble(), newColumns = mat.cols/cache.toDouble();
        _skipX = ((mat.cols-newColumns)/2);
        _skipY = ((mat.rows-newRows)/2);
        cv::Rect cropp(_skipX, _skipY, newColumns, newRows);
        mat = mat(cropp);
    }
    else {
        _skipX = 0;
        _skipY = 0;
    }
}


void ThermalThread::drawExtremes()
{
    QPainter paint;
    paint.begin(streamFrame);
    int drawx, drawy;
    double val;
    double size = 1;

    thermalMutex->lock();
    thermalMutex->unlock();

    bool resize = size > 2;

    int innerRectWidth = resize ? 1 : 2;
    int innerRectHeight = resize ? 9 : 16;
    int outerRectWidth = resize ? 3 : 4;
    int outerRectHeight = resize ? 11 : 18;
    int innerRectX= resize ? 4 : 7;
    int outerRectX = 1;
    int outerRectY = resize ? 5 : 8;

    if (_maximum->show)
    {
        drawx = _maximum->x - _skipX;// > 2 ? _maximum->x - 2 : _maximum->x;
        drawy = _maximum->y - _skipY;// > 2 ? _maximum->y - 2 : _maximum->y;
        val = _maximum->val;
        QRect rectV(drawx-innerRectX, drawy, innerRectHeight, innerRectWidth);
        QRect rectH(drawx, drawy-innerRectX, innerRectWidth, innerRectHeight);
        QRect rectVB(drawx-outerRectY, drawy-outerRectX, outerRectHeight, outerRectWidth);
        QRect rectHB(drawx-outerRectX, drawy-outerRectY, outerRectWidth, outerRectHeight);
        paint.fillRect(rectVB, QBrush(Qt::black, Qt::SolidPattern));
        paint.fillRect(rectHB, QBrush(Qt::black, Qt::SolidPattern));
        paint.fillRect(rectV, QBrush(Qt::red, Qt::SolidPattern));
        paint.fillRect(rectH, QBrush(Qt::red, Qt::SolidPattern));


        if(*_showMax)
        {
            QFont font=paint.font();
            resize ? font.setPointSize(7) : font.setPointSize(13);
            font.setBold(true);
            paint.setFont(font);
            paint.setPen(Qt::white);
            paint.drawText( resize ? drawx-12 : drawx-18, resize ? drawy+17 : drawy+30, QString::number((int)(val*10.0)/10.0) + "°C");
        }
    }

    if (_center->show)
    {
        drawx = _center->x - _skipX;// > 2 ? _maximum->x - 2 : _maximum->x;
        drawy = _center->y - _skipY;// > 2 ? _maximum->y - 2 : _maximum->y;
        val = _center->val;
        QRect rectV(drawx-innerRectX, drawy, innerRectHeight, innerRectWidth);
        QRect rectH(drawx, drawy-innerRectX, innerRectWidth, innerRectHeight);
        QRect rectVB(drawx-outerRectY, drawy-outerRectX, outerRectHeight, outerRectWidth);
        QRect rectHB(drawx-outerRectX, drawy-outerRectY, outerRectWidth, outerRectHeight);
        paint.fillRect(rectVB, QBrush(Qt::black, Qt::SolidPattern));
        paint.fillRect(rectHB, QBrush(Qt::black, Qt::SolidPattern));
        paint.fillRect(rectV, QBrush(Qt::blue, Qt::SolidPattern));
        paint.fillRect(rectH, QBrush(Qt::blue, Qt::SolidPattern));

        if(*_showCenter)
        {
            QFont font=paint.font();
            resize ? font.setPointSize(7) : font.setPointSize(13);
            font.setBold(true);
            paint.setFont(font);
            paint.setPen(Qt::white);
            paint.drawText( resize ? drawx-12 : drawx-18, resize ? drawy+17 : drawy+30, QString::number((int)(val*10.0)/10.0) + "°C");
        }
    }
    if (_blackbody->show)
    {
        drawx = _blackbody->x - _skipX;// > 2 ? _maximum->x - 2 : _maximum->x;
        drawy = _blackbody->y - _skipY;// > 2 ? _maximum->y - 2 : _maximum->y;
        val = _blackbody->val;

        QFont font = paint.font();
        font.setPointSize(1);
        font.setBold(true);
        paint.setFont(font);
        paint.setPen("yellow");
        paint.drawEllipse(QPoint(drawx, drawy), 25, 25);
        resize ? font.setPointSize(7) : font.setPointSize(16);
        paint.setFont(font);
        paint.setPen(Qt::white);
        paint.drawText( resize ? drawx-12 : drawx-18, resize ? drawy+17 : drawy+30, QString::number((int)(val*10.0)/10.0) + "°C");
    }
    paint.end();
}
