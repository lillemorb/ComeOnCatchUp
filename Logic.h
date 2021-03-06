//Logic.h
#ifndef LOGIC_H
#define LOGIC_H
#include <SFML/Graphics.hpp>
#include "Level.h"
#include "GameSounds.h"
#include <vector>

/*************************************************
Logic:
Logic takes care of everything that has to do with the game logic. 
This includes moving elements, collision detection and handling.
The main function is update, which updates all elements accordingly
 and returns an ActionResult to Game. Two functions helps with this:
 collisionHandlingPlayer and collisionBlock.

*************************************************/

class Logic
{
public:
  Logic() = default;
  ~Logic() = default;

  enum ActionResult { LevelCompleted, Dead, Reset, Continue };
  enum Move { Left, Right, Idle };
  enum Action { Jump, JumpReleased };


  void setPix(int x, int y) { xPix_ = x; yPix_ = y;}
  ActionResult update(Level &current, Action action, Move move,
		      GameSounds & gamesounds, sf::Clock & clock);

private:
  Logic(const Logic&) = delete;
  Logic(Logic&&) = delete;
  Logic& operator=(const Logic&) = delete;
  Logic& operator=(Logic&&) = delete;

  bool trigger{false};
  int xPix_{};
  int yPix_{};
  int at{0};
  const float gravityBalance {15000.0};
  sf::Time dt{};

  ActionResult collisionHandlingPlayer (
    const std::vector<PhysicalElement*> & levelVec, GameSounds & gamesounds);
  void collisionBlock(
    const std::vector<PhysicalElement*> & levelVec, unsigned int vecLoc,
    int oldVelocity, GameSounds & gamesounds);
  sf::Vector2f collisionDisplacement (
    PhysicalElement* const element, PhysicalElement* const collidingElement,
    const sf::FloatRect & area);
  float getNearestTilePosXDiff_(float currentXPos, int tilesize);
};

#endif

