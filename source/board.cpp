#include "board.h"
#include <memory>
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
        field[index] = std::make_unique<Cell>(0);
      else
        field[index] = std::make_unique<Cell>(index + 1);
    }
  srand(time(0));
  shuffle();
}

void
Board::shuffle() 
{
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
  return field[y * cols + x]->number;
}

int Board::get_rows() const { return rows; }
int Board::get_cols() const { return cols; }
int Board::get_size() const { return rows * cols; }

bool
Board::isFree(int x, int y) const
{
  if (x < 0 || x >= cols || y < 0 || y >= rows)
    return false;
  return !field[y * cols + x]->number;
}

bool
Board::isSolvable() const 
{
  std::vector<int> flat;
  flat.resize(rows * cols);
  int nullRow = 0;
  for (int i = 0; i < rows * cols; ++i) {
    if (!field[i]->number) {
      flat[i] = 0;
      nullRow = i / cols + 1;
    } else {
      flat[i] = field[i]->number;
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