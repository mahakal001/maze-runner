#include <SFML/Graphics.hpp>


#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <unistd.h>

#include "tinyxml2.h"
#include "node_structs.h"
#include "tileMap.h"
#include "player.h"

using namespace std;

Player::Player() {
  upKey = leftKey = downKey = rightKey = false;
  isMouthOpen = true;
  m_hasTakenFood = m_hasTakenPowerUp = false;
  m_lastPathChoice = moveDown;
  isBadJump = false;
}

sf::Sprite Player::returnSpriteRect() { return m_Player_Shape; }

void Player::setPlayerStyle() {
  m_Player_Shape.setTexture(m_ptileMap->m_tileset);
  m_Player_Shape.setTextureRect(sf::IntRect(40 * 4, 0, 40, 40));
  m_Player_Shape.scale(0.8, 0.8);
  m_Player_Shape.setPosition(sf::Vector2f(32 * 3, 32 * 3));
}

void Player::processEvents() {
  downKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
  upKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
  leftKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
  rightKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
}

bool Player::hasTakenBadJump() { return isBadJump; }

void Player::animatePacman() {
  if (rightKey) {
    if (isMouthOpen) {
      m_Player_Shape.setTextureRect(sf::IntRect(40 * 4, 0, 40, 40));
      isMouthOpen = false;
    } else {
      m_Player_Shape.setTextureRect(sf::IntRect(40 * 5, 0, 40, 40));
      isMouthOpen = true;
    }
  } else if (leftKey) {
    if (isMouthOpen) {
      isMouthOpen = false;
      m_Player_Shape.setTextureRect(sf::IntRect(40 * 6, 0, 40, 40));
    } else {
      m_Player_Shape.setTextureRect(sf::IntRect(40 * 7, 0, 40, 40));
      isMouthOpen = true;
    }
  } else if (upKey) {
    if (isMouthOpen) {
      isMouthOpen = false;
      m_Player_Shape.setTextureRect(sf::IntRect(40 * 8, 0, 40, 40));
    } else {
      m_Player_Shape.setTextureRect(sf::IntRect(40 * 9, 0, 40, 40));
      isMouthOpen = true;
    }
  } else if (downKey) {
    if (isMouthOpen) {
      isMouthOpen = false;
      m_Player_Shape.setTextureRect(sf::IntRect(40 * 10, 0, 40, 40));
    } else {
      m_Player_Shape.setTextureRect(sf::IntRect(40 * 11, 0, 40, 40));
      isMouthOpen = true;
    }
  }
}

