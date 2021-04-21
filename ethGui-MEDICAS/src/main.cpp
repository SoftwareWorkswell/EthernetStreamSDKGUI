#include <QDate>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QImage>
#include <QMutex>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTextStream>
#include <QTime>

#include "controllercore.h"
#include "streamframespropvider.h"
#include "streamframesprovidervisible.h"
#include "palettewrapper.h"
#include "palettevaluewrapper.h"
#include "usersettings.h"
#include "translator.h"

QImage streamFrame{};
QImage streamFrameVisible{};

QMutex *thermalMutex = new QMutex();
QMutex *visibleMutex = new QMutex();
QMutex *connectionMutex = new QMutex();

QTextStream *logOutput;

// redirects qdebug, qinfo... into log file
void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

// creates new log and initializes logOutput
QFile *createLog();

// clean logs older than 1 week
void cleanupLogs();

int main(int argc, char *argv[])
{
    // set exception handler - segv
    std::set_terminate( []()
    {
        qDebug() << "Unhandled exception\n";
        std::exit(EXIT_FAILURE);
    }
    );

    QFile *logFile = createLog();
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QCoreApplication::setApplicationVersion(APP_VERSION);
    QCoreApplication::setOrganizationName("Workswell");
    QCoreApplication::setOrganizationDomain("www.workswell.cz");
    QCoreApplication::setApplicationName(APP_NAME);

    // register custom types
    qmlRegisterType<PaletteWrapper>("Components", 1, 0, "PaletteWrapper");
    qmlRegisterType<PaletteValueWrapper>("Components", 1, 0, "PaletteValueWrapper");

    // message handler for logging
    qInstallMessageHandler(messageOutput); // Install the handler

    engine.addImageProvider(QLatin1String("streamFramesProvider"), new StreamFramesPropvider());
    engine.addImageProvider(QLatin1String("streamFramesProviderVisible"), new FramesProvider());
    engine.rootContext()->setContextProperty("_translator", new Translator());

    // create core instance which handles communication with commands defined in protocol.h
    ControllerCore *core = new ControllerCore();
    engine.rootContext()->setContextProperty("_controllerCore", core);

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::quit, &QGuiApplication::quit);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    bool returnValue = app.exec();

    logOutput->flush();
    logFile->close();
    return returnValue;
}

void messageOutput(QtMsgType type, const QMessageLogContext &, const QString &msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    *logOutput << QTime::currentTime().toString() << " ";
    switch (type) {
    case QtDebugMsg:
        *logOutput << "Debug: " << msg << "\n";
        break;
    case QtInfoMsg:
        *logOutput << "Info: " << msg << "\n";
        break;
    case QtWarningMsg:
        *logOutput << "Warning: " << msg << "\n";
        break;
    case QtCriticalMsg:
        *logOutput << "Critical: " << msg << "\n";
        break;
    case QtFatalMsg:
        *logOutput << "Fatal: " << msg << "\n";
        abort();
        break;
    }
}


QFile *createLog() {
    cleanupLogs();
    // create new log file with name containing date and time of application start
    QDate currentDate = QDate::currentDate();
    QTime currentTime = QTime::currentTime();
    QString dateString = QString::number(currentDate.year()) + "-" + QString::number(currentDate.month()) + "-" + QString::number(currentDate.day());
    QString timeString = QString::number(currentTime.hour()) + "-" + QString::number(currentTime.minute()) + "-" + QString::number(currentTime.second());
    // log is created in folder logs
    QDir logDir = QDir::currentPath() + "/logs/";
    if (!logDir.exists()) {
        logDir.mkpath(logDir.absolutePath());
    }
    // log file name will be in format: log_YYYY-MM-DD_hh-mm-ss.txt
    QString logName = "log_" + dateString + "_" + timeString + ".txt";
    logDir = QDir(QDir::currentPath() + "/logs/" + logName);
    QFile *logFile = new QFile(logDir.absolutePath());
    logFile->open(QFile::WriteOnly);
    logOutput = new QTextStream(logFile);
    return logFile;
}

void cleanupLogs() {
    QDate thisDay = QDate::currentDate();
    QDir logDir = QDir::currentPath() + "/logs/";
    // locate all files in directory logs
    QStringList allLogFiles = logDir.entryList(QDir::Files);
    for (QString logFile : allLogFiles) {
        // parse date of creation from name (log_YYYY-MM-DD_hh-mm-ss.txt)
        QList<QString> datePartStrings = logFile.split("_")[1].split("-");
        int year = datePartStrings[0].toInt();
        int month = datePartStrings[1].toInt();
        int day = datePartStrings[2].toInt();
        QDate logDate(year, month, day);
        // delete log if it's older than KEEP_LOG_DAYS
        if (logDate.daysTo(thisDay) > KEEP_LOG_DAYS) {
            logDir.remove(logFile);
        }
    }
}
