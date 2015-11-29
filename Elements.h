#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <SFML/Graphics.hpp>

//---------DRAWABLE--------------//
class DrawableElement
{
public:
DrawableElement(std::string elementID) : elementID_{elementID} {};
  virtual ~DrawableElement() = 0;
	std::string getElementID() const { return elementID_; }
  sf::Vector2f getPosition() const { return rectangle_.getPosition(); }

protected:
	std::string elementID_{};
  sf::RectangleShape rectangle_;
};

//---------PHYSICAL--------------//
class PhysicalElement : public DrawableElement
{
public:
PhysicalElement(std::string elementID): DrawableElement(elementID){}
  virtual ~PhysicalElement() = 0;
 
  struct CollisionBorders
  { bool left{true}; bool right{true}; bool up{true}; bool down{true}; };

  //Implementeras här men ärvande klasser kan implementera egen variant
  virtual void setVelocity(const sf::Vector2f vel){ velocity_ = sf::Vector2f(0,gravity_); }
  virtual void setPosition(const sf::Vector2f pos) {}
  virtual void move(const sf::Vector2f offset) {} 
  virtual sf::FloatRect getGlobalBounds() const { return rectangle_.getGlobalBounds(); }
  virtual sf::Vector2f getSize() const { return rectangle_.getSize();}
  virtual void setBelowWindow(const bool) {}
  virtual bool getBelowWindow() const { return false; }

  //Implementeras här och ärvs sedan utan polymorfi
  sf::Vector2f getVelocity() { return velocity_; }
  void setOnGround(const bool onGround) { onGround_ = onGround; }
  bool getOnGround() const { return onGround_; }
  CollisionBorders getCollisionBorders() const { return collisionBorders_; }

protected:
  CollisionBorders collisionBorders_{};
  float gravity_{0.5f};
  sf::Vector2f velocity_{sf::Vector2f (0,gravity_)};
  bool onGround_{false};
};

//---------PLAYER--------------//
class Player : public PhysicalElement
{
public:
  Player(int TILESIZE, float x, float y);
  ~Player() = default;

  void setVelocity(const sf::Vector2f vel) override { velocity_ = vel; } 
  void setPosition(const sf::Vector2f pos) override { rectangle_.setPosition(pos.x, pos.y); }
  void move(const sf::Vector2f offset) override { rectangle_.move(offset); }
  sf::FloatRect getGlobalBounds() const override;
  sf::Vector2f getSize() const override;

  //Funktioner unika för Player
  float getGravity() const { return gravity_; }
  void setFacingRight(const bool facingRight) { facingRight_ = facingRight; }
  bool getFacingRight() const { return facingRight_; }
  void resetCurrentSprite(){ currentSprite_.x = 0; }
  sf::Vector2f getCurrentSprite() const { return currentSprite_; }
  void setJump(const bool jump) { jump_ = jump; }
  void setFalling(const bool falling) { falling_ = falling; }
  void setDeath(const bool death) { death_ = death; }
  bool getDeath() const { return death_; }
  bool getJumpAllowed() const { return jumpAllowed_; }
  void setJumpAllowed(const bool jumpAllowed) { jumpAllowed_ = jumpAllowed; }
  void setWalk(const bool walk) { walk_ = walk; }
  void setAnimationcycle();
  sf::Vector2f getAnimation();

private:
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
};

//---------BLOCK--------------//
class Block : public PhysicalElement
{
public:

  Block(int TILESIZE, int x, int y);
  ~Block() = default;

  void setVelocity(const sf::Vector2f vel) override { velocity_ = vel; } 
  void setPosition(const sf::Vector2f pos) override
	{ rectangle_.setPosition(pos.x, pos.y); }
  void move(const sf::Vector2f offset) override { rectangle_.move(offset); }

  void setBelowWindow(const bool out) override { belowWindow_=out; }
  bool getBelowWindow() const override { return belowWindow_; }

private:
  bool belowWindow_{false};
};

//---------GROUND--------------//
class Ground : public PhysicalElement
{
public:
  
  Ground(int TILESIZE, int x, int y, std::string elementID,
				 CollisionBorders collisionBorders);
  ~Ground() = default;
};

//---------DOOR--------------//
class Door : public PhysicalElement
{
public:

  Door(int TILESIZE, int x, int y);
  ~Door() = default;

  sf::FloatRect getGlobalBounds() const override;
};

//---------BACKGROUND--------//
class Background : public DrawableElement
{
public:

  Background(int TILESIZE, int x, int y, std::string elementID);
  ~Background() = default;
};

#endif
