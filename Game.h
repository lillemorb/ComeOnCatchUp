//Game.cc
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Logic.h"
#include "Graphics.h"
#include "GameSounds.h"
#include <vector>
#include <fstream>
#include <iterator>
using namespace std;

// TODO: dela upp i .cc/.h-filer
// undersöka var liten minnesläcka finns (kolla mha valgrind)

class Game
{
public:
  ~Game() { delete currLevelPtr_; }
  
  int deathCounter{0};

  int run();
  void menu(vector<int> lvl);
	void delayTime();
	void load_level(vector<int> lvl, int current_level);

private:

  const int TILESIZE{32};
  const int TILES_PER_ROW{24};
  const int TILES_PER_COLUMN{18};
  unsigned int current_level{1};
  unsigned int current_menu{1};
  unsigned int current_menu_row{1};
  unsigned int menu_row{3};
  unsigned int menu_player_pos{500};
  int vector_size{TILES_PER_ROW*TILES_PER_COLUMN};
  enum GameState{Playing, Dead, VictoryScreen, Pause, LevelSel, Menu, Exit}; 
  GameState gamestate_{Playing};
  Logic::ActionResult actionResult_= Logic::Continue;
  Level* currLevelPtr_{};
  Logic logic_;
  Graphics graphics_;
  int xPix_{768};
  int yPix_{576};
  bool jumping_{false};
  sf::Clock clock;

};

#endif
