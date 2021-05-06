#include "streamframesprovidervisible.h"

StreamFramesProviderVisible::StreamFramesProviderVisible() : QQuickImageProvider(QQmlImageProviderBase::Image)
{}

QImage StreamFramesProviderVisible::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    return streamFrameVisible;
}
