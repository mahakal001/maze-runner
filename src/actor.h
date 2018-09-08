#ifndef ACTOR_H_
#define ACTOR_H_

class Actor : public sf::Drawable, public sf::Transformable {
public:
  Actor(const Actor &) = delete;
  Actor &operator=(const Actor &) = delete;
  Actor();
  void get_reference_to_tile_map(TileMap *);
  void registerWindowSize(unsigned int window_width,
                          unsigned int window_height);
  TileMap *m_ptileMap;
  float m_min_allowed_x;
  float m_min_allowed_y;
  float m_max_allowed_x;
  float m_max_allowed_y;

private:
  sf::Texture m_tileset;
  unsigned int window_width;
  unsigned int window_height;
  float boundry_size;
  float apparent_boundry;
};

#endif
