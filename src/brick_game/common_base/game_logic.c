#include <unistd.h>  // для usleep

#include "../../gui/cli/hdr/tetris_interface.h"  // для drawField
#include "../../main/main.h"                     // для shouldClose
#include "../tetris/hdr/fsm.h"
#include "brick_game.h"
#include "input.c"

void gameLoop(void) {
  FSMState state = kStateIdle;
  bool running = true;

  while (running) {
    GameInfo_t game = updateCurrentState();

    switch (state) {
      case kStateIdle:
        renderStartScreen();
        UserAction_t action;
        bool hold;
        if (getUserAction(&action, &hold) && action == Start) {
          userInput(Start, false);
          state = kStatePlaying;
        }
        break;

      case kStatePlaying:
        drawField(&game);
        if (getUserAction(&action, &hold)) {
          userInput(action, hold);
          if (action == Pause) state = kStatePaused;
          if (action == Terminate) state = kStateOver;
        }
        break;

      case kStatePaused:
        // Можно отрисовать экран паузы   
        mvprintw(0, 0, "PAUSED");
        refresh();
        if (getUserAction(&action, &hold) && action == Pause) {
          userInput(Pause, false);
          state = kStatePlaying;
        }
        break;

      case kStateOver:
        displayGameOverScreen(&game);
        if (getUserAction(&action, &hold)) {
          if (action == Start) {
            userInput(Start, false);
            state = kStatePlaying;
          } else if (action == Terminate) {
            running = false;
          }
        }
        break;
      case kStateTerminated:
        running = false;
        break;
    }
    napms(200); 
  }
}
