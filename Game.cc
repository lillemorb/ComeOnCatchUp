//Game.cc

#include <SFML/Graphics.hpp>
#include "Logic.cc"
#include "Graphics.cc"
#include "GameSounds.cc"
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

  int run()
  {

    Logic::Action action{Logic::JumpReleased};
    Logic::Move move{Logic::Idle};

    //INITIERING
    // Skapa fönster som är 768x576 pixlar (är delbart på 32), går ej att resizea
    sf::RenderWindow window(sf::VideoMode(xPix_, yPix_), "Come on, catch up!", 
			    sf::Style::Titlebar | sf::Style::Close);
    //Skicka pixelvärden till logic så den vet hur stort fönstret är
    logic_.setPix(xPix_, yPix_);
 
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
	if (actionResult_ == Logic::LevelCompleted && (current_level) < (lvl.size()/vector_size))
	  {
	    
	    delete currLevelPtr_;
	    current_level = current_level + 1;
	    vector<int> curLevel(lvl.begin() + vector_size*(current_level - 1),
				 lvl.begin() + vector_size*current_level);
	    currLevelPtr_ = new Level(TILESIZE, TILES_PER_ROW, curLevel); 
	 
	  }
	// Slutskärm ska fixas här
	else if (actionResult_ == Logic::LevelCompleted && (current_level) >= (lvl.size()/vector_size))
	  { 
	    current_menu = 2;
	    gamestate_ = VictoryScreen;
	  }
     
	//Dead
	if(actionResult_ == Logic::Dead)
	  {
	    delete currLevelPtr_;	
	    vector<int> curLevel(lvl.begin() + vector_size*(current_level - 1),
				 lvl.begin() + vector_size*current_level);
	    currLevelPtr_ = new Level(TILESIZE, TILES_PER_ROW, curLevel); 
	  }




	if (gamestate_ == Playing)
	  {	      
	    //Reset
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	      {
		delete currLevelPtr_;
		vector<int> curLevel(lvl.begin() + vector_size*(current_level - 1), 
				     lvl.begin() + vector_size*current_level);
		currLevelPtr_ = new Level(TILESIZE, TILES_PER_ROW, curLevel); 
	      }
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
	      {
		current_menu = 1;
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
	    // "OBS! du är i menu tryck upp en gång och sen enter för att spela, ner och enter för att avsluta"
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && current_menu_row < 3)
	      {
		cout << "upp i menyn" << endl;
		current_menu_row = current_menu_row + 1;
	      }
		  
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && current_menu_row > 1)
	      { 
		cout << "ner i menyn" << endl;
		current_menu_row = current_menu_row - 1;
	      }
	    if (current_menu == 1 || current_menu == 2)
	      {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		  {
		    if (current_menu == 1)
		      {
			cout << "Du är nu i huvudmenyn" << endl;
			switch (current_menu_row)
			  {
			  case 1:
			    cout << "Exit" << endl;
			    window.close();
			    break;
			  case 2:
			    cout << "Start på level 1" << endl;
			    current_level = 1;
			    gamestate_ = Playing;
			    delete currLevelPtr_;
			    vector<int> curLevel(lvl.begin() + vector_size*(current_level - 1), 
						 lvl.begin() + vector_size*current_level);
			    currLevelPtr_ = new Level(TILESIZE, TILES_PER_ROW, curLevel);
			
			    break;
			    /*
			      case 3:
			      cout << "Går till level menyn" << endl;
			      current_menu = 3;
			      break;
			    */
			  }
		   
		      }
		    else if (current_menu == 2)
		      {
			cout << "Du är nu i victory menyn" << endl;
			switch (current_menu_row)
			  {
			  case 1:
			    cout << "Exit" << endl;
			    window.close();
			    break;
			  case 2:
			    cout << "Börja om från level 1" << endl;
			    current_level = 1;
			    gamestate_ = Playing;				
			    delete currLevelPtr_;
			    vector<int> curLevel(lvl.begin() + vector_size*(current_level - 1), 
						 lvl.begin() + vector_size*current_level);
			    currLevelPtr_ = new Level(TILESIZE, TILES_PER_ROW, curLevel);
				
			  }
		      }
		    /*
		    else if (current_menu == 3)
		      {
			sleep(1);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			  {    
			    cout << "Du är nu i level menyn" << endl;
			    switch (current_menu_row)
			      {
			      case 1:
				cout << "Du försökte gå till level 1" << endl;
				current_level = 1;
				gamestate_ = Playing;
				return Logic::LevelCompleted;
				break;
			      case 2:	
				cout << "Du försökte gå till level 2" << endl;
				current_level = 2;
				gamestate_ = Playing;
				return Logic::LevelCompleted;
				break;
			      case 3:
				cout << "Du försökte gå till level 3" << endl;
				current_level = 2;
				gamestate_ = Playing;
				return Logic::LevelCompleted;

			      }	
			  
			  }
		
		      }
		    */
		  }
	      }
	  }


	// check all the window's events that were triggered since the last iteration of the loop
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

	// end the current frame
	window.display();
	  
      }
    return 0;
  }

private:

  const int TILESIZE{32};
  const int TILES_PER_ROW{24};
  const int TILES_PER_COLUMN{18};
  unsigned int current_level{1};
  unsigned int current_menu{1};
  unsigned int current_menu_row{1};
  int vector_size{TILES_PER_ROW*TILES_PER_COLUMN};
  enum GameState{Playing, VictoryScreen, Pause, LevelSel, Menu}; 
  GameState gamestate_{Playing};
  Logic::ActionResult actionResult_= Logic::Continue;
  Level* currLevelPtr_{};
  Logic logic_;
  Graphics graphics_;
  int xPix_{768};
  int yPix_{576};
  bool jumping_{false};

};

