#ifndef PALETTEWRAPPER_H
#define PALETTEWRAPPER_H


#include <QObject>


class PaletteWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString value READ value CONSTANT)
public:
    PaletteWrapper (QObject * parent = nullptr);
    PaletteWrapper (const QString & x, QObject * parent = nullptr);
    QString value() const;
private:
    QString m_value;
};
#endif // PALETTEWRAPPER_H
