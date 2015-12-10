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
  // Load textures
  if (!spriteSheet_background.loadFromFile("Sprites/Tiles.png"))
  {
    cerr << "Kunde inte ladda \"Sprites/Tiles.png\"" << endl;
    return 1;
  }

  if (!spriteSheet_player.loadFromFile("Sprites/Tomato_spritesheet.png")) 
  {
    cerr << "Kunde inte ladda \"Sprites/Tomato_spritesheet.png\"" << endl;
    return 1;
  }

  if (!spriteSheet_token.loadFromFile("Sprites/Tomato.png")) 
  {
    cerr << "Kunde inte ladda \"Sprites/Tomato.png\"" << endl;
    return 1;
  }

  if (!spriteSheet_menu.loadFromFile("Sprites/Menu_pause.png")) 
  {
    cerr << "Kunde inte ladda \"Sprites/Menu_pause.png\"" << endl;
    return 1;
  }

  if (!spriteSheet_levelSel.loadFromFile("Sprites/Menu_level_select.png")) 
  {
    cerr << "Kunde inte ladda \"Sprites/Menu_level_select.png\"" << endl;
    return 1;
  }

  if (!spriteSheet_victory.loadFromFile("Sprites/Menu_victory.png")) 
  {
    cerr << "Kunde inte ladda \"Sprites/Menu_victory.png\"" << endl;
    return 1;
  }

  if (!spriteSheet_victoryScreen.loadFromFile("Sprites/Victoryscreen.png")) 
  {
    cerr << "Kunde inte ladda \"Sprites/Victoryscreen.png\"" << endl;
    return 1;
  }

  if (!spriteSheet_paused.loadFromFile("Sprites/Paused.png")) 
  {
    cerr << "Kunde inte ladda sprite: \"Sprites/Paused.png\"." << endl;
    return 1;
  }

  if (!spriteSheet_story.loadFromFile("Sprites/Story.png"))
    {
      cerr << "Kunde inte ladda sprite: \"Sprites/Story.png\"." << endl;
      return 1;
    }

  // Load sprites

  // Door sprite
  door_sprite.setTexture(spriteSheet_background);
  door_sprite.setTextureRect(sf::IntRect(96, 0, TILESIZE, TILESIZE));

  // Block sprite
  block_sprite.setTexture(spriteSheet_background);
  block_sprite.setTextureRect(sf::IntRect(0, 0, TILESIZE, TILESIZE));

  // Start sprite
  start_sprite.setTexture(spriteSheet_background);
  start_sprite.setTextureRect(sf::IntRect(96, 64, TILESIZE, TILESIZE));

  // FORTSÄTT
  goal_sprite.setTexture(spriteSheet_background);
  goal_sprite.setTextureRect(sf::IntRect(96, 32, TILESIZE, TILESIZE));

  token_sprite.setTexture(spriteSheet_token);
  token_sprite.setTextureRect(sf::IntRect(96, 0, TILESIZE, TILESIZE));

  sign_sprite.setTexture(spriteSheet_story);

  keyboardMove_sprite.setTexture(spriteSheet_story);
  keyboardMove_sprite.setTextureRect(sf::IntRect(32, 96, TILESIZE, TILESIZE));

  keyboardJump_sprite.setTexture(spriteSheet_story);
  keyboardJump_sprite.setTextureRect(sf::IntRect(64, 96, TILESIZE, TILESIZE));

  menu_sprite.setTexture(spriteSheet_menu);
  menu_sprite.setTextureRect(sf::IntRect(0, 0, 768, 576));

  levelSel_sprite.setTexture(spriteSheet_levelSel);
  levelSel_sprite.setTextureRect(sf::IntRect(0, 0, 768, 576));

  victoryScreen_sprite.setTexture(spriteSheet_victoryScreen);
  victoryScreen_sprite.setTextureRect(sf::IntRect(0, 0, 768, 576));

  victory_sprite.setTexture(spriteSheet_victory);
  victory_sprite.setTextureRect(sf::IntRect(0, 0, 768, 576));

  paused_sprite.setTexture(spriteSheet_paused);
  paused_sprite.setTextureRect(sf::IntRect(0, 0, 768, 576));

  player_sprite.setTexture(spriteSheet_player);

  tomato2_sprite.setTexture(spriteSheet_story);

  // Ground
  // Vector with the texture coordinates for the different kinds of ground object 
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

  // Map connecting the id numbers for different ground objects with their sprite
  int groundValue = 10;
  for(auto it = groundCoordinates.begin(); it != groundCoordinates.end(); ++it)
  {
    sf::Sprite temp_sprite;
    temp_sprite.setTexture(spriteSheet_background);
    temp_sprite.setTextureRect(
      sf::IntRect(it->first, it->second, TILESIZE, TILESIZE));
    ground_sprite[groundValue] = temp_sprite;
    ++groundValue;
  }
}

