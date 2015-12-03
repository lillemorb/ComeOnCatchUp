#include <vector>
#include "Elements.h"
#include "Level.h"
//TODO: ta bort efter spårutskrift tagits bort
#include <iostream>

using namespace std;


Level::Level(int TILESIZE, int TILES_PER_ROW, const vector<int> & level_vector)
{
  for(unsigned int i{}; i < level_vector.size(); i++)
  {
    int x = i%TILES_PER_ROW;
    int y = i/TILES_PER_ROW;
	  
    // Check the ground-objects
    if(level_vector.at(i) >= 10 && level_vector.at(i) <= 25)
    {	    
      int num{level_vector.at(i)};
      string name = "G" + to_string(num);

      PhysicalElement::CollisionBorders collBorders{};

      if(num == 12 || num == 13 || num == 15 || num == 16 ||
	 num == 18 || num == 19 || num == 21 || num == 25)
	collBorders.left = false;
      if(num == 11 || num == 12 || num == 14 || num == 15 ||
	 num == 17 || num == 18 || num == 20 || num == 25)
	collBorders.right = false;
      if((num >= 11 && num <= 16) || num == 23 || num == 24)
	collBorders.up = false;
      if((num >= 14 && num <= 19) || num == 22 || num == 24)
	collBorders.down = false;

      groundPtrVector_.push_back(new Ground(TILESIZE, x, y, name, collBorders));
      drawableElementVector_.push_back(groundPtrVector_.back());

      //Ground without borders will not be handled by Logic (no collision)
      if(level_vector.at(i) != 15)
      {
	physicalElementVector_.push_back(groundPtrVector_.back());
      }
    }

    switch(level_vector.at(i))
    {
    case 01:
      playerPtr_ = new Player(TILESIZE, x, y);
      if(drawableElementVector_.size() == 0 )
      {
	drawableElementVector_.push_back(playerPtr_);
	physicalElementVector_.push_back(playerPtr_);
      }
      else
      {
	drawableElementVector_.insert(drawableElementVector_.begin(), playerPtr_);
	physicalElementVector_.insert(physicalElementVector_.begin(), playerPtr_);
      }
      break;
    case 02:
      doorPtr_ = new Door(TILESIZE, x, y);
      drawableElementVector_.push_back(doorPtr_);
      physicalElementVector_.push_back(doorPtr_);
      break;
    case 03:
      blockPtrVector_.push_back(new Block(TILESIZE, x, y));
      drawableElementVector_.push_back(blockPtrVector_.back());
      physicalElementVector_.push_back(blockPtrVector_.back());
      break;
    case 30:
      backgroundPtrVector_.push_back(new Background(TILESIZE, x, y, "Start"));
      drawableElementVector_.push_back(backgroundPtrVector_.back());
      break;
    case 31: 
      backgroundPtrVector_.push_back(new Background(TILESIZE, x, y, "Goal"));
      drawableElementVector_.push_back(backgroundPtrVector_.back());
      break;
    default:
      break;
    }
  }
}

//Release memory by deleting pointers to all objects in Level
Level::~Level()
{
  delete playerPtr_;
  delete doorPtr_;
  for(unsigned int i{}; i < groundPtrVector_.size(); ++i)
    delete groundPtrVector_.at(i);
  for(unsigned int i{}; i < blockPtrVector_.size(); ++i)
    delete blockPtrVector_.at(i);
  for(unsigned int i{}; i < backgroundPtrVector_.size(); ++i)
    delete backgroundPtrVector_.at(i);
}
