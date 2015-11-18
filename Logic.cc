//Logic.cc

#include <SFML/Graphics.hpp>
#include "Level.cc"
#include <vector>

class Logic
{
public:
  Logic() = default;

  enum ActionResult { LevelCompleted, Dead, Reset, Continue };
  enum Move { Left, Right, Idle };
  enum Action { Jump, JumpReleased, NoJump };

  void setPix(int x, int y) { xPix_ = x; yPix_ = y;}

  ActionResult update(Level &current, Action action, Move move, sf::Clock &clock)
    {
      // Hämta vektor med objekt i Level
      vector<PhysicalElement*> levelVec(current.getLevelPhysicalLayout());
 
      ActionResult result{Continue};
      sf::Time dt{clock.getElapsedTime()};

      Player* player{dynamic_cast<Player*>(levelVec.at(0))};

      float gravity_{player->getGravity()};
      // 
      float distX{};
      float velY{player->getVelocity().y};

    
      if (action == Jump && player->getOnGround() == true)
      {
	velY = -9.0f;
	player->setOnGround(false);
      }
      if (action == JumpReleased)
      {
	// Gör att man kan variera hopphöjden genom att släppa knappen tidigare
	if(velY < -4.0f)
	  velY = -4.0f;
      }

      if (move == Left)
      {
	distX = -4;
	player->setFacingRight(false);
      }
      if (move == Right)
      {
	distX = 4;
	player->setFacingRight(true);
      }

      //Rasmus: Denna bör alltid gälla, men man ramlar igenom världen ibland om den är igång alltid.
      if (player->getOnGround() == false)
      {
	velY += gravity_ * (dt.asMilliseconds()/10.0);
      }
      // Lillemor: nedanstående problem fixat med ny kollisionshantering
      // Rasmus: Nuvarande problemet är att y-värdet kan lyckas sättas till ett värde som är
      // nedanför Block:ens 
      // topp, vilket strular till det för movement i sidled. Denna fixen bör ske i collisionhandling?

      // Förflyttningen i x-led sker statiskt istället för med acceleration.
 
      player->move(sf::Vector2f(distX, round(velY*(dt.asMilliseconds()/10.0))));
      //TODO: kolla om man verkligen ska sätta något i x-led
      player->setVelocity(sf::Vector2f(distX, velY));

      // kollisionshantering spelare

      // TODO: Ska denna verkligen sättas här? Undersök.
      levelVec.at(0)->setOnGround(false);

      result = collisionHandling(levelVec);
 
      // Uppdatera position för block ett och ett, med kollisionshantering för vardera block
      for(unsigned int i{1}; i < levelVec.size(); ++i)
      {
	sf::Vector2f noVel{0,0};
	if (levelVec.at(i)->getSpriteID() == "Block" && 
	    levelVec.at(i)->getVelocity() != noVel) 
	{
	  // Uppdatera blockets position
	  levelVec.at(i)->move(levelVec.at(i)->getVelocity());
	  levelVec.at(i)->setVelocity(noVel);
	  //kollisionshantering
	  collisionBlock(levelVec, i);
	}
      }
      // Returnera actionResult;
      return result;
    }
  

private:
  //Lillemor: default-värden, men sätts om med funktionen setPix()
  int xPix_{768};
  int yPix_{576};

