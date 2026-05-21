#pragma once
#include <QAbstractListModel>
#include <QDateTime>
#include <memory>
#include "board.h"
#include "board_factory.h"

class BoardController : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int steps READ get_steps NOTIFY statsChanged)
    Q_PROPERTY(QString time READ get_time NOTIFY statsChanged)

signals:
    void boardChanged();
    void statsChanged();
    void gameFinished(QString name, int rows, int cols, int steps, int time, int score);

public:
    enum Roles {
        ValueRole = Qt::UserRole + 1,
        IsNullRole
    };

    BoardController(BoardFactory* bf) {
        board = std::unique_ptr<Board>(bf->create());
        startTime = QDateTime::currentDateTime();
    }

    int rowCount(const QModelIndex&) const override { return board->get_size(); }

    QHash<int, QByteArray> roleNames() const override {
        return {
            {ValueRole, "value"},
            {IsNullRole, "isNull"}
        };
    }

    QVariant data(const QModelIndex &index, int role) const override {
        int i = index.row() / board->get_cols();
        int j = index.row() % board->get_cols();
        if (role == ValueRole)  return board->get_cell(j, i);
        if (role == IsNullRole) return !board->get_cell(j, i);
        return QVariant();
    }

    Q_INVOKABLE void change_factory(int type, int rows = 0, int cols = 0) {
        std::unique_ptr<BoardFactory> factory;
        switch(type) {
        case 0: factory = std::make_unique<SmallBoardFactory>(); break;
        case 1: factory = std::make_unique<ClassicBoardFactory>(); break;
        case 2: factory = std::make_unique<LargeBoardFactory>(); break;
        case 3: factory = std::make_unique<RectangleBoardFactory>(); break;
        case 4: factory = std::make_unique<CustomBoardFactory>(rows, cols); break;
        default: factory = std::make_unique<ClassicBoardFactory>(); break;
        }
        beginResetModel();
        board = std::unique_ptr<Board>(factory->create());
        endResetModel();

        steps = 0;
        startTime = QDateTime::currentDateTime();
        emit statsChanged();
        emit boardChanged();
    }

    Q_INVOKABLE void move_cell(int x, int y) {
        board->move_cell(x, y);
        ++steps;
        emit dataChanged(index(0), index(board->get_size() - 1));
        emit boardChanged();
        emit statsChanged();
    }
    Q_INVOKABLE void move_cell(int dir) {
        board->move_cell(dir);
        ++steps;
        emit dataChanged(index(0), index(board->get_size() - 1));
        emit boardChanged();
        emit statsChanged();
    }
    Q_INVOKABLE void shuffle() {
        board->shuffle();
        steps = 0;
        startTime = QDateTime::currentDateTime();
        emit dataChanged(index(0), index(board->get_size() - 1));
        emit boardChanged();
        emit statsChanged();
    }

    Q_INVOKABLE void tick() { emit statsChanged(); }
    Q_INVOKABLE bool is_win() const { return board->is_win(); }
    Q_INVOKABLE int get_cols() const { return board->get_cols(); }
    Q_INVOKABLE int get_steps() const { return steps; }
    Q_INVOKABLE int get_elapsed() const { return startTime.secsTo(QDateTime::currentDateTime()); }
    Q_INVOKABLE QString get_time() const {
        int elapsed = get_elapsed();
        return QString("%1:%2")
            .arg(elapsed / 60, 2, 10, QChar('0'))
            .arg(elapsed % 60, 2, 10, QChar('0'));
    }
    Q_INVOKABLE void save_result(const QString& name) {
        int elapsed = get_elapsed();
        int rows = board->get_rows();
        int cols = board->get_cols();
        int sc = std::max(0, rows * cols * 200 - (steps * 10 + elapsed * 2));
        emit gameFinished(name, rows, cols, steps, elapsed, sc);
    }

private:
    std::unique_ptr<Board> board;
    QDateTime startTime;
    int steps = 0;
};
