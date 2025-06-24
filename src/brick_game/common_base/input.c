#include <ncurses.h>

#include "brick_game.h"

bool getUserAction(UserAction_t *action, bool *hold) {
  int ch = getch();
  *hold = false;

  switch (ch) {
    case KEY_LEFT:
      *action = Left;
      return true;
    case KEY_RIGHT:
      *action = Right;
      return true;
    case KEY_DOWN:
      *action = Down;
      return true;
    case KEY_UP:
    case ' ':
      *action = Action;
      return true;
    case 'p':
    case 'P':
      *action = Pause;
      return true;
    case 'q':
    case 'Q':
      *action = Terminate;
      return true;
    case '\n':
    case '\r':
      *action = Start;
      return true;
    default:
      return false;
  }
}