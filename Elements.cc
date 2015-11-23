#ifndef ELEMENTS_CC
#define ELEMENTS_CC

#include <SFML/Graphics.hpp>
#include <iostream>

//TODO: flytta using namespace till .cc-fil när vi delar upp i .h/.cc
//TODO: städa funktioner, var ska de ligga, uppdatera övriga filer därefter
using namespace std;

//---------DRAWABLE--------------//
class DrawableElement
{
public:
  DrawableElement(string spriteID) : spriteID_{spriteID} {};
  virtual ~DrawableElement() = default;
  string getSpriteID(){ return spriteID_; }
  sf::Vector2f getPosition() { return rectangle_.getPosition(); }

protected:
  string spriteID_{};
  sf::RectangleShape rectangle_;
};

//---------PHYSICAL--------------//
class PhysicalElement : public DrawableElement
{
public:
  PhysicalElement(string spriteID): DrawableElement(spriteID){}
  virtual ~PhysicalElement() = default;
  //virtual string getPhysicalID()=0; 

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
  enum PhysicalID_{Player, Door, Block, Ground};
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
  sf::FloatRect getGlobalBounds() override {
    sf::FloatRect largeBounds{rectangle_.getGlobalBounds()};
    sf::FloatRect smallerBounds(largeBounds.left+4.0, largeBounds.top,
				largeBounds.width-8.0, largeBounds.height);
    return smallerBounds;
  }
  virtual const sf::Vector2f getSize(){ return sf::Vector2f(getGlobalBounds().width,
							    getGlobalBounds().height); }

  //Funktioner unika för Player
  float getGravity() { return gravity_; }
  void setFacingRight(bool facingRight) { facingRight_ = facingRight; }
  bool getFacingRight() { return facingRight_; }
  int getCurrentSprite() { return currentSprite_; }
  void setJump(bool jump) { jump_ = jump; }
  bool getJumpAllowed() { return jumpAllowed_; }
  void setJumpAllowed(bool jumpAllowed) { jumpAllowed_ = jumpAllowed; }
  void setWalk(bool walk) { walk_ = walk; }
  void setAnimationcycle() { 
    if (animationcycle_ == 4)
      animationcycle_ = 0;
    else
      animationcycle_++;
  }


  int getAnimation()
  {
    if (jump_ == true)
      currentSprite_ = 7;
    else if (walk_ == true)
      currentSprite_ = animationcycle_;
    else
      currentSprite_ = (animationcycle_%2 +5);
    return currentSprite_;
  }

private:
  bool facingRight_{true};
  int spriteNumberWalking_{0};
  int spriteNumberIdle_{0};
  int walkCycle_{0};
  int spriteDelay_{0};
  int currentSprite_{0};
  bool jump_{false};
  bool walk_{false};
  bool jumpAllowed_{true};
  int animationcycle_{0};
};

//---------BLOCK--------------//
class Block : public PhysicalElement
{
public:
  
  Block(int TILESIZE, int x, int y) 
    : PhysicalElement("Block")
    { 
      rectangle_.setPosition(x*TILESIZE, y*TILESIZE); 
      rectangle_.setSize(sf::Vector2f(TILESIZE,TILESIZE));      
    }
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
  
  Ground(int TILESIZE, int x, int y, string spriteID, CollisionBorders collisionBorders)
    : PhysicalElement(spriteID)
    { 
      collisionBorders_ = collisionBorders;
      rectangle_.setPosition(x*TILESIZE, y*TILESIZE); 
      rectangle_.setSize(sf::Vector2f(TILESIZE,TILESIZE));
    }
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

  Door(int TILESIZE, int x, int y)
    : PhysicalElement("Door")
    {
      rectangle_.setPosition(x*TILESIZE, y*TILESIZE);
      rectangle_.setSize(sf::Vector2f(TILESIZE,TILESIZE));
    }
  ~Door() = default;

  void setVelocity(sf::Vector2f) override { sf::Vector2f(0,0); } 
  void setPosition(sf::Vector2f) override { }
  void move(const sf::Vector2f offset) override { }

  // Lillemor: returnerar liten bounding box så att man ser Player "gå in i" dörren
  sf::FloatRect getGlobalBounds() override {
    sf::FloatRect largeBounds{rectangle_.getGlobalBounds()};
    sf::FloatRect smallerBounds(largeBounds.left+12.0, largeBounds.top,
				largeBounds.width-24.0, largeBounds.height);
    return smallerBounds;
  }
};

//---------BACKGROUND--------//
class Background : public DrawableElement
{
public:

  Background(int TILESIZE, int x, int y, string typeID)
    : DrawableElement(typeID)
    {
      rectangle_.setPosition(x*TILESIZE, y*TILESIZE);
      rectangle_.setSize(sf::Vector2f(TILESIZE,TILESIZE));
    }
  ~Background() = default;
};

#endif
