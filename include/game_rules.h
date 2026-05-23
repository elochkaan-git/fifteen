#pragma once
#include "board.h"
#include "structs.h"

class GameRules
{
public:
  virtual bool can_move(Board* board, DIRECTION dir) = 0;
  virtual GameStatus status(const GameState* gs) = 0;
  virtual void move(Board* board, DIRECTION dir) = 0;
  virtual ~GameRules() = default;
};