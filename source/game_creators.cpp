#include <ctime>
#include <memory>
#include "board.h"
#include "game_creators.h"
#include "game.h"
#include "game_rules_creators.h"
#include "structs.h"

ClassicGame::ClassicGame(int r, int c) : Game(r, c) {}

void
ClassicGame::move(DIRECTION dir)
{ 
  if (!rules->can_move(gs->board.get(), dir))
    return;
  rules->move(gs->board.get(), dir);
  ++gs->steps;
}

time_t
ClassicGame::get_elapsed_time()
{
  return std::difftime(std::time(NULL), gs->time);
}

void
ClassicGame::save()
{
  gs->time = get_elapsed_time();
}

GameResult
ClassicGame::result()
{
  return {
    "",
    rows(),
    cols(),
    steps(),
    (int)gs->time,
    std::max(0, rows() * cols() * 200 - (steps() * 10 + (int)gs->time * 2))
  };
}

std::unique_ptr<GameRules>
ClassicGame::createRules()
{
  return std::make_unique<ClassicGameRules>();
}

std::unique_ptr<GameState>
ClassicGame::createGameState()
{
  std::unique_ptr<GameState> tgs = std::make_unique<GameState>();
  tgs->board = std::make_unique<Board>(rows_, cols_);
  tgs->steps = 0;
  std::time(&tgs->time);
  return tgs;
}

TimeGame::TimeGame(int r, int c, time_t limit) : Game(r, c), limit(limit) {}

void
TimeGame::move(DIRECTION dir)
{ 
  if (!rules->can_move(gs->board.get(), dir))
    return;
  rules->move(gs->board.get(), dir);
  ++gs->steps; 
}

time_t
TimeGame::get_elapsed_time()
{
  time_t out = limit - std::difftime(std::time(NULL), gs->time);
  return out < 0 ? 0 : out;
}

void
TimeGame::save()
{
  gs->time = limit - get_elapsed_time();
}

GameResult
TimeGame::result()
{ 
  return {
    "",
    rows(),
    cols(),
    steps(),
    (int)gs->time,
    std::max(0, (int)(rows() * cols() * 200 * 1.5 - (steps() * 10 + (int)gs->time * 2)))
  };
}

std::unique_ptr<GameRules>
TimeGame::createRules()
{
  return std::make_unique<TimeGameRules>(limit);
}

std::unique_ptr<GameState>
TimeGame::createGameState()
{
  std::unique_ptr<GameState> tgs = std::make_unique<GameState>();
  tgs->board = std::make_unique<Board>(rows_, cols_);
  tgs->steps = 0;
  std::time(&tgs->time);
  return tgs;
}