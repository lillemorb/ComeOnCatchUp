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

  ActionResult update(Level & current, Action action, Move move, sf::Clock & clock)
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
	velY += gravity_ * (dt.asMilliseconds()/10);
      }

    // Rasmus: Nuvarande problemet är att y-värdet kan lyckas sättas till ett värde som är nedanför Block:ens 
    // topp, vilket strular till det för movement i sidled. Denna fixen bör ske i collisionhandling?
    // Förflyttningen i x-led sker statiskt istället för med acceleration.
    
    player->move(sf::Vector2f(distX, round(velY*(dt.asMilliseconds()/10))));
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
  int xPix_{768};
  int yPix_{576};


  ActionResult collisionHandling (vector<PhysicalElement*> & levelVec)
  {
    ActionResult result = Continue;
    
    sf::Vector2f playerPos = levelVec.at(0)->getPosition();
    //Lillemor: kollisionshantering mot fönstrets gränser
    //Kolla mot vänster
    if(playerPos.x < 0)
      levelVec.at(0)->move(sf::Vector2f(-playerPos.x,0));
      //Kolla mot höger
    if((playerPos.x + levelVec.at(0)->getSize().x) > xPix_)
      levelVec.at(0)->move(sf::Vector2f(xPix_-(playerPos.x + levelVec.at(0)->getSize().x), 0));
    //Kolla uppåt
    if(playerPos.y < 0)
      levelVec.at(0)->move(sf::Vector2f(-playerPos.y,0));
    //Kolla nedåt och returnera död
    if(playerPos.y > yPix_)
      {
	cout << "dead" << endl;
	return Dead;
      }

    for(unsigned int i{1}; i < levelVec.size(); ++i)
      {
	sf::FloatRect area;
	if(levelVec.at(0)->getGlobalBounds().intersects(levelVec.at(i)->getGlobalBounds(), area))
	{
	  if(levelVec.at(i)->getSpriteID() == "Door")
	  {
	    cout << "Kollision med Door" << endl;

	    //Lillemor: Kolla vilken av följande rader som är korrekt.
	    //Bortkommenterad tills vidare för att slippa banbyte.
//	    result = LevelCompleted;

//	    return LevelCompleted;
	  }
	  else if(levelVec.at(i)->getSpriteID() == "Ground")
	  {
	    sf::Vector2f offset {0,0};
	    if (area.width > area.height)
	    {
	      if (area.contains({ area.left, levelVec.at(0)->getPosition().y }))
	      {
		// Up side crash => move player down
		offset.y = area.height;		      
	      }
	      else
	      {
		// Down side crash => move player back up
		levelVec.at(0)->setOnGround(true);
		offset.y = -area.height;
	      }
	    }
	    else if (area.width < area.height)
	    {
	      if (area.contains( levelVec.at(0)->getPosition().x + 
				 levelVec.at(0)->getGlobalBounds().width - 1.f, area.top + 1.f ))
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
	    levelVec.at(0)->move(offset);
	
	    result = Continue;
	  }
	  else if(levelVec.at(i)->getSpriteID() == "Block")
	  {
	    // Kollisionshantering i y-led, spelare flyttas, block ändras inte
	    sf::Vector2f offset {0,0};
	    if (area.width > area.height)
	    {
	      if (area.contains({ area.left, levelVec.at(0)->getPosition().y }))
	      {
		// Up side crash => move player down
		offset.y = area.height;	  
	      }
	      else
	      {
		// Down side crash => move player back up
		levelVec.at(0)->setOnGround(true);
		offset.y = -area.height;
	      }
	      levelVec.at(0)->move(offset);
	    }
	    else if (area.width < area.height)
	    {
	      // Kollisionshantering i x-led, blocket får uppdaterad hastighet, spelare flyttas inte
	      if (area.contains( levelVec.at(0)->getPosition().x + 
				 levelVec.at(0)->getGlobalBounds().width - 1.f, area.top + 1.f ))
	      {
		//Right side crash
		levelVec.at(i)->setVelocity(sf::Vector2f(1,0));
	      }
	      else
	      {
		//Left side crash
		levelVec.at(i)->setVelocity(sf::Vector2f(-1,0));
	      }
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
    for(unsigned int i{1}; i < levelVec.size(); ++i)
      {
	if(levelVec.at(vecLoc)->getGlobalBounds().intersects(levelVec.at(i)->getGlobalBounds(), area))
	  {
	    if(levelVec.at(i)->getSpriteID() == "Ground" || levelVec.at(i)->getSpriteID() == "Block")
	      {
		sf::Vector2f offset {0,0};
		if (area.width > area.height)
		  {
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
		  }
		else if (area.width < area.height)
		  {
		    if (area.contains( levelVec.at(vecLoc)->getPosition().x + 
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
		  }
		levelVec.at(vecLoc)->move(offset);
	      }	
	  }
      }
    //Flytta ev tillbaka spelare, OBS kollar bara höger och vänster för aktuellt block och spelaren
    if(levelVec.at(0)->getGlobalBounds().intersects(levelVec.at(vecLoc)->getGlobalBounds(), area))
      {
	sf::Vector2f offset {0,0};
	if (area.width < area.height)
	  {
	    if (area.contains( levelVec.at(0)->getPosition().x
			       + levelVec.at(0)->getGlobalBounds().width - 1.f, area.top + 1.f ))
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
	levelVec.at(0)->move(offset);
      } 
  }	  
 
};
