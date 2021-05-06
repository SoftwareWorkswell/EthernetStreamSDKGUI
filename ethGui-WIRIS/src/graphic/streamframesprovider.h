#ifndef STREAMFRAMESPROPVIDER_H
#define STREAMFRAMESPROPVIDER_H

#include <QQuickImageProvider>

extern QImage streamFrame;

class StreamFramesProvider : public QQuickImageProvider
{
public:
    StreamFramesProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
};

#endif // STREAMFRAMESPROPVIDER_H
