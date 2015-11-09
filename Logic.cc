//Logic.cc

#include <SFML/Graphics.hpp>
#include "Level.cc"
#include <vector>

class Logic
{
public:
  Logic() = default;

  enum ActionResult { GameCompleted, LevelCompleted, Dead, Reset, Continue };
  enum Action { Left, Right, Nothing };

  ActionResult update(Level & current, Action action)
    {
      // Hämta vektor med objekt i Level
      // TODO: ändra funktionsanropet till att hämta Physical-vektorn
      vector<PhysicalElement*> levelVec(current.getLevelPhysicalLayout());

      // TODO: leta upp Player i vektorn,  nu antar vi att
      // den ligger på plats 0
//      unsigned int playerIndex{0}; 
      ActionResult result{Continue};

      // Kolla om action är giltig

      // Utför action 
      if(action == Left)
      {
	levelVec.at(0)->setVelocity(sf::Vector2f(-1,0));
      }
      else if(action == Right)
      {
	levelVec.at(0)->setVelocity(sf::Vector2f(1,0));
      }
      else
      {
	levelVec.at(0)->setVelocity(sf::Vector2f(0,0));
      }

      // Uppdatera players position
      sf::Vector2f playerPos{levelVec.at(0)->getPosition().x
	  + levelVec.at(0)->getVelocity().x, levelVec.at(0)->getPosition().y
	  + levelVec.at(0)->getVelocity().y,};
//      levelVec.at(0)->setPosition(
//	levelVec.at(0)->getPosition() + levelVec.at(0)->getVelocity());
      levelVec.at(0)->setPosition(playerPos);
      //cout << "Players pos: " << playerPos.x << " " << playerPos.y << endl;

      // Kollisionshantering
      result = collisionHandling (levelVec);

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
    //levelVec.at(0)->setOnGround(false);
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
		//cout << "offset x:" << offset.x << " y:" <<offset.y << endl;
		return Continue;
	      }
	    else if(levelVec.at(i)->getSpriteID() == "Block")
	      {
		if (area.contains( levelVec.at(0)->getPosition().x + levelVec.at(0)->getGlobalBounds().width - 1.f, area.top + 1.f ))
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
		    if (area.contains( levelVec.at(vecLoc)->getPosition().x + levelVec.at(vecLoc)->getGlobalBounds().width - 1.f, area.top + 1.f ))
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
