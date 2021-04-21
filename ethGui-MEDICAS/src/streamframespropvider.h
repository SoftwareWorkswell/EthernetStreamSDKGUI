#ifndef STREAMFRAMESPROPVIDER_H
#define STREAMFRAMESPROPVIDER_H

#include <QDebug>
#include <QImage>
#include <QQuickImageProvider>

extern QImage streamFrame;

class StreamFramesPropvider : public QQuickImageProvider
{
public:
    StreamFramesPropvider()
        : QQuickImageProvider(QQmlImageProviderBase::Image)
    {
    }
    QImage requestImage(const QString &, QSize *, const QSize &) override { return streamFrame; }
};

#endif // STREAMFRAMESPROPVIDER_H
