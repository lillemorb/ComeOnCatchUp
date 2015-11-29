//Graphics.h
#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <SFML/Graphics.hpp>

#include "Level.h"
#include <vector>

class Graphics
{
public:
  
  int initGraphics();
  void drawLevel(Level & current, sf::RenderWindow & window);

private:

  const int TILESIZE{32};
  int deathCounter{0};
  sf::Sprite door_sprite;
  sf::Sprite block_sprite;
  sf::Sprite start_sprite;
  sf::Sprite goal_sprite;
  sf::Sprite player_sprite;
  std::map<int, sf::Sprite> ground_sprite;
  sf::Texture spriteSheet_background;
  sf::Texture spriteSheet_player;
};
#endif
