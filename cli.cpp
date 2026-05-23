#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <map>
#include <memory>
#include <ncurses.h>
#include <string>
#include <time.h>
#include "game.h"
#include "game_creators.h"
#include "scoreboard.h"
#include "structs.h"

GameResult
solving(Game* game)
{ 
  game->start();
  GameResult result = {"", 0, 0, 0, 0, 0};
  int choice = 0;
  bool last_run = false;
  int term_rows, term_cols;
  int cell_width = 4;
  int board_rows = game->rows();
  int board_cols = game->cols();
  getmaxyx(stdscr, term_rows, term_cols);

  do {
    GameStatus status = game->status();
    clear();
    getmaxyx(stdscr, term_rows, term_cols);

    int pair = status == GameStatus::Win ? 2 : 1;
    pair = status == GameStatus::Lose ? 3 : pair;
    int start_y = (term_rows - board_rows * 3) / 2;
    int start_x = (term_cols - board_cols * cell_width) / 2;
    
    for (int i = 0; i < board_rows; ++i) {
      move(start_y + i * 3, start_x);
      for (int j = 0; j < board_cols; ++j) {
        attron(COLOR_PAIR(pair));
        printw("%*c", cell_width, ' ');
        attroff(COLOR_PAIR(pair));
      }
    }

    for (int i = 0; i < board_rows; ++i) {
      move(start_y + i * 3 + 1, start_x);
      for (int j = 0; j < board_cols; ++j) {
        int cell = game->cell(j, i);
        int cell_len = snprintf(NULL, 0, "%d", cell);
        int pad = (cell_width - cell_len) / 2 + (cell_width - cell_len) % 2;
        

        attron(COLOR_PAIR(pair));
        if (!cell) printw("%*c", cell_width, ' ');
        else printw("%*s%d%*s", pad, "", cell, cell_width - cell_len - pad, "");;
        attroff(COLOR_PAIR(pair));
      }
    }

    for (int i = 0; i < board_rows; ++i) {
      move(start_y + i * 3 + 2, start_x);
      for (int j = 0; j < board_cols; ++j) {
        attron(COLOR_PAIR(pair));
        printw("%*c", cell_width, ' ');
        attroff(COLOR_PAIR(pair));
      }
    }

    int size = (term_cols - strlen("Z - to shuffle cells")) / 2;
    mvprintw(start_y + board_rows * 3 + 1, size, "%s", "q - to quit in menu");
    mvprintw(start_y + board_rows * 3 + 2, size, "%s", "Z - to shuffle cells");
    attron(COLOR_PAIR(pair));
    int elapsed = (int)game->get_elapsed_time();
    size = (term_cols - snprintf(NULL, 0, "Steps: %d  Time: %02d:%02d", game->steps(), elapsed / 60, elapsed % 60)) / 2;
    mvprintw(start_y + board_rows * 3 + 3, size, "Steps: %d  Time: %02d:%02d", game->steps(), elapsed / 60, elapsed % 60);
    attroff(COLOR_PAIR(pair));

    if (status == GameStatus::Win) {
      result = game->result();
      int input = 0, i = 0;

      const char *msg = "You win! Enter your name: ";
      char namebuf[20] = {0};
      attron(A_REVERSE);
      mvprintw(start_y + board_rows * 3 + 4, (term_cols - strlen(msg)) / 2, "%s", msg);
      move(start_y + board_rows * 3 + 4, (term_cols - strlen(msg)) / 2 + strlen(msg));
      curs_set(1);
      timeout(-1);

      while (1) {
        input = getch();
        if (isalnum(input) && i < 20) {
          printw("%c", input);
          namebuf[i++] = input;
        }
        else if (input == KEY_BACKSPACE && i > 0) {
          printw("\b \b");
          namebuf[--i] = 0; 
        }
        else if (input == '\n' || input == '\r')
          break;
      }
      curs_set(0);
      attroff(A_REVERSE);
      refresh();

      result.name = namebuf;
      break;
    } else if (status == GameStatus::Lose) {
      const char *msg = "You lose! Sad :(";
      attron(A_REVERSE);
      mvprintw(start_y + board_rows * 3 + 4, (term_cols - strlen(msg)) / 2, "%s", msg);
      timeout(-1);
      attroff(A_REVERSE);
      refresh();
      getch();
      break;
    } else if (choice == 'q') {
      break;
    }

    refresh();

    timeout(1000);
    choice = getch();
    switch (choice) {
      case KEY_UP: game->move(DIRECTION::DOWN); break;
      case KEY_RIGHT: game->move(DIRECTION::LEFT); break;
      case KEY_DOWN: game->move(DIRECTION::UP); break;
      case KEY_LEFT: game->move(DIRECTION::RIGHT); break;
      case 'Z': game->shuffle(); break;
    }
  } while(1);

  return result;
}

