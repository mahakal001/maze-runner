#define TotalGhost 2
#define TotalLives 3

class Game {
public:
  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;
  Game();
  ~Game();

  void run(int frameRate);
  void processEvents();
  void update(sf::Time TimePerFrame);
  void render();
  void init_ghostArmy();

private:
  sf::RenderWindow m_window;
  std::string mapPNG;

  unsigned int m_window_width;
  unsigned int m_window_height;

  sf::Texture m_tileset;
  TileMap m_tileMap;
  Player m_player;
  std::vector<sf::Font*> pointersTo_fontMemory;
  std::vector<Ghost*> pointersTo_ghostInstances;
  Ghost *m_ghosts[TotalGhost];
  ScoreElement m_curScore;
  ScoreElement m_HighScore;
  ScoreElement m_Lives;
  ScoreElement m_Bonus;
  ScoreElement m_SuperTime;
  ScoreElement m_BadJump;
};

