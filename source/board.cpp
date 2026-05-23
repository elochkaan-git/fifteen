#include "board.h"
#include <utility>
#include <vector>
#include <cstdlib>
#include <ctime>

Board::Board(int n, int m) 
  : rows(n)
  , cols(m) 
{
  field.resize(n * m);
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j) {
      int index = i * cols + j;
      if (index + 1 == n * m)
        field[index] = 0;
      else
        field[index] = index + 1;
    }
  shuffle();
}

void
Board::shuffle()
{
  int blankIndex = -1;
  for (int i = 0; i < rows * cols; i++) {
    if (field[i] == 0) {
      blankIndex = i;
      break;
    }
  }

  int moves = 500;
  int prevMove = -1;

  for (int step = 0; step < moves; step++) {
    int r = blankIndex / cols;
    int c = blankIndex % cols;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    int opposite[] = {1, 0, 3, 2};

    std::vector<int> valid;
    for (int d = 0; d < 4; d++) {
      int nr = r + dr[d];
      int nc = c + dc[d];
      if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && d != prevMove)
        valid.push_back(d);
    }

    int chosen = valid[rand() % valid.size()];
    int nr = r + dr[chosen];
    int nc = c + dc[chosen];
    int neighborIndex = nr * cols + nc;

    std::swap(field[blankIndex], field[neighborIndex]);
    blankIndex = neighborIndex;
    prevMove = opposite[chosen];
  }
}

void
Board::swap(int x1, int y1, int x2, int y2)
{
  std::swap(field[y1 * cols + x1], field[y2 * cols + x2]);
}

int
Board::get_cell(int x, int y) const 
{
  if (x < 0 || x >= cols || y < 0 || y >= rows)
    return 0;
  return field[y * cols + x];
}

int Board::get_rows() const { return rows; }
int Board::get_cols() const { return cols; }
int Board::get_size() const { return rows * cols; }