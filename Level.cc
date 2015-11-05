#ifndef LEVEL_CC
#define LEVEL_CC

#include <vector>
#include "Elements.cc"
//TODO: ta bort efter spårutskrift tagits bort
#include <iostream>

using namespace std;

class Level
{
public:
  Level(int TILESIZE, int TILES_PER_ROW, vector<int> level_vector)
    {
      for(unsigned int i{}; i < level_vector.size(); i++)
      {
	int x = i%TILES_PER_ROW;
	int y = i/TILES_PER_ROW;

	switch(level_vector.at(i))
	{
	case 01:
	  groundPtrVector_.push_back(new Ground(TILESIZE, x, y));
	  drawableElementVector_.push_back(groundPtrVector_.back());
	  physicalElementVector_.push_back(groundPtrVector_.back());
	  break;
	case 02:
	  playerPtr_ = new Player();
	  playerPtr_->setPosition(x, y);
	  drawableElementVector_.push_back(playerPtr_);
	  physicalElementVector_.push_back(playerPtr_);
	  break;
	case 03:
	  doorPtr_ = new Door();
	  doorPtr_->setPosition(x, y);
	  drawableElementVector_.push_back(doorPtr_);
	  physicalElementVector_.push_back(doorPtr_);
	  break;
	case 04:
	  blockPtrVector_.push_back(new Block(TILESIZE, x, y));
	  drawableElementVector_.push_back(blockPtrVector_.back());
	  physicalElementVector_.push_back(blockPtrVector_.back());
	  break;
	default:
	  break;
	}
      }
    }

// Frigör minne genom att deleta pekare till alla objekt i Level
  ~Level()
    {
      cout << "Tar bort Level" << endl;
      delete playerPtr_;
      delete doorPtr_;
      for(unsigned int i{}; i < groundPtrVector_.size(); ++i)
	delete groundPtrVector_.at(i);
      for(unsigned int i{}; i < blockPtrVector_.size(); ++i)
	delete blockPtrVector_.at(i);
    };

  vector<DrawableElement*> &  getLevelDrawableLayout()
    {
      return drawableElementVector_;
    }

  vector<PhysicalElement*> & getLevelPhysicalLayout()
    {
      return physicalElementVector_;
    }

private:
  // Håller reda på objekten med dynamiskt allokerat minne
  // för att inte få minnesfel
  Player* playerPtr_{};
  Door* doorPtr_{};
  vector<Block*> blockPtrVector_{};
  vector<Ground*> groundPtrVector_{};
  vector<DrawableElement*> drawableElementVector_{};
  vector<PhysicalElement*> physicalElementVector_{}; 
};

#endif
