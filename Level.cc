#include <vector>
#include "Elements.cc"
//TODO: ta bort efter spårutskrift tagits bort
#include <iostream>

using namespace std;

class Level
{
public:
  Level(int TILESIZE, int TILES_PER_ROW, int TILES_PER_COLUMN, vector<int> level_vector)
    {

      //Flyttat ned push_back av objektens adresser till i getLevelDrawableLayout()
      for(unsigned int i{}; i < level_vector.size(); i++)
      {
	int x = i%TILES_PER_ROW;
	int y = i/TILES_PER_ROW;

	switch(level_vector.at(i))
	{
	case 01:
	  ground_vector_.push_back(Ground(TILESIZE, x, y));
//	  drawableElement_vector_.push_back(&ground_vector_.back());
	  break;
	case 02:
	  player_.setPosition(x, y);
//	  drawableElement_vector_.push_back(&player_);
	  //FELSÖKNING: spårutskrift
/*
	  cout << "Nu stoppas Player in i vektorn: " << endl;
	  cout << drawableElement_vector_.back()->getSpriteID() << endl;
	  cout << "Adresser: " << endl << &player_ << endl
	       << drawableElement_vector_.back() << endl;
*/
	  //Slut spårutskrift
	  break;
	case 03:
	  door_.setPosition(x, y);
//	  drawableElement_vector_.push_back(&door_);
	  break;
	case 04:
	  block_vector_.push_back(Block(TILESIZE, x, y));
//	  drawableElement_vector_.push_back(&block_vector_.back());
	  break;
	default:
	  break;
	}
      }
    }    

  ~Level() = default;

  vector<DrawableElement*> getLevelDrawableLayout()
  {
    //Stoppar in adresserna till objekten i drawableElement_vector_ eftersom....
    drawableElement_vector_.clear();
    drawableElement_vector_.push_back(&player_);
    drawableElement_vector_.push_back(&door_);
    for(unsigned int i{}; i < block_vector_.size(); ++i)
      drawableElement_vector_.push_back(&(block_vector_.at(i)));
    for(unsigned int i{}; i < ground_vector_.size(); ++i)
      drawableElement_vector_.push_back(&(ground_vector_.at(i)));

    //.....player_ inte hade kvar sin adress som hade stoppats in i drawableElements
    // i konstruktorn till Level, player_ hade bytt adress => vektorn pekade fel => seg fault

    //Spårutskrifter
/*
    cout << &player_ << endl << drawableElement_vector_.at(0) << endl;

    DrawableElement* tempPtr = &player_;
    cout << tempPtr->getSpriteID() << ": utskrift fungerar" << endl << tempPtr << endl << endl;
    vector<DrawableElement*> tempVec{};
    tempVec.push_back(&player_);
    cout << tempVec.back()->getSpriteID() << ": utskrift fungerar igen" << endl << endl;
*/
/*
    for(unsigned int i{}; i < drawableElement_vector_.size(); ++i)
    {
      cout << drawableElement_vector_.at(i)->tempFun() << endl;
      cout << drawableElement_vector_.at(i)->getSpriteID() << endl;
    }
*/
    // Slut spårutskrifter

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
  vector<Block> block_vector_{};
  vector<Ground> ground_vector_{};
  vector<DrawableElement*> drawableElement_vector_{};
  
};

