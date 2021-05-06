#include "palettewrapper.h"

PaletteWrapper::PaletteWrapper(QObject * parent)
    : QObject(parent){}

PaletteWrapper::PaletteWrapper (const QString & x, QObject * parent)
    : QObject(parent), m_value(x){}

QString PaletteWrapper::value() const
{
    return m_value;
}
