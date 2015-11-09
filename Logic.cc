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

  ActionResult update(Level & current, Action action, Move move)
  {
    // Hämta vektor med objekt i Level
    // TODO: ändra funktionsanropet till att hämta Physical-vektorn
    vector<PhysicalElement*> levelVec(current.getLevelPhysicalLayout());

    // TODO: leta upp Player i vektorn,  nu antar vi att
    // den ligger på plats 0
    //      unsigned int playerIndex{0}; 
    ActionResult result{Continue};
    int velX{};
    int velY{};

    Player* player{dynamic_cast<Player*>(levelVec.at(0))};
    // Kolla om action är giltig

    // Utför action 
    //    if(action == Jump && !player->getJump)
    if(action == Jump)
      {
	if(!player->getJumpAllowed())
	  {
	    velY = -3;
	  }
      }
    else
      {
	if(player->getPosition().y < 384)
	  {
	    velY = 3;
	  }
	  else 
	  {
	    velY = 0;
	    player->setJumpAllowed(false);
	  }
      }
    
    if(move == Left)
      {
	velX = -3;
      }
    else if(move == Right)
      {
	velX = 3;
      }
    else
      {
      }

    player->setVelocity(sf::Vector2f(velX,velY));

    // Uppdatera players position
    sf::Vector2f playerPos{player->getPosition().x
	+ player->getVelocity().x, player->getPosition().y
	+ player->getVelocity().y,};
    //      levelVec.at(0)->setPosition(
    //	levelVec.at(0)->getPosition() + levelVec.at(0)->getVelocity());
    player->setPosition(playerPos);
    //cout << "Players pos: " << playerPos.x << " " << playerPos.y << endl;

    // Kollisionshantering

    // Uppdatera position för block

    // Kollisionshantering

    // Returnera actionResult;

    return result;
  }



private:

};
