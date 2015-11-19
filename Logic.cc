//Logic.cc

#include <SFML/Graphics.hpp>
#include "Level.cc"
#include "GameSounds.cc"
#include <vector>


class Logic
{
public:
  Logic() = default;

  enum ActionResult { LevelCompleted, Dead, Reset, Continue };
  enum Move { Left, Right, Idle };
  enum Action { Jump, JumpReleased, NoJump };

  void setPix(int x, int y) { xPix_ = x; yPix_ = y;}

  ActionResult update(Level &current, Action action, Move move, sf::Clock &clock,
		      GameSounds &gamesounds)
    {
      // Hämta vektor med objekt i Level
      vector<PhysicalElement*> levelVec(current.getLevelPhysicalLayout());
 
      ActionResult result{Continue};
      sf::Time dt{clock.getElapsedTime()};
      //TODO: player ska vara playerPtr
      Player* player{dynamic_cast<Player*>(levelVec.at(0))};

      //TODO: gravity_ ska heta gravity ty inte datamedlem
      float gravity_{player->getGravity()};
      float distX{};
      float velY{player->getVelocity().y};

      if (action == Jump && player->getOnGround() == true)
	{
	  velY = -9.0f;
	  player->setOnGround(false);
	  player->setJump(true);
	  gamesounds.getJumpSound();
	}
      // Gör att man kan variera hopphöjden genom att släppa knappen tidigare
      if (action == JumpReleased)
	{
	  if(velY < -4.0f)
	    velY = -4.0f;
	}

      if (move == Left)
	{
	  distX = -4;
	  player->setFacingRight(false);
	  player->setWalk(true);
	}
      if (move == Right)
	{
	  distX = 4;
	  player->setFacingRight(true);
	  player->setWalk(true);
	}

      if (player->getOnGround() == false)
	{
	  velY += gravity_ * (dt.asMilliseconds()/10.0);
	  if (velY >= 8)
	    velY = 8.0f;
	}
      else
	{
	  player->setJump(false);
	  velY = gravity_;
	}
      if (!(move == Left || move == Right))
	{
	  player->setWalk(false);
	}

      // Förflyttningen i x-led sker statiskt istället för med acceleration.
      float y = velY*(dt.asMilliseconds()/10.0);
      player->move(sf::Vector2f(distX, round(y)));

      player->setVelocity(sf::Vector2f(distX, velY));

      // kollisionshantering spelare
      player->setOnGround(false);
      result = collisionHandlingPlayer(levelVec, gamesounds);
 
      // Uppdatera position för block ett och ett, med kollisionshantering för vardera block
      for(unsigned int i{1}; i < levelVec.size(); ++i)
      {
	float velYBlock{levelVec.at(i)->getVelocity().y};

	if(levelVec.at(i)->getSpriteID() == "Block")
	{
	  if (levelVec.at(i)->getOnGround() == false)
	  {
	    velYBlock += gravity_ * (dt.asMilliseconds()/10.0);
	    if (velYBlock >= 8)
	      velYBlock = 8.0f;
	  }
	  else
	  {
	    velYBlock = gravity_;
	  }
	  if(levelVec.at(i)->getBelowWindow() == false)
	  {
	    levelVec.at(i)->move(sf::Vector2f(levelVec.at(i)->getVelocity().x,
					      velYBlock*(dt.asMilliseconds()/10.0)));  
	    levelVec.at(i)->setVelocity(sf::Vector2f(0, velYBlock));
	  
	    //kollisionshantering
	    levelVec.at(i)->setOnGround(false);
	    collisionBlock(levelVec, i, gamesounds);
	  }
	}
      }
      // Returnera actionResult;
      return result;
    }

private:
  int xPix_{768};
  int yPix_{576};

  //Lillemor: observera att vid kollisionshantering är det alltid getGlobalBounds som måste
  //användas och inte getPosition (åtminstone för Player och Door), eftersom deras bounding box
  //inte är lika stor som en tile
  ActionResult collisionHandlingPlayer (vector<PhysicalElement*> & levelVec, GameSounds &gamesounds)
    {
      ActionResult result{Continue};
       Player* playerPtr{dynamic_cast<Player*>(levelVec.at(0))};

      //Check for collision against window borders
      sf::Vector2f playerPos = sf::Vector2f(playerPtr->getGlobalBounds().left, 
					    playerPtr->getGlobalBounds().top);
      sf::Vector2f playerSize = playerPtr->getSize();

      //Check left border
      if(playerPos.x < 0)
	playerPtr->move(sf::Vector2f(-playerPos.x,0));
      //Check right border
      else if((playerPos.x + playerSize.x) > xPix_)
	playerPtr->move(sf::Vector2f(xPix_-(playerPos.x + playerSize.x), 0));

      //Check upper border
      if(playerPos.y < 0)
	playerPtr->move(sf::Vector2f(0,-playerPos.y));
      //Check lower border and if collision return Dead
      else if(playerPos.y > yPix_)
      {
	cout << "dead" << endl;
	gamesounds.getDeathSound();
	return Dead;
      }
      // TODO: GetSpriteID ska vara getPhysicalID när den funktionen är implementerad - tänkte
      // vi förut. Vad behövs egentligen för Logic resp Graphics, tänk lite mer //Lillemor
      for(unsigned int i{1}; i < levelVec.size(); ++i)
      {
	sf::FloatRect area;
	if(playerPtr->getGlobalBounds().intersects(levelVec.at(i)->getGlobalBounds(), area))
	{
	  if(levelVec.at(i)->getSpriteID() == "Door")
	  {
	    cout << "Kollision med Door" << endl;
     	    result = LevelCompleted;
	  }
	  else if(dynamic_cast<Ground*>(levelVec.at(i)))
	  {
	    sf::Vector2f offset {0,0};

	    offset = collisionDisplacement(playerPtr, levelVec.at(i), area);
	    if(offset.y < 0)
	      playerPtr->setOnGround(true);
	    playerPtr->move(offset);
	  }
	  else if(levelVec.at(i)->getSpriteID() == "Block")
	  {
	    // Kollisionshantering i y-led, spelare flyttas, block flyttas inte
	    sf::Vector2f offset {0,0};

	    offset = collisionDisplacement(playerPtr, levelVec.at(i), area);
	    if(offset.y < 0)
	      playerPtr->setOnGround(true);

	    // If Player collided with a Block on the x-axis, that Block
	    // will get a velocity and Player will not be moved back
	    if(offset.x < 0)
	    {
	      levelVec.at(i)->setVelocity(sf::Vector2f(2, levelVec.at(i)->getVelocity().y));
	      gamesounds.getBoxSound();
	    }
	    else if(offset.x > 0)
	    {
	      levelVec.at(i)->setVelocity(sf::Vector2f(-2, levelVec.at(i)->getVelocity().y));
	      gamesounds.getBoxSound();
	    }
	    offset.x = 0;

	    playerPtr->move(offset);
	  }
	}
      }
      return result;
    }

