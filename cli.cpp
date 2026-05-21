#include <cstddef>
#include <cstdio>
#include <cstring>
#include <map>
#include <memory>
#include <ncurses.h>
#include <string>
#include <time.h>
#include "board_factory.h"
#include "scoreboard.h"

Player
solving(std::unique_ptr<Board> board)
{ 
  Player result = {"", 0, 0, 0, 0, 0};
  int choice = 0;
  int steps = 0;
  bool last_run = false;
  int term_rows, term_cols;
  int cell_width = 4;
  int board_rows = board->get_rows();
  int board_cols = board->get_cols();
  getmaxyx(stdscr, term_rows, term_cols);

  time_t start = time(NULL);
  do {
    clear();
    getmaxyx(stdscr, term_rows, term_cols);

    int pair = last_run ? 2 : 1;
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
        int cell = board->get_cell(j, i);
        int cell_len = snprintf(NULL, 0, "%d", cell); // длина числа
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

    int s = (term_cols - strlen("Z - to shuffle cells")) / 2;
    mvprintw(start_y + board_rows * 3 + 1, s, "%s", "q - to quit in menu");
    mvprintw(start_y + board_rows * 3 + 2, s, "%s", "Z - to shuffle cells");
    attron(COLOR_PAIR(pair));
    int elapsed = (int)(time(NULL) - start);
    s = (term_cols - snprintf(NULL, 0, "Steps: %d  Time: %02d:%02d", steps, elapsed / 60, elapsed % 60)) / 2;
    mvprintw(start_y + board_rows * 3 + 3, s, "Steps: %d  Time: %02d:%02d", steps, elapsed / 60, elapsed % 60);
    attroff(COLOR_PAIR(pair));

    if (last_run && board->is_win()) {
      const char *msg = "You win! Enter your name: ";
      char namebuf[20];
      attron(A_REVERSE);
      mvprintw(start_y + board_rows * 3 + 4, (term_cols - strlen(msg)) / 2, "%s", msg);
      move(start_y + board_rows * 3 + 4, (term_cols - strlen(msg)) / 2 + strlen(msg));
      echo();
      curs_set(1);
      timeout(-1);
      scanw("%19s", namebuf);
      noecho();
      curs_set(0);
      attroff(A_REVERSE);
      refresh();

      result.name = namebuf;
      result.rows = board_rows;
      result.cols = board_cols;
      result.steps = steps;
      result.time = (int)(time(NULL) - start);
      result.score = std::max(0, board_rows * board_cols * 200 - (steps * 10 + (int)result.time * 2));
    }

    refresh();
    if (last_run) break;

    timeout(1000);
    choice = getch();
    switch (choice) {
      case KEY_UP: steps += board->move_cell(2); break;
      case KEY_RIGHT: steps += board->move_cell(3); break;
      case KEY_DOWN: steps += board->move_cell(0); break;
      case KEY_LEFT: steps += board->move_cell(1); break;
      case 'Z': board->shuffle(); steps = 0; start = time(NULL); break;
    }
    if (board->is_win() || choice == 'q') last_run = true;
  } while(1);

  return result;
}

void
printsb(Scoreboard* sb) {
  clear();
  int term_rows, term_cols;
  getmaxyx(stdscr, term_rows, term_cols);
  char buf[8], timebuf[6];
  attron(A_REVERSE);
  mvprintw(0, (term_cols - 46) / 2, "%20s %7s %5s %6s %7s", "name", "board", "steps", "time", "score");
  for (int i = 0; i < sb->get_players().size(); ++i) {
    Player p = sb->get_players()[i];
    snprintf(buf, sizeof(buf), "%dx%d", p.rows, p.cols);
    snprintf(timebuf, sizeof(timebuf), "%02d:%02d", p.time / 60, p.time % 60);
    mvprintw(i+1, (term_cols - 46) / 2, "%20s %7s %5d %6s %7d", p.name.c_str(), buf, p.steps, timebuf, p.score);
  }
  attroff(A_REVERSE);
  timeout(-1);
  getch();
  refresh();
}

void
main_loop()
{
  Scoreboard sb = Scoreboard();

  std::map<std::string, std::unique_ptr<BoardFactory>> factories;
  factories["small"] = std::make_unique<SmallBoardFactory>();
  factories["classic"] = std::make_unique<ClassicBoardFactory>();
  factories["large"] = std::make_unique<LargeBoardFactory>();
  factories["rectangle"] = std::make_unique<RectangleBoardFactory>();

  int choice = 0, index = 0;
  int rows = 0, cols = 0;
  int row_max_len = 0;
  int term_rows, term_cols;
  int start_x, start_y;
  bool custom_selected = false;
  std::vector<std::string> keys;

  for (const auto& [key, value] : factories) {
    keys.push_back(key);
    if (key.length() > row_max_len) row_max_len = key.length();
  }
  keys.push_back("custom");
  keys.push_back("scoreboard");
  

  do {
    getmaxyx(stdscr, term_rows, term_cols);
    clear();
    start_y = (term_rows - keys.size()) / 2;
    start_x = (term_cols - row_max_len) / 2;
    for (int i = 0; i < keys.size(); ++i) {
      move(start_y + i, start_x);
      if (i == index) attron(A_REVERSE);
      printw("%s", keys[i].c_str());
      if (i == index) attroff(A_REVERSE);
    }

    refresh();
    timeout(-1);
    choice = getch();
    Player p;
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

          p = solving(std::unique_ptr<Board>(CustomBoardFactory(rows, cols).create()));
        } else if (keys[index] == "scoreboard") {
          printsb(&sb);
        } else
          p = solving(std::unique_ptr<Board>(factories[keys[index]]->create()));
        break;
    }
    if (!p.name.empty()) { sb.add(p); }
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
  
  main_loop();

  attrset(A_NORMAL);
  endwin();
  return 0;
}