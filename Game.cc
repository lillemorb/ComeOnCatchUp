#include <SFML/Graphics.hpp>
//#include "Level.cc"
#include "Logic.cc"
#include "Graphics.cc"
#include "Logic.cc"
#include <vector>
using namespace std;

// TODO: dela upp i .cc/.h-filer
// undersöka var liten minnesläcka finns (kolla mha valgrind)

class Game
{
public:
  ~Game() { delete currLevelPtr_; }

int run()
{

  Action action;
    
  //INITIERING
  // Skapa fönster som är 768x576 pixlar (är delbart på 32), går ej att resizea
  sf::RenderWindow window(sf::VideoMode(768, 576), "Come on, catch up!", sf::Style::Titlebar | sf::Style::Close);
  
  // Level 1
  vector<int> level1{
    00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
      00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
      00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
      00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
      00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
      00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
      00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
      00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
      00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
      00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
      00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
      00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
      00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
      00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
      00, 00, 00, 00, 00, 00, 00, 00, 00, 02, 00, 00, 04, 00, 00, 00, 00, 03, 00, 00, 00, 00, 00, 00,
      01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01,
      01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01,
      01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01
      };
  
  // Pga problem med att Level deletar sina pekare då ett tillfälligt objekt
  // tas bort, håller vi reda på aktuell Level med en pekare istället
  // Medans vi fortfarande bara har en Level
  currLevelPtr_ = new Level(TILESIZE, TILES_PER_ROW, TILES_PER_COLUMN, level1);
  // level_vector_.push_back(Level(TILESIZE, TILES_PER_ROW, TILES_PER_COLUMN, level1));
  
  //LOOP
  // run the program as long as the window is open
  while (window.isOpen())
    {
      // check all the window's events that were triggered since the last iteration of the loop
      sf::Event event;

      // TA IN INPUT
      while (window.pollEvent(event))
	{
	  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || 
	      sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	    {
	      //Flytta vänster
	      action = Left;
	    }
	  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || 
		   sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	    {
	      //Flytta höger
	      action = Right;
	    }
	  /*	  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || 
		  sf::Keyboard::isKeyPressed(sf::Keyboard::W) || 
		  sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	    {
	      //Hoppa
	      action = Jump;
	      }*/
	  else if (event.type == sf::Event::Closed)
	    {
	      // "close requested" event: we close the window
	      window.close();
	    }
	  else
	    {
	      action = Nothing;
	    }
	}
      
      // clear the window with black color
      window.clear(sf::Color(200, 255, 255, 255));
      
      // UPPDATERA LOGIC
      logic_.update(action);
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
  const int TILES_PER_COLUMN{18};
  unsigned int current_level{0};
  enum GameState{Playing, ShowScreen, Exit};
  enum ActionResult{Nothing, GameCompleted, LevelCompleted, Dead, Reset, Quit};
  GameState gamestate_{Playing};
  ActionResult actionResult_{Nothing};
//  vector<Level> level_vector_{};
  Level* currLevelPtr_{};
  Logic logic_;
  Graphics graphics_;
 

};
