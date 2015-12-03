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
  Logic::Action action{Logic::JumpReleased};
  Logic::Move move{Logic::Idle};    

  // Init
  // Skapa fönster som är 768x576 pixlar (är delbart på 32), går ej att resizea
  sf::RenderWindow window(sf::VideoMode(xPix_, yPix_), "Come on, catch up!", 
			  sf::Style::Titlebar | sf::Style::Close);
 
  window.setVerticalSyncEnabled(true);
  GameSounds gamesounds;
  gamesounds.startBackgroundMusic();

  // För över Level.txt till vektorn curLevel.
  ifstream is("Level.txt");
  if(!is)
  {
    cerr << "Couldn't open Level.txt" << endl;
    return 1;
  }
  istream_iterator<int> start(is), end;
  vector<int> lvl(start, end);
  // Lillemor: använd load_level
  vector<int> curLevel(lvl.begin() + vector_size*(current_level - 1), 
		       lvl.begin() + vector_size*current_level);
  currLevelPtr_ = new Level(TILESIZE, TILES_PER_ROW, curLevel);

  //LOOP
  // run the program as long as the window is open
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
    // Slutskärm ska fixas här
    else if (actionResult_ == Logic::LevelCompleted &&
	     (current_level) >= (lvl.size()/vector_size) &&
	     gamestate_ == Playing )
    { 
      menu_row_count = 3;
      current_menu_row = 1;
      currentmenu_ = VictoryMenu;
      menu_player_pos = 300;
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
	currentmenu_ = MainMenu;
	current_menu_row = 1;
	menu_player_pos = 300;
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
    else if (gamestate_ == Menu || gamestate_ == VictoryScreen || gamestate_ == LevelSel) 
    {	
      menu(lvl);
      if (gamestate_ == Exit)
	window.close();
    }


    // check all the window's events that were triggered since the last iteration
    // of the loop
    // INPUT
    while (window.pollEvent(event))
    { 
      if (event.type == sf::Event::Closed || 
	  sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
      {
	// "close requested" event: we close the window
	window.close();
      }
      /*
      // Rasmus: Eventuellt för senare bruk, Lillemor: gärna inte spela musik/hantera knapptryckningar
      // när spelet inte är i fokus
      else if (event.type == sf::Event::LostFocus)
      {
      GameState = Pause;
      }
      else if (event.type == sf::Event::GainedFocus)
      {
      GameState = Playing;
      }
      */
      if (gamestate_ == Playing)
      {
	if (event.type == sf::Event::KeyPressed && 
	    ((event.key.code == sf::Keyboard::Up) || 
	     (event.key.code == sf::Keyboard::W) || 
	     (event.key.code == sf::Keyboard::Space)) && jumping_ == false)
	{ 
	  //Hoppa
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
    }


    // clear the window with black color
    window.clear(sf::Color(200, 255, 255, 255));
    if (gamestate_ == Playing)
    {
      // update logic
      actionResult_ = logic_.update((*currLevelPtr_), action, move, gamesounds);
      // draw
      graphics_.drawLevel((*currLevelPtr_), window);
    }
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
    // Drawing menus
    // Mycket av detta antar jag ska göras på annat ställe men för snabb fix
    // så gör jag detta här
    else if (gamestate_ == Menu || gamestate_ == VictoryScreen
	     || gamestate_ == LevelSel)
    {
      graphics_.drawMenu(menu_player_pos, window);
    }
    // end the current frame
    window.display();	  
  }
  return 0;
}

//---------MENU--------------//
void Game::menu(const vector<int> & lvl)
{  
  // "OBS! du är i menu tryck upp en gång och sen enter för att spela,
  // ner och enter för att avsluta"
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && current_menu_row > 1)
  {
    if (currentmenu_ == MainMenu)
      menu_player_pos = menu_player_pos - 75; 
    else if (currentmenu_ == VictoryMenu)
      menu_player_pos = menu_player_pos - 75;
    else if (currentmenu_ == LevelMenu) 
      menu_player_pos = menu_player_pos - 50;
    delayTime();
    current_menu_row = current_menu_row - 1;
  }
		  
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && current_menu_row < menu_row_count)
  { 	
    if (currentmenu_ == MainMenu)
      menu_player_pos = menu_player_pos + 75; 
    else if (currentmenu_ == VictoryMenu)
      menu_player_pos = menu_player_pos + 75;
    else if (currentmenu_ == LevelMenu) 
      menu_player_pos = menu_player_pos + 50;
    delayTime();
    current_menu_row = current_menu_row + 1;
  }

  // Main menu
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && currentmenu_ == MainMenu)
  {

    switch (current_menu_row)
    {
    case 1:
      if (gamestate_ == VictoryScreen)
	{
	  current_level = 1;
	  load_level(lvl, current_level);
	}
      gamestate_ = Playing;	     
    case 2:
      menu_player_pos = 200;
      current_menu_row = 1; 
      currentmenu_ = LevelMenu;
      menu_row_count = 5;
      delayTime();
      break;
    case 3:
      gamestate_ = Exit;
      break;
    }       
  }

  // Victory_screen
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && currentmenu_ == VictoryMenu)
  {
    switch (current_menu_row)
    {
    case 1:
      current_level = 1;
      load_level(lvl, current_level);
    case 2:
      menu_player_pos = 200;
      current_menu_row = 1; 
      currentmenu_ = MainMenu;
      menu_row_count = 3;
      delayTime();
    case 4:
      gamestate_ = Exit;
      break;
    }
  } 
  // Level menu
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && currentmenu_ == LevelMenu)
  {	

    switch (current_menu_row)
    {
    case 1:
      menu_player_pos = 300;
      current_menu_row = 1; 
      currentmenu_ = MainMenu;
      menu_row_count = 3;
      delayTime();
    case 2:
      current_level = 1;
      load_level(lvl, current_level);
      break;
    case 3:	
      current_level = 2;
      load_level(lvl, current_level);
      break;
    case 4:
      current_level = 3;
      load_level(lvl, current_level);
      break;
    case 5:
      current_level = 4;
      load_level(lvl, current_level);
      break;
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
//Load current_level
void Game::load_level(const vector<int> & lvl, int current_level)
{
  delete currLevelPtr_;
  vector<int> curLevel(lvl.begin() + vector_size*(current_level - 1), 
		       lvl.begin() + vector_size*current_level);
  currLevelPtr_ = new Level(TILESIZE, TILES_PER_ROW, curLevel);
  gamestate_ = Playing;
}
