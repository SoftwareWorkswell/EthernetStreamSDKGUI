#include "translator.h"


Translator::Translator(QObject *parent) : QObject(parent) {
    m_Translator = new QTranslator();
    QFile file(_lastLanguageFileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        selectLanguage(file.readLine());
        file.close();
    }
}

QString Translator::emptyString() { return ""; }

void Translator::selectLanguage(QString language) {
    // load qm file from resources and install translator
    if (language == QString("CZECH")) {
        m_Translator->load("ts_cz", QLatin1String(":/translations/"));
        QLocale::setDefault(QLocale::Czech);
        bool tmp = qApp->installTranslator(m_Translator);
        qDebug() << "Translator installed: " << language << " " << tmp;
    } else // if(language == QString("en"))
    {
        qApp->removeTranslator(m_Translator);
    }
    saveLanguage(language);
    emit languageChanged();
}

void Translator::saveLanguage(QString &language) {
    QFile file(_lastLanguageFileName);
    if (file.exists()) {
        file.remove();
    }
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << language;
        file.close();
    }
}
