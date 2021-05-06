#ifndef STREAMFRAMESPROVIDERVISIBLE_H
#define STREAMFRAMESPROVIDERVISIBLE_H


#include <QQuickImageProvider>

extern QImage streamFrameVisible;


class StreamFramesProviderVisible : public QQuickImageProvider
{
public:
    StreamFramesProviderVisible();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
};

#endif // STREAMFRAMESPROVIDERVISIBLE_H
