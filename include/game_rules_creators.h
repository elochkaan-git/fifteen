#pragma once
#include "game_rules.h"
#include "structs.h"
#include <ctime>

std::pair<int, int> find_null(Board* board);

class ClassicGameRules : public GameRules
{
public:
  bool can_move(Board* board, DIRECTION dir) override;
  GameStatus status(const GameState* gs) override;
  void move(Board* board, DIRECTION dir) override;
};

class TimeGameRules : public GameRules
{
public:
  TimeGameRules(time_t limit);
  bool can_move(Board* board, DIRECTION dir) override;
  GameStatus status(const GameState* gs) override;
  void move(Board* board, DIRECTION dir) override;

private:
  time_t limit;
};