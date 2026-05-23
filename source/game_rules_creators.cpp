#include "game_rules_creators.h"
#include "structs.h"
#include <ctime>

std::pair<int, int>
find_null(Board* board)
{
  int rows = board->get_rows();
  int cols = board->get_cols();

  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      if (!board->get_cell(j, i))
        return {j, i};
  return {0, 0};
}

bool
ClassicGameRules::can_move(Board* board, DIRECTION dir)
{
  int dx[] = { 0,  1,  0, -1};
  int dy[] = {-1,  0,  1,  0};
  std::pair<int, int> null_cell = find_null(board);

  int nj = null_cell.first + dx[dir];
  int ni = null_cell.second + dy[dir];

  if (ni >= 0 && ni < board->get_rows() && nj >= 0 && nj < board->get_cols())
    return 1;
  return 0;
}

GameStatus
ClassicGameRules::status(const GameState* gs)
{
  int rows = gs->board->get_rows(), cols = gs->board->get_cols();
  if (gs->board->get_cell(cols-1, rows-1))
    return GameStatus::Playing;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      int index = i * cols + j;
      if (index + 1 == rows * cols) continue;
      if (gs->board->get_cell(j, i) != index + 1)
        return GameStatus::Playing;
    }
  }
  return GameStatus::Win;
}

void
ClassicGameRules::move(Board* board, DIRECTION dir)
{
  int dx[] = { 0,  1,  0, -1};
  int dy[] = {-1,  0,  1,  0};
  std::pair<int, int> null_cell = find_null(board);

  int nj = null_cell.first + dx[dir];
  int ni = null_cell.second + dy[dir];
  board->swap(null_cell.first, null_cell.second, nj, ni);
}

TimeGameRules::TimeGameRules(time_t limit) : limit(limit) {}

bool
TimeGameRules::can_move(Board* board, DIRECTION dir)
{
  int dx[] = { 0,  1,  0, -1};
  int dy[] = {-1,  0,  1,  0};
  std::pair<int, int> null_cell = find_null(board);

  int nj = null_cell.first + dx[dir];
  int ni = null_cell.second + dy[dir];

  if (ni >= 0 && ni < board->get_rows() && nj >= 0 && nj < board->get_cols())
    return 1;
  return 0;
}

GameStatus
TimeGameRules::status(const GameState* gs)
{ 
  if (limit - std::difftime(std::time(NULL), gs->time) < 0)
    return GameStatus::Lose;
  int rows = gs->board->get_rows(), cols = gs->board->get_cols();
  if (gs->board->get_cell(cols-1, rows-1))
    return GameStatus::Playing;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      int index = i * cols + j;
      if (index + 1 == rows * cols) continue;
      if (gs->board->get_cell(j, i) != index + 1)
        return GameStatus::Playing;
    }
  }
  return GameStatus::Win;
}

void
TimeGameRules::move(Board* board, DIRECTION dir)
{
  int dx[] = { 0,  1,  0, -1};
  int dy[] = {-1,  0,  1,  0};
  std::pair<int, int> null_cell = find_null(board);

  int nj = null_cell.first + dx[dir];
  int ni = null_cell.second + dy[dir];
  board->swap(null_cell.first, null_cell.second, nj, ni);
}

