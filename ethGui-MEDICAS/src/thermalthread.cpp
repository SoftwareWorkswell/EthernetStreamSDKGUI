#include "thermalthread.h"

void ThermalThread::run()
{
    this->setPriority(Priority::HighestPriority);
#ifdef _WIN32
    const std::string addr(("rtspsrc location=rtsp://" + this->_ssrc.toStdString() + ":8554/thermal latency=250 ! rtph264depay ! avdec_h264 ! videoconvert ! appsink"));
#else
    const std::string addr("rtspsrc location=rtsp://" + this->_ssrc.toStdString() + ":8554/thermal latency=300 caps = \"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\" ! rtph264depay ! decodebin ! videoconvert ! appsink");
#endif
    cv::VideoCapture cap(addr, cv::CAP_GSTREAMER);
    if(!cap.isOpened())
    {
        qDebug() << "Thermal: Input error";

        *_stream = false;
        return;
    }

    cv::Mat frame;
    qDebug() << "Thermal thread started";
    while(*_stream)
    {
        cap >> frame;
        if (frame.empty())
        {
            emit disconnected();
            qDebug() << "thermal: empty frame occurance, stopping stream!";
            break;
        }
        cv::cvtColor(frame, frame,cv::ColorConversionCodes::COLOR_BGR2RGB);
        // if(streamFrame)
        //    delete streamFrame;
        // copy data so qimage does not share data with cv::Mat
        static uchar *data = nullptr;
        if (!data) {
            data = new uchar[frame.rows * frame.step];
        }
        memcpy(data, frame.data, frame.rows * frame.step);
        streamFrame = QImage(data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888).copy();
        drawExtremes();
        drawRois();
        drawRoiArea();
        drawHead();
        if(*_stream)
            emit imageSourceChanged();
    }
    *_stream = false;
    cap.release();
    qDebug() << "Thermal thread finished";
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

void ThermalThread::drawRoiBorders(int x, int y, int w, int h, QPainter & paint)
{
    paint.drawLine(x, y, x + w, y);
    paint.drawLine(x, y, x, y + h);
    paint.drawLine(x + w, y, x + w, y + h);
    paint.drawLine(x, y + h, x + w, y + h);
}


void ThermalThread::drawRoiCorners(int x, int y, int w, int h, QPainter & paint)
{
    paint.drawLine(x, y, x, y + 5);
    paint.drawLine(x, y, x + 5, y);
    paint.drawLine(x, y + h, x + 5, y + h);
    paint.drawLine(x, y + h, x, y + h - 5);
    paint.drawLine(x + w, y, x + w - 5, y);
    paint.drawLine(x + w, y, x + w, y + 5);
    paint.drawLine(x + w, y + h, x + w - 5, y + h);
    paint.drawLine(x + w, y + h, x + w, y + h - 5);
}

void ThermalThread::drawSelectedRoiCorner(int x, int y, int w, int h, int corner, QPainter & paint)
{
    QPen oldPen = paint.pen();
    paint.setPen(Qt::green);
    switch(corner)
    {
        case 0:
            paint.drawLine(x, y, x, y + 5);
            paint.drawLine(x, y, x + 5, y);
            break;
        case 2:
            paint.drawLine(x, y + h, x + 5, y + h);
            paint.drawLine(x, y + h, x, y + h - 5);
            break;
        case 1:
            paint.drawLine(x + w, y, x + w - 5, y);
            paint.drawLine(x + w, y, x + w, y + 5);
            break;
        case 3:
            paint.drawLine(x + w, y + h, x + w - 5, y + h);
            paint.drawLine(x + w, y + h, x + w, y + h - 5);
            break;
        default:
            break;
    }
    paint.setPen(oldPen);
}

void ThermalThread::drawRois()
{
    QPainter paint;
    paint.begin(&streamFrame);
    paint.setPen(Qt::black);
    int x, y, w, h;
    if (_userRoiContainer->_userRoi1->show != 0 )
    {
        paint.setPen((_userRoiContainer->_currentRoi == 0) ? Qt::red : Qt::black);
        x = _userRoiContainer->_userRoi1->x;
        y = _userRoiContainer->_userRoi1->y;
        w = _userRoiContainer->_userRoi1->w;
        h = _userRoiContainer->_userRoi1->h;
        if(_userRoiContainer->_userRoi1->show == 1)
            drawRoiBorders(x, y, w, h, paint);
        else if(_userRoiContainer->_userRoi1->show == 2)
            drawRoiCorners(x, y, w, h, paint);
        if(_userRoiContainer->showNames && !_userRoiContainer->_userRoi1->name.isEmpty())
        {
            drawRoiName(x, y, _userRoiContainer->_userRoi1->name,paint);
        }
        if(_userRoiContainer->_currentRoi == 0)
            drawSelectedRoiCorner(x, y, w, h, _userRoiContainer->_userRoi1->selectedCorner, paint);
    }
    if (_userRoiContainer->_userRoi2->show != 0)
    {
        paint.setPen((_userRoiContainer->_currentRoi == 1) ? Qt::red : Qt::black);
        x = _userRoiContainer->_userRoi2->x;
        y = _userRoiContainer->_userRoi2->y;
        w = _userRoiContainer->_userRoi2->w;
        h = _userRoiContainer->_userRoi2->h;
        if(_userRoiContainer->_userRoi2->show == 1)
            drawRoiBorders(x, y, w, h, paint);
        else if(_userRoiContainer->_userRoi2->show == 2)
            drawRoiCorners(x, y, w, h, paint);
        if(_userRoiContainer->showNames && !_userRoiContainer->_userRoi2->name.isEmpty())
        {
            drawRoiName(x, y, _userRoiContainer->_userRoi2->name,paint);
        }
        if(_userRoiContainer->_currentRoi == 1)
            drawSelectedRoiCorner(x, y, w, h, _userRoiContainer->_userRoi2->selectedCorner, paint);
    }
    if (_userRoiContainer->_userRoi3->show != 0)
    {
        paint.setPen((_userRoiContainer->_currentRoi == 2) ? Qt::red : Qt::black);
        x = _userRoiContainer->_userRoi3->x;
        y = _userRoiContainer->_userRoi3->y;
        w = _userRoiContainer->_userRoi3->w;
        h = _userRoiContainer->_userRoi3->h;
        if(_userRoiContainer->_userRoi3->show == 1)
            drawRoiBorders(x, y, w, h, paint);
        else if(_userRoiContainer->_userRoi3->show == 2)
            drawRoiCorners(x, y, w, h, paint);
        if(_userRoiContainer->showNames && !_userRoiContainer->_userRoi3->name.isEmpty())
        {
            drawRoiName(x, y, _userRoiContainer->_userRoi3->name,paint);
        }
        if(_userRoiContainer->_currentRoi == 2)
            drawSelectedRoiCorner(x, y, w, h, _userRoiContainer->_userRoi3->selectedCorner, paint);
    }
    if (_userRoiContainer->_userRoi4->show != 0)
    {
        paint.setPen((_userRoiContainer->_currentRoi == 3) ? Qt::red : Qt::black);
        x = _userRoiContainer->_userRoi4->x;
        y = _userRoiContainer->_userRoi4->y;
        w = _userRoiContainer->_userRoi4->w;
        h = _userRoiContainer->_userRoi4->h;
        if(_userRoiContainer->_userRoi4->show == 1)
            drawRoiBorders(x, y, w, h, paint);
        else if(_userRoiContainer->_userRoi4->show == 2)
            drawRoiCorners(x, y, w, h, paint);
        if(_userRoiContainer->showNames && !_userRoiContainer->_userRoi4->name.isEmpty())
        {
            drawRoiName(x, y, _userRoiContainer->_userRoi4->name,paint);
        }
        if(_userRoiContainer->_currentRoi == 3)
            drawSelectedRoiCorner(x, y, w, h, _userRoiContainer->_userRoi4->selectedCorner, paint);
    }
    if(_maximum->show)
        drawRoiExtremes(paint);
    paint.end();

}

void ThermalThread::drawRoiExtremes(QPainter & paint)
{
    int drawx, drawy;
    double val;
    double size = 1;

    bool resize = size > 2;
    QMutexLocker locker(thermalMutex);

    int innerRectWidth = resize ? 1 : 2;
    int innerRectHeight = resize ? 6 : 12;
    int outerRectWidth = resize ? 2 : 3;
    int outerRectHeight = resize ? 8 : 14;
    int innerRectX= resize ? 3 : 5;
    int outerRectX = 1;
    int outerRectY = resize ? 3 : 6;

    if (_userRoiContainer->_userRoi1->show != 0)
    {
        drawx = _userRoiContainer->_userRoi1->max.x - _skipX;// > 2 ? _maximum->x - 2 : _maximum->x;
        drawy = _userRoiContainer->_userRoi1->max.y - _skipY;// > 2 ? _maximum->y - 2 : _maximum->y;
        val = _userRoiContainer->_userRoi1->max.val;
        QRect rectV(drawx-innerRectX, drawy, innerRectHeight, innerRectWidth);
        QRect rectH(drawx, drawy-innerRectX, innerRectWidth, innerRectHeight);
        QRect rectVB(drawx-outerRectY, drawy-outerRectX, outerRectHeight, outerRectWidth);
        QRect rectHB(drawx-outerRectX, drawy-outerRectY, outerRectWidth, outerRectHeight);
        paint.fillRect(rectVB, QBrush(Qt::black, Qt::SolidPattern));
        paint.fillRect(rectHB, QBrush(Qt::black, Qt::SolidPattern));
        paint.fillRect(rectV, QBrush(Qt::red, Qt::SolidPattern));
        paint.fillRect(rectH, QBrush(Qt::red, Qt::SolidPattern));

        QFont font=paint.font();
        resize ? font.setPointSize(5) : font.setPointSize(10);
        font.setBold(true);
        paint.setFont(font);
        paint.setPen(Qt::white);
        paint.drawText( resize ? drawx-10 : drawx-15, resize ? drawy+12 : drawy+22, QString::number((int)(val*10.0)/10.0) + *_units);
    }
    if (_userRoiContainer->_userRoi2->show != 0)
    {
        drawx = _userRoiContainer->_userRoi2->max.x - _skipX;// > 2 ? _maximum->x - 2 : _maximum->x;
        drawy = _userRoiContainer->_userRoi2->max.y - _skipY;// > 2 ? _maximum->y - 2 : _maximum->y;
        val = _userRoiContainer->_userRoi2->max.val;
        QRect rectV(drawx-innerRectX, drawy, innerRectHeight, innerRectWidth);
        QRect rectH(drawx, drawy-innerRectX, innerRectWidth, innerRectHeight);
        QRect rectVB(drawx-outerRectY, drawy-outerRectX, outerRectHeight, outerRectWidth);
        QRect rectHB(drawx-outerRectX, drawy-outerRectY, outerRectWidth, outerRectHeight);
        paint.fillRect(rectVB, QBrush(Qt::black, Qt::SolidPattern));
        paint.fillRect(rectHB, QBrush(Qt::black, Qt::SolidPattern));
        paint.fillRect(rectV, QBrush(Qt::red, Qt::SolidPattern));
        paint.fillRect(rectH, QBrush(Qt::red, Qt::SolidPattern));

        QFont font=paint.font();
        resize ? font.setPointSize(5) : font.setPointSize(10);
        font.setBold(true);
        paint.setFont(font);
        paint.setPen(Qt::white);
        paint.drawText( resize ? drawx-10 : drawx-15, resize ? drawy+12 : drawy+22, QString::number((int)(val*10.0)/10.0) + *_units);

    }
    if (_userRoiContainer->_userRoi3->show != 0)
    {
        drawx = _userRoiContainer->_userRoi3->max.x - _skipX;// > 2 ? _maximum->x - 2 : _maximum->x;
        drawy = _userRoiContainer->_userRoi3->max.y - _skipY;// > 2 ? _maximum->y - 2 : _maximum->y;
        val = _userRoiContainer->_userRoi3->max.val;
        QRect rectV(drawx-innerRectX, drawy, innerRectHeight, innerRectWidth);
        QRect rectH(drawx, drawy-innerRectX, innerRectWidth, innerRectHeight);
        QRect rectVB(drawx-outerRectY, drawy-outerRectX, outerRectHeight, outerRectWidth);
        QRect rectHB(drawx-outerRectX, drawy-outerRectY, outerRectWidth, outerRectHeight);
        paint.fillRect(rectVB, QBrush(Qt::black, Qt::SolidPattern));
        paint.fillRect(rectHB, QBrush(Qt::black, Qt::SolidPattern));
        paint.fillRect(rectV, QBrush(Qt::red, Qt::SolidPattern));
        paint.fillRect(rectH, QBrush(Qt::red, Qt::SolidPattern));

        QFont font=paint.font();
        resize ? font.setPointSize(5) : font.setPointSize(10);
        font.setBold(true);
        paint.setFont(font);
        paint.setPen(Qt::white);
        paint.drawText( resize ? drawx-10 : drawx-15, resize ? drawy+12 : drawy+22, QString::number((int)(val*10.0)/10.0) + *_units);

    }
    if (_userRoiContainer->_userRoi4->show != 0)
    {
        drawx = _userRoiContainer->_userRoi4->max.x - _skipX;// > 2 ? _maximum->x - 2 : _maximum->x;
        drawy = _userRoiContainer->_userRoi4->max.y - _skipY;// > 2 ? _maximum->y - 2 : _maximum->y;
        val = _userRoiContainer->_userRoi4->max.val;
        QRect rectV(drawx-innerRectX, drawy, innerRectHeight, innerRectWidth);
        QRect rectH(drawx, drawy-innerRectX, innerRectWidth, innerRectHeight);
        QRect rectVB(drawx-outerRectY, drawy-outerRectX, outerRectHeight, outerRectWidth);
        QRect rectHB(drawx-outerRectX, drawy-outerRectY, outerRectWidth, outerRectHeight);
        paint.fillRect(rectVB, QBrush(Qt::black, Qt::SolidPattern));
        paint.fillRect(rectHB, QBrush(Qt::black, Qt::SolidPattern));
        paint.fillRect(rectV, QBrush(Qt::red, Qt::SolidPattern));
        paint.fillRect(rectH, QBrush(Qt::red, Qt::SolidPattern));

        QFont font=paint.font();
        resize ? font.setPointSize(5) : font.setPointSize(10);
        font.setBold(true);
        paint.setFont(font);
        paint.setPen(Qt::white);
        paint.drawText( resize ? drawx-10 : drawx-15, resize ? drawy+12 : drawy+22, QString::number((int)(val*10.0)/10.0) + *_units);
    }
}

void ThermalThread::drawExtremes()
{
    QPainter paint;
    paint.begin(&streamFrame);
    int drawx, drawy;
    double val;
    double size = 1;

    bool resize = size > 2;
    QMutexLocker locker(thermalMutex);

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

        QFont font=paint.font();
        resize ? font.setPointSize(7) : font.setPointSize(13);
        font.setBold(true);
        paint.setFont(font);
        paint.setPen(Qt::white);
        paint.drawText( resize ? drawx-12 : drawx-18, resize ? drawy+17 : drawy+30, QString::number((int)(val*10.0)/10.0) + *_units);
    }

    if (_crossUser->show)
    {
        drawx = _crossUser->x - _skipX;// > 2 ? _maximum->x - 2 : _maximum->x;
        drawy = _crossUser->y - _skipY;// > 2 ? _maximum->y - 2 : _maximum->y;
        val = _crossUser->val;
        qDebug() << "drawing croos user" << drawx << drawy << val;
        QRect rectV(drawx-innerRectX, drawy, innerRectHeight, innerRectWidth);
        QRect rectH(drawx, drawy-innerRectX, innerRectWidth, innerRectHeight);
        QRect rectVB(drawx-outerRectY, drawy-outerRectX, outerRectHeight, outerRectWidth);
        QRect rectHB(drawx-outerRectX, drawy-outerRectY, outerRectWidth, outerRectHeight);
        paint.fillRect(rectVB, QBrush(Qt::black, Qt::SolidPattern));
        paint.fillRect(rectHB, QBrush(Qt::black, Qt::SolidPattern));
        paint.fillRect(rectV, QBrush(Qt::yellow, Qt::SolidPattern));
        paint.fillRect(rectH, QBrush(Qt::yellow, Qt::SolidPattern));

        QFont font=paint.font();
        resize ? font.setPointSize(7) : font.setPointSize(13);
        font.setBold(true);
        paint.setFont(font);
        paint.setPen(Qt::white);
        paint.drawText( resize ? drawx-12 : drawx-18, resize ? drawy+17 : drawy+30, QString::number((int)(val*10.0)/10.0) + *_units);
    }

    if (_blackbody->show)
    {
        drawx = _blackbody->x - _skipX; // > 2 ? _maximum->x - 2 : _maximum->x;
        drawy = _blackbody->y - _skipY; // > 2 ? _maximum->y - 2 : _maximum->y;
        val = _blackbody->val;

        double coef = _vals.supportedBlackbodyMaskSizeCoeffs.at(*_blackbodyMaskSize);
        double radius = _vals.supportedBlackbodySizeCoeffs.at(*_blackbodySize);

        double scale = (double) _blackbody->scale / 100;
        if(scale < 0.7)
            scale = 0.7;
        if(scale > 1.3)
            scale = 1.3;

        switch (*_blackbodySize){
        case 0:
            coef = coef*1.7;//(coef-1)*3+1;
            break;
        case 1:
            coef = coef*1.7;
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            radius = 1;
            break;
        }
        coef = coef+1;

        double maskRadius = scale*coef*radius+0.9;
        //double maskRadiusSq =  maskRadius*maskRadius;
        radius = scale*radius + 0.9;
        //double bbRadiusSq =  radius*radius;



        QFont font = paint.font();
        font.setPointSize(1);
        font.setBold(true);
        paint.setFont(font);
        paint.setPen("yellow");
        paint.drawEllipse(QPoint(drawx, drawy), (int)radius, (int)radius);
        paint.drawEllipse(QPoint(drawx, drawy), (int)maskRadius, (int)maskRadius);
        resize ? font.setPointSize(7) : font.setPointSize(16);
        paint.setFont(font);
        paint.setPen(Qt::white);
        paint.drawText( resize ? drawx-12 : drawx-18, resize ? drawy+12 : drawy+40, QString::number((int)(val*10.0)/10.0) + *_units);
    }
    paint.end();
}

