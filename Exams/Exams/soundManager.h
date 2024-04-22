#pragma once
#include "tools.h"
#include "SFML/Audio.h"
#include "ressourcesManager.h"

typedef struct {
	sfSound* sound;
	sfSoundBuffer* buffer;
}customSound;

typedef struct Sound Sound;
struct Sound
{
	Sound* pNext;

	union MyUnion
	{
		sfMusic* music;
		sfSound* sound;
		//customSound* customSound;
	};
	sfSoundBuffer* buffer;
	char name[30];
	ressourceType type;
	State state;

};

Sound* soundBegin;

float SFXVolume;
float musicVolume;

/// <summary>
/// Loads Sounds and Musics needed in the current state based on there type
/// </summary>
/// <param name="_state">Current State</param>
void Sound_Onload(State _state);

/// <summary>
/// Adds a Sound or a Music in the linked list of all Sounds
/// </summary>
/// <param name="_sound">The complete structure of the Sounds</param>
void AddSound(Sound* _sound);

/// <summary>
/// Removes a Sound from the linked list of all Sounds to lighten the memory
/// </summary>
/// <param name="_sound">The structure of the Sound you want to remove</param>
/// <returns>The next Sound in the linked list</returns>
Sound* RemoveSound(Sound* _sound);

/// <summary>
/// Removes all Sounds from the linked list
/// </summary>
void RemoveAllSounds();

void RemoveAllSoundsButALL();

/// <summary>
/// Plays a Sound or a Music in the linked list and loop it or not
/// </summary>
/// <param name="_name">The name of the Sound</param>
/// <param name="_loop">sfTrue if you want to loop it, sfFalse if not or to stop the loop</param>
void PlayASound(char* _name, sfBool _loop);

/// <summary>
/// Changes the volume of Sounds or Musics based on the user choice
/// </summary>
/// <param name="_type">For Sound or Music</param>
/// <param name="_volume">Volume to set (between 0 (mute) and 100 (full volume)).</param>
void ChangeVolume(ressourceType _type, float _volume);
