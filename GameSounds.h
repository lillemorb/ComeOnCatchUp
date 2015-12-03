//GameSounds.h
#ifndef GAMESOUNDS_H
#define GAMESOUNDS_H
#include <SFML/Audio.hpp>
#include <iostream>

class GameSounds
{
public:
  GameSounds() { loadSounds(); }
  ~GameSounds() = default;

  void getDeathSound() { death_sound_.play(); }
  void getJumpSound() { jump_sound_.play(); }
  void getBoxSound();

  void getLevelClearedSound() { levelCleared_sound_.play();}
  bool isLevelClearedSoundPlaying();

  void getPauseSound() { pause_sound_.play(); }
  bool isPauseSoundPlaying();

  void startBackgroundMusic();
  void pauseBackgroundMusic();
  void resumeBackgroundMusic();
  bool isBackgroundMusicPlaying();
  
private:
  GameSounds(const GameSounds&) = delete;
  GameSounds& operator=(const GameSounds&) = delete;

  void loadSounds(); 

  sf::SoundBuffer death_soundBuffer_;
  sf::SoundBuffer jump_soundBuffer_;
  sf::SoundBuffer box_soundBuffer_;
  sf::SoundBuffer levelCleared_soundBuffer_;
  sf::SoundBuffer pause_soundBuffer_;

  int delay_ {0};

  sf::Sound death_sound_;
  sf::Sound jump_sound_;
  sf::Sound box_sound_;
  sf::Sound levelCleared_sound_;
  sf::Sound pause_sound_;

  sf::Music music_;
};

#endif