  //Lillemor: observera att vid kollisionshantering är det alltid getGlobalBounds som måste
  //användas och inte getPosition (åtminstone för Player och Door), eftersom deras bounding box
  //inte är lika stor som en tile
  ActionResult collisionHandling (vector<PhysicalElement*> & levelVec)
    {
      ActionResult result = Continue;
       Player* player{dynamic_cast<Player*>(levelVec.at(0))};

      // Lillemor: uppdaterat så att kollision mot fönstrets gränser sker mot
      // players bounding box ist f players tilesize
      sf::Vector2f playerPos = sf::Vector2f(player->getGlobalBounds().left, 
					    player->getGlobalBounds().top);
      sf::Vector2f playerSize = sf::Vector2f(player->getGlobalBounds().width, 
					    player->getGlobalBounds().height);

      //Check for collision against window borders

      //Check left border
      if(playerPos.x < 0)
	player->move(sf::Vector2f(-playerPos.x,0));
      //Check right border
      else if((playerPos.x + playerSize.x) > xPix_)
	player->move(sf::Vector2f(xPix_-(playerPos.x + playerSize.x), 0));

      //Check upper border
      if(playerPos.y < 0)
	player->move(sf::Vector2f(-playerPos.y,0));
      //Check lower border and if collision return Dead
      else if(playerPos.y > yPix_)
      {
	cout << "dead" << endl;
	return Dead;
      }
      // TODO: GetSpriteID ska vara getPhysicalID när den funktionen är implementerad - tänkte
      // vi förut. Vad behövs egentligen för Logic resp Graphics, tänk lite mer //Lillemor
      for(unsigned int i{1}; i < levelVec.size(); ++i)
      {
	sf::FloatRect area;
	if(player->getGlobalBounds().intersects(levelVec.at(i)->getGlobalBounds(), area))
	{
	  if(levelVec.at(i)->getSpriteID() == "Door")
	  {
	    cout << "Kollision med Door" << endl;

	    // Lillemor: om LevelCompleted returneras faller Player halvt genom golvet vid 
	    // kollision med Door vilket kan ses innan nästa bana laddas. 
     	    result = LevelCompleted;
	    // return LevelCompleted;
	  }
	  else if(dynamic_cast<Ground*>(levelVec.at(i)))
	  {
	    sf::Vector2f offset {0,0};
	    string tempSpriteID{levelVec.at(i)->getSpriteID()};	    
	    //PÅBÖRJAT: ny kollisionshantering
	    //Lillemor: har städat undan kollision vänster-höger, upp-ned i mindre funktioner

	    //Ground med nedre gräns, flytta ned
	    if(levelVec.at(i)->getSpriteID() == "G12")
	      offset.y = area.height;	
	    //Ground med vänster gräns, flytta vänster
	    else if(levelVec.at(i)->getSpriteID() == "G14")
	    {
	      offset.x = -area.width;
	    }
	    //Ground med höger gräns, flytta höger
	    else if(levelVec.at(i)->getSpriteID() == "G16")
	    {
	      offset.x = area.width;
	    }
	    //Ground med övre gräns, flytta upp
	    else if(levelVec.at(i)->getSpriteID() == "G18")
	    {
	      //Lillemor: måste komma ihåg att sätta setOnGround(true) då Player står på mark!!
	      player->setOnGround(true);
	      offset.y = -area.height;
	    }
	    //Mitten, inga kanter, gör inget. Lillemor: ska nu inte vara med i vektorn med
	    //PhysicalElements
	    // else if(levelVec.at(i)->getSpriteID() == "G15")
	    // {}

	    // Lillemor: Player fastnade vid gång från höger till vänster om inte denna sats
	    // fanns med
	    else if( tempSpriteID == "G25") 
	    {
	      offset = collisionUpDown(player, area);
	      if(offset.y < 0)
		player->setOnGround(true);
	    }
	    else
	    {
	      //Måste ha noggrannare villkor för att inte få problem med att
	      //player fastnar vid gång höger->vänster mot vissa block
	      if (area.width > area.height)
	      {
		offset = collisionUpDown(player, area);
		if(offset.y < 0)
		  player->setOnGround(true);
	      }
	      else if (area.width < area.height)
	      {
		offset = collisionLeftRight(player, area);
	      }
	    }
	    player->move(offset);
	
	    // Lillemor: om kollision med Door ska man inte fortsätta med denna bana
	    if (result != LevelCompleted)
	      result = Continue;
	  }
	  else if(levelVec.at(i)->getSpriteID() == "Block")
	  {
	    // Kollisionshantering i y-led, spelare flyttas, block ändras inte
	    sf::Vector2f offset {0,0};
	    if (area.width > area.height)
	    {
/*
	      if (area.contains({ area.left, player->getPosition().y }))
	      {
		// Up side crash => move player down
		offset.y = area.height;	  
	      }
	      else
	      {
		// Down side crash => move player back up
		player->setOnGround(true);
		offset.y = -area.height;
	      }
*/
	      offset = collisionUpDown(player, area);
	      if(offset.y < 0)
		player->setOnGround(true);	      
	      player->move(offset);
	    }
	    else if (area.width < area.height)
	    {
	      // Kollisionshantering i x-led, blocket får uppdaterad hastighet, spelare flyttas inte
/*
	      if (area.contains( player->getGlobalBounds().left + 
				 player->getGlobalBounds().width - 1.f, area.top + 1.f ))
	      {
		//Right side crash
		levelVec.at(i)->setVelocity(sf::Vector2f(1,0));
	      }
	      else
	      {
		//Left side crash
		levelVec.at(i)->setVelocity(sf::Vector2f(-1,0));
	      }
*/
	      offset = collisionLeftRight(player, area);
	      // Kollisionshantering i x-led, blocket får uppdaterad hastighet,
	      // spelare flyttas inte
	      if(offset.x < 0)
		levelVec.at(i)->setVelocity(sf::Vector2f(1,0));
	      else if(offset.x > 0)
		levelVec.at(i)->setVelocity(sf::Vector2f(-1,0));
	    }	
	    result = Continue;
	  }
	}
      }
      return result;
    }
  

