#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Level.cc"
#include <vector>


class Game
{
public:
  int run();

private:
  const int TILESIZE{32};
  const int TILES_PER_ROW{24};
  const int TILES_PER_COLUMN{18};
  unsigned int current_level{0};
  enum GameState{Playing, ShowScreen, Exit};
  enum ActionResult{Nothing, GameCompleted, LevelCompleted, Dead, Reset, Quit};
  GameState gamestate_{Playing};
  ActionResult actionResult_{Nothing};
  vector<Level> level_vector_{};
  /*
  GameLogic gameLogic_;
  Graphics graphics_;
  */

};


#endif
