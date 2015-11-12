//Game.cc

#include <SFML/Graphics.hpp>
#include "Logic.cc"
#include "Graphics.cc"
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

  Logic::Action action;
  Logic::Move move;

  //INITIERING
  // Skapa fönster som är 768x576 pixlar (är delbart på 32), går ej att resizea
  sf::RenderWindow window(sf::VideoMode(xPix_, yPix_), "Come on, catch up!", sf::Style::Titlebar | sf::Style::Close);
  //Skicka pixelvärden till logic så den vet hur stort fönstret är
  logic_.setPix(xPix_, yPix_);
  
  window.setVerticalSyncEnabled(true);

  // Lillemor: Gör om till funktion som läser in alla filer och spara i vektor

  // För över Level1 till vektorn.
  ifstream is("Level1.txt");
  istream_iterator<int> start(is), end;
  // Level 1
  vector<int> level1(start, end);

  currLevelPtr_ = new Level(TILESIZE, TILES_PER_ROW, level1);
  
  //LOOP
  // run the program as long as the window is open
  while (window.isOpen())
    {
      sf::Event event;
      sf::Clock clock;
      sf::Time dt_{clock.getElapsedTime()};

      // Continue to next level
      if (actionResult_ == Logic::LevelCompleted)
	{
	  cout << "level 2" << endl;
	  current_level = current_level + 1;
	  if ( current_level == 2)
	  {
	    ifstream is("Level2.txt");
	    istream_iterator<int> start(is), end;
	    // Level 1
	    vector<int> level2(start, end);
	    currLevelPtr_ = new Level(TILESIZE, TILES_PER_ROW, level2);
	  }
	  else if( current_level > 2)
	  {
	    ifstream is("Level1.txt");
	    istream_iterator<int> start(is), end;
	    // Level 1
	    vector<int> level1(start, end);
	    currLevelPtr_ = new Level(TILESIZE, TILES_PER_ROW, level1);
	    current_level = 1;
	  }
	}

      // check all the window's events that were triggered since the last iteration of the loop
      // TA IN INPUT
      while (window.pollEvent(event))
	{
	  clock.restart();
	  
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

	  // Rasmus: Potentiell TODO: Om man hoppar upp på en kant så fortsätter
	  // man hoppa direkt om man håller inne hoppknappen.
	  if (jumping_ == false && 
	      (event.type == sf::Event::KeyPressed && 
	       ((event.key.code == sf::Keyboard::Up) || 
		(event.key.code == sf::Keyboard::W) || 
		(event.key.code == sf::Keyboard::Space))))
	    {
	      //Hoppa
		  action = Logic::Jump;
		  jumping_ = true;
	    }
	  else if (event.type == sf::Event::KeyReleased && 
		   ((event.key.code == sf::Keyboard::Up) || 
		    (event.key.code == sf::Keyboard::W) || 
		    (event.key.code == sf::Keyboard::Space)))
	    {
	      action = Logic::JumpReleased;
	      jumping_ = false;
	    }
	  else
	    {
	      action = Logic::NoJump;
	    }
	}
      
      // clear the window with black color
      window.clear(sf::Color(200, 255, 255, 255));
      
      // UPPDATERA LOGIC
      actionResult_ = logic_.update((*currLevelPtr_), action, move, clock);

      // RITA
      graphics_.drawLevel((*currLevelPtr_), window);
      
      // end the current frame
      window.display();
    }
  
  return 0;
}

private:
  const int TILESIZE{32};
  const int TILES_PER_ROW{24};
  unsigned int current_level{1};
  enum GameState{Playing, ShowScreen, Pause, Exit}; 
  GameState gamestate_{Playing};
  Logic::ActionResult actionResult_= Logic::Continue;
  Level* currLevelPtr_{};
  Logic logic_;
  Graphics graphics_;
  int xPix_{768};
  int yPix_{576};
  bool jumping_{false};
};
