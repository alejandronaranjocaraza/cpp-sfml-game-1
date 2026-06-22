#include <iostream>
#include "include/game.h"

int main()
{
  // Init srand
  std::srand(static_cast<unsigned>(time(NULL)));

  Game game;

  // Event loop
  while(game.running() && !game.ended_game()) {

    // Update
    game.update();

    // Render
    game.render();

  }


  // End of application
  return 0;
}
