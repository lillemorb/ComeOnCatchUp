#ifndef GAMESOUNDS_CC
#define GAMESOUNDS_CC

#include <SFML/Audio.hpp>


class GameSounds
{
public:
  
  void loadSounds()
  {
    if (!jump_soundBuffer_.loadFromFile("Sounds/Player_jump.wav"))
      {
	cerr << "Kunde inte ladda Player_jump.wav" << endl;
	// Fixa felhantering
	// return 1;
      }
    
    if (!death_soundBuffer_.loadFromFile("Sounds/Player_death.wav"))
      {
	cerr << "Kunde inte ladda Player_death.wav" << endl;
	// Fixa felhantering
	// return 1;
      }

    if (!box_soundBuffer_.loadFromFile("Sounds/Box_push.wav"))
      {
	cerr << "Kunde inte ladda Box_push.wav" << endl;
	// Fixa felhantering
	// return 1;
      }

    death_sound_.setBuffer(death_soundBuffer_);
    jump_sound_.setBuffer(jump_soundBuffer_);
    box_sound_.setBuffer(box_soundBuffer_);
  }
 
  void getDeathSound() { death_sound_.play(); }
  void getJumpSound() { jump_sound_.play(); }
  void getBoxSound() 
  { 
    if(delay_ == 10)
      {
	box_sound_.play();
	delay_ = 0;
      }
    else 
      delay_++;
  }
  
private:
  sf::SoundBuffer death_soundBuffer_;
  sf::SoundBuffer jump_soundBuffer_;
  sf::SoundBuffer box_soundBuffer_;

  int delay_ {0};
  sf::Sound death_sound_;
  sf::Sound jump_sound_;
  sf::Sound box_sound_;
};

#endif
