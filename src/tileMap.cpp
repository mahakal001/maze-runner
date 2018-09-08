#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "tinyxml2.h"
#include "node_structs.h"
#include "tileMap.h"


#include <unistd.h>

using namespace tinyxml2;
using namespace std;

TileMap::TileMap() {
  // cout << "tile map constructor\n";

  mapPNG = "../resources/images/mazeTile.png";
  if (!m_tileset.loadFromFile(mapPNG))
    cout << "unable to load mapPNG" << endl;

  m_wall.id = WallId;
  m_wall.x = 0;
  m_wall.y = 64;

  m_l1Food.id = L1FoodId;
  m_l1Food.x = 0;
  m_l1Food.y = 32;

  m_l1PowerUp.id = L1powerUpId;
  m_l1PowerUp.x = 32;
  m_l1PowerUp.y = 32;

  parseXML();
  init_map();
  hasMapChanged = false;
}

bool TileMap::parseXML() {
  XMLDocument doc;

  doc.LoadFile("../resources/levels/maze.xml");
  // doc.Print();
  cout << "file loaded by doc\n";

  if (!get_mapMeta_data(&doc))
    cout << "fail in extracting meta data from xml file" << endl;
  if (!get_map(&doc))
    cout << "unable to get map " << endl;

  return true;
}

bool TileMap::get_mapMeta_data(XMLDocument *doc) {
  XMLElement *map = doc->FirstChildElement("map");
  if (!map)
    cout << "unalbe to get \"map\" id" << endl;
  m_tileWidth = std::stoi(map->Attribute("tilewidth"));
  m_tileHeight = std::stoi(map->Attribute("tileheight"));
  m_total_map_width = std::stoi(map->Attribute("width"));
  m_total_map_height = std::stoi(map->Attribute("height"));
  return true;
}

bool TileMap::get_map(XMLDocument *doc) {
  XMLNode *pDataNode =
      doc->FirstChildElement()->FirstChildElement("layer")->FirstChildElement(
          "data");
  std::string str = pDataNode->FirstChild()->ToText()->Value();
  /* The map data needs to be converted
   * to integers before we can make sense
   * from it  */
  std::stringstream ss(str);
  int i;
  while (ss >> i) {
    m_mapData.push_back(i);
    if (ss.peek() == ',' || ss.peek() == ' ')
      ss.ignore();
  }
  return true;
}

void TileMap::update(sf::Time deltaTime) {
  if (hasMapChanged)
    updateMap();

  hasMapChanged = false;
}

void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const {

    for(auto i=0;i < visiblePath.size();i++)
      target.draw(*visiblePath[i],states);
  for (auto i = 0; i < m_mapData.size(); i++)
    target.draw(*m_pMapSprites[i], states);

}

void TileMap::setSpriteColor(int tileIndex,sf::Color color){
   sf::Sprite* p_sprite = m_pMapSprites[tileIndex];
   p_sprite->setColor(color);
   return;
}

TileMap::~TileMap()
{
  for(auto p:m_pMapSprites){
  delete p;
  cout<<"freed\n";
  }
  for(auto p:mapGraph){
  delete p;
  cout<<"  shades freed\n";

  }
  cout << "hello\n" ;
  return;
}
void TileMap::init_map() {
  for (auto i = 0; i < m_mapData.size(); i++) {
    sf::Sprite *sprite = new sf::Sprite(m_tileset);
    struct m_graphNode *node = new m_graphNode;

    node->x = (i % 32) * 32;
    node->y = (i / 32) * 32;
    node->nodeIndex = i;

    m_pMapSprites[i] = sprite;
    sprite->setPosition(node->x, node->y);

    mapGraph.push_back(node);

    switch (m_mapData[i]) {
    case WallId:
      sprite->setTextureRect(sf::IntRect(m_wall.x, m_wall.y, 32, 32));
      node->typeOfNode = WallId;
      node->ch = '#';
      break;
    case L1FoodId:
      sprite->setTextureRect(sf::IntRect(m_l1Food.x, m_l1Food.y, 32, 32));
      node->typeOfNode = L1FoodId;
      node->ch = ' ';
      break;
    case L1powerUpId:
      sprite->setTextureRect(sf::IntRect(m_l1PowerUp.x, m_l1PowerUp.y, 32, 32));
      node->typeOfNode = L1powerUpId;
      node->ch = ' ';
      break;
    default:
      m_pMapSprites[i]->setTextureRect(sf::IntRect(512, 512, 32, 32));
      node->typeOfNode = 0;
      node->ch = ' ';
      break;
    }
  }
}

