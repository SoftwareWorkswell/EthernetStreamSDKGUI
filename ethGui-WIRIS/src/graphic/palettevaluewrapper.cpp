#include "palettevaluewrapper.h"

PaletteValueWrapper::PaletteValueWrapper(QObject * parent)
    :QObject(parent)
{}

PaletteValueWrapper::PaletteValueWrapper(const QString & r, const QString & g, const QString & b, QObject * parent) : QObject(parent), m_r(r), m_g(g), m_b(b){}

QString PaletteValueWrapper::r() const
{
    return m_r;
}

QString PaletteValueWrapper::g() const
{
    return m_g;
}

QString PaletteValueWrapper::b() const
{
    return m_b;
}
