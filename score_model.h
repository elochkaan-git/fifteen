#pragma once
#include <QAbstractTableModel>
#include <qqml.h>
#include "scoreboard.h"

class ScoreboardModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit ScoreboardModel(Scoreboard* sb, QObject* parent = nullptr)
        : QAbstractTableModel(parent), sb(sb) {}

    int rowCount(const QModelIndex& = {}) const override { return 16; }
    int columnCount(const QModelIndex& = {}) const override { return 5; }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (role != Qt::DisplayRole) return {};
        const Player& p = sb->get_players()[index.row()];
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
        case 0: return "Name";
        case 1: return "Board";
        case 2: return "Steps";
        case 3: return "Time";
        case 4: return "Score";
        }
        return {};
    }

public slots:
    void onGameFinished(QString name, int rows, int cols, int steps, int time, int score) {
        sb->add({name.toStdString(), rows, cols, steps, time, score});
        sb->save();
    }

private:
    Scoreboard* sb;
};