//---------DRAW VICTORY SCREEN-----//
void Graphics::drawVictoryScreen(sf::RenderWindow& window)
{
  window.draw(victoryScreen_sprite);
}

//---------DRAW MENU--------------//
void Graphics::drawMenu(int posY, sf::RenderWindow& window, CurrentMenu currentMenu)
{
  token_sprite.setPosition(sf::Vector2f(280, posY));
  if(currentMenu == MainMenu)
    {
      window.draw(menu_sprite);
    }
  else if(currentMenu == VictoryMenu)
    {
      window.draw(victory_sprite);
    }
  else if(currentMenu == LevelMenu)
    {
      window.draw(levelSel_sprite);
    }
  window.draw(token_sprite);
}

//---------DRAW LEVEL--------------//
void Graphics::drawLevel(Level & current, sf::RenderWindow& window)
{
  vector<DrawableElement*> levelVec(current.getLevelDrawableLayout());

  Player* player{dynamic_cast<Player*>(levelVec.at(0))};

  // Draw everything but Player
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
    else if (tempID == "Tomato2") 
    {
      tomato2_sprite.setPosition(tempPos.x-32, tempPos.y);
      tomato2_sprite.setTextureRect(
	sf::IntRect(player->getStoryAnimation()*32, 0, TILESIZE, TILESIZE));
      window.draw(tomato2_sprite);
    }
    else if (tempID == "Sign") 
    {
      if(player->getStoryAnimation() < 2)
	sign_sprite.setTextureRect(sf::IntRect(0, 64, TILESIZE, TILESIZE));
      else
	sign_sprite.setTextureRect(sf::IntRect(96, 64, TILESIZE, TILESIZE));
      sign_sprite.setPosition(tempPos.x, tempPos.y);
      window.draw(sign_sprite);
    }
    else if (tempID == "KeyboardMove") 
    {
      keyboardMove_sprite.setPosition(tempPos.x, tempPos.y);
      window.draw(keyboardMove_sprite);
    }
    else if (tempID == "KeyboardJump") 
    {
      keyboardJump_sprite.setPosition(tempPos.x, tempPos.y);
      window.draw(keyboardJump_sprite);
    }
    //Check if the first character is 'G' for Ground
    else if (tempID[0] == 'G')
    {
      int position = stoi(tempID.substr(1));
      ground_sprite[position].setPosition(tempPos.x, tempPos.y);
      window.draw(ground_sprite[position]);
    }
  }

  //Draw Player
  sf::Vector2f tempPos = levelVec.at(0)->getPosition();
  sf::Vector2f sprite_position = player->getAnimation();
  if (player->getDeath() == true)
  {
    player_sprite.setTextureRect(
      sf::IntRect(getDeathCounter()*32, sprite_position.y, TILESIZE, TILESIZE));
    setDeathCounter(getDeathCounter()+1);
  }
  else
    player_sprite.setTextureRect(
      sf::IntRect(sprite_position.x, sprite_position.y, TILESIZE, TILESIZE));

  player_sprite.setPosition(tempPos.x, tempPos.y);
  if (player->getFacingRight() == false)
  {
    //Set new origin for correct mirroring
    player_sprite.setOrigin(TILESIZE, 0); 
    //Mirror the sprite
    player_sprite.setScale(-1.0, 1); 
  }
  else
  {
    player_sprite.setOrigin(0, 0); 
    player_sprite.setScale(1, 1);
  }
  window.draw(player_sprite); 

}
