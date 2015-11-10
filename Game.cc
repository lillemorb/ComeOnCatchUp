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
  sf::RenderWindow window(sf::VideoMode(768, 576), "Come on, catch up!", sf::Style::Titlebar | sf::Style::Close);
  
  // För över Level1 till vektorn.
  ifstream is("Level1.txt");
  istream_iterator<int> start(is), end;
  // Level 1
  vector<int> level1(start, end);



  // Pga problem med att Level deletar sina pekare då ett tillfälligt objekt
  // tas bort, håller vi reda på aktuell Level med en pekare istället
  // Medans vi fortfarande bara har en Level
  currLevelPtr_ = new Level(TILESIZE, TILES_PER_ROW, level1);
  // level_vector_.push_back(Level(TILESIZE, TILES_PER_ROW, TILES_PER_COLUMN, level1));
  
  //LOOP
  // run the program as long as the window is open
  while (window.isOpen())
    {
      // check all the window's events that were triggered since the last iteration of the loop
      sf::Event event;
      sf::Clock clock;
      // TA IN INPUT

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
	}


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

	  // Kan hoppa genom att hålla inne knappen, kan vara värt att göra så det är delay mellan varje knapptryck
	  // för att ge bättre respons för användaren.
	  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || 
	      sf::Keyboard::isKeyPressed(sf::Keyboard::W) || 
	      sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	    {
	      //Hoppa
	      action = Logic::Jump;
	    }
	  else if (event.type == sf::Event::KeyReleased && ((event.key.code == sf::Keyboard::Up) || 
							    (event.key.code == sf::Keyboard::W) || 
							    (event.key.code == sf::Keyboard::Space)))
	    {
	      action = Logic::JumpReleased;
	    }
	  else
	    {
	      action = Logic::Nothing;
	    }
	}
      
      // clear the window with black color
      window.clear(sf::Color(200, 255, 255, 255));
      
      // UPPDATERA LOGIC
      logic_.update((*currLevelPtr_), action, move, clock);
      // RITA
      graphics_.drawLevel((*currLevelPtr_), window);
      //level_vector_.at(0).draw(window);
      
      // end the current frame
      window.display();
    }
  
  return 0;
}

private:
  const int TILESIZE{32};
  const int TILES_PER_ROW{24};
//  const int TILES_PER_COLUMN{18};
  unsigned int current_level{1};
  enum GameState{Playing, ShowScreen, Pause, Exit}; 
  GameState gamestate_{Playing};
  Logic::ActionResult actionResult_= Logic::Continue;
//  vector<Level> level_vector_{};
  Level* currLevelPtr_{};
  Logic logic_;
  Graphics graphics_;
 

};
