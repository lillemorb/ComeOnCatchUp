#include <SFML/Graphics.hpp>
#include <iostream>
#include "Elements.h"

//TODO: flytta using namespace till .cc-fil när vi delar upp i .h/.cc
//TODO: städa funktioner, var ska de ligga, uppdatera övriga filer därefter
using namespace std;

// Returnerar en mindre bounding box för player än tilesize
sf::FloatRect Player::getGlobalBounds() {
	sf::FloatRect largeBounds{rectangle_.getGlobalBounds()};
	sf::FloatRect smallerBounds(largeBounds.left+4.0, largeBounds.top,
															largeBounds.width-8.0, largeBounds.height);
	return smallerBounds;
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

Block::Block(int TILESIZE, int x, int y) 
	: PhysicalElement("Block")
{ 
	rectangle_.setPosition(x*TILESIZE, y*TILESIZE); 
	rectangle_.setSize(sf::Vector2f(TILESIZE,TILESIZE));      
}
Ground::Ground(int TILESIZE, int x, int y, string elementID, CollisionBorders collisionBorders)
	: PhysicalElement(elementID)
{ 
	collisionBorders_ = collisionBorders;
	rectangle_.setPosition(x*TILESIZE, y*TILESIZE); 
	rectangle_.setSize(sf::Vector2f(TILESIZE,TILESIZE));
}
Door::Door(int TILESIZE, int x, int y)
	: PhysicalElement("Door")
{
	rectangle_.setPosition(x*TILESIZE, y*TILESIZE);
	rectangle_.setSize(sf::Vector2f(TILESIZE,TILESIZE));
}

sf::FloatRect Door::getGlobalBounds() {
	sf::FloatRect largeBounds{rectangle_.getGlobalBounds()};
	sf::FloatRect smallerBounds(largeBounds.left+15.0, largeBounds.top+26.0,
															largeBounds.width-30.0, largeBounds.height-26.0);
	return smallerBounds;
}

Background::Background(int TILESIZE, int x, int y, string elementID)
	: DrawableElement(elementID)
{
	rectangle_.setPosition(x*TILESIZE, y*TILESIZE);
	rectangle_.setSize(sf::Vector2f(TILESIZE,TILESIZE));
}
