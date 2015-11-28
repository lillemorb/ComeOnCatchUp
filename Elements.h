#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

//---------DRAWABLE--------------//
class DrawableElement
{
public:
  DrawableElement(string elementID) : elementID_{elementID} {};
  virtual ~DrawableElement() = default;
  string getElementID(){ return elementID_; }
  sf::Vector2f getPosition() { return rectangle_.getPosition(); }

protected:
  string elementID_{};
  sf::RectangleShape rectangle_;
};

//---------PHYSICAL--------------//
class PhysicalElement : public DrawableElement
{
public:
  PhysicalElement(string elementID): DrawableElement(elementID){}
  virtual ~PhysicalElement() = default;
 
  struct CollisionBorders
  { bool left{true}; bool right{true}; bool up{true}; bool down{true}; };

  //Följande funktioner implementeras som tomma för de objekt
  //som de inte berör
  virtual void setVelocity(sf::Vector2f vel)  = 0;
  virtual void setPosition(sf::Vector2f pos)  = 0;
  virtual void move(const sf::Vector2f offset) = 0; 

  //Alla ärvande klasser implementerar egen variant
  virtual sf::FloatRect getGlobalBounds() = 0;

  //Implementeras här men ärvande klasser kan implementera egen variant
  //( i Player returnerar den storleken på bounding box ist f rectangle )
  virtual const sf::Vector2f getSize(){ return rectangle_.getSize();}

  virtual void setBelowWindow(bool) {}
  virtual bool getBelowWindow() { return false; }

  //Implementeras här och ärvs sedan utan polymorfi
  sf::Vector2f getVelocity() { return velocity_; }
  void setOnGround(bool onGround) { onGround_ = onGround; }
  bool getOnGround() { return onGround_; }
  CollisionBorders getCollisionBorders() { return collisionBorders_; }

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
  //Player(int TILESIZE, int x, int y)
  Player(int TILESIZE, float x, float y)
    : PhysicalElement("Player")
    {
      rectangle_.setPosition(x*TILESIZE, y*TILESIZE);      
      rectangle_.setSize(sf::Vector2f(TILESIZE,TILESIZE));
    }
  ~Player() = default;

  void setVelocity(sf::Vector2f vel) override { velocity_ = vel; } 
  void setPosition(sf::Vector2f pos) override { rectangle_.setPosition(pos.x, pos.y); }
  void move(const sf::Vector2f offset) override { rectangle_.move(offset); }

  // Returnerar en mindre bounding box för player än tilesize
  sf::FloatRect getGlobalBounds() override;
  virtual const sf::Vector2f getSize(){ return sf::Vector2f(getGlobalBounds().width,
							    getGlobalBounds().height); }

  //Funktioner unika för Player
  float getGravity() { return gravity_; }
  void setFacingRight(bool facingRight) { facingRight_ = facingRight; }
  bool getFacingRight() { return facingRight_; }
  void resetCurrentSprite() { currentSprite_.x = 0; }
  sf::Vector2f getCurrentSprite() { return currentSprite_; }
  void setJump(bool jump) { jump_ = jump; }
  void setFalling(bool falling) { falling_ = falling; }
  void setDeath(bool death) { death_ = death; }
  bool getDeath() { return death_; }
  bool getJumpAllowed() { return jumpAllowed_; }
  void setJumpAllowed(bool jumpAllowed) { jumpAllowed_ = jumpAllowed; }
  void setWalk(bool walk) { walk_ = walk; }
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

  void setVelocity(sf::Vector2f vel) override { velocity_ = vel; } 
  void setPosition(sf::Vector2f pos) override { rectangle_.setPosition(pos.x, pos.y); }
  void move(const sf::Vector2f offset) override { rectangle_.move(offset); }
  sf::FloatRect getGlobalBounds() override { return rectangle_.getGlobalBounds(); }
  
  void setBelowWindow(bool out) override { belowWindow_=out; }
  bool getBelowWindow() override {return belowWindow_;}

private:
  bool belowWindow_{false};
};

//---------GROUND--------------//
class Ground : public PhysicalElement
{
public:
  
  Ground(int TILESIZE, int x, int y, string elementID, CollisionBorders collisionBorders);
  ~Ground() = default;

  void setVelocity(sf::Vector2f) override { velocity_ = sf::Vector2f(0,0); } 
  void setPosition(sf::Vector2f) override { }
  void move(const sf::Vector2f offset) override { }
  sf::FloatRect getGlobalBounds() override { return rectangle_.getGlobalBounds(); }
};

//---------DOOR--------------//
class Door : public PhysicalElement
{
public:

  Door(int TILESIZE, int x, int y);
  ~Door() = default;

  void setVelocity(sf::Vector2f) override { sf::Vector2f(0,0); } 
  void setPosition(sf::Vector2f) override { }
  void move(const sf::Vector2f offset) override { }

  // returnerar liten bounding box så att man ser Player "gå in i" dörren
  sf::FloatRect getGlobalBounds() override;
};

//---------BACKGROUND--------//
class Background : public DrawableElement
{
public:

  Background(int TILESIZE, int x, int y, string elementID);
  ~Background() = default;
};

#endif
