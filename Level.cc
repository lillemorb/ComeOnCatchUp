#include <vector>
#include "Elements.h"
#include "Level.h"

using namespace std;


Level::Level(int TILESIZE, int TILES_PER_ROW, const vector<int> & level_vector)
{
  for(unsigned int i{}; i < level_vector.size(); i++)
  {
    int x = i%TILES_PER_ROW;
    int y = i/TILES_PER_ROW;
	  
    // Create Ground objects
    if(level_vector.at(i) >= 10 && level_vector.at(i) <= 25)
    {	    
      int num{level_vector.at(i)};
      string name = "G" + to_string(num);

      // Set which borders should be collision handled for each type
      // of Ground object
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
	physicalElementVector_.push_back(groundPtrVector_.back());
    }

    // Create level elements
    switch(level_vector.at(i))
    {
    case 01:
      // Player is put in the first position of the element vectors
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
    case 32: 
      backgroundPtrVector_.push_back(new Background(TILESIZE, x, y, "Tomato2"));
      drawableElementVector_.push_back(backgroundPtrVector_.back());
      break;
    case 33: 
      backgroundPtrVector_.push_back(new Background(TILESIZE, x, y, "Sign"));
      drawableElementVector_.push_back(backgroundPtrVector_.back());
      break;
    case 34: 
      backgroundPtrVector_.push_back(new Background(TILESIZE, x, y, "KeyboardMove"));
      drawableElementVector_.push_back(backgroundPtrVector_.back());
      break;
    case 35: 
      backgroundPtrVector_.push_back(new Background(TILESIZE, x, y, "KeyboardJump"));
      drawableElementVector_.push_back(backgroundPtrVector_.back());
      break;
    case 36:
      triggerPtrVector_.push_back(new Trigger(TILESIZE, x, y));
      physicalElementVector_.push_back(triggerPtrVector_.back());
      break;
    default:
      break;
    }
  }
}

// Release memory by deleting pointers to all objects in Level
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
  for(unsigned int i{}; i < triggerPtrVector_.size(); ++i)
    delete triggerPtrVector_.at(i);
}
