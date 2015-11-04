#include <vector>
#include "Elements.cc"

using namespace std;

class Level
{
public:
  Level(int TILESIZE, int TILES_PER_ROW, int TILES_PER_COLUMN, vector<int> level_vector)
    {

      for(unsigned int i{}; i < level_vector.size(); i++)
      {
	int x = i%TILES_PER_ROW;
	int y = i/TILES_PER_ROW;

	switch(level_vector.at(i))
	{
	case 01:
	  ground_vector_.push_back(Ground(TILESIZE, x, y));
	  drawableElement_vector_.push_back(&ground_vector_.back());
	  break;
	case 02:
	  player_.setPosition(x, y);
	  drawableElement_vector_.push_back(&player_);
	  break;
	case 03:
	  door_.setPosition(x, y);
	  drawableElement_vector_.push_back(&door_);
	  break;
	case 04:
	  block_vector_.push_back(Block(TILESIZE, x, y));
	  drawableElement_vector_.push_back(&block_vector_.back());
	  break;
	default:
	  break;
	}
      }
    }
    

  ~Level() = default;

  vector<DrawableElement*> getLevelDrawableLayout() 
  {
    return drawableElement_vector_;
  }

  //vector<PhysicalElement*> getLevelPhysicalLayout();
  
  void draw(sf::RenderWindow& window)
    {
      player_.draw(window);
      door_.draw(window);
      for(unsigned int i{}; i < ground_vector_.size(); i++)
      {
	ground_vector_.at(i).draw(window);
      }
      for(unsigned int i{}; i < block_vector_.size(); i++)
      {
	block_vector_.at(i).draw(window);
      }
    }


private:

  Player player_;
  Door door_;
  vector<Block> block_vector_;
  vector<Ground> ground_vector_;
  vector<DrawableElement*> drawableElement_vector_;
  
};

