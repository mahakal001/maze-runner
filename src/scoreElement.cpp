#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <iostream>

#include "scoreElement.h"

ScoreElement::ScoreElement() {
  std::cout << " i am score Element constructor \n";
}

void ScoreElement::init_ScoreElem(int x, int y, int value, std::string str) {
  x_pos = x;
  y_pos = y;
  m_fixStr = str;

  std::cout << m_fixStr << std::endl;

  m_text.setFont(*m_pFont);
  m_text.setPosition(x_pos, y_pos);
  m_text.setOutlineColor(sf::Color(255, 126, 214));
  m_text.setFillColor(sf::Color::Black);
  m_text.setOutlineThickness(2);
  m_text.setCharacterSize(30);
  m_value = value;

  updateElement();
}

void ScoreElement::setVal(int val) { m_value = val; }

int ScoreElement::getVal(void) { return m_value; }

void ScoreElement::set_reference_to_font(sf::Font *pFont) { m_pFont = pFont; }

void ScoreElement::updateElement() {
  m_text.setString(m_fixStr + "\n " + std::to_string(m_value));
}

void ScoreElement::draw(sf::RenderTarget &target,
                        sf::RenderStates states) const {
  target.draw(m_text);
}
