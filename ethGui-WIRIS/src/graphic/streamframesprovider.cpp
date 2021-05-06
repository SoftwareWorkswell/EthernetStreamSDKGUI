#include "streamframesprovider.h"

StreamFramesProvider::StreamFramesProvider() : QQuickImageProvider(QQmlImageProviderBase::Image)
{}

QImage StreamFramesProvider::requestImage(const QString &, QSize *, const QSize &)
{
    return streamFrame;
}
