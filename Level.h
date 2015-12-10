#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "Elements.h"
/*************************************************
Level:
Decides the strukture of the levels by taking in a 
vector with integers that represent the different
elements. This is sent to the constructor with the 
size of every tile and the windows dimensions 
expressed in tiles. Every tile is asigned an element
by the layout of the vector except for those that
are 00, they are just "empty space" where the player
can move. Level can provide pointers to the elements
when the level is drawn or updated.
*************************************************/
class Level
{
public:
  Level(int TILESIZE, int TILES_PER_ROW, const std::vector<int> & level_vector);
  ~Level();

  std::vector<DrawableElement*> & getLevelDrawableLayout(){ return drawableElementVector_; }
  std::vector<PhysicalElement*> & getLevelPhysicalLayout(){ return physicalElementVector_; }

private:
  Level(const Level&) = delete;
  Level& operator=(const Level&) = delete;

  Player* playerPtr_{};
  Door* doorPtr_{};
  std::vector<Block*> blockPtrVector_{};
  std::vector<Ground*> groundPtrVector_{};
  std::vector<Background*> backgroundPtrVector_{};
  std::vector<Trigger*> triggerPtrVector_{};
  std::vector<DrawableElement*> drawableElementVector_{};
  std::vector<PhysicalElement*> physicalElementVector_{}; 
};

#endif
