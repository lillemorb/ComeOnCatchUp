//Game.h
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Logic.h"
#include "Graphics.h"
#include "GameSounds.h"
#include <vector>

// TODO: undersöka var liten minnesläcka finns (kolla mha valgrind)

class Game
{
public:
  Game() { logic_.setPix(xPix_, yPix_); }
  ~Game() { delete currLevelPtr_; }

  int run();
  void menu(const std::vector<int> & lvl);
  void delayTime();
  void load_level(const std::vector<int> & lvl, int current_level);

private:

  const int TILESIZE{32};
  const int TILES_PER_ROW{24};
  const int TILES_PER_COLUMN{18};
  unsigned int current_level{1};
  unsigned int current_menu_row{1};
  // Number of rows for the specifik menu
  unsigned int menu_row_count{3};
  unsigned int menu_player_pos{};
  int vector_size{TILES_PER_ROW*TILES_PER_COLUMN};
  enum GameState{Playing, Dead, VictoryScreen, Pause, LevelSel, Menu, Exit};
  enum CurrentMenu{MainMenu, VictoryMenu, LevelMenu};
  GameState gamestate_{Playing};
  CurrentMenu currentmenu_{};
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
