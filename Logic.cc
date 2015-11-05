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
      unsigned int playerIndex{0}; 
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
      levelVec.at(0)->setPosition(
	levelVec.at(0)->getPosition() + levelVec.at(0)->getVelocity());

      // Kollisionshantering

      // Uppdatera position för block

      // Kollisionshantering

      // Returnera actionResult;

      return result;
    }



private:

};
