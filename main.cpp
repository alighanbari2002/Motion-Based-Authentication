#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "DataReadingHandler.h"
#include <QSensorFilter>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<DataReadingHandler>("com.DataReadingHandler.h", 1, 0, "DataReadingHandler");
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/QtSensorAuth/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
