#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <queue>
#include <random>
#include <unistd.h>

#include "tinyxml2.h"
#include "node_structs.h"
#include "tileMap.h"
#include "ghost.h"


using namespace std;

Ghost::Ghost() {
  cout << " Ghost constructor executed " << endl;
  m_amIspecial = false;
}

void Ghost::setSpecialAttribute(bool flag) { m_amIspecial = flag; }

void Ghost::setGhostStyle(int i, int j, int x, int y, int init_direction) {
  m_Ghost_Shape.setTexture(m_ptileMap->m_tileset);
  m_Ghost_Shape.setTextureRect(sf::IntRect(40 * i, 0, 40, 40));
  m_Ghost_Shape.scale(0.8, 0.8); // scaling to 32 by 32 tile
  m_Ghost_Shape.setPosition(sf::Vector2f(32 * x, 32 * y));

  path.push(init_direction);
  path.push(init_direction);
  path.push(init_direction);
  path.push(init_direction);
  m_counter = 4;                     // initally we have two direction in "path"
  m_lastPathChoice = init_direction; // initializing our direction
}

void Ghost::update(sf::Time deltaTime) {
  if (m_counter == 0) // if we are out of path
  {
    int TileIndex =
        m_ptileMap->getTileIndex((int)getXpos(),  // calc current position ...
                                 (int)getYpos()); // ... of ghost in map
    findPath(TileIndex);
  } else {
    turn(path.front()); // turn ghost to path.front()
    path.pop();         // pop the above executed "step" from queue
    m_counter--;        // cached number of path decreased by 1
  }
  //sleep(1);
  cout << " ghost updated\n";
}

void Ghost::turn(int nextDirection) {
  switch (nextDirection) {
  case moveLeft:
    m_Ghost_Shape.setPosition(getXpos() - 16, getYpos());
    break;
  case moveRight:
    m_Ghost_Shape.setPosition(getXpos() + 16, getYpos());
    break;
  case moveUp:
    m_Ghost_Shape.setPosition(getXpos(), getYpos() - 16);
    break;
  case moveDown:
    m_Ghost_Shape.setPosition(getXpos(), getYpos() + 16);
    break;
  default:
    break;
  }

  return;
}

int Ghost::showMePath_Random() {
  /* Make a random selection from valid moves */
  return legalTurn[rand() % legalTurn.size()];
}

void Ghost::findPath(int curPosIndex) {
  int tmpIndex = curPosIndex;
  int curXpos = getXpos();
  int curYpos = getYpos();

  int random_luck = 1;
  if (m_amIspecial)
    random_luck = rand() % 2 + 1;

  /*
   * For each valid step ghost found (which is also a 32 by 32 tile) ,the
   * ghost cover it in two steps of  16 pixel each.
   * This is because we always want out ghost to lie completely in a tile
   * after execution of each step.This simplifies the game play.
   */
  m_cachedStepsCount = cachedStepsCount;
  m_counter = 2 * m_cachedStepsCount;


  if (random_luck == 1) {
    for (auto k = 0; k < m_cachedStepsCount; k++) {
      setPossibleMoves(tmpIndex);
      eliminate_vibrating_behaviour();
      getLegalTurns();
      auto suggestedTurn = showMePath_Random();
      addToPath(suggestedTurn);
      updatePosition(suggestedTurn, tmpIndex, curXpos, curYpos);
    }
  } else if(random_luck == 3) {
    for (auto i = 0; i < m_cachedStepsCount; i++) {
      setPossibleMoves(tmpIndex);
      eliminate_vibrating_behaviour();
      getLegalTurns();
      auto suggestedTurn = showMePath_AI_1(curXpos, curYpos);
      addToPath(suggestedTurn);
      updatePosition(suggestedTurn, tmpIndex, curXpos, curYpos);
    }
    cout << m_target_x << "    " << m_target_y << endl;
  }
  else if(random_luck == 2){
    
    showMePath_AI_2(m_ptileMap->getTileIndex(curXpos,curYpos),
                    m_ptileMap->getTileIndex(m_target_x,m_target_y)); 
  }

  m_counter = path.size();


  return;
}

void Ghost::updatePosition(int turn, int &tmpIndex, int &curXpos,
                           int &curYpos) {
  switch (turn) {
  case moveLeft:
    tmpIndex = tmpIndex - 1;
    curXpos -= 32;
    m_lastPathChoice = moveLeft;
    break;
  case moveRight:
    tmpIndex = tmpIndex + 1;
    curXpos += 32;
    m_lastPathChoice = moveRight;
    break;
  case moveUp:
    tmpIndex = tmpIndex - 32;
    curYpos -= 32;
    m_lastPathChoice = moveUp;
    break;
  case moveDown:
    tmpIndex = tmpIndex + 32;
    curYpos += 32;
    m_lastPathChoice = moveDown;
    //      sleep(2);
    break;
  default:
    break;
  }
}

void Ghost::addToPath(int turn) {
  switch (turn) {
  case moveLeft:
    std::cout << " left ";
    path.push(moveLeft);
    path.push(moveLeft);
    break;
  case moveRight:
    std::cout << " right ";
    path.push(moveRight);
    path.push(moveRight);
    break;
  case moveUp:
    std::cout << " up ";
    path.push(moveUp);
    path.push(moveUp);
    break;
  case moveDown:
    std::cout << " down ";
    path.push(moveDown);
    path.push(moveDown);
    //      sleep(2);
    break;
  default:
    break;
  }
}

  int Ghost::showMePath_AI_2(int StartTileIndex,int GoalTileIndex) {

      m_graphNode *goalNode = m_ptileMap->return_graphNode(GoalTileIndex);
      m_graphNode *startNode = m_ptileMap->return_graphNode(StartTileIndex);

    auto  route = graph_search(startNode,goalNode,m_ptileMap);

    for(;!route.empty();)
    {
    //  cout << route.front() << "  ";
      addToPath(route.front());
      route.pop();
    }

    cout << endl;
  
   
    return 1;
  }

