#pragma once
#include <ctime>
#include <memory>
#include <string>
#include "board.h"

enum DIRECTION
{
  UP,
  RIGHT,
  DOWN,
  LEFT
};

enum GameStatus
{
  Playing,
  Win,
  Lose
};

struct GameState
{
  std::unique_ptr<Board> board;
  int steps;
  time_t time;
};

struct GameResult
{
  std::string name;
  int rows;
  int cols;
  int steps;
  int time;
  int score;
};