void ThermalThread::drawRoiArea()
{
    // draw only if one of rois is edited
    if(_userRoiContainer->_currentRoi != -1)
    {
        QPainter paint;
        paint.begin(&streamFrame);
        paint.setPen(QPen(Qt::black, 2, Qt::DashLine));
        QMutexLocker locker(thermalMutex);

        int width = streamFrame.width();
        int height = streamFrame.height();
        // borders 4% of size
        int borderX = width / 25;
        int borderY = height / 25;
        // rounded rect with radius 50 pixels
        paint.drawRoundedRect(borderX, borderY, width - (2 * borderX), height - (2 * borderY), 50, 50);
    }
}

void ThermalThread::drawRoiName(int x, int y, QString &text, QPainter & paint)
{
    QMutexLocker locker(thermalMutex);
    QFont font = paint.font();
    font.setPointSize(9);
    font.setBold(true);
    paint.setFont(font);
    paint.drawText(x+3, y-1, text);
}

void ThermalThread::drawHead()
{
    if(_head->show)
    {
        QMutexLocker locker(thermalMutex);
        QPainter paint;
        paint.begin(&streamFrame);
        paint.setPen(QPen(Qt::black, 2));
        paint.drawEllipse(QPoint(_head->x, 256 + _head->y), (int)sqrt(7329), (int)sqrt(14410));
    }
}
