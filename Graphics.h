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

  void drawMenu(int posY, sf::RenderWindow & window);
  void drawLevel(Level & current, sf::RenderWindow & window);

  void setDeathCounter(int value) { deathCounter = value; }
  int getDeathCounter() { return deathCounter; }

private:

  const int TILESIZE{32};
  int deathCounter{0};
  sf::Sprite door_sprite;
  sf::Sprite block_sprite;
  sf::Sprite start_sprite;
  sf::Sprite goal_sprite;
  sf::Sprite player_sprite;
  std::map<int, sf::Sprite> ground_sprite;
  sf::Sprite menu_sprite;
  sf::Texture spriteSheet_background;
  sf::Texture spriteSheet_player;
  sf::Texture spriteSheet_menu;

  int initGraphics();
};
#endif