  void collisionBlock(vector<PhysicalElement*> & levelVec, unsigned int vecLoc)
    {
      sf::FloatRect area;
       Player* player{dynamic_cast<Player*>(levelVec.at(0))};

      for(unsigned int i{1}; i < levelVec.size(); ++i)
      {
	if(levelVec.at(vecLoc)->getGlobalBounds().intersects(levelVec.at(i)->getGlobalBounds(), area))
	{
	  //Lillemor: Kollar mot första bokstaven för olika Ground-objekt
	  if(levelVec.at(i)->getSpriteID().at(0) == 'G' || levelVec.at(i)->getSpriteID() == "Block")
	  {
	    sf::Vector2f offset {0,0};
	    if (area.width > area.height)
	    {
// Lillemor: det borkommenterade har ersatts med kortare hjälpfunktioner, samma
// har gjort vid kollision mot Ground ovan
/*
	      if (area.contains({ area.left, levelVec.at(vecLoc)->getPosition().y }))
	      {
		// Up side crash => move block down
		offset.y = area.height;		      
	      }
	      else
	      {
		// Down side crash => move block back up
		levelVec.at(vecLoc)->setOnGround(true);
		offset.y = -area.height;
	      }
*/
	      offset = collisionUpDown(levelVec.at(vecLoc), area);
	      if(offset.y < 0)
		levelVec.at(vecLoc)->setOnGround(true);
	    }
	    else if (area.width < area.height)
	    {
/*
	      if (area.contains( levelVec.at(vecLoc)->getGlobalBounds().left + 
				 levelVec.at(vecLoc)->getGlobalBounds().width - 1.f, area.top + 1.f ))
	      {
		//Right side crash
		offset.x = -area.width;
	      }
	      else
	      {
		//Left side crash
		offset.x = area.width;
	      }
*/
	      offset = collisionLeftRight(levelVec.at(vecLoc), area);
	    }
	    levelVec.at(vecLoc)->move(offset);
	  }	
	}
      }
      //Flytta ev tillbaka spelare, OBS kollar bara höger och vänster för aktuellt block och spelaren
      if(player->getGlobalBounds().intersects(levelVec.at(vecLoc)->getGlobalBounds(), area))
      {
	sf::Vector2f offset {0,0};
/*
	if (area.width < area.height)
	{
	  if (area.contains( player->getGlobalBounds().left
			     + player->getGlobalBounds().width - 1.f, area.top + 1.f ))
	  {
	    //Right side crash
	    offset.x = -area.width;
	  }
	  else
	  {
	    //Left side crash
	    offset.x = area.width;
	  }
	}
*/
	offset = collisionLeftRight(player, area);
	player->move(offset);
      } 
    }	  
  
  // Helper function, checks for collision up/down and returns resulting displacement
  sf::Vector2f collisionUpDown(PhysicalElement* element, sf::FloatRect area)
    {
      sf::Vector2f offset(0,0);
      if (area.contains({ area.left, element->getPosition().y }))
      {
	// Up side crash => move element down
	offset.y = area.height;		      
      }
      else
      {
	// Down side crash => move element back up
	offset.y = -area.height;
      }
      return offset;
    } 

  // Helper function, checks for collision left/right and returns resulting displacement
  sf::Vector2f collisionLeftRight(PhysicalElement* element, sf::FloatRect area)
    {
      sf::Vector2f offset(0,0);
      // Lillemor: uppdaterat så att kollision fungerar med eventuellt mindre bounding box
      if (area.contains( element->getGlobalBounds().left + 
			 element->getGlobalBounds().width - 1.f, area.top + 1.f ))
      {
	//Right side crash
	offset.x = -area.width;
      }
      else
      {
	//Left side crash
	offset.x = area.width;
      }
      return offset;
    }

};



