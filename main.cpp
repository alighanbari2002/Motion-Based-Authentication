#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "textboxhandler.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<TextBoxHandler>("com.textbox", 1, 0, "TextBoxHandler");
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
