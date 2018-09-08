#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include "tinyxml2.h"
#include "node_structs.h"
#include "tileMap.h"
#include "ghost.h"
#include "player.h"
#include "scoreElement.h"

#include "game.h"


int main(int argc, char **argv) {
  Game game;

  srand(time(NULL));
  game.run(60);

  return 0;
}
