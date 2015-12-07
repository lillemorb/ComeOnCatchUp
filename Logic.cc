//Logic.cc
#include "Logic.h"
#include <SFML/Graphics.hpp>
#include "Level.h"
#include "GameSounds.h"
#include <vector>
#include <iomanip>

using namespace std;

//---------UPDATE--------------//

Logic::ActionResult Logic::update(
  Level & current, Action action, Move move, GameSounds & gamesounds, sf::Clock & clock)
{
  // Fetch vector with objects in Level
  vector<PhysicalElement*> levelVec(current.getLevelPhysicalLayout());
	
  ActionResult result{Continue};

  Player* playerPtr{dynamic_cast<Player*>(levelVec.at(0))};
  playerPtr->setDeath(false);

  //Lillemor: del av försök till att göra allt tidsberoende 
  //Clock flyttas ut till Game för att enkelt kunna kolla att 
  //framet tagit tillräckligt lång tid
  dt = clock.getElapsedTime();

  //Gamla:
//  sf::Time dt{clock.getElapsedTime()};
//  clock.restart();

  //Lillemor: del av försök till att göra allt tidsberoende
  if(dt.asMicroseconds() > 20000)
  {
    //Gamla:
    //dt = sf::microseconds(16667) + clock.getElapsedTime();

    //Nya:
    dt = sf::microseconds(16667);
  }
  at += dt.asMicroseconds();

  if (at > 200000.0)
  {
    if (trigger == true)
      {
      playerPtr->setStoryAnimation(1);
      }
    else
      playerPtr->setStoryAnimation(0);
    playerPtr->setAnimationcycle();
    at = 0;
  }

  float gravity{playerPtr->getGravity()};
  float velY{playerPtr->getVelocity().y};
  float distX{0};

  if (action == Jump && playerPtr->getOnGround() == true &&
      playerPtr->getJumpAllowed() == true)
  {
    velY = -9.0f;
    playerPtr->setOnGround(false);
    playerPtr->setJump(true);
    playerPtr->setJumpAllowed(false);
    gamesounds.getJumpSound();
  }

  // Make it possible to vary the jumping height depending on when
  // the jumpbutton is realeased
  if (action == JumpReleased)
  {
    if(velY < -4.0f)
      velY = -4.0f;
    playerPtr->setJumpAllowed(true);
  }

  if (move == Left)
  {
    distX = -4;
    playerPtr->setFacingRight(false);
    playerPtr->setWalk(true);
  }
  if (move == Right)
  {
    distX = 4;
    playerPtr->setFacingRight(true);
    playerPtr->setWalk(true);
  }

  if (playerPtr->getOnGround() == false)
  {
    velY += gravity * (dt.asMicroseconds()/gravityBalance);
    if (velY >= 8)
      velY = 8.0f;
  }
  else if (velY > 0)
  {
    playerPtr->setJump(false);
    velY = gravity;
  }

  if (!(move == Left || move == Right))
  {
    playerPtr->setWalk(false);
  }

  if (velY > 0)
  {
    playerPtr->setJump(false);
    playerPtr->setFalling(true);
  }
  distX = distX*dt.asMicroseconds()/gravityBalance;

  float y = velY*(dt.asMicroseconds()/gravityBalance);
  if (y > 14)
    y = 14;
  // Movement on x-axis is static, not with acceleration.
  playerPtr->move(sf::Vector2f(distX, y));

  playerPtr->setVelocity(sf::Vector2f(distX, velY));

  // collisionhandling player
  playerPtr->setOnGround(false);
  result = collisionHandlingPlayer(levelVec, gamesounds);
 
  // Update position for block one by one, with collision handling for each block
  for(unsigned int i{1}; i < levelVec.size(); ++i)
  {
    float velYBlock{levelVec.at(i)->getVelocity().y};

    if(levelVec.at(i)->getElementID() == "Block")
    {
      if (levelVec.at(i)->getOnGround() == false)
      {
	velYBlock += gravity * (dt.asMicroseconds()/gravityBalance);
	if (velYBlock >= 8)
	  velYBlock = 8.0f;
      }
      else
      {
	velYBlock = gravity;
      }
      if(levelVec.at(i)->getBelowWindow() == false)
      {
	float y = velYBlock*(dt.asMicroseconds()/gravityBalance);
	if (y > 14)
	  y = 14;

	//Lillemor: del av försök till att göra allt tidsberoende 
	float distXBlock{levelVec.at(i)->getVelocity().x*dt.asMicroseconds()/gravityBalance};
/* // Lillemor: Låt stå ett tag	
	if(distXBlock != 0)
	  cout << "dt: " << setw(5) << dt.asMicroseconds() << " distX: " << setw(10) << distX << " distXBlock: " << setw(10) << distXBlock << " block vel: " << levelVec.at(i)->getVelocity().x << endl;


//	cout << "dt block: " << setw(7) << dt.asMicroseconds() << endl;
//	levelVec.at(i)->move(sf::Vector2f(levelVec.at(i)->getVelocity().x, y));  
*/
	int oldBlockVelocity = levelVec.at(i)->getVelocity().x;

	levelVec.at(i)->move(sf::Vector2f(distXBlock, y));  
	levelVec.at(i)->setVelocity(sf::Vector2f(0, velYBlock));
	  
	// collisionhandling
	levelVec.at(i)->setOnGround(false);
	collisionBlock(levelVec, i, oldBlockVelocity, gamesounds);
      }
    }
  }

// Returnera actionResult;
  return result;
}


