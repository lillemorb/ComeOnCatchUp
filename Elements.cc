#ifndef ELEMENTS_CC
#define ELEMENTS_CC

#include <SFML/Graphics.hpp>

//TODO: flytta using namespace till .cc-fil när vi delar upp i .h/.cc
//TODO: städa funktioner, var ska de ligga, uppdatera övriga filer därefter
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
  
  //ska bara finnas för player och block
  void move(const sf::Vector2f offset){rectangle_.move(offset);}

  //virtual void setPosition(int x, int y)  = 0;
  void setOnGround(bool onGround) { onGround_ = onGround; }
  bool getOnGround() { return onGround_; }
  virtual sf::FloatRect getGlobalBounds() = 0;
// Innan ändring
//  sf::FloatRect getGlobalBounds(){ return rectangle_.getGlobalBounds(); }
  const sf::Vector2f getSize(){ return rectangle_.getSize(); }

protected:
  enum PhysicalID_{Player, Door, Block, Ground};
  sf::Vector2f velocity_{};
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
  float getGravity() { return gravity_; }
  void setFacingRight(bool facingRight) { facingRight_ = facingRight; }
  bool getFacingRight() { return facingRight_; }

	// Returnerar en (något) mindre bounding box för player än tilesize
	// TODO: fixa bug för att sedan minska globalBounds ytterligare tills den stämmer med
	// utseendet på players sprite. Bug: Player vibrerar vid kollision med Block vid mindre
	// bounding box, bland annat.
	sf::FloatRect getGlobalBounds() override {
		sf::FloatRect largeBounds{rectangle_.getGlobalBounds()};
		sf::FloatRect smallerBounds(largeBounds.left+2.0, largeBounds.top, largeBounds.width-4.0, largeBounds.height);
		return smallerBounds;
	}

private:
  float gravity_{0.5f};
  bool facingRight_{true};
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
	sf::FloatRect getGlobalBounds() override { return rectangle_.getGlobalBounds(); }
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
	sf::FloatRect getGlobalBounds() override { return rectangle_.getGlobalBounds(); }
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
