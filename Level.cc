#include <vector>
#include "Elements.cc"

using namespace std;


class Level
{
public:
  Level(int TILESIZE, int TILES_PER_ROW, int TILES_PER_COLUMN, vector<int> level_vector)
  {
    for(int i{}; i < level_vector.size(); i++)
      {
	int x = i%TILES_PER_ROW;
	int y = i/TILES_PER_ROW;

	switch(level_vector.at(i))
	  {
	  case 01:
	    ground_vector_.push_back(Ground(TILESIZE, x, y));
	    break;
	  case 02:
	    player_.setPosition(x, y);
	    break;
	  default:
	    break;
	  }
      }
  }
    

  ~Level() = default;

  //vector<DrawableElement*> getLevelDrawableLayout();
  //vector<PhysicalElement*> getLevelPhysicalLayout();
  
  void draw(sf::RenderWindow& window)
  {
    player_.draw(window);
    for(int i{}; i < ground_vector_.size(); i++)
      {
	ground_vector_.at(i).draw(window);
      }
  }


private:

  Player player_;
  //Door door_;
  //vector<Block> block_vector_;
  vector<Ground> ground_vector_;
  
};

