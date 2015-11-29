//Logic.cc
#include "Logic.h"
#include <SFML/Graphics.hpp>
#include "Level.h"
#include "GameSounds.h"
#include <vector>

//---------UPDATE--------------//

Logic::ActionResult Logic::update(
	Level & current, Action action, Move move, GameSounds & gamesounds)
{
	// Fetch vector with objects in Level
	vector<PhysicalElement*> levelVec(current.getLevelPhysicalLayout());
	
	ActionResult result{Continue};

	Player* playerPtr{dynamic_cast<Player*>(levelVec.at(0))};
	playerPtr->setDeath(false);
	sf::Time dt{clock.getElapsedTime()};
	clock.restart();
	if(dt.asMicroseconds() > 20000)
		dt = clock.getElapsedTime();
	at += dt.asMicroseconds();

	if (at > 200000.0)
	{
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
				levelVec.at(i)->move(sf::Vector2f(levelVec.at(i)->getVelocity().x, y));  
				levelVec.at(i)->setVelocity(sf::Vector2f(0, velYBlock));
	  
				// collisionhandling
				levelVec.at(i)->setOnGround(false);
				collisionBlock(levelVec, i, gamesounds);
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
	sf::Time dt{clock.getElapsedTime()};

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
		return Dead;
	}
   
	for(unsigned int i{1}; i < levelVec.size(); ++i)
	{
		sf::FloatRect area;
		if(playerPtr->getGlobalBounds().intersects(levelVec.at(i)->getGlobalBounds(), area))
		{
			if(levelVec.at(i)->getElementID() == "Door")
			{
				//0.5 s delay
				while(dt.asSeconds() < 0.5)
					dt = clock.getElapsedTime();
				result = LevelCompleted;
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

				// Check if there is a block on top on this block => this block shall then not be moved
				bool blockCanBeMoved{true};
				if(offset.x != 0)
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
				float boxX = dt.asMicroseconds();
				if (boxX > 2.0)
					boxX = 2.0;

				// Move block if no block on top
				if(blockCanBeMoved && offset.x < 0)
				{
					levelVec.at(i)->setVelocity(
						sf::Vector2f(boxX,levelVec.at(i)->getVelocity().y));
					gamesounds.getBoxSound();
				}
				else if(blockCanBeMoved && offset.x > 0)
				{
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
	const vector<PhysicalElement*> & levelVec, unsigned int vecLoc, GameSounds & gamesounds)
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

	for(unsigned int i{1}; i < levelVec.size(); ++i)
	{
		// Check so that you're not trying to do a collision with oneself
		if( levelVec.at(vecLoc) != levelVec.at(i) &&
				levelVec.at(vecLoc)->getGlobalBounds().intersects(
					levelVec.at(i)->getGlobalBounds(), area))
		{
			// Lillemor: Kollar mot första bokstaven för olika Ground-objekt
			if(levelVec.at(i)->getElementID().at(0) == 'G' ||
				 levelVec.at(i)->getElementID() == "Block")
			{
				sf::Vector2f offset {0,0};

				offset = collisionDisplacement(levelVec.at(vecLoc), levelVec.at(i), area);
				if(offset.y < 0)
					levelVec.at(vecLoc)->setOnGround(true);
				levelVec.at(vecLoc)->move(offset);
			}	
		}
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
