#include <cstring>
#include <map>
#include <memory>
#include <ncurses.h>
#include <string>
#include "board_factory.h"

void
solving(std::unique_ptr<Board> board)
{ 
  int choice = 0;
  bool last_run = false;
  int term_rows, term_cols;
  int cell_width = 4;
  int board_rows = board->get_rows();
  int board_cols = board->get_cols();
  getmaxyx(stdscr, term_rows, term_cols);

  do {
    clear();
    getmaxyx(stdscr, term_rows, term_cols);

    int start_y = (term_rows - board_rows * 3) / 2;
    int start_x = (term_cols - board_cols * cell_width) / 2;
    
    for (int i = 0; i < board_rows; ++i) {
      move(start_y + i * 3, start_x);
      for (int j = 0; j < board_cols; ++j) {
        int pair = last_run ? 2 : 1;

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
        int pair = last_run ? 2 : 1;

        attron(COLOR_PAIR(pair));
        if (!cell) printw("%*c", cell_width, ' ');
        else printw("%*s%d%*s", pad, "", cell, cell_width - cell_len - pad, "");;
        attroff(COLOR_PAIR(pair));
      }
    }

    for (int i = 0; i < board_rows; ++i) {
      move(start_y + i * 3 + 2, start_x);
      for (int j = 0; j < board_cols; ++j) {
        int pair = last_run ? 2 : 1;

        attron(COLOR_PAIR(pair));
        printw("%*c", cell_width, ' ');
        attroff(COLOR_PAIR(pair));
      }
    }

    int s = (term_cols - strlen("Z - to shuffle cells")) / 2;
    mvprintw(start_y + board_rows * 3 + 1, s, "%s", "q - to quit in menu");
    mvprintw(start_y + board_rows * 3 + 2, s, "%s", "Z - to shuffle cells");

    if (last_run && board->is_win()) {
      const char *msg = "You win! (Press any key to continue)";
      move(start_y + board_rows * 3 + 3, (term_cols - strlen(msg)) / 2);
      attron(A_REVERSE);
      addstr(msg);
      attroff(A_REVERSE);
    }

    refresh();
    if (last_run) break;

    choice = getch();
    switch (choice) {
      case KEY_UP: board->move_cell(2); break;
      case KEY_RIGHT: board->move_cell(3); break;
      case KEY_DOWN: board->move_cell(0); break;
      case KEY_LEFT: board->move_cell(1); break;
      case 'Z': board->shuffle(); break;
    }
    if (board->is_win() || choice == 'q') last_run = true;
  } while(1);

  if (board->is_win()) { getch(); }
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

          solving(std::unique_ptr<Board>(CustomBoardFactory(rows, cols).create()));
        } else
          solving(std::unique_ptr<Board>(factories[keys[index]]->create()));
        break;
    }
  } while (choice != 'q');

  attrset(A_NORMAL);
  endwin();
  return 0;
}