#ifndef TILEMAP_INCLUDED
#define TILEMAP_INCLUDED

#define WallId 65
#define L1FoodId 33
#define L1powerUpId 34

class TileMap : public sf::Drawable, public sf::Transformable {
public:
  TileMap(const TileMap &) = delete;
  TileMap &operator=(const TileMap &) = delete;
  TileMap();
  ~TileMap();
  bool updateMap();
  bool reLoadMap();
  bool parseXML();
  bool get_mapMeta_data(tinyxml2::XMLDocument *);
  bool get_map(tinyxml2::XMLDocument *);
  void processEvents();
  void storeXcord();
  void storeYcord();
  void update(sf::Time deltaTime);
  void storeTileReference_by_XY();
  void init_map();
  int  getTileId(int Px, int Py);
  int  getTileIndex(int Px, int Py);
  int  returnTileId(int TileIndex);
  bool replace_LastAccessedTile_By(int new_TileId);
  void setSpriteColor(int tileIndex,sf::Color);
  void registerWindowSize(unsigned int window_width,
                          unsigned int window_height);
void pushVisiblePath(sf::RectangleShape*);
void resetVisiblePath(void);
  
  void designGraphOfTileMap(void); 
  void testGraph(void);
  struct m_graphNode* return_graphNode(int tileIndex) { return mapGraph[tileIndex];}

 // private:
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

  sf::Texture m_tileset;
  sf::Sprite *m_pMapSprites[640]; // 32*20
  std::vector<int> m_mapData;
  std::vector<sf::RectangleShape*> visiblePath;


  std::vector<m_graphNode*> mapGraph ;

  struct m_wall {
    unsigned int id;
    unsigned int x;
    unsigned int y;
  } m_wall;

  struct m_l1Food {
    unsigned int id;
    unsigned int x;
    unsigned int y;
  } m_l1Food;

  struct m_l1PowerUp {
    unsigned int id;
    unsigned int x;
    unsigned int y;
  } m_l1PowerUp;

  std::string mapPNG;

  unsigned int m_tileWidth;
  unsigned int m_tileHeight;
 unsigned int m_total_map_width;
  unsigned int m_total_map_height;

  unsigned int m_window_width;
  unsigned int m_window_height;
  unsigned int m_lastTileAccessed;

  bool hasMapChanged;
};

#endif

