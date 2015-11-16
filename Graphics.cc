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
  
  void drawLevel(Level & current, sf::RenderWindow& window)
  {
    vector<DrawableElement*> levelVec(current.getLevelDrawableLayout());

    // Går detta att flytta ut på ett snyggt sätt?
    int TILESIZE = 32;


    // Dessa ska bara göras en gång
    // Inläsning av texturer
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


    // Inläsning av sprites
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
    sf::Sprite start_sprite;
    start_sprite.setTexture(spriteSheet_background);
    start_sprite.setTextureRect(sf::IntRect(96, 64, TILESIZE, TILESIZE));
    sf::Sprite goal_sprite;
    goal_sprite.setTexture(spriteSheet_background);
    goal_sprite.setTextureRect(sf::IntRect(96, 32, TILESIZE, TILESIZE));
    
    // Rasmus: Försökte göra det smidigare men lyckades inte. 
    // Går att ha en funktion för inläsningen, men raderna blir 
    // långa och man spar in en rad per del. 

    // Ground
    sf::Sprite g10_sprite;
    g10_sprite.setTexture(spriteSheet_background);
    g10_sprite.setTextureRect(sf::IntRect(32, 0, TILESIZE, TILESIZE));
    sf::Sprite g11_sprite;
    g11_sprite.setTexture(spriteSheet_background);
    g11_sprite.setTextureRect(sf::IntRect(0, 128, TILESIZE, TILESIZE));
    sf::Sprite g12_sprite;
    g12_sprite.setTexture(spriteSheet_background);
    g12_sprite.setTextureRect(sf::IntRect(32, 128, TILESIZE, TILESIZE));
    sf::Sprite g13_sprite;
    g13_sprite.setTexture(spriteSheet_background);
    g13_sprite.setTextureRect(sf::IntRect(64, 128, TILESIZE, TILESIZE));
    sf::Sprite g14_sprite;
    g14_sprite.setTexture(spriteSheet_background);
    g14_sprite.setTextureRect(sf::IntRect(0, 96, TILESIZE, TILESIZE));
    sf::Sprite g15_sprite;
    g15_sprite.setTexture(spriteSheet_background);
    g15_sprite.setTextureRect(sf::IntRect(32, 96, TILESIZE, TILESIZE));
    sf::Sprite g16_sprite;
    g16_sprite.setTexture(spriteSheet_background);
    g16_sprite.setTextureRect(sf::IntRect(64, 96, TILESIZE, TILESIZE));
    sf::Sprite g17_sprite;
    g17_sprite.setTexture(spriteSheet_background);
    g17_sprite.setTextureRect(sf::IntRect(0, 64, TILESIZE, TILESIZE));
    sf::Sprite g18_sprite;
    g18_sprite.setTexture(spriteSheet_background);
    g18_sprite.setTextureRect(sf::IntRect(32, 64, TILESIZE, TILESIZE));
    sf::Sprite g19_sprite;
    g19_sprite.setTexture(spriteSheet_background);
    g19_sprite.setTextureRect(sf::IntRect(64, 64, TILESIZE, TILESIZE));
    sf::Sprite g20_sprite;
    g20_sprite.setTexture(spriteSheet_background);
    g20_sprite.setTextureRect(sf::IntRect(0, 32, TILESIZE, TILESIZE));
    sf::Sprite g21_sprite;
    g21_sprite.setTexture(spriteSheet_background);
    g21_sprite.setTextureRect(sf::IntRect(32, 32, TILESIZE, TILESIZE));
    sf::Sprite g22_sprite;
    g22_sprite.setTexture(spriteSheet_background);
    g22_sprite.setTextureRect(sf::IntRect(64, 0, TILESIZE, TILESIZE));
    sf::Sprite g23_sprite;
    g23_sprite.setTexture(spriteSheet_background);
    g23_sprite.setTextureRect(sf::IntRect(64, 32, TILESIZE, TILESIZE));
    
    // De följande är inte inlagda i bilden ännu
    sf::Sprite g24_sprite;
    g24_sprite.setTexture(spriteSheet_background);
    g24_sprite.setTextureRect(sf::IntRect(96, 96, TILESIZE, TILESIZE));
    sf::Sprite g25_sprite;
    g25_sprite.setTexture(spriteSheet_background);
    g25_sprite.setTextureRect(sf::IntRect(96, 128, TILESIZE, TILESIZE));


    // Rita ut allt utom Player
    for(unsigned int i{1}; i < levelVec.size(); i++)
      {
	string tempID = levelVec.at(i)->getSpriteID();
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
        else if (tempID == "Ground" || tempID == "G10")
	  {
	    g10_sprite.setPosition(tempPos.x, tempPos.y);
	    window.draw(g10_sprite);
	  }
	else if (tempID == "G11") 
	  {
	    g11_sprite.setPosition(tempPos.x, tempPos.y);
	    window.draw(g11_sprite);
	  }
	else if (tempID == "G12") 
	  {
	    g12_sprite.setPosition(tempPos.x, tempPos.y);
	    window.draw(g12_sprite);
	  }
	else if (tempID == "G13") 
	  {
	    g13_sprite.setPosition(tempPos.x, tempPos.y);
	    window.draw(g13_sprite);
	  }	
	else if (tempID == "G14") 
	  {
	    g14_sprite.setPosition(tempPos.x, tempPos.y);
	    window.draw(g14_sprite);
	  }	
	else if (tempID == "G15") 
	  {
	    g15_sprite.setPosition(tempPos.x, tempPos.y);
	    window.draw(g15_sprite);
	  }	
	else if (tempID == "G16") 
	  {
	    g16_sprite.setPosition(tempPos.x, tempPos.y);
	    window.draw(g16_sprite);
	  }	
	else if (tempID == "G17") 
	  {
	    g17_sprite.setPosition(tempPos.x, tempPos.y);
	    window.draw(g17_sprite);
	  }	
	else if (tempID == "G18") 
	  {
	    g18_sprite.setPosition(tempPos.x, tempPos.y);
	    window.draw(g18_sprite);
	  }	
	else if (tempID == "G19") 
	  {
	    g19_sprite.setPosition(tempPos.x, tempPos.y);
	    window.draw(g19_sprite);
	  }	
	else if (tempID == "G20") 
	  {
	    g20_sprite.setPosition(tempPos.x, tempPos.y);
	    window.draw(g20_sprite);
	  }	
	else if (tempID == "G21") 
	  {
	    g21_sprite.setPosition(tempPos.x, tempPos.y);
	    window.draw(g21_sprite);
	  }	
	else if (tempID == "G22") 
	  {
	    g22_sprite.setPosition(tempPos.x, tempPos.y);
	    window.draw(g22_sprite);
	  }
	else if (tempID == "G23") 
	  {
	    g23_sprite.setPosition(tempPos.x, tempPos.y);
	    window.draw(g23_sprite);
	  }
	else if (tempID == "G24") 
	  {
	    g24_sprite.setPosition(tempPos.x, tempPos.y);
	    window.draw(g24_sprite);
	  }
	else if (tempID == "G25") 
	  {
	    g25_sprite.setPosition(tempPos.x, tempPos.y);
	    window.draw(g25_sprite);
	  }
      }

    // Utritning av Player
	sf::Vector2f tempPos = levelVec.at(0)->getPosition();
	player_sprite.setPosition(tempPos.x, tempPos.y);
	Player* player{dynamic_cast<Player*>(levelVec.at(0))};
	if (player->getFacingRight() == false)
	  {
	    //Ändrar origin för korrekt spegling
	    player_sprite.setOrigin(TILESIZE, 0); 
	    //speglar spriten
	    player_sprite.setScale(-1.0, 1); 
	  }
	window.draw(player_sprite);
      
  }

};
