#pragma once
#include <vector>

class Board
{
public:
    Board(int n, int m);
    void shuffle();
    void swap(int x1, int y1, int x2, int y2);
    int get_cell(int x, int y) const;
    int get_size() const;
    int get_cols() const;
    int get_rows() const;

private:
    int rows, cols;
    std::vector<int> field;
};
