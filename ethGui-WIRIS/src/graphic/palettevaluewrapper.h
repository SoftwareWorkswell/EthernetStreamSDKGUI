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
    PaletteValueWrapper(QObject * parent = nullptr);
    PaletteValueWrapper(const QString & r, const QString & g, const QString & b, QObject * parent = nullptr);

    QString r() const;
    QString g() const;
    QString b() const;
private:
    QString m_r;
    QString m_g;
    QString m_b;
};

#endif // PALETTEVALUEWRAPPER_H
