#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <unistd.h>

#include "tinyxml2.h"
#include "node_structs.h"
#include "tileMap.h"
#include "actor.h"


using namespace std;

Actor::Actor() {
  m_min_allowed_x = 0;
  m_min_allowed_y = 0;
  m_max_allowed_x = 32 * 32;
  m_max_allowed_y = 32 * 20;
  cout << " Actor constructor executed " << endl;
}

void Actor::get_reference_to_tile_map(TileMap *tilemap) {
  m_ptileMap = tilemap;
}

void Actor::registerWindowSize(unsigned int width, unsigned int height) {
  window_width = width;
  window_height = height;
}
