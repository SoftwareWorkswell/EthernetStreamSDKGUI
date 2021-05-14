#include "thermalthread.h"
#include <QDebug>

void ThermalThread::run()
{
    msleep(1000);
    while(!(*_cooledDown) && *(_stream))
    {
        streamFrame = QImage(640, 480, QImage::Format_RGB888);
        streamFrame.fill(QColor("black"));
        QPainter p(&streamFrame);
        p.setPen(QColor("White"));
        QFont font  = p.font();
        font.setPixelSize(20);
        p.setFont(font);
        if(std::stoi(_cooldownTime->toStdString()) == -1)
            p.drawText(50, 240, "Please switch the thermal camera button to ON position.");
        else
        {
            int sec = std::stoi(_cooldownTime->toStdString());
            p.drawText(90, 240, "Camera is cooling down. Please Wait " + QString::number(sec / 60) + " min " + QString::number(sec % 60) + " sec.");
            p.drawText(90, 260, "Keep the cap mounted, camera will perform");
            p.drawText(90, 280, "shutter calibration after cooldown.");
        }
        emit imageSourceChanged();
        msleep(200);
    }
    if(!(*_stream))
        return;
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
        if(!*(_stream) || ! cap.read(frame) || frame.empty())
        {
            qDebug() << "thermal: empty frame occurance, stopping stream!";
            break;
        }
        cv::cvtColor(frame, frame,cv::ColorConversionCodes::COLOR_BGR2RGB);
        zoomImage(frame);
        // copy data so qimage does not share data with cv::Mat
        static uchar *data = nullptr;
        if (!data) {
            data = new uchar[frame.rows * frame.step];
        }
        memcpy(data, frame.data, frame.rows * frame.step);
        if(*(_type) == _vals.GIS_TYPE)
            streamFrame = QImage(data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888).scaled(frame.cols *2, frame.rows *2);
        else
            streamFrame = QImage(data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        if(*_type == _vals.GIS_TYPE)
            drawGisExtremes();
        else
            drawWirisExtremes();
        if(*_stream)
            emit imageSourceChanged();
    }
    cap.release();
    *_stream = false;
}

