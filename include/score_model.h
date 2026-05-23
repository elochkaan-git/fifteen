#pragma once
#include <QAbstractTableModel>
#include <qqml.h>
#include <memory>
#include "scoreboard.h"
#include "structs.h"

class ScoreboardModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit ScoreboardModel(QObject* parent = nullptr)
        : QAbstractTableModel(parent), sb(std::make_unique<Scoreboard>()) {}

    int rowCount(const QModelIndex& = {}) const override { return 16; }
    int columnCount(const QModelIndex& = {}) const override { return 5; }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (role != Qt::DisplayRole) return {};
        const GameResult& p = sb->get_results()[index.row()];
        if (p.name.empty()) return {};
        char buf[8];
        switch (index.column()) {
            case 0: return QString::fromStdString(p.name);
            case 1: snprintf(buf, sizeof(buf), "%dx%d", p.rows, p.cols); return QString(buf);
            case 2: return p.steps;
            case 3: return QString("%1:%2").arg(p.time/60,2,10,QChar('0')).arg(p.time%60,2,10,QChar('0'));
            case 4: return p.score;
        }
        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if (role != Qt::DisplayRole || orientation != Qt::Horizontal) return {};
        switch (section) {
        case 0: return "Имя";
        case 1: return "Поле";
        case 2: return "Ходы";
        case 3: return "Время";
        case 4: return "Счет";
        }
        return {};
    }

public slots:
    void onGameFinished(GameResult r) {
        beginResetModel();
        sb->add(r);
        sb->save();
        endResetModel();
    }

private:
    std::unique_ptr<Scoreboard> sb;
};
