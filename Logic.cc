//Logic.cc
#include "Logic.h"
#include <SFML/Graphics.hpp>
#include "Level.h"
#include "GameSounds.h"
#include <vector>

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

  dt = clock.getElapsedTime();

  //Adjust high clock values to prevent faulty movement
  if(dt.asMicroseconds() > 20000)
    dt = sf::microseconds(16667);
  at += dt.asMicroseconds();

  //Update sprite animation according to animation rate at
  if (at > 200000.0)
  {
    if (trigger == true)
      playerPtr->setStoryAnimation(1);
    else
      playerPtr->setStoryAnimation(0);

    playerPtr->setAnimationcycle();
    at = 0;
  }

  float gravity{playerPtr->getGravity()};
  float velY{playerPtr->getVelocity().y};
  float distX{0};

  // Jump if on ground and jump key pressed
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

  // Move left
  if (move == Left)
  {
    distX = -4;
    playerPtr->setFacingRight(false);
    playerPtr->setWalk(true);
  }
  // Move right
  if (move == Right)
  {
    distX = 4;
    playerPtr->setFacingRight(true);
    playerPtr->setWalk(true);
  }

  // Set velocity on y axis with gravity
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

  // Set animation to idle
  if (!(move == Left || move == Right))
  {
    playerPtr->setWalk(false);
  }

  // Set animation to falling 
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

  // Collisionhandling player
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

	float distXBlock{
	  levelVec.at(i)->getVelocity().x*dt.asMicroseconds()/gravityBalance};
	int oldBlockVelocity = levelVec.at(i)->getVelocity().x;

	levelVec.at(i)->move(sf::Vector2f(distXBlock, y));  
	levelVec.at(i)->setVelocity(sf::Vector2f(0, velYBlock));
	  
	// Collisionhandling block
	levelVec.at(i)->setOnGround(false);
	collisionBlock(levelVec, i, oldBlockVelocity, gamesounds);
      }
    }
  }

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
   
  // Check and handle collision against all other level elements
  for(unsigned int i{1}; i < levelVec.size(); ++i)
  {
    sf::FloatRect area;
    if(playerPtr->getGlobalBounds().intersects(levelVec.at(i)->getGlobalBounds(), area))
    {
      // Collision with door
      if(levelVec.at(i)->getElementID() == "Door")
      {
	gamesounds.pauseBackgroundMusic();
	gamesounds.getLevelClearedSound();
	trigger = false;

	//Delay loading next level until sound is played
	while(gamesounds.isLevelClearedSoundPlaying())
	{ }
	result = LevelCompleted;
	gamesounds.startBackgroundMusic();
      }
      // Check for collision aginst this to trigger cutscenes
      else if(levelVec.at(i)->getElementID() == "Trigger")
      {
	trigger = true;
      }
      // Collision with ground
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
      // Collision with block
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
	bool blockCanBeMoved{true};
	if(offset.x != 0)
	{
	  // Check if block is on ground
	  if(!levelVec.at(i)->getOnGround())
	    blockCanBeMoved = false;
	  // Check if block has another block on top of it => will not be moved
	  else
	  {
	    for(unsigned int j{1}; j < levelVec.size(); ++j)
	    {
	      // Create a rectangle like the current block moved two pixels up,
	      // check this for collision with other block
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

	// Set block velocity on the x axis
	float blockX = 2 * dt.asMicroseconds() / gravityBalance;
	if (blockX > 2.0)
	  blockX = 2.0;

	// Move block if no block on top
	// Move block to the right
	if(blockCanBeMoved && offset.x < 0)
	{
	  levelVec.at(i)->setVelocity(
	    sf::Vector2f(blockX,levelVec.at(i)->getVelocity().y));
	  gamesounds.getBoxSound();
	}
	// Move block to the left
	else if(blockCanBeMoved && offset.x > 0)
	{
	  levelVec.at(i)->setVelocity(
	    sf::Vector2f(-blockX, levelVec.at(i)->getVelocity().y));
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
    // Check so that one is not trying to do a collision with oneself
    if( levelVec.at(vecLoc) != levelVec.at(i) && 
	levelVec.at(vecLoc)->getGlobalBounds().intersects(
	  levelVec.at(i)->getGlobalBounds(), area))
    {
      // Triggers an event when a block falls through a trigger element
      if(levelVec.at(i)->getElementID() == "Trigger")
	trigger = true;

      // Collision with block
      if(levelVec.at(i)->getElementID() == "Block")
      {
	offset = sf::Vector2f(0,0);

	offset = collisionDisplacement(levelVec.at(vecLoc), levelVec.at(i), area);

	if(offset.y < 0)
	  levelVec.at(vecLoc)->setOnGround(true);

	levelVec.at(vecLoc)->move(offset);	  
      }
      // Collision with ground
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

	    if((xCorrection > 0 && xCorrection <= 4 &&
		(collBorders.right == false  ||
		 (collBorders.right == true && oldVelocity < 0))) ||
	       (xCorrection < 0 && xCorrection >= -4 &&
		(collBorders.left == false ||
		 (collBorders.left == true && oldVelocity > 0))))
	    {
	      xCorrection = 0;
	    }
	    else
	    {
	      levelVec.at(vecLoc)->setOnGround(false);
	    }
	    
	    offset.x = xCorrection;
	    levelVec.at(vecLoc)->setVelocity(
	      sf::Vector2f(0,levelVec.at(vecLoc)->getVelocity().y));
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
    levelVec.at(vecLoc)->setVelocity(
      sf::Vector2f(0,levelVec.at(vecLoc)->getVelocity().y));
  }
     
  // Move back player if the block couldn't be moved
  if(playerPtr->getGlobalBounds().intersects(
       levelVec.at(vecLoc)->getGlobalBounds(), area))
  {
    sf::Vector2f offset {0,0};

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

//---------GET NEAREST TILEPOS X DIFF--------------//
float Logic::getNearestTilePosXDiff_(float currentXPos, int tilesize)
{
  float xCorrection{};
  // How many whole tiles is to the left of currentXPos
  int integerDivResult{ static_cast<int>(currentXPos / tilesize)};

  // Workaround to get the same result as currentXPos%tilesize,
  // which is not a legal operation for float%integer
  if((currentXPos - integerDivResult * tilesize) <= tilesize/2) 
    xCorrection = -(currentXPos - integerDivResult * tilesize);
  else
    xCorrection = tilesize - (currentXPos - integerDivResult * tilesize);

  return xCorrection;
}