void ThermalThread::zoomImage(cv::Mat& mat)
{
    QMutexLocker lock(thermalMutex.get());
    QString cache = "1";
    if (_zoom != nullptr)
        cache = **_zoom;
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


void ThermalThread::drawWirisExtremes()
{
    QPainter paint;
    int drawx, drawy;
    double val;
    double size = 1;

    thermalMutex->lock();
    if (_zoom != nullptr)
        size = (**_zoom).toDouble();
    thermalMutex->unlock();

    bool resize = size > 2;

    int innerRectWidth = resize ? 1 : 2;
    int innerRectHeight = resize ? 9 : 16;
    int outerRectWidth = resize ? 3 : 4;
    int outerRectHeight = resize ? 11 : 18;
    int innerRectX= resize ? 4 : 7;
    int outerRectX = 1;
    int outerRectY = resize ? 5 : 8;

    if (*_showCenter)
    {
        drawx = _center->x - _skipX;// > 2 ? _maximum->x - 2 : _maximum->x;
        drawy = _center->y - _skipY;// > 2 ? _maximum->y - 2 : _maximum->y;
        val = _center->val;
        paint.begin(&streamFrame);
        QRect rectV( resize ? streamFrame.width()/2-4 : streamFrame.width()/2-8, streamFrame.height()/2, resize ? 9 : 17, resize ? 1 : 2);
        QRect rectH(streamFrame.width()/2, resize ? streamFrame.height()/2-4 : streamFrame.height()/2-8, resize ? 1 : 2, resize ? 9 : 17);
        paint.fillRect(rectV, QBrush(Qt::white, Qt::SolidPattern));
        paint.fillRect(rectH, QBrush(Qt::white, Qt::SolidPattern));
        if(*_showVals)
        {
            QFont font=paint.font();
            resize ? font.setPointSize(7) : font.setPointSize(13);
            font.setBold(true);
            paint.setFont(font);
            paint.setPen(Qt::white);
            paint.drawText( resize ? drawx-12 : drawx-18, resize ? drawy+17 : drawy+30, QString::number((int)(val*10.0)/10.0) + *_unitSign);
        }
        paint.end();
    }

    if (_maximum->show)
    {
        drawx = _maximum->x - _skipX;// > 2 ? _maximum->x - 2 : _maximum->x;
        drawy = _maximum->y - _skipY;// > 2 ? _maximum->y - 2 : _maximum->y;
        val = _maximum->val;
        paint.begin(&streamFrame);
        QRect rectV(drawx-innerRectX, drawy, innerRectHeight, innerRectWidth);
        QRect rectH(drawx, drawy-innerRectX, innerRectWidth, innerRectHeight);
        QRect rectVB(drawx-outerRectY, drawy-outerRectX, outerRectHeight, outerRectWidth);
        QRect rectHB(drawx-outerRectX, drawy-outerRectY, outerRectWidth, outerRectHeight);
        paint.fillRect(rectVB, QBrush(Qt::black, Qt::SolidPattern));
        paint.fillRect(rectHB, QBrush(Qt::black, Qt::SolidPattern));
        paint.fillRect(rectV, QBrush(Qt::red, Qt::SolidPattern));
        paint.fillRect(rectH, QBrush(Qt::red, Qt::SolidPattern));
        if(*_showVals)
        {
            QFont font=paint.font();
            resize ? font.setPointSize(7) : font.setPointSize(13);
            font.setBold(true);
            paint.setFont(font);
            paint.setPen(Qt::white);
            paint.drawText( resize ? drawx-12 : drawx-18, resize ? drawy+17 : drawy+30, QString::number((int)(val*10.0)/10.0) + *_unitSign);
        }
        paint.end();
    }

    if (_minimum->show)
    {
        drawx = _minimum->x - _skipX;// > 2 ? _maximum->x - 2 : _maximum->x;
        drawy = _minimum->y - _skipY;// > 2 ? _maximum->y - 2 : _maximum->y;
        val = _minimum->val;
        paint.begin(&streamFrame);
        QRect rectV(drawx-innerRectX, drawy, innerRectHeight, innerRectWidth);
        QRect rectH(drawx, drawy-innerRectX, innerRectWidth, innerRectHeight);
        QRect rectVB(drawx-outerRectY, drawy-outerRectX, outerRectHeight, outerRectWidth);
        QRect rectHB(drawx-outerRectX, drawy-outerRectY, outerRectWidth, outerRectHeight);
        paint.fillRect(rectVB, QBrush(Qt::black, Qt::SolidPattern));
        paint.fillRect(rectHB, QBrush(Qt::black, Qt::SolidPattern));
        paint.fillRect(rectV, QBrush(Qt::blue, Qt::SolidPattern));
        paint.fillRect(rectH, QBrush(Qt::blue, Qt::SolidPattern));

        if(*_showVals && *_type != _vals.WIRIS_SECURITY_TYPE)
        {
            QFont font=paint.font();
            resize ? font.setPointSize(7) : font.setPointSize(13);
            font.setBold(true);
            paint.setFont(font);
            paint.setPen(Qt::white);
            paint.drawText( resize ? drawx-12 : drawx-18, resize ? drawy+17 : drawy+30, QString::number((int)(val*10.0)/10.0) + *_unitSign);
        }
        paint.end();
    }
}


void ThermalThread::drawGisExtremes()
{
    QPainter paint;
    int drawx, drawy;
    double val;
    double size = 1;

    thermalMutex->lock();
    if (_zoom != nullptr)
        size = (**_zoom).toDouble();
    thermalMutex->unlock();

    bool resize = size > 2;
    int outerRectSz = resize ? 25 : 35;
    if (*_showCenter)
    {
        drawx = _center->x*2 - outerRectSz/2; // > 2 ? _maximum->x - 2 : _maximum->x;
        drawy = _center->y*2 - outerRectSz/2;// > 2 ? _maximum->y - 2 : _maximum->y;
        val = _center->val;
        paint.begin(&streamFrame);
        paint.drawImage(QRect(drawx, drawy, outerRectSz, outerRectSz), QImage(":/img/png/cross_center.png"));
        if(*_showVals)
        {
            int offX = resize ? 0 : 4;
            int offY = resize ? 30: 50;
            QFont font=paint.font();
            resize ? font.setPointSize(4) : font.setPointSize(8);
            font.setBold(true);
            paint.setFont(font);
            paint.setPen(Qt::white);
            paint.drawText(drawx + offX, drawy + offY, QString::number((int)(val*10.0)/10.0) + *_unitSign);
        }
        paint.end();
    }
    if (_maximum->show)
    {
        int offX = resize ? 0 : 4;
        int offY = resize ? 30: 50;
        drawx = _maximum->x*2 - outerRectSz/2;// > 2 ? _maximum->x - 2 : _maximum->x;
        drawy = _maximum->y*2 - outerRectSz/2;// > 2 ? _maximum->y - 2 : _maximum->y;
        val = _maximum->val;
        paint.begin(&streamFrame);
        paint.drawImage(QRect(drawx, drawy, outerRectSz, outerRectSz), QImage(":/img/png/cross_max.png"));

        if(*_showVals)
        {
            QFont font=paint.font();
            resize ? font.setPointSize(4) : font.setPointSize(8);
            font.setBold(true);
            paint.setFont(font);
            paint.setPen(Qt::white);
            if(_maximum->y*2 > streamFrame.height()/2)
                offY = resize ? -3: -5;
            if(_maximum->x*2 < streamFrame.height()/8)
                offX = resize ? 15: 25;
            if(_maximum->x*2 > streamFrame.height()/8*7)
                offX = resize ? -15: -25;
            paint.drawText(drawx + offX, drawy + offY , QString::number((int)(val*10.0)/10.0) + *_unitSign);
        }
        paint.end();
    }

    if (_minimum->show)
    {
        int offX = resize ? 0 : 4;
        int offY = resize ? 30: 50;
        drawx = _minimum->x*2 - outerRectSz/2;// > 2 ? _maximum->x - 2 : _maximum->x;
        drawy = _minimum->y*2 - outerRectSz/2;// > 2 ? _maximum->y - 2 : _maximum->y;
        val = _minimum->val;
        paint.begin(&streamFrame);
        paint.drawImage(QRect(drawx, drawy, outerRectSz, outerRectSz), QImage(":/img/png/cross_min.png"));
        if(*_showVals && *_type != _vals.WIRIS_SECURITY_TYPE)
        {
            QFont font=paint.font();
            resize ? font.setPointSize(4) : font.setPointSize(8);
            font.setBold(true);
            paint.setFont(font);
            paint.setPen(Qt::white);
            if(_minimum->y*2 > streamFrame.height()/2)
                offY = resize ? -3: -5;
            if(_minimum->x*2 < streamFrame.height()/8)
                offX = resize ?  15: 25;
            if(_minimum->x*2 > streamFrame.height()/8*7)
                offX = resize ? -15: -25;
            paint.drawText( drawx + offX, drawy + offY, QString::number((int)(val*10.0)/10.0) + *_unitSign);
        }
        paint.end();
    }
}