int Ghost::showMePath_AI_1(int curXpos, int curYpos) {
  int minDistance = 9999;
  int optimalMove = 9999;
  int estimatedSeperation = 9999;
  cout << " legal moves " << legalTurn.size() << endl;
  // exit(EXIT_FAILURE);
  for (auto i = 0; i < legalTurn.size(); i++) {
    switch (legalTurn[i]) {
    case moveLeft:
      estimatedSeperation =
          abs((curXpos - 32) - m_target_x) + abs(curYpos - m_target_y);
      if (estimatedSeperation <= minDistance) {
        minDistance = estimatedSeperation;
        optimalMove = moveLeft;
      }
      break;
    case moveRight:
      estimatedSeperation =
          abs((curXpos + 32) - m_target_x) + abs(curYpos - m_target_y);
      if (estimatedSeperation <= minDistance) {
        minDistance = estimatedSeperation;
        optimalMove = moveRight;
      }
      break;
    case moveUp:
      estimatedSeperation =
          abs((curXpos)-m_target_x) + abs((curYpos - 32) - m_target_y);
      if (estimatedSeperation <= minDistance) {
        minDistance = estimatedSeperation;
        optimalMove = moveUp;
      }
      break;
    case moveDown:
      estimatedSeperation =
          abs((curXpos)-m_target_x) + abs((curYpos + 32) - m_target_y);
      if (estimatedSeperation <= minDistance) {
        minDistance = estimatedSeperation;
        optimalMove = moveDown;
      }
      break;
    default:
      std::cout << " if you can see me implies you have got a bug";
      sleep(10);
      break;
    }
  }

  return optimalMove;
}



void Ghost::getLegalTurns(void) {
  /* Gather the valid moves in a vector */
  legalTurn.resize(0);
  for (auto j = 0; j < 4; j++)
    if (possibleTurn[j] == 1)
      legalTurn.push_back(j);

  /* But wait...,suppose while moving LEFT ,the ghost reaches a position from
   * where the only possible way is back i.e RIGHT.
   * But bcoz we just set the RIGHT as invalid(though it wasn't)
   * implies our vector becomes empty.
   * In this case, we reset the only valid move to valid ( @/@ )
   */
  if (0 == legalTurn.size()) {
    switch (m_lastPathChoice) {
    case moveUp:
      possibleTurn[moveDown] = 1;
      legalTurn.push_back(moveDown);
      break;
    case moveDown:
      possibleTurn[moveUp] = 1;
      legalTurn.push_back(moveUp);
      break;
    case moveLeft:
      possibleTurn[moveRight] = 1;
      legalTurn.push_back(moveRight);
      break;
    case moveRight:
      possibleTurn[moveLeft] = 1;
      legalTurn.push_back(moveLeft);
      break;
    default:
      cout << " oHNO ,this must not have been reached \n";
      sleep(20);
      break;
    }
  }
}

void Ghost::eliminate_vibrating_behaviour(void) {
  /*
   * We have already got possible moves in array.
   * Suppose Ghost is moving to LEFT, then we must eleminate the RIGHT
   * move whether it is valid or not.Otherwise our ghost will spend most
   * of its time vibrating .
   */

  switch (m_lastPathChoice) {
  case moveUp:
    possibleTurn[moveDown] = 0;
    break;
  case moveDown:
    possibleTurn[moveUp] = 0;
    break;
  case moveLeft:
    possibleTurn[moveRight] = 0;
    break;
  case moveRight:
    possibleTurn[moveLeft] = 0;
    break;
  }
  return;
}

void Ghost::setPossibleMoves(int tmpIndex) {
  /*
   * max four turns are possible in a give situation .so we use an
   * array of size four.for example : if ghost has a valid "up" motion,
   * then the value at the index = moveUp ( from enum Direction in ghost.h)
   * is set to 1
   *      possibleTurn[moveUp] = 1;
   * */

  std::fill(possibleTurn, possibleTurn + 4, 0);

  if (m_ptileMap->returnTileId(tmpIndex + 1) != WallId)
    possibleTurn[moveRight] = 1;
  if (m_ptileMap->returnTileId(tmpIndex + 32) != WallId)
    possibleTurn[moveDown] = 1;
  if (m_ptileMap->returnTileId(tmpIndex - 32) != WallId)
    possibleTurn[moveUp] = 1;
  if (m_ptileMap->returnTileId(tmpIndex - 1) != WallId)
    possibleTurn[moveLeft] = 1;
}

void Ghost::registerTargetPosition(int x, int y) {
  m_target_x = x;
  m_target_y = y;
  return;
}

bool Ghost::checkForCollision(int x, int y) {
  if (m_ptileMap->getTileId((x / 32) * 32, (y / 32) * 32) == WallId) {
    cout << "collision succedde\n";
    return true;
  }
  return false;
}

float Ghost::getXpos() { return m_Ghost_Shape.getPosition().x; }

float Ghost::getYpos() { return m_Ghost_Shape.getPosition().y; }

void Ghost::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(m_Ghost_Shape, states);
}

sf::Sprite Ghost::returnSpriteRect() { return m_Ghost_Shape; }

