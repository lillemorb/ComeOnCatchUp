#ifndef ELEMENTS_CC
#define ELEMENTS_CC

#include <SFML/Graphics.hpp>

//TODO: flytta using namespace till .cc-fil när vi delar upp i .h/.cc
using namespace std;

//---------DRAWABLE--------------//
class DrawableElement
{
public:
  DrawableElement(string spriteID) : spriteID_{spriteID} {};
  virtual ~DrawableElement() = default;//=0 i design spec
  virtual string getSpriteID(){ return spriteID_; }
  sf::Vector2f getPosition() { return rectangle_.getPosition(); }

protected:
  string spriteID_{};
  sf::RectangleShape rectangle_;
};

//---------PHYSICAL--------------//
class PhysicalElement : public DrawableElement
{
public:
  PhysicalElement(string spriteID): DrawableElement(spriteID){};
  virtual ~PhysicalElement() = default;//=0 i design spec
  //virtual string getPhysicalID()=0; 

  sf::Vector2f getVelocity() { return velocity_; }

  //Följande funktioner implementeras som tomma för de objekt
  //som de inte berör
  virtual void setVelocity(sf::Vector2f vel)  = 0;
  virtual void setPosition(sf::Vector2f pos)  = 0;
  //virtual void setPosition(int x, int y)  = 0;

protected:
  enum PhysicalID_{Player, Door, Block, Ground};
  sf::Vector2f velocity_{};

};

//---------PLAYER--------------//
class Player : public PhysicalElement
{
public:
  Player(int TILESIZE, int x, int y)
    : PhysicalElement("Player")
    {
      rectangle_.setPosition(x*TILESIZE, y*TILESIZE);      
      rectangle_.setSize(sf::Vector2f(TILESIZE,TILESIZE));
    }
  ~Player() = default;
  void setVelocity(sf::Vector2f vel) override { velocity_ = vel; } 
  void setPosition(sf::Vector2f pos) override { rectangle_.setPosition(pos.x, pos.y); }
  //void setPosition(int x, int y) override { rectangle_.setPosition(x*32, y*32); }
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
  //void setPosition(int x, int y) override { rectangle_.setPosition(x*32, y*32); }
};

//---------GROUND--------------//
class Ground : public PhysicalElement
{
public:
  
  Ground(int TILESIZE, int x, int y)
    : PhysicalElement("Ground")
    { 
      rectangle_.setPosition(x*TILESIZE, y*TILESIZE); 
      rectangle_.setSize(sf::Vector2f(TILESIZE,TILESIZE));
    }
  ~Ground() = default;
  void setVelocity(sf::Vector2f) override { velocity_ = sf::Vector2f(0,0); } 
  void setPosition(sf::Vector2f) override { }
  //void setPosition(int, int) override { } 
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
  //void setPosition(int x, int y) override { rectangle_.setPosition(x*32, y*32); }
};


#endif
