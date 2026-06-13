#include "board.h"
#include <cassert>

int main() {
  Board board(3, 10);

  assert(board.get_rows() == 3);
  assert(board.get_cols() == 10);
}
