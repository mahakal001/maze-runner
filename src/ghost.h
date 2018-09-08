#include <queue>
#include <vector>
#include "actor.h"
#define cachedStepsCount 5

class Ghost : public Actor {
public:
  Ghost(const Ghost &) = delete;
  Ghost &operator=(const Ghost &) = delete;
  Ghost();
  void update(sf::Time deltaTime);
  template <typename... Args> void setPosition(Args &&... args) {

    m_Ghost_Shape.setPosition(std::forward<Args>(args)...);
  }
  bool checkForCollision(int x, int y);

  float getXpos();
  float getYpos();
  void findPath(int curPosIndex);
  void turn(int nextDirectionToMove);
  sf::Sprite returnSpriteRect(void);
  void setSpecialAttribute(bool);
  void registerTargetPosition(int x, int y);

  void setGhostStyle(int x_inTexture, int y_inTexture, int x_onScreen,
                     int y_onScreen, int init_direction);
  void setPossibleMoves(int tmpIndex);
  void eliminate_vibrating_behaviour(void);
  void getLegalTurns(void);
  int showMePath_AI_1(int, int);
  int showMePath_AI_2(int,int);
  int showMePath_Random();
  void addToPath(int turn);
  void updatePosition(int, int &, int &, int &);

private:
  virtual void draw(sf::RenderTarget &target,
                    sf::RenderStates states) const override;
sf::Texture m_tileset;
  bool hasCollide;
  float col_speed;
  float angle;
  float m_radius;

  int m_counter;
  int m_lastPathChoice;
  int m_target_x;
  int m_target_y;
  int m_cachedStepsCount;
  // int m_seed;

  sf::Sprite m_Ghost_Shape;
  bool m_amIspecial;
  enum direction { moveLeft, moveRight, moveUp, moveDown };
  std::queue<int> path;
  int possibleTurn[4];
  std::vector<int> legalTurn;
};

