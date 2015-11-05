#include <SFML/Graphics.hpp>

//TODO: flytta using namespace till .cc-fil när vi delar upp i .h/.cc
using namespace std;

//---------DRAWABLE--------------//
class DrawableElement
{
public:
  DrawableElement(string spriteID) : spriteID_{spriteID} {};
  virtual ~DrawableElement() = default;//=0 i design spec
  virtual string getSpriteID(){ return spriteID_; } //=0 i design spec
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
  
protected:
  enum PhysicalID_{Player, Door, Block, Ground};
};


//---------PLAYER--------------//
class Player : public PhysicalElement
{
public:
  Player()
    : PhysicalElement("Player")
    {
      rectangle_.setSize(sf::Vector2f(32,32));
    }
  ~Player() = default;
  
  void setPosition(int x, int y) { rectangle_.setPosition(x*32, y*32); }
};

//---------GROUND--------------//
class Ground : public PhysicalElement
{
public:
  
  Ground(int TILESIZE, int x, int y)
    : PhysicalElement("Ground")
    { 
      rectangle_.setPosition(x*TILESIZE, y*TILESIZE); 
      rectangle_.setSize(sf::Vector2f(32,32));
    }
  ~Ground() = default;
 
};


//---------DOOR--------------//
class Door : public PhysicalElement
{
public:

  Door()
    : PhysicalElement("Door")
    {
      rectangle_.setSize(sf::Vector2f(32,32));
    }

  ~Door() = default;
 
  void setPosition(int x, int y) { rectangle_.setPosition(x*32, y*32); }

};


//---------BLOCK--------------//
class Block : public PhysicalElement
{
public:
  
  Block(int TILESIZE, int x, int y) 
    : PhysicalElement("Block")
    { 
      rectangle_.setPosition(x*TILESIZE, y*TILESIZE); 
      rectangle_.setSize(sf::Vector2f(32,32));
    }
  ~Block() = default;

};
