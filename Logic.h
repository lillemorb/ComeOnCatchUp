//Logic.h
#ifndef LOGIC_H
#define LOGIC_H
#include <SFML/Graphics.hpp>
#include "Level.h"
#include "GameSounds.h"
#include <vector>

class Logic
{
public:
  Logic() = default;

  enum ActionResult { LevelCompleted, Dead, Reset, Continue };
  enum Move { Left, Right, Idle };
  enum Action { Jump, JumpReleased };

  sf::Clock clock;
  void setPix(int x, int y) { xPix_ = x; yPix_ = y;}
  ActionResult update(Level &current, Action action, Move move,
											GameSounds & gamesounds);

private:
  int xPix_{768};
  int yPix_{576};
  int at{0};
  const float gravityBalance {15000.0};

  ActionResult collisionHandlingPlayer (
		const std::vector<PhysicalElement*> & levelVec, GameSounds & gamesounds);
  void collisionBlock(
		const std::vector<PhysicalElement*> & levelVec, unsigned int vecLoc,
		GameSounds & gamesounds);
	sf::Vector2f collisionDisplacement (
		PhysicalElement* const element, PhysicalElement* const collidingElement,
		const sf::FloatRect & area);
};

#endif

