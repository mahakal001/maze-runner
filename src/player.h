#include "actor.h"

class Player : public Actor {
public:
  Player(const Player &) = delete;
  Player &operator=(const Player &) = delete;
  Player();
  void processEvents();
  void update(sf::Time deltaTime);
  template <typename... Args> void setPosition(Args &&... args) {

    m_Player_Shape.setPosition(std::forward<Args>(args)...);
  }
  bool checkForCollision(int x, int y);

  float getXpos();
  float getYpos();
  // float setAngle(float )
  sf::Sprite returnSpriteRect();
  void takeAction(int tileId, int x, int y);

  void setPlayerStyle();
  bool hasTakenBadJump();
  void animatePacman();
  bool hasTakenFood();
  bool hasTakenPowerUp();

private:
  virtual void draw(sf::RenderTarget &target,
                    sf::RenderStates states) const override;

  sf::Texture m_tileset;
  bool hasCollide;
  float col_speed;
  float angle;
  float m_radius;

  unsigned int acc;

  unsigned int m_bulletFaceAngle;

  sf::Sprite m_Player_Shape;

  bool upKey, leftKey, rightKey, downKey;
  enum direction { moveLeft, moveRight, moveUp, moveDown };
  int m_lastPathChoice;

  bool isMouthOpen, m_hasTakenFood, m_hasTakenPowerUp;
  bool isBadJump;
};
