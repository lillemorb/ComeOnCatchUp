#include <SFML/Graphics.hpp>
// Todo: Ändra inkludering av Elments.cc 
#include "Elements.cc"

class Graphics
{
public:
  void drawLevel(Level current, sf::RenderWindow& window)
    {
      vector<DrawableElement*> levelVec = current.getLevelDrawableLayout();
      sf::RectangleShape rectangle_;
      rectangle_.setSize(sf::Vector2f(32,32));
      for(unsigned int i{}; i < levelVec.size(); i++)
      {
	string tempID = levelVec.at(i)->getSpriteID();
	sf::Vector2f tempPos = levelVec.at(i)->getPosition();
	  
	if (tempID == "Player")
	{
	  rectangle_.setFillColor(sf::Color(255, 0, 0)); 
	  rectangle_.setPosition(tempPos.x,tempos.y);
	  
	}
	else if (tempID == "Ground") 
	{
	  rectangle_.setFillColor(sf::Color(150, 255, 100));
	  rectangle_.setPosition(tempPos.x,tempos.y);
	}
	window.draw(rectangle_);
      }
    }
}
