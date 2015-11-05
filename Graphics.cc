#include <SFML/Graphics.hpp>
#include <iostream>

// Todo: Ändra inkludering då filer delas upp i .h/.cc
//#include "Elements.cc"
#include "Level.cc"
#include <vector>

using namespace std;

class Graphics
{
public:
<<<<<<< HEAD
  
  void drawLevel(Level current, sf::RenderWindow& window)
    {
      vector<DrawableElement*> levelVec(current.getLevelDrawableLayout());

      // Går detta att flytta ut på ett snyggt sätt?
      int TILESIZE = 32;

      // Dessa ska bara göras en gång
      sf::Texture spriteSheet_background;
      if (!spriteSheet_background.loadFromFile("Sprites/Tiles.png"))
	{
	  cerr << "Kunde inte ladda backgrounds sprites" << endl;
	  // Fixa felhantering! 
	  // return 1;
	}

      sf::Texture spriteSheet_player;
      if (!spriteSheet_player.loadFromFile("Sprites/Tomato.png")) 
	{
	  cerr << "Kunde inte ladda players sprite" << endl;
	  // Fixa felhantering! 
	  //return 1;
	}

      sf::Sprite player_sprite;
      player_sprite.setTexture(spriteSheet_player);
      player_sprite.setTextureRect(sf::IntRect(0, 0, TILESIZE, TILESIZE));
      sf::Sprite ground_sprite;
      ground_sprite.setTexture(spriteSheet_background);
      ground_sprite.setTextureRect(sf::IntRect(32, 0, TILESIZE, TILESIZE));
      sf::Sprite door_sprite;
      door_sprite.setTexture(spriteSheet_background);
      door_sprite.setTextureRect(sf::IntRect(96, 0, TILESIZE, TILESIZE));
      sf::Sprite block_sprite;
      block_sprite.setTexture(spriteSheet_background);
      block_sprite.setTextureRect(sf::IntRect(0, 0, TILESIZE, TILESIZE));



=======
  void drawLevel(Level & current, sf::RenderWindow& window)
    {
      vector<DrawableElement*> levelVec(current.getLevelDrawableLayout());
      sf::RectangleShape rectangle_;
      rectangle_.setSize(sf::Vector2f(32,32));
>>>>>>> 141909247685cfe1e880868fe8024e1af7453b1c
      for(unsigned int i{}; i < levelVec.size(); i++)
      {
	string tempID = levelVec.at(i)->getSpriteID();
	sf::Vector2f tempPos = levelVec.at(i)->getPosition();
	  
	if (tempID == "Player")
	{
	  player_sprite.setPosition(tempPos.x, tempPos.y);
	  window.draw(player_sprite);
	}
	else if (tempID == "Ground") 
	{
	  ground_sprite.setPosition(tempPos.x, tempPos.y);
	  window.draw(ground_sprite);
	}
	else if (tempID == "Door") 
	{
	  door_sprite.setPosition(tempPos.x, tempPos.y);
	  window.draw(door_sprite);
	}
	else if (tempID == "Block") 
	{
	  block_sprite.setPosition(tempPos.x, tempPos.y);
	  window.draw(block_sprite);
	}
      }
    }

private:


};
