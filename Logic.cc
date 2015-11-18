//Logic.cc

#include <SFML/Graphics.hpp>
#include "Level.cc"
#include <vector>
#include <SFML/Audio.hpp>
//#include <SFML/Sound.hpp>

class Logic
{
public:
  Logic() = default;

  enum ActionResult { LevelCompleted, Dead, Reset, Continue };
  enum Move { Left, Right, Idle };
  enum Action { Jump, JumpReleased, NoJump };

  void setPix(int x, int y) { xPix_ = x; yPix_ = y;}


  /*
  sf::SoundBuffer jump_soundBuffer;
  if (!jump_soundBuffer.loadFromFile("Sounds/Player_jump.wav"))
    {
      cerr << "Kunde inte ladda Player_jump.wav" << endl;
      // Fixa felhantering
      // return 1;
    }
  sf::Sound jump_sound;
  jump_sound.SetBuffer(jump_soundBuffer);
  
  sf::SoundBuffer box_soundBuffer;
    if (!box_soundBuffer.loadFromFile("Sounds/Box_push.wav"))
    {
      cerr << "Kunde inte ladda Box_push.wav" << endl;
      // Fixa felhantering
      // return 1;
    }
  sf::Sound box_sound;
  box_sound.SetBuffer(box_soundBuffer);
  */


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
	  player->setJump(true);
	  //jump_sound.Play();
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
	  player->setWalk(true);
	}
      if (move == Right)
	{
	  distX = 4;
	  player->setFacingRight(true);
	  player->setWalk(true);
	}


      //Rasmus: Denna bör alltid gälla, men man ramlar igenom världen
      //ibland om den är igång alltid.
      if (player->getOnGround() == false)
	{
	  velY += gravity_ * (dt.asMilliseconds()/10.0);
	  if (velY >= 8)
	    velY = 8.0f;
	}
      else
	{
	  player->setJump(false);
	}
      if (!(move == Left || move == Right))
	{
	  player->setWalk(false);
	}


      // Förflyttningen i x-led sker statiskt istället för med acceleration.

      player->move(sf::Vector2f(distX, round(velY*(dt.asMilliseconds()/10.0))));
      //TODO: kolla om man verkligen ska sätta något i x-led
      player->setVelocity(sf::Vector2f(distX, velY));

      // kollisionshantering spelare

      // TODO: Ska denna verkligen sättas här? Undersök.
      levelVec.at(0)->setOnGround(false);

      result = collisionHandlingPlayer(levelVec);
 
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
  ActionResult collisionHandlingPlayer (vector<PhysicalElement*> & levelVec)
    {
      ActionResult result = Continue;
       Player* playerPtr{dynamic_cast<Player*>(levelVec.at(0))};
       //cout << "x: " << playerPtr->getGlobalBounds().left
       //    << "y: " << playerPtr->getGlobalBounds().top << endl;

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
	playerPtr->move(sf::Vector2f(-playerPos.y,0));
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
	    string tempSpriteID{levelVec.at(i)->getSpriteID()};	    

	    offset = collisionDisplacement(playerPtr, levelVec.at(i), area);
	    if(offset.y < 0)
	      playerPtr->setOnGround(true);
	    playerPtr->move(offset);

//	    cout << tempSpriteID << endl;

/*
	    //Ground med nedre gräns, flytta ned
	    if(levelVec.at(i)->getSpriteID() == "G12")
	      offset.y = area.height;	
	    //Ground med vänster gräns, flytta vänster
	    else if(levelVec.at(i)->getSpriteID() == "G14")
	      offset.x = -area.width;
	    //Ground med höger gräns, flytta höger
	    else if(levelVec.at(i)->getSpriteID() == "G16")
	      offset.x = area.width;
	    //Ground med övre gräns, flytta upp
	    else if(levelVec.at(i)->getSpriteID() == "G18")
	    {
	      playerPtr->setOnGround(true);
	      offset.y = -area.height;
	    }
	    else if( tempSpriteID == "G25") 
	    {
	      offset = collisionUpDown(playerPtr, area);
	      if(offset.y < 0)
		playerPtr->setOnGround(true);
	    }
	    //Måste ha noggrannare villkor för att inte få problem med att
	    //player fastnar vid gång höger->vänster mot vissa block
	    else
	    {
	      if (area.width > area.height)
	      {
		offset = collisionUpDown(playerPtr, area);
		if(offset.y < 0)
		  playerPtr->setOnGround(true);
	      }
	      else if (area.width < area.height)
	      {
		offset = collisionLeftRight(playerPtr, area);
	      }
	    }
	    playerPtr->move(offset);
*/
	  }
	  else if(levelVec.at(i)->getSpriteID() == "Block")
	  {
	    // Kollisionshantering i y-led, spelare flyttas, block flyttas inte
	    sf::Vector2f offset {0,0};
	    if (area.width > area.height)
	    {
	      offset = collisionUpDown(playerPtr, area);
	      if(offset.y < 0)
		playerPtr->setOnGround(true);	      
	      playerPtr->move(offset);
	    }
	    else if (area.width < area.height)
	    {
	      offset = collisionLeftRight(playerPtr, area);
	      // Kollisionshantering i x-led, blocket får uppdaterad hastighet,
	      // spelare flyttas inte tillbaka
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
       Player* playerPtr{dynamic_cast<Player*>(levelVec.at(0))};

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
	      offset = collisionUpDown(levelVec.at(vecLoc), area);
	      if(offset.y < 0)
		levelVec.at(vecLoc)->setOnGround(true);
	    }
	    else if (area.width < area.height)
	    {
	      offset = collisionLeftRight(levelVec.at(vecLoc), area);
	    }
	    levelVec.at(vecLoc)->move(offset);
	  }	
	}
      }
      //Flytta ev tillbaka spelare, OBS kollar bara höger och vänster för aktuellt block och spelaren
      if(playerPtr->getGlobalBounds().intersects(levelVec.at(vecLoc)->getGlobalBounds(), area))
      {
	sf::Vector2f offset {0,0};
	offset = collisionLeftRight(playerPtr, area);
	playerPtr->move(offset);
      } 
    }	  

  //Lillemor: obs, bara för Ground tills vidare
  sf::Vector2f collisionDisplacement(PhysicalElement* element, PhysicalElement* collidingElement, sf::FloatRect area)
    {
//      string tempSpriteID{element->getSpriteID()};
      sf::Vector2f offset{0,0};
      PhysicalElement::CollisionBorders collBorders(collidingElement->getCollisionBorders());
      bool up{collBorders.up};
      bool down{collBorders.down};
      bool left{collBorders.left};
      bool right{collBorders.right};
      //cout << "Width: " << area.width << " Height: " << area.height << endl;

/*
      //Ground med enbart övre gräns, flytta upp
      if(up && !(down && left && right))
	offset.y = -area.height;
      //Ground med enbart nedre gräns, flytta ned
      else if(down && !(up && left && right))
	offset.y = area.height;	
      //Ground med enbart vänster gräns, flytta vänster
      else if(left && !(up && down && right))
	offset.x = -area.width;
      //Ground med enbart höger gräns, flytta höger
      else if(right && !(up && down && left))
	offset.x = area.width;
      //Ground med enbart vänster och höger gräns
      else if(right && left && !(up && down))
	offset = collisionLeftRight(element, area);
      //Ground med enbart övre och undre gräns
      else if(!(right && left) && up && down)
      {
	offset = collisionUpDown(element, area);
	cout << "Övre + nedre" << endl;
      }
      else if ((up || down) && area.width > area.height)
*/
      if ((up || down) && area.width > area.height)
      {
	if (area.contains({ area.left, element->getGlobalBounds().top }))
//	if (area.contains({ area.left, element->getPosition().y }))
	{
	  // Up side crash => move player down
	  if(down)
	    offset.y = area.height;		      
	}
	else
	{
	  // Down side crash => move player back up
	  if(up)
	  {
	    //	    cout << "*****************UPP**************" << endl;
	    offset.y = -area.height;
	  }
	}
      }
      else if ((left || right) && area.width < area.height)
      {
	if (area.contains( element->getGlobalBounds().left + 
			   element->getGlobalBounds().width - 1.f, area.top + 1.f ))
	{
	  //Right side crash
	  if(left)
	  {
	    //cout << "*****************HÖGER**************" << endl;
	    offset.x = -area.width;
	  }
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



