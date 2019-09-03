#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqmlcontext.h>
#include <QMutex>

//#include <gst/gst.h>
//#include <glib.h>

#include "controllercore.h"
#include "streamframespropvider.h"
#include "streamframesprovidervisible.h"
#include "palettewrapper.h"
#include "palettevaluewrapper.h"

QImage *streamFrame = new QImage();
QImage *streamFrameVisible = new QImage();

QMutex *thermalMutex = new QMutex();
QMutex *visibleMutex = new QMutex();
QMutex *connectionMutex = new QMutex();


int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<PaletteWrapper>("Components", 1, 0, "PaletteWrapper");
    qmlRegisterType<PaletteValueWrapper>("Components", 1, 0, "PaletteValueWrapper");
    engine.rootContext()->setContextProperty("_controllerCore", new ControllerCore());
    engine.addImageProvider(QLatin1String("streamFramesProvider"), new StreamFramesPropvider());
    engine.addImageProvider(QLatin1String("streamFramesProviderVisible"), new StreamFramesProviderVisible());

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::quit, &QGuiApplication::quit);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
