#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <ncursesw/ncurses.h> // Include ncursesw for Windows
#else
    #include <ncurses.h> // Include ncurses for Unix-like systems
#endif

#define TARGET_COLOR 1
#define USER_COLOR 2
#define ERROR_COLOR 3

void init_ncurses()
{
  initscr(); // Initialize ncurses
  cbreak(); // Disable line buffering
  noecho(); // Don't echo user input
  nodelay(stdscr, TRUE); // Set non-blocking mode
  timeout(10000); // Wait this much miliseconds before returing ERR
}

void init_colors()
{
  start_color();
  init_pair(TARGET_COLOR, 8, 0);
  init_pair(USER_COLOR, COLOR_WHITE, 0);
  init_pair(ERROR_COLOR, COLOR_RED, 0);
}

// TODO: Implement wrong/correct word selector
int main(int argc, char *argv[])
{
  int cursor_position = 0;
  int error_counter = 0;

  init_ncurses();
  init_colors();
  attron(COLOR_PAIR(TARGET_COLOR));
  // TODO: Implement system for reading text.
  char* target = "Business casual is an ambiguously defined dress code that has been adopted by many professional and white-collar workplaces in Western countries. It entails neat yet casual attire and is generally more casual than informal attire but more formal than casual or smart casual attire. Casual Fridays preceded widespread acceptance of business casual attire in many offices.";
  printw("%s", target);
  refresh();
  move(cursor_position / COLS, cursor_position % COLS);
  attron(COLOR_PAIR(USER_COLOR));
  while(true) {
    int x = getch();
    // BUG: Commands are accepted.
    if(x != ERR) {
      switch(x) {
        case KEY_ENTER: 
        case '\n':
          break;
        case KEY_BACKSPACE:
        case 127:
        case '\b':
          if(cursor_position - 1 >= 0) {
            cursor_position--;
          }
          attron(COLOR_PAIR(TARGET_COLOR));
          mvprintw(cursor_position / COLS, cursor_position % COLS, "%c", target[cursor_position]);
          move(cursor_position / COLS, cursor_position % COLS);
          break;
        default:
          cursor_position++;
          if(x == target[cursor_position - 1]) { // Correct character
            attron(COLOR_PAIR(USER_COLOR));
            printw("%c", x);
          } else { // Wrong character
            attron(COLOR_PAIR(ERROR_COLOR));
            printw("%c", x);
            error_counter++;
          }
      }
      if(cursor_position >= strlen(target)) {
        break;
      }
    }
  }

  endwin();
  // BUG: Accuracy can go negative.
  printf("Text length: %d, Error count: %d, Accuracy: %.2f%%\n", (int)strlen(target), error_counter, (1 - (float)error_counter / strlen(target)) * 100);
  
  return EXIT_SUCCESS;
}
