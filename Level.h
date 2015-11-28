#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "Elements.h"
#include <iostream>

using namespace std;

class Level
{
public:
  Level(int TILESIZE, int TILES_PER_ROW, vector<int> level_vector);
  ~Level();

  vector<DrawableElement*> &  getLevelDrawableLayout(){ return drawableElementVector_; }
  vector<PhysicalElement*> & getLevelPhysicalLayout(){ return physicalElementVector_; }

private:
  Player* playerPtr_{};
  Door* doorPtr_{};
  vector<Block*> blockPtrVector_{};
  vector<Ground*> groundPtrVector_{};
  vector<Background*> backgroundPtrVector_{};
  vector<DrawableElement*> drawableElementVector_{};
  vector<PhysicalElement*> physicalElementVector_{}; 
};

#endif
