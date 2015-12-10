//Game.cc
#include "Game.h"
#include <SFML/Graphics.hpp>
#include "Logic.h"
#include "Graphics.h"
#include "GameSounds.h"
#include <vector>
#include <fstream>
#include <iterator>
#include <iostream>
using namespace std;

//---------RUN--------------// 
int Game::run()
{
  // Init

  Logic::Action action{Logic::JumpReleased};
  Logic::Move move{Logic::Idle};    

  //Create non resizeable window 768x576 pixels large
  sf::RenderWindow window(sf::VideoMode(xPix_, yPix_), "Come on, catch up!", 
			  sf::Style::Titlebar | sf::Style::Close);
 
  window.setVerticalSyncEnabled(true);

  GameSounds gamesounds;
  gamesounds.startBackgroundMusic();

  // Transfer contents in Level.txt to vector curLevel
  ifstream is("Level.txt");
  if(!is)
  {
    cerr << "Couldn't open Level.txt" << endl;
    return 1;
  }
  istream_iterator<int> start(is), end;
  vector<int> lvl(start, end);

  //Initiate main menu (which is the same as victory menu)
  gamestate_ = Victory;
  menu_row_count = 3;
  current_menu_row = 1;
  currentmenu_ = Graphics::VictoryMenu;
  menu_player_pos = token_pos_victory_;

  //LOOP
  //Run the program as long as the window is open
  while (window.isOpen())
  {
    sf::Event event;

    // Continue to next level
    if (actionResult_ == Logic::LevelCompleted &&
	(current_level) < (lvl.size()/vector_size))
    {
      current_level = current_level + 1;
      load_level(lvl, current_level);
    }
    //Victory
    else if (actionResult_ == Logic::LevelCompleted &&
	     (current_level) >= (lvl.size()/vector_size) &&
	     gamestate_ == Playing )
    { 
      gamestate_ = VictoryScreen;
    }
    //Dead
    else if(actionResult_ == Logic::Dead)
    {
      gamestate_ = Dead;
      clock.restart();
      sf::Time dt{clock.getElapsedTime()};
      if (graphics_.getDeathCounter() < 6)
      {
	while(dt.asSeconds() < 0.2)
	{
	  dt = clock.getElapsedTime();
	}
      }
    }

    // INPUT

    //Non-event based input
    if (gamestate_ == Playing)
    {	      
      //Reset
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
      {
	load_level(lvl, current_level);
      }
      //Menu
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
      {
	menu_row_count = 3;
	currentmenu_ = Graphics::MainMenu;
	current_menu_row = 1;
	menu_player_pos = token_pos_menu_;
	oldgamestate_ = gamestate_;
	gamestate_ = Menu;
      }
      //Move left	  
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || 
	  sf::Keyboard::isKeyPressed(sf::Keyboard::A))
      {
	move = Logic::Left;
      }
      //Move right
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || 
	       sf::Keyboard::isKeyPressed(sf::Keyboard::D))
      {
	move = Logic::Right;
      }
      //Idle
      else
      {
	move = Logic::Idle;
      }
    }
    else if (gamestate_ == Menu || gamestate_ == Victory || gamestate_ == LevelSel) 
    {	
      menu(lvl);
      if (gamestate_ == Exit)
	window.close();
    }


    // Check all the window's events that were triggered since the last iteration
    // of the loop
    while (window.pollEvent(event))
    { 
      // "Close requested" event: we close the window
      if (event.type == sf::Event::Closed || 
	  sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
      {
	window.close();
      }
      // Pause when moving focus from window 
      else if (event.type == sf::Event::LostFocus)
      {
	oldgamestate_ = gamestate_;
	gamestate_ = Pause;
      }
      // Continue (from pause) when moving focus to window
      else if (event.type == sf::Event::GainedFocus)
      {
	gamestate_ = oldgamestate_;
      }
      // Pause/continue by pressing 'P'-key
      else if (event.type == sf::Event::KeyReleased && 
	       (event.key.code == sf::Keyboard::P)) 
      {
	if(gamestate_ != Pause)
	{
	  oldgamestate_ = gamestate_;
	  gamestate_ = Pause;
	}
	else
	{
	  gamestate_ = oldgamestate_;
	}
      }

      if (gamestate_ == Playing)
      {
	//Jump
	if (event.type == sf::Event::KeyPressed && 
	    ((event.key.code == sf::Keyboard::Up) || 
	     (event.key.code == sf::Keyboard::W) || 
	     (event.key.code == sf::Keyboard::Space)) && jumping_ == false)
	{ 

	  jumping_ = true;
	  action = Logic::Jump;
	}
	else if (event.type == sf::Event::KeyReleased && 
		 ((event.key.code == sf::Keyboard::Up) || 
		  (event.key.code == sf::Keyboard::W) || 
		  (event.key.code == sf::Keyboard::Space)))
	{
	  action = Logic::JumpReleased;
	  jumping_ = false;
	}
      }
      else if (gamestate_ == VictoryScreen)
      {
	if (event.type == sf::Event::KeyReleased && 
	    (event.key.code == sf::Keyboard::Return))
	{
	  gamestate_ = Victory;
	  menu_row_count = 3;
	  current_menu_row = 1;
	  currentmenu_ = Graphics::VictoryMenu;
	  menu_player_pos = token_pos_victory_;
	}
      }
    }

    window.clear(sf::Color(200, 255, 255, 255));

    if (gamestate_ == Playing)
    {
      //If music not playing (paused), play
      if(!gamesounds.isBackgroundMusicPlaying())
      {
	while(gamesounds.isPauseSoundPlaying())
	{ }
	gamesounds.getPauseSound();
	while(gamesounds.isPauseSoundPlaying())
	{ }
	gamesounds.resumeBackgroundMusic();
      }

      // Update logic
      actionResult_ = logic_.update((*currLevelPtr_), action, move, gamesounds, clock);
      clock.restart();
      // Draw level
      graphics_.drawLevel((*currLevelPtr_), window);
    }
    else if (gamestate_ == Pause)
    {
      //Pause music and play pause sound
      if(gamesounds.isBackgroundMusicPlaying())
      {
	gamesounds.pauseBackgroundMusic();
	gamesounds.getPauseSound();
      }
      // Draw level/menu
      if(oldgamestate_ == Playing)
	graphics_.drawLevel((*currLevelPtr_), window);
      else if(oldgamestate_ == Menu || oldgamestate_ == Victory ||
	      oldgamestate_ == LevelSel)
	graphics_.drawMenu(menu_player_pos, window, currentmenu_);
      else if(oldgamestate_ == VictoryScreen)
	graphics_.drawVictoryScreen(window);

      // Draw 'Paused' over current graphics in window
      graphics_.drawPaused(window);
    }
    //Do not continue until death animation is done 
    else if (gamestate_ == Dead)
    {
      graphics_.drawLevel((*currLevelPtr_), window);
      if (graphics_.getDeathCounter() > 6)
      {
	graphics_.setDeathCounter(0);
	load_level(lvl, current_level);
	actionResult_ = Logic::Continue;
      }
    }
    // Draw VictoryScreen
    else if (gamestate_ == VictoryScreen)
    {
      if(!gamesounds.isBackgroundMusicPlaying())
      {
	while(gamesounds.isPauseSoundPlaying())
	{ }
	gamesounds.getPauseSound();
	while(gamesounds.isPauseSoundPlaying())
	{ }
	gamesounds.resumeBackgroundMusic();
      }
      graphics_.drawVictoryScreen(window);
    }
    // Draw menus
    else if (gamestate_ == Menu || gamestate_ == Victory
	     || gamestate_ == LevelSel)
    {
      //if music not playing (paused), play 
      if(!gamesounds.isBackgroundMusicPlaying())
      {
	while(gamesounds.isPauseSoundPlaying())
	{ }
	gamesounds.getPauseSound();
	while(gamesounds.isPauseSoundPlaying())
	{ }
	gamesounds.resumeBackgroundMusic();
      }
      graphics_.drawMenu(menu_player_pos, window, currentmenu_);
    }

    // End the current frame
    window.display();

    // If frame has completed too quickly, wait
    while(clock.getElapsedTime().asMicroseconds() < 16667)
    { }
  }
  return 0;
}

