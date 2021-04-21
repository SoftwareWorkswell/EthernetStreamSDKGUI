#ifndef STREAMFRAMESPROVIDERVISIBLE_H
#define STREAMFRAMESPROVIDERVISIBLE_H

#include <QDebug>
#include <QImage>
#include <QQuickImageProvider>

extern QImage streamFrameVisible;

class FramesProvider : public QQuickImageProvider {
public:
    FramesProvider() : QQuickImageProvider(QQmlImageProviderBase::Image) {}
    QImage requestImage(const QString &, QSize *, const QSize &) override { return streamFrameVisible; }
};

#endif // STREAMFRAMESPROVIDERVISIBLE_H
