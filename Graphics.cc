//Graphics.cc
#include "Graphics.h"
#include <SFML/Graphics.hpp>
#include "Level.h"
#include <vector>

#include <iostream>

using namespace std;

//---------INITGRAPHICS--------------//
int Graphics::initGraphics()
{
  // Inläsning av texturer
  if (!spriteSheet_background.loadFromFile("Sprites/Tiles.png"))
  {
    cerr << "Kunde inte ladda backgrounds sprites" << endl;
    return 1;
  }

  if (!spriteSheet_player.loadFromFile("Sprites/Tomato_spritesheet.png")) 
  {
    cerr << "Kunde inte ladda players sprite" << endl;
    return 1;
  }

  // Inläsning av sprites
  // door_sprite
  door_sprite.setTexture(spriteSheet_background);
  door_sprite.setTextureRect(sf::IntRect(96, 0, TILESIZE, TILESIZE));
  // block_sprite
  block_sprite.setTexture(spriteSheet_background);
  block_sprite.setTextureRect(sf::IntRect(0, 0, TILESIZE, TILESIZE));
  // start_sprite
  start_sprite.setTexture(spriteSheet_background);
  start_sprite.setTextureRect(sf::IntRect(96, 64, TILESIZE, TILESIZE));
  // goal_sprite
  goal_sprite.setTexture(spriteSheet_background);
  goal_sprite.setTextureRect(sf::IntRect(96, 32, TILESIZE, TILESIZE));
  // player_sprite
  player_sprite.setTexture(spriteSheet_player);

  // Ground
  // Innehåller vilka positioner som varje unik sprite har
  std::vector<std::pair<int, int>> groundCoordinates = {
    {32, 0},
    {0, 128},
    {32, 128},
    {64, 128},
    {0, 96},
    {32, 96},
    {64, 96},
    {0, 64},
    {32, 64},
    {64, 64},
    {0, 32},
    {32, 32},
    {64, 0},
    {64, 32},
    {96, 96},
    {96, 128}};

  // Map från 10-25 som innhåller grounds olika värden
  //std::map<int, sf::Sprite> ground_sprite;
  int groundValue = 10;
  for(auto it = groundCoordinates.begin(); it != groundCoordinates.end(); ++it)
  {
    sf::Sprite temp_sprite;
    temp_sprite.setTexture(spriteSheet_background);
    temp_sprite.setTextureRect(sf::IntRect(it->first, it->second,
					   TILESIZE, TILESIZE));
    ground_sprite[groundValue] = temp_sprite;
    ++groundValue;
  }
} 

//---------DRAWLEVEL--------------//
void Graphics::drawLevel(Level & current, sf::RenderWindow& window)
{
  vector<DrawableElement*> levelVec(current.getLevelDrawableLayout());

  // Rita ut allt utom Player
  for(unsigned int i{1}; i < levelVec.size(); i++)
  {
    string tempID = levelVec.at(i)->getElementID();
    sf::Vector2f tempPos = levelVec.at(i)->getPosition();
	
    if (tempID == "Door") 
    {
      door_sprite.setPosition(tempPos.x, tempPos.y);
      window.draw(door_sprite);
    }
    else if (tempID == "Block") 
    {
      block_sprite.setPosition(tempPos.x, tempPos.y);
      window.draw(block_sprite);
    }
    else if (tempID == "Start") 
    {
      start_sprite.setPosition(tempPos.x, tempPos.y);
      window.draw(start_sprite);
    }
    else if (tempID == "Goal") 
    {
      goal_sprite.setPosition(tempPos.x, tempPos.y);
      window.draw(goal_sprite);
    }
    // Kollar om första tecknet är G (för Ground)
    else if (tempID[0] == 'G')
    {
      int position = stoi(tempID.substr(1));
      ground_sprite[position].setPosition(tempPos.x, tempPos.y);
      window.draw(ground_sprite[position]);
    }
  }

  // Utritning av Player
    
  sf::Vector2f tempPos = levelVec.at(0)->getPosition();
  Player* player{dynamic_cast<Player*>(levelVec.at(0))};
  sf::Vector2f sprite_position = player->getAnimation();
  if (player->getDeath() == true)
  {
    player_sprite.setTextureRect(
      sf::IntRect(deathCounter*32, sprite_position.y, TILESIZE, TILESIZE));
    deathCounter++;
    if (deathCounter > 5)
      deathCounter = 0;
  }
  else
    player_sprite.setTextureRect(
      sf::IntRect(sprite_position.x, sprite_position.y, TILESIZE, TILESIZE));

  player_sprite.setPosition(tempPos.x, tempPos.y);
  if (player->getFacingRight() == false)
  {
    //Ändrar origin för korrekt spegling
    player_sprite.setOrigin(TILESIZE, 0); 
    //speglar spriten
    player_sprite.setScale(-1.0, 1); 
  }
  else
  {
    player_sprite.setOrigin(0, 0); 
    player_sprite.setScale(1, 1);
  }
  window.draw(player_sprite); 

}
