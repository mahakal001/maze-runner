#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <unistd.h>

#include "tinyxml2.h"
#include "node_structs.h"
#include "tileMap.h"
#include "scoreElement.h"
#include "ghost.h"
#include "player.h"
#include "game.h"

using namespace std;

Game::Game()
    : m_tileMap(),
      m_window(sf::VideoMode(32 * 36, 32 * 22), "   P@CM@N     ") {
    m_player.registerWindowSize(m_window.getSize().x, m_window.getSize().y);
    m_tileMap.registerWindowSize(m_window.getSize().x, m_window.getSize().y);
    m_player.get_reference_to_tile_map(&m_tileMap);

    m_player.setPlayerStyle();
    init_ghostArmy();

    sf::Font *font = new sf::Font();
    if (!(font->loadFromFile("../resources/font/ubuntu-mono-B.ttf"))) {
        cout << " error while loading font\n";
        exit(EXIT_FAILURE);
    }

    pointersTo_fontMemory.push_back(font);

    m_curScore.set_reference_to_font(font);
    m_HighScore.set_reference_to_font(font);
    m_Bonus.set_reference_to_font(font);
    m_Lives.set_reference_to_font(font);
    m_SuperTime.set_reference_to_font(font);
    m_BadJump.set_reference_to_font(font);

    m_HighScore.init_ScoreElem(32 * 33, 32 * 1, 0, "HighScore");
    m_curScore.init_ScoreElem(32 * 33, 32 * 3, 0, "Score");
    m_Lives.init_ScoreElem(32 * 33, 32 * 5, TotalLives, "Lives");
    m_Bonus.init_ScoreElem(32 * 33, 32 * 7, 0, "Bonus");
    m_SuperTime.init_ScoreElem(32 * 33, 32 * 9, 0, "Super TIme");
    m_BadJump.init_ScoreElem(32 * 33, 32 * 11, 0, "Bad Jump Count : ");
}

void Game::run(int frameRate) {
    sf::Clock clock;
    while (m_window.isOpen()) {
        m_tileMap.designGraphOfTileMap();
        processEvents();
        update(clock.restart());
        render();
    }
}

Game::~Game()
{
    for(auto p: pointersTo_fontMemory){
        cout << "going to free it\n";
        delete(p);
    }
    for(auto q:m_ghosts){
        cout << "going to free it\n";
        delete(q);
    }

    return;
}
void Game::processEvents() {
    sf::Time delay = sf::milliseconds(80);
    sf::sleep(delay);

    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            m_window.close();
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape)
                m_window.close();
            m_player.processEvents();
        }
    }
}

void Game::update(sf::Time TimePerFrame) {
    using namespace std;
    for (auto i = 0; i < TotalGhost; i++)
        if (m_ghosts[i]->returnSpriteRect().getGlobalBounds().intersects(
                    m_player.returnSpriteRect().getGlobalBounds())) {
            m_Lives.setVal(m_Lives.getVal() - 1);

            init_ghostArmy();
            m_player.setPosition(15 * 32, 14 * 32);
            sleep(1);
            break;
        }

    m_player.update(TimePerFrame);
    if (m_player.hasTakenBadJump())
        m_BadJump.setVal(m_BadJump.getVal() + 1);

    if (m_player.hasTakenFood())
        m_curScore.setVal(m_curScore.getVal() + 10);
    else if (m_player.hasTakenPowerUp())
        m_SuperTime.setVal(m_SuperTime.getVal() + 20);

    for (auto i = 0; i < TotalGhost; i++) {
        m_ghosts[i]->registerTargetPosition(m_player.getXpos(), m_player.getYpos());
        m_ghosts[i]->update(TimePerFrame);
    }

    m_BadJump.updateElement();
    m_curScore.updateElement();
    m_SuperTime.updateElement();
    m_Lives.updateElement();
    m_tileMap.update(TimePerFrame);
}

void Game::render() {
    using namespace std;
    m_window.clear(sf::Color::White);
    m_window.draw(m_tileMap);
    m_window.draw(m_player);
    for (auto i = 0; i < TotalGhost; i++)
        m_window.draw(*m_ghosts[i]);

    cout << "goint to draw score\n";
    m_window.draw(m_curScore);
    m_window.draw(m_HighScore);
    m_window.draw(m_Lives);
    m_window.draw(m_Bonus);
    m_window.draw(m_SuperTime);
    m_window.draw(m_BadJump);

    m_window.display();
}

void Game::init_ghostArmy() {
    for (auto i = 0; i < TotalGhost; i++) {
        m_ghosts[i] = new Ghost(); // create ghosts
        m_ghosts[i]->get_reference_to_tile_map(&m_tileMap);
        m_ghosts[i]->setGhostStyle(i % 4, 0, 12, 10, 0);
        m_ghosts[i]->setSpecialAttribute(true);
    }
}