void TileMap::designGraphOfTileMap(void) {
  cout << "************ size of node is ***********\n";
  cout << mapGraph.size();
  cout << "*****************************************\n";
  for (auto i = 0; i < mapGraph.size(); i++) {
    if (i % 32 == 0)
      cout << "\n";
    std::cout << mapGraph[i]->ch << " ";
  }

  cout << "\n";
  /*************************************************/
  for (auto i = 0; i < mapGraph.size(); i++) {
    if (i % 32 == 0)
      mapGraph[i]->left = nullptr;
    else
      mapGraph[i]->left = mapGraph[i - 1];

    if ((i + 1) % 32 == 0)
      mapGraph[i]->right = nullptr;
    else
      mapGraph[i]->right = mapGraph[i + 1];

    if (i / 32 == 0)
      mapGraph[i]->above = nullptr;
    else
      mapGraph[i]->above = mapGraph[i - 32];

    if (i / 32 == m_total_map_height - 1)
      mapGraph[i]->below = nullptr;
    else
      mapGraph[i]->below = mapGraph[i + 32];
  }

  testGraph();
  return;
}

void TileMap::testGraph() {
  cout << "-----------------------------------------------\n";
  auto tmpNode_0 = mapGraph[0];
  for (; tmpNode_0;) {
    auto tmpNode = tmpNode_0;
    for (; tmpNode;) {
      cout << tmpNode->ch << " ";
      tmpNode = tmpNode->right;
    }
    cout << "\n";
    tmpNode_0 = tmpNode_0->below;
  }
}

void TileMap::registerWindowSize(unsigned int width, unsigned int height) {
  m_window_width = width;
  m_window_height = height;
  cout << " width " << m_window_width << " height " << m_window_height << endl;
}

bool TileMap::reLoadMap() {
  for (auto i = 0; i < m_mapData.size(); i++) {
    
    switch (m_mapData[i]) {
    case WallId:
      m_pMapSprites[i]->setTextureRect(sf::IntRect(m_wall.x, m_wall.y, 32, 32));
      break;
    case L1FoodId:
      m_pMapSprites[i]->setTextureRect(
          sf::IntRect(m_l1Food.x, m_l1Food.y, 32, 32));
      break;
    case L1powerUpId:
      m_pMapSprites[i]->setTextureRect(
          sf::IntRect(m_l1PowerUp.x, m_l1PowerUp.y, 32, 32));
      break;
    default:
      m_pMapSprites[i]->setTextureRect(sf::IntRect(512, 512, 32, 32));
      break;
    }
  }
}

bool TileMap::updateMap() {
  switch (m_mapData[m_lastTileAccessed]) {
  case WallId:
    m_pMapSprites[m_lastTileAccessed]->setTextureRect(
        sf::IntRect(m_wall.x, m_wall.y, 32, 32));
    break;
  case L1FoodId:
    m_pMapSprites[m_lastTileAccessed]->setTextureRect(
        sf::IntRect(m_l1Food.x, m_l1Food.y, 32, 32));
    break;
  case L1powerUpId:
    m_pMapSprites[m_lastTileAccessed]->setTextureRect(
        sf::IntRect(m_l1PowerUp.x, m_l1PowerUp.y, 32, 32));
    break;
  default:
    m_pMapSprites[m_lastTileAccessed]->setTextureRect(
        sf::IntRect(512, 512, 32, 32));
    break;
  }
}

void TileMap::resetVisiblePath(void)
{
  for(auto i = 0 ;i < visiblePath.size() ;i++)
  {
    delete visiblePath[i];
  }
  visiblePath.clear();
  return;
}

void TileMap::pushVisiblePath(sf::RectangleShape *rect)
{
  visiblePath.push_back(rect);
  return;
}

int TileMap::getTileId(int Px, int Py) {
  int index = (Py / m_tileWidth) * m_total_map_width + (Px / m_tileWidth);
  m_lastTileAccessed = index;
  return m_mapData[index];
}

int TileMap::getTileIndex(int Px, int Py) {
  int index = (Py / m_tileWidth) * m_total_map_width + (Px / m_tileWidth);
  return index;
}

int TileMap::returnTileId(int tileIndex) { return m_mapData[tileIndex]; }

bool TileMap::replace_LastAccessedTile_By(int new_TileId) {
 m_mapData[m_lastTileAccessed] = new_TileId;
  hasMapChanged = true;
  return true;
}

