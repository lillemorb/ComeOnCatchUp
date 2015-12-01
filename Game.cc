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


	// ska nog fixas på annat ställe
	// Menu sprites and textures
	sf::Sprite menu_sprite;
	sf::Texture spritesheet_menu;
	spritesheet_menu.loadFromFile("Sprites/Tomato.png");


	//INITIERING
	// Skapa fönster som är 768x576 pixlar (är delbart på 32), går ej att resizea
	sf::RenderWindow window(sf::VideoMode(xPix_, yPix_), "Come on, catch up!", 
													sf::Style::Titlebar | sf::Style::Close);
	//Skicka pixelvärden till logic så den vet hur stort fönstret är
	//Anna:flyttat till konstruktorn
	//logic_.setPix(xPix_, yPix_);
 
	window.setVerticalSyncEnabled(true);
	graphics_.initGraphics();
	GameSounds gamesounds;
	gamesounds.loadSounds();

	// För över Level.txt till vektorn curLevel.
	ifstream is("Level.txt");
	if(!is)
	{
		cerr << "Couldn't open Level.txt" << endl;
		return 1;
	}
	istream_iterator<int> start(is), end;
	vector<int> lvl(start, end);
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
		// Slutskärm ska fixas här
		else if (actionResult_ == Logic::LevelCompleted &&
						 (current_level) >= (lvl.size()/vector_size) &&
						 gamestate_ == Playing )
	  { 
	    menu_row = 3;
	    current_menu_row = 1;
	    current_menu = 2; 
	    menu_player_pos = 500;
	    gamestate_ = VictoryScreen;    
	  }
		//Dead
		else if(actionResult_ == Logic::Dead)
	  {
	    gamestate_ = Dead;
	    clock.restart();
	    sf::Time dt{clock.getElapsedTime()};
	    if (deathCounter < 6)
			{
				deathCounter++;
				while(dt.asSeconds() < 0.2)
				{
					dt = clock.getElapsedTime();
				}
				if (deathCounter == 5) 
				{
					load_level(lvl, current_level);
					deathCounter++;
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
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
			{
				menu_row = 3;
				current_menu = 1;
				current_menu_row = 1;
				menu_player_pos = 500;
				gamestate_ = Menu;
			}
	  
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || 
					sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				//Flytta vänster
				move = Logic::Left;
			}
	    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || 
							 sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				//Flytta höger
				move = Logic::Right;
			}
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
		// TA IN INPUT

		while (window.pollEvent(event))
	  {
	  
	    if (event.type == sf::Event::Closed || 
					sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				// "close requested" event: we close the window
				window.close();
			}
	    /*
	    // Rasmus: Eventuellt för senare bruk
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
	    // UPPDATERA LOGIC
	    actionResult_ = logic_.update((*currLevelPtr_), action, move, gamesounds);
	    // RITA
	    graphics_.drawLevel((*currLevelPtr_), window);
	  }
		else if (gamestate_ == Dead)
	  {
	    graphics_.drawLevel((*currLevelPtr_), window);
	    if (deathCounter > 5)
			{
				deathCounter = 0;
				gamestate_ = Playing;
			}
	  }
		// Drawing menus
		// Mycket av detta antar jag ska göras på annat ställe men för snabb fix
		// så gör jag detta här
		else if (gamestate_ == Menu || gamestate_ == VictoryScreen
						 || gamestate_ == LevelSel)
	  {
	    menu_sprite.setTexture(spritesheet_menu);
	    menu_sprite.setTextureRect(sf::IntRect(96, 0, 32, 32));
	    menu_sprite.setPosition(sf::Vector2f(200, menu_player_pos));
	    window.draw(menu_sprite);
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && current_menu_row < menu_row)
	{
		if (current_menu == 3) 
	  {
	    menu_player_pos = menu_player_pos - 50;
	  } 
		else if (current_menu == 1) 
	  {	
	    menu_player_pos = menu_player_pos - 100; 
	  }
		else if (current_menu == 2)
	  {
	    menu_player_pos = menu_player_pos - 100;
	  }
		cout << current_menu << endl;
		delayTime();
		cout << "upp i menyn" << endl;
		current_menu_row = current_menu_row + 1;
	}
		  
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && current_menu_row > 1)
	{ 	
		if (current_menu == 3) {menu_player_pos = menu_player_pos + 50;} 
		else {	menu_player_pos = menu_player_pos + 100; }
	
		delayTime();
	
		cout << "ner i menyn" << endl;
		current_menu_row = current_menu_row - 1;
	}
	// Rasmus: Gör en koll ifall man tryckt enter, se därefter vilken av menyerna det var
	// det optimerar koden mer och utför samma sak som nu.
	// Daniel: Problemet är att case 2 i main menu ändrar current_menu till 3
	// och det fuckar, 
	// men detta kanske funkar?

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && current_menu == 1)
	{
		delayTime();
		// Main menu
	
		cout << "Du är nu i huvudmenyn" << endl;
		switch (current_menu_row)
	  {
	  case 1:
	    cout << "Exit" << endl;
	    gamestate_ = Exit;
	    break;		     
	  case 2:
	    cout << "Går till level menyn" << endl;
	    menu_player_pos = 500;
	    current_menu_row = 1; 
	    current_menu = 3;
	    menu_row = 5;
	    break;
	  case 3:
	    cout << "Start på level 1" << endl;
	    current_level = 1;
	
	    load_level(lvl, current_level);
	    break;
	      
	  }	   
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && current_menu == 2)
		// Victory_screen
	{

		cout << "Du är nu i victory menyn" << endl;
		switch (current_menu_row)
	  {
	  case 1:
	    cout << "Exit" << endl;
	    gamestate_ = Exit;
	    break;
	  case 2:
	    cout << "Går till main menyn" << endl;
	    menu_player_pos = 500;
	    current_menu_row = 1; 
	    current_menu = 1;
	    menu_row = 3;
	  case 3:
	    cout << "Börja om från level 1" << endl;
	    current_level = 1;
	    load_level(lvl, current_level);			
	      
	  }
	} 
	// Level menu
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && current_menu == 3)
	{	
		delayTime();

		cout << "Du är nu i level menyn" << endl;
		switch (current_menu_row)
	  {
	  case 1:
	    cout << "Går till main menyn" << endl;
	    menu_player_pos = 500;
	    current_menu_row = 1; 
	    current_menu = 1;
	    menu_row = 3;
	  case 2:
	    cout << "Du försökte gå till level 1" << endl;
	    current_level = 1;
	    load_level(lvl, current_level);
	    break;
	  case 3:	
	    cout << "Du försökte gå till level 2" << endl;
	    current_level = 2;
	    load_level(lvl, current_level);
	    break;
	  case 4:
	    cout << "Du försökte gå till level 3" << endl;
	    current_level = 3;
	    load_level(lvl, current_level);
	    break;
	  case 5:
	    cout << "Du försökte gå till level 4" << endl;
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
	while(dt.asSeconds() < 0.5)
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
