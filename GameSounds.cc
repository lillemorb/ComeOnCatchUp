//GameSounds.cc
#include "GameSounds.h"
#include <SFML/Audio.hpp>
#include <iostream>
using namespace std;

void GameSounds::loadSounds()
{
  //Lillemor: kan de här göras i konstruktorn? Annars har vi
  
  if (!jump_soundBuffer_.loadFromFile("Sounds/Player_jump.wav"))
  {
    cerr << "Kunde inte ladda \"Sounds/Player_jump.wav\"."
	 << endl;
    // Fixa felhantering
    // return 1;
  }
    
  if (!death_soundBuffer_.loadFromFile("Sounds/Player_death.wav"))
  {
    cerr << "Kunde inte ladda \"Sounds/Player_death.wav\"."
	 << endl;
    // Fixa felhantering
    // return 1;
  }

  if (!box_soundBuffer_.loadFromFile("Sounds/Box_push.wav"))
  {
    cerr << "Kunde inte ladda \"Sounds/Box_push.wav\"."
	 << endl;
    // Fixa felhantering
    // return 1;
  }

  if (!levelCleared_soundBuffer_.loadFromFile("Sounds/Level_cleared.wav"))
  {
    cerr << "Kunde inte ladda \"Sounds/Level_cleared.wav\"." << endl;
    // Fixa felhantering
    // return 1;
  }

  if(!music_.openFromFile("Sounds/background_music.wav"))
  {
    cerr << "Kunde inte ladda \"Sounds/background_music.wav\"." 
	 << endl;
    // Fixa felhantering
    // return 1;	  
  }

  //Justera beroende på volymen på bakgrundsvolymen vi väljer
  music_.setVolume(42);
  music_.setLoop(true);

  death_sound_.setBuffer(death_soundBuffer_);
  jump_sound_.setBuffer(jump_soundBuffer_);
  box_sound_.setBuffer(box_soundBuffer_);
  levelCleared_sound_.setBuffer(levelCleared_soundBuffer_);
}

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

bool GameSounds::isLevelClearedSoundPlaying()
{
  return levelCleared_sound_.getStatus() == sf::SoundSource::Playing;
}  

void GameSounds::startBackgroundMusic()
{
  if(music_.getStatus() == sf::SoundSource::Playing ||
     music_.getStatus() == sf::SoundSource::Paused)
  {
    music_.stop();
  }
  music_.play();
}

void GameSounds::pauseBackgroundMusic()
{
  if(music_.getStatus() == sf::SoundSource::Playing)
    music_.pause();
}

void GameSounds::resumeBackgroundMusic()
{
  if(music_.getStatus() == sf::SoundSource::Paused)
    music_.play();
}

bool GameSounds::isBackgroundMusicPlaying()
{
  return music_.getStatus() == sf::SoundSource::Playing;
}  