void Player::update(sf::Time deltaTime) {

  int dis = 16 ;
  int x = static_cast<int> (getXpos());
  int y = static_cast<int> (getYpos());

  m_hasTakenFood = m_hasTakenPowerUp = false;
  isBadJump = false;
  float seconds = deltaTime.asSeconds();

  if (leftKey) {
    cout << "left key stille press" << endl;
    if (x - dis >= m_min_allowed_x) {
      if (checkForCollision(x + 32 - dis, y) || checkForCollision(x - dis, y) ||
          checkForCollision(x + 32 - dis, y + 31) ||
          checkForCollision(x - dis, y + 31)) {
        switch (m_lastPathChoice) {
        case moveUp:
          y -= 16;
          if (checkForCollision(x + 32 - dis, y) ||
              checkForCollision(x - dis, y) ||
              checkForCollision(x + 32 - dis, y + 31) ||
              checkForCollision(x - dis, y + 31)) {

            y -= 16;
            if (checkForCollision(x + 32 - dis, y) ||
                checkForCollision(x - dis, y) ||
                checkForCollision(x + 32 - dis, y + 31) ||
                checkForCollision(x - dis, y + 31)) {
              m_Player_Shape.setTextureRect(sf::IntRect(40 * 6, 0, 40, 40));
              leftKey = false;

              return;
            } else
              isBadJump = true;
          }
          break;
        case moveDown:
          y += 16;
          if (checkForCollision(x + 32 - dis, y) ||
              checkForCollision(x - dis, y) ||
              checkForCollision(x + 32 - dis, y + 31) ||
              checkForCollision(x - dis, y + 31)) {

            y += 16;
            if (checkForCollision(x + 32 - dis, y) ||
                checkForCollision(x - dis, y) ||
                checkForCollision(x + 32 - dis, y + 31) ||
                checkForCollision(x - dis, y + 31)) {

              m_Player_Shape.setTextureRect(sf::IntRect(40 * 6, 0, 40, 40));
              leftKey = false;
              return;
            } else
              isBadJump = true;
          }
          break;
        default:
          m_Player_Shape.setTextureRect(sf::IntRect(40 * 6, 0, 40, 40));
          leftKey = false;
          return;
        }
      }

      int tileId = m_ptileMap->getTileId(((x - dis) / 32) * 32, y);
      takeAction(tileId, x - dis, y);
      m_lastPathChoice = moveLeft;
    }
  } else if (rightKey) {
    if (x + dis < m_max_allowed_x) {

      if (checkForCollision(x + dis, y) ||
          checkForCollision(x + 32 + dis - 1, y) ||
          checkForCollision(x + dis, y + 31) ||
          checkForCollision(x + dis + 31, y + 31)) {
        switch (m_lastPathChoice) {
        case moveUp:
          y -= 16;
          if (checkForCollision(x + dis, y) ||
              checkForCollision(x + 32 + dis - 1, y) ||
              checkForCollision(x + dis, y + 31) ||
              checkForCollision(x + dis + 31, y + 31)) {
            y -= 16;
            if (checkForCollision(x + dis, y) ||
                checkForCollision(x + 32 + dis - 1, y) ||
                checkForCollision(x + dis, y + 31) ||
                checkForCollision(x + dis + 31, y + 31)) {
              m_Player_Shape.setTextureRect(sf::IntRect(40 * 4, 0, 40, 40));
              rightKey = false;
              return;
            } else
              isBadJump = true;
          }
          break;
        case moveDown:
          y += 16;
          if (checkForCollision(x + dis, y) ||
              checkForCollision(x + 32 + dis - 1, y) ||
              checkForCollision(x + dis, y + 31) ||
              checkForCollision(x + dis + 31, y + 31)) {
            y += 16;
            if (checkForCollision(x + dis, y) ||
                checkForCollision(x + 32 + dis - 1, y) ||
                checkForCollision(x + dis, y + 31) ||
                checkForCollision(x + dis + 31, y + 31)) {
              m_Player_Shape.setTextureRect(sf::IntRect(40 * 4, 0, 40, 40));
              rightKey = false;
              return;
            } else
              isBadJump = true;
          }
          break;
        default:
          m_Player_Shape.setTextureRect(sf::IntRect(40 * 4, 0, 40, 40));
          rightKey = false;
          return;
        }
      }
      int tileId = m_ptileMap->getTileId(((x + 32 + dis - 1) / 32) * 32, y);
      takeAction(tileId, x + dis, y);
      m_lastPathChoice = moveRight;
    }
  } else if (upKey) {
    if (y - dis >= 0) {
      if (checkForCollision(x, y - dis) || checkForCollision(x, y + 31 - dis) ||
          checkForCollision(x + 31, y - dis) ||
          checkForCollision(x + 31, y + 31 - dis)) {
        switch (m_lastPathChoice) {
        case moveRight:
          x += 16;
          if (checkForCollision(x, y - dis) ||
              checkForCollision(x, y + 31 - dis) ||
              checkForCollision(x + 31, y - dis) ||
              checkForCollision(x + 31, y + 31 - dis)) {
            x += 16;
            if (checkForCollision(x, y - dis) ||
                checkForCollision(x, y + 31 - dis) ||
                checkForCollision(x + 31, y - dis) ||
                checkForCollision(x + 31, y + 31 - dis)) {
              m_Player_Shape.setTextureRect(sf::IntRect(40 * 6, 0, 40, 40));
              upKey = false;
              return;
            } else
              isBadJump = true;
          }
          break;
        case moveLeft:
          x -= 16;
          if (checkForCollision(x, y - dis) ||
              checkForCollision(x, y + 31 - dis) ||
              checkForCollision(x + 31, y - dis) ||
              checkForCollision(x + 31, y + 31 - dis)) {
            x -= 16;
            if (checkForCollision(x, y - dis) ||
                checkForCollision(x, y + 31 - dis) ||
                checkForCollision(x + 31, y - dis) ||
                checkForCollision(x + 31, y + 31 - dis)) {
              m_Player_Shape.setTextureRect(sf::IntRect(40 * 6, 0, 40, 40));
              upKey = false;
              return;
            } else
              isBadJump = true;
          }
          break;
        default:
          m_Player_Shape.setTextureRect(sf::IntRect(40 * 8, 0, 40, 40));
          upKey = false;
          return;
          break;
        }
      }
      int tileId = m_ptileMap->getTileId(x, (((int)y - dis) / 32) * 32);
      takeAction(tileId, x, y - dis);
      m_lastPathChoice = moveUp;
    }
  } else if (downKey) {
    if (y + 32 <= m_max_allowed_y) {
      if (checkForCollision(x, y + dis) || checkForCollision(x, y + 31 + dis) ||
          checkForCollision(x + 31, y + dis) ||
          checkForCollision(x + 31, y + 31 + dis)) {
        switch (m_lastPathChoice) {
        case moveRight:
          x += 16;
          if (checkForCollision(x, y + dis) ||
              checkForCollision(x, y + 31 + dis) ||
              checkForCollision(x + 31, y + dis) ||
              checkForCollision(x + 31, y + 31 + dis)) {
            x += 16;
            if (checkForCollision(x, y + dis) ||
                checkForCollision(x, y + 31 + dis) ||
                checkForCollision(x + 31, y + dis) ||
                checkForCollision(x + 31, y + 31 + dis)) {
              m_Player_Shape.setTextureRect(sf::IntRect(40 * 10, 0, 40, 40));
              downKey = false;
              return;
            } else
              isBadJump = true;
          }
          break;
        case moveLeft:
          x -= 16;
          if (checkForCollision(x, y + dis) ||
              checkForCollision(x, y + 31 + dis) ||
              checkForCollision(x + 31, y + dis) ||
              checkForCollision(x + 31, y + 31 + dis)) {
            x -= 16;
            if (checkForCollision(x, y + dis) ||
                checkForCollision(x, y + 31 + dis) ||
                checkForCollision(x + 31, y + dis) ||
                checkForCollision(x + 31, y + 31 + dis)) {
              m_Player_Shape.setTextureRect(sf::IntRect(40 * 10, 0, 40, 40));
              downKey = false;
              return;
            } else
              isBadJump = true;
          }
          break;
        default:
          m_Player_Shape.setTextureRect(sf::IntRect(40 * 10, 0, 40, 40));
          downKey = false;
          return;
          break;
        }
      }

      int tileId =
          m_ptileMap->getTileId(x, (((int)y + 32 + dis - 1) / 32) * 32);
      takeAction(tileId, x, y + dis);
      m_lastPathChoice = moveDown;
    }
  }

  if (leftKey || rightKey || downKey || upKey)
    animatePacman();
}

bool Player::hasTakenFood() { return m_hasTakenFood; }

bool Player::hasTakenPowerUp() { return m_hasTakenPowerUp; }

void Player::takeAction(int tileId, int x, int y) {
  switch (tileId) {
  case L1FoodId:
    m_Player_Shape.setPosition(x, y);
    m_ptileMap->replace_LastAccessedTile_By(0);
    m_hasTakenFood = true;
    break;
  case L1powerUpId:
    m_Player_Shape.setPosition(x, y);
    m_ptileMap->replace_LastAccessedTile_By(0);
    m_hasTakenPowerUp = true;
    break;
  case 0:
    m_Player_Shape.setPosition(x, y);
    break;
  default:
    std::cout << "must not be here\n";
  }
}

bool Player::checkForCollision(int x, int y) {
  if (m_ptileMap->getTileId((x / 32) * 32, (y / 32) * 32) == WallId) {
    cout << "collision succedde\n";
    return true;
  }
  return false;
}

float Player::getXpos() { return m_Player_Shape.getPosition().x; }

float Player::getYpos() { return m_Player_Shape.getPosition().y; }

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(m_Player_Shape, states);
}