//---------COLLISIONSHANDLINGPLAYER--------------//
Logic::ActionResult Logic::collisionHandlingPlayer (
  const vector<PhysicalElement*> & levelVec,GameSounds & gamesounds)
{
  ActionResult result{Continue};
  Player* playerPtr{dynamic_cast<Player*>(levelVec.at(0))};

  // Check for collision against window borders
  sf::Vector2f playerPos = sf::Vector2f(playerPtr->getGlobalBounds().left, 
					playerPtr->getGlobalBounds().top);
  sf::Vector2f playerSize = playerPtr->getSize();

  // Check left border
  if(playerPos.x < 0)
    playerPtr->move(sf::Vector2f(-playerPos.x,0));
  // Check right border
  else if((playerPos.x + playerSize.x) > xPix_)
    playerPtr->move(sf::Vector2f(xPix_-(playerPos.x + playerSize.x), 0));

  // Check upper border
  if(playerPos.y < 0)
    playerPtr->move(sf::Vector2f(0,-playerPos.y));
  // Check lower border and if collision return Dead
  else if(playerPos.y + playerSize.y > yPix_)
  {
    playerPtr->move(sf::Vector2f(0,yPix_-(playerPos.y + playerSize.y)));
    playerPtr->resetCurrentSprite();
    playerPtr->setDeath(true);
    gamesounds.getDeathSound();
    trigger = false;
    return Dead;
  }
   
  for(unsigned int i{1}; i < levelVec.size(); ++i)
  {
    sf::FloatRect area;
    if(playerPtr->getGlobalBounds().intersects(levelVec.at(i)->getGlobalBounds(), area))
    {
      if(levelVec.at(i)->getElementID() == "Door")
      {
	gamesounds.pauseBackgroundMusic();
	gamesounds.getLevelClearedSound();
	trigger = false;

	//Delay the duration of level cleared sound
	while(gamesounds.isLevelClearedSoundPlaying())
	{ }
	result = LevelCompleted;
	gamesounds.startBackgroundMusic();
      }
      else if(levelVec.at(i)->getElementID() == "Trigger")
      {
	trigger = true;
      }
      else if(dynamic_cast<Ground*>(levelVec.at(i)))
      {
	sf::Vector2f offset {0,0};

	offset = collisionDisplacement(playerPtr, levelVec.at(i), area);
	if(offset.y < 0)
	{
	  playerPtr->setOnGround(true);
	  playerPtr->setFalling(false);
	}
	playerPtr->move(offset);
      }
      else if(levelVec.at(i)->getElementID() == "Block")
      {
	// Collision in y-axis, player is moved, block is not moved
	sf::Vector2f offset {0,0};

	offset = collisionDisplacement(playerPtr, levelVec.at(i), area);
	if(offset.y < 0)
	{
	  playerPtr->setOnGround(true);
	  playerPtr->setFalling(false);
	}

	// If Player collided with a Block on the x-axis, that Block
	// will get a velocity and Player will not be moved back

	// Lillemor:
	// Check if there is a block on top on this block or if this
	// block is falling => then this block shall not be moved				
	bool blockCanBeMoved{true};
	if(offset.x != 0)
	{
	  // Check if block is on ground
	  if(!levelVec.at(i)->getOnGround())
	    blockCanBeMoved = false;
	  // Check if block has another block on top of it
	  else
	  {
	    for(unsigned int j{1}; j < levelVec.size(); ++j)
	    {
	      // Make rectangle like this block but two pixels up
	      sf::FloatRect thisBlock(levelVec.at(i)->getGlobalBounds());		
	      sf::FloatRect thisBlockMovedUp(thisBlock.left, thisBlock.top-2,
					     thisBlock.width, thisBlock.height);
	      if(levelVec.at(j)->getElementID() == "Block" &&
		 levelVec.at(j) != levelVec.at(i) &&
		 thisBlockMovedUp.intersects(levelVec.at(j)->getGlobalBounds(), area))
	      {
		blockCanBeMoved = false;
	      }
	    }  
	  }  
	}

	//Lillemor: del av försök till att göra allt tidsberoende  
	float boxX = 2 * dt.asMicroseconds() / gravityBalance;
	if (boxX > 2.0)
	  boxX = 2.0;

	// Move block if no block on top
	// Move block to the right
	if(blockCanBeMoved && offset.x < 0)
	{
/*
  // Lillemor: låt stå ett tag
	  cout << "offset.x: " << setw(8) << offset.x 
	       << " boxX (right): " << setw(8) << boxX << endl;
*/
	  levelVec.at(i)->setVelocity(
	    sf::Vector2f(boxX,levelVec.at(i)->getVelocity().y));
	  gamesounds.getBoxSound();
	}
	// Move block to the left
	else if(blockCanBeMoved && offset.x > 0)
	{
/*
  // Lillemor: låt stå ett tag
	  cout << "offset.x: " << setw(8) << offset.x
	       << " boxX (left): " << setw(8) << -boxX << endl;
*/
	  levelVec.at(i)->setVelocity(
	    sf::Vector2f(-boxX, levelVec.at(i)->getVelocity().y));
	  gamesounds.getBoxSound();
	}		  
	offset.x = 0;
	playerPtr->move(offset);
      }
    }
  }
  return result;      
}

