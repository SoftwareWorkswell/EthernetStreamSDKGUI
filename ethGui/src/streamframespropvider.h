#ifndef STREAMFRAMESPROPVIDER_H
#define STREAMFRAMESPROPVIDER_H


#include <QQuickImageProvider>
#include <QDebug>

extern QImage *streamFrame;

class StreamFramesPropvider : public QQuickImageProvider
{
public:
    StreamFramesPropvider()
        : QQuickImageProvider(QQmlImageProviderBase::Image)
{
}

QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override
{
 //qDebug() << "requestImage";
 return *streamFrame;
}

};

#endif // STREAMFRAMESPROPVIDER_H
