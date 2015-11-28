//GameSounds.cc
#include "GameSounds.h"
#include <SFML/Audio.hpp>
#include <iostream>
using namespace std;

void GameSounds::loadSounds()
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
