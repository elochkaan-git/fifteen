#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "board_controller.h"
#include "score_model.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    BoardController bc;
    ScoreboardModel sbm;
    QObject::connect(&bc, &BoardController::gameFinished, &sbm, &ScoreboardModel::onGameFinished);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("board", &bc);
    engine.rootContext()->setContextProperty("scoreboardModel", &sbm);

    engine.loadFromModule("fifteen", "Main");
    return app.exec();
}
