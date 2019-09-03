#ifndef STREAMFRAMESPROVIDERVISIBLE_H
#define STREAMFRAMESPROVIDERVISIBLE_H


#include <QQuickImageProvider>
#include <QDebug>

extern QImage *streamFrameVisible;


class StreamFramesProviderVisible : public QQuickImageProvider
{
public:
    StreamFramesProviderVisible()
        : QQuickImageProvider(QQmlImageProviderBase::Image)
{
}

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override
    {
     //qDebug() << "requestImageVisible";
     return *streamFrameVisible;
    }
};

#endif // STREAMFRAMESPROVIDERVISIBLE_H
