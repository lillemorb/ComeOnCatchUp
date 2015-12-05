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
  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

  const int TILESIZE{32};
  const int TILES_PER_ROW{24};
  const int TILES_PER_COLUMN{18};
  unsigned int current_level{1};
  unsigned int current_menu_row{1};
  // Number of rows for the specifik menu
  unsigned int menu_row_count{3};
  unsigned int menu_player_pos{};
  int vector_size{TILES_PER_ROW*TILES_PER_COLUMN};
  enum GameState{Playing, Dead, VictoryScreen, Victory, Pause, LevelSel, Menu, Exit};
  GameState gamestate_{Playing};
  GameState oldgamestate_{Playing};
  Graphics::CurrentMenu currentmenu_= Graphics::MainMenu;
  Logic::ActionResult actionResult_= Logic::Continue;
  Level* currLevelPtr_{};
  Logic logic_;
  Graphics graphics_;
  int xPix_{768};
  int yPix_{576};
  bool jumping_{false};
  sf::Clock clock;
  int token_pos_menu_{190};
  int token_pos_victory_{180};
  int token_pos_level_{200};
};

#endif
