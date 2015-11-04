#include <SFML/Graphics.hpp>
#include <iostream>
// Todo: Ändra inkludering då filer delas upp i .h/.cc
//#include "Elements.cc"
#include "Level.cc"
#include <vector>

using namespace std;

class Graphics
{
public:
  void drawLevel(Level current, sf::RenderWindow& window)
    {
      cout << "HEJ HEJ" << endl;
      vector<DrawableElement*> levelVec{current.getLevelDrawableLayout()};
      cout << levelVec.at(0) << endl;
      cout << levelVec.at(0)->tempFun() << endl;
      if(dynamic_cast<DrawableElement*>(levelVec.at(0)))
	cout << "DrawableElement" << endl;
      if(dynamic_cast<Player*>(levelVec.at(0)))
	cout << "Player" << endl;

      cout << levelVec.at(0)->getSpriteID() << endl;
      cout << "Lite text" << endl;
      sf::RectangleShape rectangle_;
      rectangle_.setSize(sf::Vector2f(32,32));
      for(unsigned int i{}; i < levelVec.size(); i++)
      {
	string tempID = levelVec.at(i)->getSpriteID();
	sf::Vector2f tempPos = levelVec.at(i)->getPosition();
	  
	if (tempID == "Player")
	{
	  rectangle_.setFillColor(sf::Color(255, 0, 0)); 
	  rectangle_.setPosition(tempPos.x,tempPos.y);	  
	}
	else if (tempID == "Ground") 
	{
	  rectangle_.setFillColor(sf::Color(150, 255, 100));
	  rectangle_.setPosition(tempPos.x,tempPos.y);
	}
	window.draw(rectangle_);
      }
    }

};
