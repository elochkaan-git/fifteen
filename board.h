#pragma once
#include <vector>
#include <memory>
#include "cell.h"
#include "cell_factory.h"
#include <cstdlib>
#include <ctime>

class Board
{
public:
    Board(int n, int m) : rows(n), cols(m) {
        field.resize(n * m);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j) {
                int index = i * cols + j;
                if (index + 1 == n * m)
                    field[index] = std::unique_ptr<Cell>(CellFactory::create(0));
                else
                    field[index] = std::unique_ptr<Cell>(CellFactory::create(index + 1));
            }
        srand(time(0));
        shuffle();
    }
    void move_cell(int x, int y) {
        int index = y * cols + x;
        if (field[index]->isNull())
            return;
        if (isFree(x, y-1))
            std::swap(field[index], field[index - cols]);
        else if (isFree(x, y+1))
            std::swap(field[index], field[index + cols]);
        else if (isFree(x-1, y))
            std::swap(field[index], field[index - 1]);
        else if (isFree(x+1, y))
            std::swap(field[index], field[index + 1]);
    }
    void move_cell(int dir) {
        int dx[] = { 0,  1,  0, -1};
        int dy[] = {-1,  0,  1,  0};

        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j) {
                int index = i * cols + j;
                if (field[index]->isNull()) {
                    int ni = i + dy[dir];
                    int nj = j + dx[dir];

                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                        std::swap(field[index], field[ni * cols + nj]);
                        return;
                    }
                }
            }
    }
    bool is_win() const {
        if (!field[rows * cols - 1]->isNull())
            return false;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int index = i * cols + j;
                if (index + 1 == rows * cols) continue;
                if (dynamic_cast<NumberCell*>(field[index].get())->getNumber() != index + 1)
                    return false;
            }
        }
        return true;
    }
    void shuffle() {
        do {
            for (int i = rows - 1; i >= 0; --i) {
                for (int j = cols - 1; j >= 0; --j) {
                    int index = i * cols + j;
                    int ri = rand() % (i + 1);
                    int rj = rand() % (j + 1);
                    std::swap(field[index], field[ri * cols + rj]);
                }
            }
        } while (!isSolvable());
    }
    int get_cell(int x, int y) const {
        if (x < 0 || x >= cols || y < 0 || y >= rows)
            return 0;
        if (field[y * cols + x]->isNull())
            return 0;
        return dynamic_cast<NumberCell*>(field[y * cols + x].get())->getNumber();
    }
    int get_size() const { return rows * cols; }
    int get_cols() const { return cols; }
    int get_rows() const { return rows; }

private:
    int rows, cols;
    std::vector<std::unique_ptr<Cell>> field;

    bool isFree(int x, int y) const {
        if (x < 0 || x >= cols || y < 0 || y >= rows)
            return false;
        return field[y * cols + x]->isNull();
    }
    bool isSolvable() const {
        std::vector<int> flat;
        flat.resize(rows * cols);
        int nullRow = 0;
        for (int i = 0; i < rows * cols; ++i) {
            if (field[i]->isNull()) {
                flat[i] = 0;
                nullRow = i / cols + 1;
            } else {
                flat[i] = dynamic_cast<NumberCell*>(field[i].get())->getNumber();
            }
        }

        int inversions = 0;
        for (int i = 0; i < rows * cols; ++i)
            for (int j = i+1; j < rows * cols; ++j)
                if (flat[i] && flat[j] && flat[i] > flat[j])
                    inversions++;

        if (!(cols % 2))
            return !((inversions + nullRow) % 2);
        return !(inversions % 2);
    }
};
