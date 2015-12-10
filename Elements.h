//Elements.h
#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <SFML/Graphics.hpp>

/*************************************************
DrawableElement:
Implements an abstract class for a drawable element,
one tile in size. Has functionality to allow
Graphics to draw it on it's correct position in the
game window.

PhysicalElement:
Implements an abstract class for a physical element,
one tile in size. Has functionality to create
movement and detect collision, which is useb by
Logic. Inherits from DrawableElement.

Player:
Implements a class for the main character in the
game. Has functionality for controlling animations
used by Graphics, plus additional functionality for 
in game physics and game logic. Inherits from 
PhysicalElement.

Block:
Implements a class for a movable block. Has 
additional functionality for in game physics and
game logic. Inherits from PhysicalElement.

Ground: 
Implements a class for ground elements. Can not be
moved in the game window after its creation.
There are several types with different collision
borders. Inherits from PhysicalElement.

Door:
Implements a class for the goal of each level.
Can not be moved in the game window after its
creation. Inherits from PhysicalElement.

Background:
Implements a class to be able to make different 
graphical elements in a level. Inherits from
DrawableElement.

Trigger:
Implements a class for invisible elements that are
used for triggers for animations. Inherits from
PhysicalElement.

*************************************************/

//---------DRAWABLE ELEMENT--------------//
class DrawableElement
{
public:
DrawableElement(const std::string & elementID) : elementID_{elementID} {};
  virtual ~DrawableElement() = 0;
  std::string getElementID() const { return elementID_; }
  sf::Vector2f getPosition() const { return rectangle_.getPosition(); }

protected:
  std::string elementID_{};
  sf::RectangleShape rectangle_;

private:
  DrawableElement(const DrawableElement&) = delete;
  DrawableElement& operator=(const DrawableElement&) = delete;
};

//---------PHYSICAL ELEMENT--------------//
class PhysicalElement : public DrawableElement
{
public:
PhysicalElement(const std::string & elementID): DrawableElement(elementID){}
  virtual ~PhysicalElement() = 0;
 
  struct CollisionBorders
  { bool left{true}; bool right{true}; bool up{true}; bool down{true}; };

  //Implemented here, inherited classes can implement their own version.
  //Implemented in: Player, Block
  virtual void setVelocity(const sf::Vector2f & vel){ velocity_ = sf::Vector2f(0,gravity_); }
  //Implemented in: Player, Block
  virtual void setPosition(const sf::Vector2f & pos) {}
  //Implemented in: Player, Block
  virtual void move(const sf::Vector2f & offset) {} 
  //Implemented in: Player, Door
  virtual sf::FloatRect getGlobalBounds() const { return rectangle_.getGlobalBounds(); }
  //Implemented in: Player
  virtual sf::Vector2f getSize() const { return rectangle_.getSize();}
  //Implemented in: Block
  virtual void setBelowWindow( bool out ) {}
  //Implemented in: Block
  virtual bool getBelowWindow() const { return false; }

  //Implemented here, inherited classes do not implement own version
  sf::Vector2f getVelocity() const { return velocity_; }
  void setOnGround( bool onGround ) { onGround_ = onGround; }
  bool getOnGround() const { return onGround_; }
  CollisionBorders getCollisionBorders() const { return collisionBorders_; }

protected:
  CollisionBorders collisionBorders_{};
  float gravity_{0.5f};
  sf::Vector2f velocity_{sf::Vector2f (0,gravity_)};
  bool onGround_{false};

private:
  PhysicalElement(const PhysicalElement&) = delete;
  PhysicalElement& operator=(const PhysicalElement&) = delete;
};

//---------PLAYER--------------//
class Player : public PhysicalElement
{
public:
  Player(int TILESIZE, float x,  float y);
  ~Player() = default;

  void setVelocity(const sf::Vector2f & vel) override { velocity_ = vel; } 
  void setPosition(const sf::Vector2f & pos) override { rectangle_.setPosition(pos.x, pos.y); }
  void move(const sf::Vector2f & offset) override { rectangle_.move(offset); }
  sf::FloatRect getGlobalBounds() const override;
  sf::Vector2f getSize() const override;

  //Unique functions for Player
  float getGravity() const { return gravity_; }
  bool getFacingRight() const { return facingRight_; }
  void setFacingRight( bool facingRight ) { facingRight_ = facingRight; }
  sf::Vector2f getCurrentSprite() const { return currentSprite_; }
  void resetCurrentSprite(){ currentSprite_.x = 0; }
  void setJump( bool jump ) { jump_ = jump; }
  void setFalling( bool falling ) { falling_ = falling; }
  bool getDeath() const { return death_; }
  void setDeath( bool death ) { death_ = death; }
  bool getJumpAllowed() const { return jumpAllowed_; }
  void setJumpAllowed( bool jumpAllowed ) { jumpAllowed_ = jumpAllowed; }
  void setWalk( bool walk ) { walk_ = walk; }
  void setAnimationcycle();
  sf::Vector2f getAnimation();
  void setStoryAnimation(int x);
  int getStoryAnimation() { return storyAnimation_; }


private:
  Player(const Player&) = delete;
  Player& operator=(const Player&) = delete;

  bool facingRight_{true};
  int spriteNumberWalking_{0};
  int spriteNumberIdle_{0};
  int walkCycle_{0};
  int spriteDelay_{0};
  sf::Vector2f currentSprite_{0, 0};
  bool jump_{false};
  bool falling_{false};
  bool walk_{false};
  bool death_{false};
  bool jumpAllowed_{true};
  int animationcycle_{0};
  int storyAnimation_{0};
};

//---------BLOCK--------------//
class Block : public PhysicalElement
{
public:

  Block(int TILESIZE, int x, int y);
  ~Block() = default;

  void setVelocity(const sf::Vector2f & vel) override { velocity_ = vel; } 
  void setPosition(const sf::Vector2f & pos) override { rectangle_.setPosition(pos.x, pos.y); }
  void move(const sf::Vector2f & offset) override { rectangle_.move(offset); }

  bool getBelowWindow() const override { return belowWindow_; }
  void setBelowWindow( bool out ) override { belowWindow_=out; }

private:
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;

  bool belowWindow_{false};
};

//---------GROUND--------------//
class Ground : public PhysicalElement
{
public:
  
  Ground(int TILESIZE, int x, int y, const std::string & elementID,
	 const CollisionBorders & collisionBorders);
  ~Ground() = default;

private:
  Ground(const Ground&) = delete;
  Ground& operator=(const Ground&) = delete;
};

//---------DOOR--------------//
class Door : public PhysicalElement
{
public:

  Door(int TILESIZE, int x, int y);
  ~Door() = default;

  sf::FloatRect getGlobalBounds() const override;

private:
  Door(const Door&) = delete;
  Door& operator=(const Door&) = delete;
};

//---------BACKGROUND--------//
class Background : public DrawableElement
{
public:

  Background(int TILESIZE, int x, int y, const std::string & elementID);
  ~Background() = default;

private:
  Background(const Background&) = delete;
  Background& operator=(const Background&) = delete;
};

//---------TRIGGER-----------//
class Trigger : public PhysicalElement
{
public:

  Trigger(int TILESIZE, int x, int y);
  ~Trigger() = default;

private:
  Trigger(const Trigger&) = delete;
  Trigger& operator=(const Trigger&) = delete;
};

#endif
