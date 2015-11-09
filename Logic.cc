//Logic.cc

#include <SFML/Graphics.hpp>
#include "Level.cc"
#include <vector>

class Logic
{
public:
  Logic() = default;

  enum ActionResult { GameCompleted, LevelCompleted, Dead, Reset, Continue };
  enum Move { Left, Right, Idle };
  enum Action { Jump, Nothing };

  ActionResult update(Level & current, Action action, Move move, sf::Clock & clock)
  {
    // Hämta vektor med objekt i Level
    // TODO: ändra funktionsanropet till att hämta Physical-vektorn
    vector<PhysicalElement*> levelVec(current.getLevelPhysicalLayout());

    // TODO: leta upp Player i vektorn,  nu antar vi att
    // den ligger på plats 0
    //      unsigned int playerIndex{0}; 
    ActionResult result{Continue};
    sf::Time dt{clock.getElapsedTime()};
    int velX{};
    int velY{};
    int gravity_{4};
    int maxVelY_{96};


    Player* player{dynamic_cast<Player*>(levelVec.at(0))};
    // Kolla om action är giltig

    // Utför action 
    //    if(action == Jump && !player->getJump)
    if(action == Jump && player->getOnGround() == true)
      {
	velY = -32;
	player->setJumpAllowed(false);
	//player->setOnGround(false);
      }
    else 
      {
	if(player->getOnGround() == false)
	  {
	    velY = velY+gravity_;
	  }
	else
	  {
	    velY = 0;
	  }
      }
    
    if(move == Left)
      {
	velX = -4;
      }
    if(move == Right)
      {
	velX = 4;
      }
    
    player->setVelocity(sf::Vector2f(velX,velY));

    // Uppdatera players position
    sf::Vector2f playerPos{
      player->getPosition().x + player->getVelocity().x, 
	player->getPosition().y + player->getVelocity().y + gravity_};
    // Behöver få in tidskonstanten * Player->getVelocity


    player->setPosition(playerPos);

    //kollisionshantering
    result = collisionHandling(levelVec);

      // Uppdatera position för block, ett för ett
      for(unsigned int i{1}; i < levelVec.size(); ++i)
	{
	  sf::Vector2f noVel{0,0};
	  if (levelVec.at(i)->getSpriteID() == "Block" && levelVec.at(i)->getVelocity() != noVel) 
	    {
	      // Uppdatera blockets position, samma som för player ovan, ska kanske ändras lite på båda ställen
	      sf::Vector2f playerPos{levelVec.at(i)->getPosition().x
		  + levelVec.at(i)->getVelocity().x, levelVec.at(i)->getPosition().y
		  + levelVec.at(i)->getVelocity().y,};
	      levelVec.at(i)->setPosition(playerPos);
	      levelVec.at(i)->setVelocity(noVel);
	      //kollisionshantering
	      collisionBlock(levelVec, i);
	    }
	}
      // Returnera actionResult;
      return result;
    }


private:

  ActionResult collisionHandling (vector<PhysicalElement*> & levelVec)
  {
    levelVec.at(0)->setOnGround(false);
    for(unsigned int i{1}; i < levelVec.size(); ++i)
      {
	sf::FloatRect area;
	if(levelVec.at(0)->getGlobalBounds().intersects(levelVec.at(i)->getGlobalBounds(), area))
	  {
	    if(levelVec.at(i)->getSpriteID() == "Door")
	      return LevelCompleted;
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
		//cout << "offset x:" << offset.x << " y:" <<offset.y << endl;
		return Continue;
	      }
	    else if(levelVec.at(i)->getSpriteID() == "Block")
	      {
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
	  }
      }
    return Continue;
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
	    if (area.contains( levelVec.at(0)->getPosition().x + levelVec.at(0)->getGlobalBounds().width - 1.f, area.top + 1.f ))
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