  void collisionBlock(vector<PhysicalElement*> & levelVec, unsigned int vecLoc, GameSounds gamesounds)
    {
      sf::FloatRect area;
      Player* playerPtr{dynamic_cast<Player*>(levelVec.at(0))};

      //Check for collision against window borders
      sf::Vector2f blockPos = levelVec.at(vecLoc)->getPosition();
      sf::Vector2f blockSize = levelVec.at(vecLoc)->getSize();

      //Check left border
      if(blockPos.x < 0)
	levelVec.at(vecLoc)->move(sf::Vector2f(-blockPos.x,0));
      //Check right border
      else if((blockPos.x + blockSize.x) > xPix_)
	levelVec.at(vecLoc)->move(sf::Vector2f(xPix_-(blockPos.x + blockSize.x), 0));

      //Check upper border
      if(blockPos.y < 0)
	levelVec.at(vecLoc)->move(sf::Vector2f(-blockPos.y,0));
      //Check lower border and if collision set velocity 0,0
      else if(blockPos.y > yPix_+33)
  	levelVec.at(vecLoc)->setBelowWindow(true);

      for(unsigned int i{1}; i < levelVec.size(); ++i)
      {
	// Första villkoret kollar att man inte försöker kollisionshantera kollision med sig själv
	if( levelVec.at(vecLoc) != levelVec.at(i) &&
	    levelVec.at(vecLoc)->getGlobalBounds().intersects(levelVec.at(i)->getGlobalBounds(), area))
	{
	  //Lillemor: Kollar mot första bokstaven för olika Ground-objekt
	  if(levelVec.at(i)->getSpriteID().at(0) == 'G' || levelVec.at(i)->getSpriteID() == "Block")
	  {
	    sf::Vector2f offset {0,0};

	    offset = collisionDisplacement(levelVec.at(vecLoc), levelVec.at(i), area);
	    if(offset.y < 0)
	      levelVec.at(vecLoc)->setOnGround(true);
	    levelVec.at(vecLoc)->move(offset);
	  }	
	}
      }
      //Flytta tillbaka spelare om Block:et inte gick att flytta
      if(playerPtr->getGlobalBounds().intersects(levelVec.at(vecLoc)->getGlobalBounds(), area))
      {
	sf::Vector2f offset {0,0};

	// Lillemor: Obs, kollar i både x- och y-led, behöver kanske  bara kolla x-led...
	offset = collisionDisplacement(playerPtr, levelVec.at(vecLoc), area);
	if(offset.y < 0)
	  playerPtr->setOnGround(true);
	playerPtr->move(offset);
      } 
    }	  

  sf::Vector2f collisionDisplacement(PhysicalElement* element, PhysicalElement* collidingElement,
				     sf::FloatRect area)
    {
      sf::Vector2f offset{0,0};
      PhysicalElement::CollisionBorders collBorders(collidingElement->getCollisionBorders());
      bool up{collBorders.up};
      bool down{collBorders.down};
      bool left{collBorders.left};
      bool right{collBorders.right};

      if ((up || down) && area.width > area.height)
      {
	if (area.contains({ area.left, element->getGlobalBounds().top }))
	{
	  // Up side crash => move player down
	  if(down)
	    {
	    offset.y = area.height;

	    /*
	      // Rasmus: Ifall man vill skippa att man glider i taket används följande
	      // Nackdelen är att man hoppar efteråt om man inte släpper hopp-knappen
	    if(element->getVelocity().y < -2.0f)
	      element->setVelocity(sf::Vector2f(0, -2.0f));
	    */
	    }
	}
	else
	{
	  // Down side crash => move player back up
	  if(up)
	    offset.y = -area.height;
	}
      }
      else if ((left || right) && area.width < area.height)
      {
	if (area.contains( element->getGlobalBounds().left + 
			   element->getGlobalBounds().width - 1.f, area.top + 1.f ))
	{
	  //Right side crash
	  if(left)
	    offset.x = -area.width;
	}
	else
	{
	  //Left side crash
	  if(right)
	    offset.x = area.width;
	}
      }  
      return offset;
    }  

};



