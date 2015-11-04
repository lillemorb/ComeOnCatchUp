#include <SFML/Graphics.hpp>

//TODO: flytta using namespace till .cc-fil när vi delar upp i .h/.cc
using namespace std;

class DrawableElement
{
public:
  virtual ~DrawableElement() = default;
  virtual string getSpriteID() = 0;
  void draw(sf::RenderWindow& window){ window.draw(rectangle_); }

protected:
  sf::RectangleShape rectangle_;
};


class Player : public DrawableElement
{
public:
  Player()
    {
      rectangle_.setSize(sf::Vector2f(32,32));
      rectangle_.setFillColor(sf::Color(255, 0, 0));
    }
  virtual ~Player() = default;

  string getSpriteID() override
    {
      return "Player";
    }

  void setPosition(int x, int y) { rectangle_.setPosition(x*32, y*32); }
};


class Ground : public DrawableElement
{
public:
  
  Ground(int TILESIZE, int x, int y) 
    { 
      rectangle_.setPosition(x*TILESIZE, y*TILESIZE); 
      rectangle_.setSize(sf::Vector2f(32,32));
      rectangle_.setFillColor(sf::Color(150, 255, 100));
    }
  virtual ~Ground() = default;
  string getSpriteID() override
    {
      return "Ground";
    }
};