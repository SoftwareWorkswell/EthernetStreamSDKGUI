#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqmlcontext.h>
#include <QMutex>

#include <src/graphic/streamframesprovider.h>
#include <src/graphic/streamframesprovidervisible.h>
#include <src/graphic/palettewrapper.h>
#include <src/graphic/palettevaluewrapper.h>
#include <src/graphic/controllercoreinterface.h>

QImage streamFrame {};
QImage streamFrameVisible {};

std::shared_ptr<QMutex> thermalMutex = std::make_shared<QMutex>();
std::shared_ptr<QMutex> visibleMutex = std::make_shared<QMutex>();
std::shared_ptr<QMutex> connectionMutex = std::make_shared<QMutex>();

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<PaletteWrapper>("Components", 1, 0, "PaletteWrapper");
    qmlRegisterType<PaletteValueWrapper>("Components", 1, 0, "PaletteValueWrapper");

    engine.rootContext()->setContextProperty("_controllerCore", new ControllerCoreInterface());
    engine.addImageProvider(QLatin1String("streamFramesProvider"), new StreamFramesProvider());
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
