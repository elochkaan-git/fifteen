#pragma once
#include <ctime>
#include "game.h"
#include "structs.h"

class ClassicGame : public Game
{
public:
  ClassicGame(int r, int c);
  void move(DIRECTION dir) override;
  time_t get_elapsed_time() override;
  GameResult result() override;

private:
  std::unique_ptr<GameRules> createRules() override;
  std::unique_ptr<GameState> createGameState() override;
};

class TimeGame : public Game
{
public:
  TimeGame(int r, int c, time_t limit);
  void move(DIRECTION dir) override;
  time_t get_elapsed_time() override;
  GameResult result() override;

private:
  time_t limit;
  std::unique_ptr<GameRules> createRules() override;
  std::unique_ptr<GameState> createGameState() override;
};