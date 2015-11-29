#include "Elements.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

//---------DRAWABLE--------------//
DrawableElement::~DrawableElement() = default;

//---------PHYSICAL--------------//
PhysicalElement::~PhysicalElement() = default;

//---------PLAYER--------------//
Player::Player(int TILESIZE, float x, float y)
	: PhysicalElement("Player")
{
	rectangle_.setPosition(x*TILESIZE, y*TILESIZE);      
	rectangle_.setSize(sf::Vector2f(TILESIZE,TILESIZE));
}

// Returnerar en mindre bounding box för player än tilesize
sf::FloatRect Player::getGlobalBounds() const {
	sf::FloatRect largeBounds{rectangle_.getGlobalBounds()};
	sf::FloatRect smallerBounds(largeBounds.left+4.0, largeBounds.top,
															largeBounds.width-8.0, largeBounds.height);
	return smallerBounds;
}

sf::Vector2f Player::getSize() const
{
	return sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height);
}

void Player::setAnimationcycle() { 
	if (animationcycle_ == 4)
		animationcycle_ = 0;
	else
		animationcycle_++;
}

sf::Vector2f Player::getAnimation()
{
	if (death_ == true)
	{
		currentSprite_.x = animationcycle_*32;
		currentSprite_.y = 96;
	}
	else if (jump_ == true)
	{
		currentSprite_.x = 0;
		currentSprite_.y = 64;
	}
	else if (falling_ == true)
	{
		currentSprite_.x = 32;
		currentSprite_.y = 64;
	}
	else if (walk_ == true && jump_ == false && falling_ == false)
	{
		currentSprite_.x = animationcycle_*32;
		currentSprite_.y = 0;
	}
	else
	{
		currentSprite_.x = animationcycle_*32;
		currentSprite_.y = 32;
	}
	return currentSprite_;
}

//---------BLOCK--------------//
Block::Block(int TILESIZE, int x, int y) 
	: PhysicalElement("Block")
{ 
	rectangle_.setPosition(x*TILESIZE, y*TILESIZE); 
	rectangle_.setSize(sf::Vector2f(TILESIZE,TILESIZE));      
}

//---------GROUND--------------//
Ground::Ground(int TILESIZE, int x, int y, string elementID,
							 CollisionBorders collisionBorders)
	: PhysicalElement(elementID)
{ 
	collisionBorders_ = collisionBorders;
	rectangle_.setPosition(x*TILESIZE, y*TILESIZE); 
	rectangle_.setSize(sf::Vector2f(TILESIZE,TILESIZE));
}

//---------DOOR--------------//
Door::Door(int TILESIZE, int x, int y)
	: PhysicalElement("Door")
{
	rectangle_.setPosition(x*TILESIZE, y*TILESIZE);
	rectangle_.setSize(sf::Vector2f(TILESIZE,TILESIZE));
}

// returnerar liten bounding box så att man ser Player "gå in i" dörren
sf::FloatRect Door::getGlobalBounds() const {
	sf::FloatRect largeBounds{rectangle_.getGlobalBounds()};
	sf::FloatRect smallerBounds(largeBounds.left+15.0, largeBounds.top+26.0,
															largeBounds.width-30.0, largeBounds.height-26.0);
	return smallerBounds;
}

//---------BACKGROUND--------//
Background::Background(int TILESIZE, int x, int y, string elementID)
	: DrawableElement(elementID)
{
	rectangle_.setPosition(x*TILESIZE, y*TILESIZE);
	rectangle_.setSize(sf::Vector2f(TILESIZE,TILESIZE));
}
