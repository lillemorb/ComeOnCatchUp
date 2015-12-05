//Graphics.h
#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <SFML/Graphics.hpp>

#include "Level.h"
#include <vector>

class Graphics
{
public:
  Graphics() { initGraphics(); }
  ~Graphics() = default;

  enum CurrentMenu{MainMenu, VictoryMenu, LevelMenu};

  void drawVictoryScreen(sf::RenderWindow & window);
  void drawMenu(int posY, sf::RenderWindow & window, CurrentMenu currentMenu);
  void drawLevel(Level & current, sf::RenderWindow & window);
  void drawPaused(sf::RenderWindow & window)
  {
    window.draw(paused_sprite);
  }

  void setDeathCounter(int value) { deathCounter = value; }
  int getDeathCounter() { return deathCounter; }

private:
  Graphics(const Graphics&) = delete;
  Graphics& operator=(const Graphics&) = delete;

  const int TILESIZE{32};
  int deathCounter{0};
  sf::Sprite door_sprite;
  sf::Sprite block_sprite;
  sf::Sprite start_sprite;
  sf::Sprite goal_sprite;
  sf::Sprite player_sprite;
  std::map<int, sf::Sprite> ground_sprite;
  sf::Sprite token_sprite;
  sf::Sprite menu_sprite;
  sf::Sprite victory_sprite;
  sf::Sprite victoryScreen_sprite;
  sf::Sprite paused_sprite;
  sf::Sprite tomato2_sprite;
  sf::Sprite keyboardJump_sprite;
  sf::Sprite keyboardMove_sprite;
  sf::Sprite sign_sprite;
  sf::Texture spriteSheet_background;
  sf::Texture spriteSheet_player;
  sf::Texture spriteSheet_token;
  sf::Texture spriteSheet_menu;
  sf::Texture spriteSheet_victory;
  sf::Texture spriteSheet_victoryScreen;
  sf::Texture spriteSheet_paused;
  sf::Texture spriteSheet_story;

  int initGraphics();
};
#endif