//---------COLLISIONBLOCK--------------//
void Logic::collisionBlock(
  const vector<PhysicalElement*> & levelVec, unsigned int vecLoc, int oldVelocity, GameSounds & gamesounds)
{
  sf::FloatRect area;
  Player* playerPtr{dynamic_cast<Player*>(levelVec.at(0))};

  // Check for collision against window borders
  sf::Vector2f blockPos = levelVec.at(vecLoc)->getPosition();
  sf::Vector2f blockSize = levelVec.at(vecLoc)->getSize();

  // Check left border
  if(blockPos.x < 0)
    levelVec.at(vecLoc)->move(sf::Vector2f(-blockPos.x,0));
  // Check right border
  else if((blockPos.x + blockSize.x) > xPix_)
    levelVec.at(vecLoc)->move(sf::Vector2f(xPix_-(blockPos.x + blockSize.x), 0));

  // Check upper border
  if(blockPos.y < 0)
    levelVec.at(vecLoc)->move(sf::Vector2f(-blockPos.y,0));
  // Check lower border and if collision set velocity 0,0
  else if(blockPos.y > yPix_+33)
    levelVec.at(vecLoc)->setBelowWindow(true);

  sf::Vector2f offset {0,0};
  for(unsigned int i{1}; i < levelVec.size(); ++i)
  {
    // Check so that you're not trying to do a collision with oneself
    if( levelVec.at(vecLoc) != levelVec.at(i) && 
	levelVec.at(vecLoc)->getGlobalBounds().intersects(
	  levelVec.at(i)->getGlobalBounds(), area))
    {
      // Triggers an event when a block falls through a trigger element
      // (last level)
      if(levelVec.at(i)->getElementID() == "Trigger")
	trigger = true;

      if(levelVec.at(i)->getElementID() == "Block")
      {
	offset = sf::Vector2f(0,0);

	offset = collisionDisplacement(levelVec.at(vecLoc), levelVec.at(i), area);

	if(offset.y < 0)
	  levelVec.at(vecLoc)->setOnGround(true);

	levelVec.at(vecLoc)->move(offset);	  
      }
      else if(levelVec.at(i)->getElementID().at(0) == 'G')	 
      {
	offset = sf::Vector2f(0,0);

	offset = collisionDisplacement(levelVec.at(vecLoc), levelVec.at(i), area);
	if(offset.y < 0)
	{
	  levelVec.at(vecLoc)->setOnGround(true);

	  //Check if a block is only a few pixels from the edge of a Ground element
	  //If so (and other conditions met), move block on the x axis to an even tile
	  //Prevents being able to push a Block over a one tile wide hole in the ground
	  PhysicalElement::CollisionBorders collBorders(
	    levelVec.at(i)->getCollisionBorders());
	  if(area.width <= 4)
	  {
	    blockPos = levelVec.at(vecLoc)->getPosition();
	    int tilesize{static_cast<int>(levelVec.at(vecLoc)->getSize().x)};	    
	    float xCorrection{getNearestTilePosXDiff_(blockPos.x, tilesize)};

	    //TODO Lillemor: gör så att detta villkor beror av tid
	    if((xCorrection > 0 && xCorrection <= 4 &&
		(collBorders.right == false  || (collBorders.right == true && oldVelocity < 0))) ||
	       (xCorrection < 0 && xCorrection >= -4 &&
		(collBorders.left == false || (collBorders.left == true && oldVelocity > 0))))
	    {
	      xCorrection = 0;
	    }
	    else
	    {
	      levelVec.at(vecLoc)->setOnGround(false);
	    }
	    
	    offset.x = xCorrection;
	    levelVec.at(vecLoc)->setVelocity(sf::Vector2f(0,levelVec.at(vecLoc)->getVelocity().y));
	  }
	}
	levelVec.at(vecLoc)->move(offset);
      }	
    }
  }
  // If Block is falling, its position is checked and corrected so that it is an
  // even tile on the x axis. To ensure that a Block falls straight down when it
  // is pushed past an edge
  if(offset.y == 0)
  {
    blockPos = levelVec.at(vecLoc)->getPosition();
    int tilesize{static_cast<int>(levelVec.at(vecLoc)->getSize().x)};
    float xCorrection{};	

    xCorrection = getNearestTilePosXDiff_(blockPos.x, tilesize);
    levelVec.at(vecLoc)->move(sf::Vector2f(xCorrection,0));
    levelVec.at(vecLoc)->setVelocity(sf::Vector2f(0,levelVec.at(vecLoc)->getVelocity().y));
  }
     
  // Move back player if the block couldn't be moved
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

//---------COLLISIONDISPLACEMENT--------------//
sf::Vector2f Logic::collisionDisplacement(
  PhysicalElement* const element, PhysicalElement* const collidingElement,
  const sf::FloatRect & area)
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
	if(element->getVelocity().y < -2.0f)
	  element->setVelocity(sf::Vector2f(0, -2.0f));
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
		       element->getGlobalBounds().width - 0.1, area.top + 0.1 ))
    {
      //Right side crash
      if(left)
	offset.x = -area.width;

/*
    // Lillemor: låt stå ett tag
      if(collidingElement->getElementID() == "Block")
      {
	cout << "Right crash, area top: " << area.top << " left: " << area.left
	     << " bottom: " << area.top + area.height << " right: " << area.left + area.width << endl;
	cout << "x: " << element->getGlobalBounds().left + element->getGlobalBounds().width - 0.001
	     << " y: " << area.top + 0.001 << endl;
      }
*/
    }
    else
    {
      //Left side crash
      if(right)
	offset.x = area.width;
/*
  // Lillemor: låt stå ett tag
      if(collidingElement->getElementID() == "Block")
      {
	cout << "Left crash, area top: " << area.top << " left: " << area.left
	     << " bottom: " << area.top + area.height << " right: " << area.left + area.width << endl;
	cout << "x: " << element->getGlobalBounds().left + element->getGlobalBounds().width - 0.001
	     << " y: " << area.top + 0.001 << endl;
      }
*/
    }
  }  
  return offset;
}  

//---------GETNERARESTTILEPOSXDIFF--------------//
float Logic::getNearestTilePosXDiff_(float currentXPos, int tilesize)
{
  float xCorrection{};
  // How many whole tiles is to the left of currentXPos
  int integerDivResult{ static_cast<int>(currentXPos / tilesize)};

  // Workaround to get the same result as currentXPos%tilesize,
  // which is not a legal operation for float%integer
  if((currentXPos - integerDivResult * tilesize) <= tilesize/2) 
  {
    xCorrection = -(currentXPos - integerDivResult * tilesize);
  }
  else
  {
    xCorrection = tilesize - (currentXPos - integerDivResult * tilesize);
  }

  return xCorrection;
}
