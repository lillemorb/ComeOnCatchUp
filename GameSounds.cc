//GameSounds.cc
#include "GameSounds.h"
#include <SFML/Audio.hpp>
#include <iostream>
using namespace std;

//---------LOADSOUNDS--------------//
int GameSounds::loadSounds()
{
  // Load sound files
  if (!jump_soundBuffer_.loadFromFile("Sounds/Player_jump.wav"))
  {
    cerr << "Kunde inte ladda \"Sounds/Player_jump.wav\"."
	 << endl;
    return 1;
  }
    
  if (!death_soundBuffer_.loadFromFile("Sounds/Player_death.wav"))
  {
    cerr << "Kunde inte ladda \"Sounds/Player_death.wav\"."
	 << endl;
    return 1;
  }

  if (!box_soundBuffer_.loadFromFile("Sounds/Box_push.wav"))
  {
    cerr << "Kunde inte ladda \"Sounds/Box_push.wav\"."
	 << endl;
    return 1;
  }

  if (!levelCleared_soundBuffer_.loadFromFile("Sounds/Level_cleared.wav"))
  {
    cerr << "Kunde inte ladda \"Sounds/Level_cleared.wav\"." << endl;
    return 1;
  }

  if (!pause_soundBuffer_.loadFromFile("Sounds/Pause.wav"))
  {
    cerr << "Kunde inte ladda \"Sounds/Pause.wav\"." << endl;
    return 1;
  }

  if(!music_.openFromFile("Sounds/background_music.wav"))
  {
    cerr << "Kunde inte ladda \"Sounds/background_music.wav\"." 
	 << endl;
    return 1;	  
  }

  music_.setVolume(42);
  music_.setLoop(true);

  // Connect sounds with sound buffers
  death_sound_.setBuffer(death_soundBuffer_);
  jump_sound_.setBuffer(jump_soundBuffer_);
  box_sound_.setBuffer(box_soundBuffer_);
  levelCleared_sound_.setBuffer(levelCleared_soundBuffer_);
  pause_sound_.setBuffer(pause_soundBuffer_);
}

//---------GETBOXSOUND--------------//
void GameSounds::getBoxSound() 
{ 
  if(delay_ == 10)
  {
    box_sound_.play();
    delay_ = 0;
  }
  else 
    delay_++;
}

//---------ISLEVELCLEAREDSOUNDPLAYING----------//
bool GameSounds::isLevelClearedSoundPlaying()
{
  return levelCleared_sound_.getStatus() == sf::SoundSource::Playing;
}  

bool GameSounds::isPauseSoundPlaying()
{
  return pause_sound_.getStatus() == sf::SoundSource::Playing;
}

//---------STARTBACKGROUNDMUSIC--------------//
void GameSounds::startBackgroundMusic()
{
  if(music_.getStatus() == sf::SoundSource::Playing ||
     music_.getStatus() == sf::SoundSource::Paused)
  {
    music_.stop();
  }
  music_.play();
}

//---------PAUSEBACKGROUNDMUSIC--------------//
void GameSounds::pauseBackgroundMusic()
{
  if(music_.getStatus() == sf::SoundSource::Playing)
    music_.pause();
}

//---------RESUMEBACKGROUNDMUSIC--------------//
void GameSounds::resumeBackgroundMusic()
{
  if(music_.getStatus() == sf::SoundSource::Paused)
    music_.play();
}

//---------ISBACKGROUNDMUSICPLAYING--------------//
bool GameSounds::isBackgroundMusicPlaying()
{
  return music_.getStatus() == sf::SoundSource::Playing;
}  
