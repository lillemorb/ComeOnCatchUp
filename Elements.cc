#include <SFML/Graphics.hpp>

class Player
{
public:
  Player()
  {
    rectangle_.setSize(sf::Vector2f(32,32));
    rectangle_.setFillColor(sf::Color(255, 0, 0));
  }

  void setPosition(int x, int y) { rectangle_.setPosition(x*32, y*32); }
  void draw(sf::RenderWindow& window){ window.draw(rectangle_); }

private:
  sf::RectangleShape rectangle_;

};


class Ground
{
public:
  
  Ground(int TILESIZE, int x, int y) 
  { 
    rectangle_.setPosition(x*TILESIZE, y*TILESIZE); 
    rectangle_.setSize(sf::Vector2f(32,32));
    rectangle_.setFillColor(sf::Color(150, 255, 100));
  }

  void draw(sf::RenderWindow& window){ window.draw(rectangle_); }

private:
  sf::RectangleShape rectangle_;
};
