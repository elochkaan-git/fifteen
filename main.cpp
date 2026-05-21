#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "board_controller.h"
#include "board_factory.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    ClassicBoardFactory cbf;
    BoardController bc(&cbf);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("board", &bc);

    engine.loadFromModule("fifteen", "Main");
    return app.exec();
}