//---------MENU--------------//
void Game::menu(const vector<int> & lvl)
{  
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && current_menu_row > 1)
  {
    if (currentmenu_ == Graphics::MainMenu)
      menu_player_pos = menu_player_pos - 46; 
    else if (currentmenu_ == Graphics::VictoryMenu)
      menu_player_pos = menu_player_pos - 46;
    else if (currentmenu_ == Graphics::LevelMenu) 
      menu_player_pos = menu_player_pos - 37;
    delayTime();
    current_menu_row = current_menu_row - 1;
  }
		  
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
      current_menu_row < menu_row_count)
  { 	
    if (currentmenu_ == Graphics::MainMenu)
      menu_player_pos = menu_player_pos + 46; 
    else if (currentmenu_ == Graphics::VictoryMenu)
      menu_player_pos = menu_player_pos + 46;
    else if (currentmenu_ == Graphics::LevelMenu) 
      menu_player_pos = menu_player_pos + 37;
    delayTime();
    current_menu_row = current_menu_row + 1;
  }

  // Main menu
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) &&
      currentmenu_ == Graphics::MainMenu)
  {
    switch (current_menu_row)
    {
    case 1:
      if (gamestate_ == Victory)
      {
	current_level = 1;
	load_level(lvl, current_level);
      }
      gamestate_ = Playing;
      break;	     
    case 2:
      menu_player_pos = token_pos_level_;
      current_menu_row = 1; 
      currentmenu_ = Graphics::LevelMenu;
      menu_row_count = 7;
      gamestate_ = LevelSel;
      delayTime();
      break;
    case 3:
      gamestate_ = Exit;
      break;
    }       
  }

  // Start/Victory_menu
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) &&
      currentmenu_ == Graphics::VictoryMenu)
  {
    switch (current_menu_row)
    {
    case 1:
      current_level = 1;
      load_level(lvl, current_level);
      break;
    case 2:
      menu_player_pos = token_pos_level_;
      current_menu_row = 1; 
      currentmenu_ = Graphics::LevelMenu;
      menu_row_count = 7;
      oldgamestate_ = gamestate_;
      gamestate_ = LevelSel;
      delayTime();
      break;
    case 3:
      gamestate_ = Exit;
      break;
    }
  } 
  // Level_menu
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) &&
      currentmenu_ == Graphics::LevelMenu)
  {	

    switch (current_menu_row)
    {
    case 1:
      current_level = 1;
      load_level(lvl, current_level);
      break;
    case 2:	
      current_level = 2;
      load_level(lvl, current_level);
      break;
    case 3:
      current_level = 3;
      load_level(lvl, current_level);
      break;
    case 4:
      current_level = 4;
      load_level(lvl, current_level);
      break;
    case 5:
      current_level = 5;
      load_level(lvl, current_level);
      break;
    case 6:
      current_level = 6;
      load_level(lvl, current_level);
      break;
    case 7:
      menu_player_pos = token_pos_menu_;
      current_menu_row = 1;
      if(oldgamestate_== Victory || oldgamestate_ == LevelSel)
      {
	currentmenu_ = Graphics::VictoryMenu;
	gamestate_ = Victory;
      }
      else
      {
	currentmenu_ = Graphics::MainMenu;
	gamestate_ = Menu;
      }
      menu_row_count = 3;
      delayTime();
    }	   
  }
}

//---------DELAYTIME--------------//
void Game::delayTime()
{
  clock.restart();
  sf::Time dt{clock.getElapsedTime()};
  while(dt.asSeconds() < 0.2)
  {
    dt = clock.getElapsedTime();
  }
}
  
//---------LOAD_LEVEL--------------//
void Game::load_level(const vector<int> & lvl, int current_level)
{
  delete currLevelPtr_;
  vector<int> curLevel(lvl.begin() + vector_size*(current_level - 1), 
		       lvl.begin() + vector_size*current_level);
  currLevelPtr_ = new Level(TILESIZE, TILES_PER_ROW, curLevel);
  gamestate_ = Playing;
}
