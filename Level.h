#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "Elements.h"

class Level
{
public:
  Level(int TILESIZE, int TILES_PER_ROW, const std::vector<int> & level_vector);
  ~Level();

  std::vector<DrawableElement*> & getLevelDrawableLayout(){ return drawableElementVector_; }
  std::vector<PhysicalElement*> & getLevelPhysicalLayout(){ return physicalElementVector_; }

private:
  Player* playerPtr_{};
  Door* doorPtr_{};
  std::vector<Block*> blockPtrVector_{};
  std::vector<Ground*> groundPtrVector_{};
  std::vector<Background*> backgroundPtrVector_{};
  std::vector<DrawableElement*> drawableElementVector_{};
  std::vector<PhysicalElement*> physicalElementVector_{}; 
};

#endif
