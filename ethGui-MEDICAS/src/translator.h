#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QObject>
#include <QTranslator>

class Translator : public QObject
{
    Q_OBJECT
    // empty string is used to notify texts about language change
    Q_PROPERTY(QString emptyString READ emptyString NOTIFY languageChanged)

public:
    Translator(QObject * parent = nullptr);
    QString emptyString();
    Q_INVOKABLE void selectLanguage(QString language);
signals:
    void languageChanged();
private:
    QTranslator * m_Translator;
    const QString _lastLanguageFileName = "languageCache";
    void saveLanguage(QString &language);
};

#endif // TRANSLATOR_H
