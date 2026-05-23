#pragma once
#include <ctime>
#include <memory>
#include "board.h"
#include "game_rules.h"
#include "structs.h"

class Game
{
public:
  Game(int r, int c) : rows_(r), cols_(c) {};
  void start() { rules = createRules(); gs = createGameState(); }
  virtual void move(DIRECTION dir) = 0;
  void shuffle() { gs->board->shuffle(); gs->steps = 0; std::time(&gs->time); }
  int cell(int x, int y) const { return gs->board->get_cell(x, y); }
  int size() const { return gs->board->get_size(); }
  int rows() const { return gs->board->get_rows(); }
  int cols() const { return gs->board->get_cols(); }
  int steps() const { return gs->steps; }
  virtual time_t get_elapsed_time() = 0;
  virtual GameResult result() = 0;
  GameStatus status() { return rules->status(gs.get()); }
  virtual ~Game() = default;

protected:
  std::unique_ptr<GameRules> rules;
  std::unique_ptr<GameState> gs;
  virtual std::unique_ptr<GameRules> createRules() = 0;
  virtual std::unique_ptr<GameState> createGameState() = 0;
  int rows_ = 0, cols_ = 0;
};