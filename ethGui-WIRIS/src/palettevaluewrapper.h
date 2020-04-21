#ifndef PALETTEVALUEWRAPPER_H
#define PALETTEVALUEWRAPPER_H


#include <QObject>


class PaletteValueWrapper : public QObject
{
        Q_OBJECT
        Q_PROPERTY(QString r READ r CONSTANT)
        Q_PROPERTY(QString g READ g CONSTANT)
        Q_PROPERTY(QString b READ b CONSTANT)
public:
    PaletteValueWrapper (QObject * parent = nullptr){}
    PaletteValueWrapper (const QString & r, const QString & g, const QString & b, QObject * parent = nullptr) : QObject(parent), m_r(r), m_g(g), m_b(b){}

    QString r() const
    {
        return m_r;
    }
    QString g() const
    {
        return m_g;
    }
    QString b() const
    {
        return m_b;
    }

private:
        QString m_r;
        QString m_g;
        QString m_b;
};

#endif // PALETTEVALUEWRAPPER_H
