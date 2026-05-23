#pragma once
#include <QAbstractListModel>
#include <memory>
#include <QDateTime>
#include "game.h"
#include "game_creators.h"
#include "structs.h"

class BoardController : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int steps READ get_steps NOTIFY statsChanged)
    Q_PROPERTY(QString time READ get_time NOTIFY statsChanged)
    Q_PROPERTY(int status READ get_status NOTIFY statsChanged)

signals:
    void boardChanged();
    void statsChanged();
    void gameFinished(GameResult r);

public:
    enum Roles {
        ValueRole = Qt::UserRole + 1,
        IsNullRole
    };

    BoardController() : game(std::make_unique<ClassicGame>(4, 4)) {
        game->start();
    }

    int rowCount(const QModelIndex&) const override { return game->size(); }

    QHash<int, QByteArray> roleNames() const override {
        return {
            {ValueRole, "value"},
            {IsNullRole, "isNull"}
        };
    }

    QVariant data(const QModelIndex &index, int role) const override {
        int i = index.row() / game->cols();
        int j = index.row() % game->cols();
        int val = game->cell(j, i);
        if (role == ValueRole)  return val;
        if (role == IsNullRole) return val == 0;
        return QVariant();
    }

    Q_INVOKABLE void change_game(int type, int r = 4, int c = 4, int l = 5999) {
        beginResetModel();
        switch(type) {
            case 0: game = std::make_unique<ClassicGame>(3, 3); break;
            case 1: game = std::make_unique<ClassicGame>(4, 4); break;
            case 2: game = std::make_unique<TimeGame>(r, c, l); break;
            default: game = std::make_unique<ClassicGame>(4, 4); break;
        }
        game->start();
        endResetModel();
        emit statsChanged();
        emit boardChanged();
    }

    Q_INVOKABLE void move_cell(int dir) {
        game->move(static_cast<DIRECTION>(dir));
        emit dataChanged(index(0), index(game->size() - 1));
        emit boardChanged();
        emit statsChanged();
    }

    Q_INVOKABLE void shuffle() {
        game->shuffle();
        emit dataChanged(index(0), index(game->size() - 1));
        emit boardChanged();
        emit statsChanged();
    }

    Q_INVOKABLE void tick() { emit statsChanged(); }
    int get_status() const { return static_cast<int>(game->status()); }
    Q_INVOKABLE int get_cols() const { return game->cols(); }
    Q_INVOKABLE int get_steps() const { return game->steps(); }
    Q_INVOKABLE int get_elapsed() const { return game->get_elapsed_time(); }

    Q_INVOKABLE QString get_time() const {
        int elapsed = game->get_elapsed_time();
        return QString("%1:%2")
            .arg(elapsed / 60, 2, 10, QChar('0'))
            .arg(elapsed % 60, 2, 10, QChar('0'));
    }
    Q_INVOKABLE void save_result(const QString& name) {
        GameResult r = game->result();
        r.name = name.toStdString();
        emit gameFinished(r);
    }

private:
    std::unique_ptr<Game> game;
};
