#pragma once
#include <QAbstractListModel>
#include <memory>
#include "board.h"
#include "board_factory.h"

class BoardController : public QAbstractListModel
{
    Q_OBJECT

signals:
    void boardChanged();

public:
    enum Roles {
        ValueRole = Qt::UserRole + 1,
        IsNullRole
    };

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

        if (role == ValueRole)
            return board->get_cell(j, i);
        if (role == IsNullRole)
            return !board->get_cell(j, i);

        return QVariant();
    }

    BoardController(BoardFactory* bf) {
        board = std::unique_ptr<Board>(bf->create());
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

        emit boardChanged();
    }

    Q_INVOKABLE void move_cell(int x, int y) {
        board->move_cell(x, y);
        emit dataChanged(index(0), index(board->get_size() - 1));
        emit boardChanged();
    }
    Q_INVOKABLE void move_cell(int dir) {
        board->move_cell(dir);
        emit dataChanged(index(0), index(board->get_size() - 1));
        emit boardChanged();
    }
    Q_INVOKABLE bool is_win() const { return board->is_win(); }
    Q_INVOKABLE void shuffle() {
        board->shuffle();
        emit dataChanged(index(0), index(board->get_size() - 1));
        emit boardChanged();
    }
    Q_INVOKABLE int get_cols() const { return board->get_cols(); }
private:
    std::unique_ptr<Board> board;
};