void
printsb(Scoreboard* sb) {
  clear();
  GameResult p;
  int term_rows, term_cols;
  getmaxyx(stdscr, term_rows, term_cols);
  char buf[8], timebuf[6];
  attron(A_REVERSE);
  mvprintw(0, (term_cols - 46) / 2, "%20s %7s %5s %6s %7s", "name", "board", "steps", "time", "score");
  for (int i = 0; i < sb->get_results().size(); ++i) {
    p = sb->get_results()[i];
    snprintf(buf, sizeof(buf), "%dx%d", p.rows, p.cols);
    snprintf(timebuf, sizeof(timebuf), "%02d:%02d", p.time / 60, p.time % 60);
    mvprintw(i+1, (term_cols - 46) / 2, "%20s %7s %5d %6s %7d", p.name.c_str(), buf, p.steps, timebuf, p.score);
  }
  attroff(A_REVERSE);
  timeout(-1);
  refresh();
  getch();
}

void
main_loop()
{
  Scoreboard sb = Scoreboard();
  GameResult p;

  std::map<std::string, std::unique_ptr<Game>> factories;
  factories["classic"] = std::make_unique<ClassicGame>(4, 4);
  factories["small"] = std::make_unique<ClassicGame>(3, 3);
  factories["time limit"] = std::make_unique<TimeGame>(2, 2, 10);

  int choice = 0, index = 0;
  int rows = 0, cols = 0;
  int row_max_len = 0;
  int term_rows, term_cols;
  int start_x, start_y;
  std::vector<std::string> keys;

  for (const auto& [key, value] : factories) {
    keys.push_back(key);
    if (key.length() > row_max_len) row_max_len = key.length();
  }
  keys.push_back("custom");
  keys.push_back("scoreboard");
  

  do {
    clear();
    getmaxyx(stdscr, term_rows, term_cols);
    start_y = (term_rows - keys.size()) / 2;
    start_x = (term_cols - row_max_len) / 2;
    for (int i = 0; i < keys.size(); ++i) {
      if (i == index) attron(A_REVERSE);
      mvprintw(start_y + i, start_x, "%s", keys[i].c_str());
      if (i == index) attroff(A_REVERSE);
    }
    refresh();

    timeout(-1);
    choice = getch();
    
    switch (choice) {
      case KEY_UP:   if (index > 0) --index; break;
      case KEY_DOWN: if (index < keys.size()-1) ++index; break;
      case ' ':
          if (keys[index] == "custom") {
          const char* msg = "Size (rows x cols): ";
          mvprintw(start_y + keys.size() + 1, (term_cols - strlen(msg)) / 2, "%s", msg);
          echo();
          curs_set(1);
          scanw("%d %d", &rows, &cols);
          noecho();
          curs_set(0);

          if ((rows + 3) * 3 > term_rows || (cols + 2) * 4 > term_cols) {
            clear();
            const char* msg = "Terminal too small";
            start_x = (term_cols - strlen(msg)) / 2;
            start_y = term_rows / 2;
            attron(A_REVERSE);
            mvprintw(start_y, start_x, "%s", msg);
            attroff(A_REVERSE);
            getch();
            break;
          } else if (rows < 2 || cols < 2) {
            clear();
            const char* msg = "Minimal size of fifteen is 2x2";
            start_x = (term_cols - strlen(msg)) / 2;
            start_y = term_rows / 2;
            attron(A_REVERSE);
            mvprintw(start_y, start_x, "%s", msg);
            attroff(A_REVERSE);
            getch();
            break;
          }

          p = solving(std::make_unique<ClassicGame>(rows, cols).get());
        } else if (keys[index] == "scoreboard") {
          printsb(&sb);
        } else
          p = solving(factories[keys[index]].get());
        break;
    }
    if (!p.name.empty()) { sb.add(p); p.name = ""; }
  } while (choice != 'q');
  sb.save();
}

int
main ()
{
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, true);
  start_color();
  use_default_colors();

  init_pair(1, COLOR_BLACK, COLOR_BLUE);
  init_pair(2, COLOR_BLACK, COLOR_GREEN);
  init_pair(3, COLOR_BLACK, COLOR_RED);
  
  main_loop(); 

  attrset(A_NORMAL);
  endwin();
  return 0;
}