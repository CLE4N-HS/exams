#include "soundManager.h"

// TODO : comme pour le texture manager mais avec les sons

void Sound_Onload(State _state)
{
	ressources* tmpRessources = ResBegin;
	while (tmpRessources != NULL)
	{
		if (tmpRessources->state == _state)
		{
			if (tmpRessources->type == SOUNDFX)
			{
				Sound* tmpSound = (Sound*)calloc(1, sizeof(Sound));
				strcpy(tmpSound->name, tmpRessources->name);
				tmpSound->type = tmpRessources->type;
				tmpSound->state = tmpRessources->state;

				tmpSound->sound = sfSound_create();
				tmpSound->buffer = sfSoundBuffer_createFromFile(tmpRessources->path);
				sfSound_setBuffer(tmpSound->sound, tmpSound->buffer);
				//tmpSound->sound = sfSound_create();
				//sfSoundBuffer* tmpBuffer = sfSoundBuffer_createFromFile(tmpRessources->path);
				//sfSound_setBuffer(tmpSound->sound, tmpBuffer);
				//sfSoundBuffer_destroy(tmpBuffer);

				AddSound(tmpSound);
			}
			else if (tmpRessources->type == MUSIC)
			{
				Sound* tmpSound = (Sound*)calloc(1, sizeof(Sound));
				strcpy(tmpSound->name, tmpRessources->name);
				tmpSound->type = tmpRessources->type;
				tmpSound->state = tmpRessources->state;

				tmpSound->music = sfMusic_createFromFile(tmpRessources->path);
				
				AddSound(tmpSound);
			}
		}
		tmpRessources = tmpRessources->pNext;
	}
}

void AddSound(Sound* _sound)
{
	_sound->pNext = soundBegin;
	soundBegin = _sound;
}

Sound* RemoveSound(Sound* _sound)
{
	if (_sound == soundBegin)
	{
		Sound* tmpSound = _sound->pNext;
		soundBegin = tmpSound;
		if (tmpSound->type == MUSIC) {
			sfMusic_destroy(_sound->music);
		}
		else if (tmpSound->type == SOUNDFX) {
			sfSound_destroy(_sound->sound);
			sfSoundBuffer_destroy(tmpSound->buffer);
		}
		free(_sound);
		return tmpSound;
	}
	else
	{
		Sound* tmpSound = soundBegin;
		while (tmpSound->pNext != _sound)
		{
			tmpSound = tmpSound->pNext;
		}
		tmpSound->pNext = _sound->pNext;
		if (tmpSound->type == MUSIC) {
			sfMusic_destroy(_sound->music);
		}
		else if (tmpSound->type == SOUNDFX) {
			sfSound_destroy(_sound->sound);
			sfSoundBuffer_destroy(tmpSound->buffer);
		}
		free(_sound);
		return tmpSound->pNext;
	}
}

void RemoveAllSounds()
{
	Sound* tmpSound = soundBegin;
	while (tmpSound != NULL)
	{
		if (tmpSound->type == MUSIC) {
			sfMusic_stop(tmpSound->music);
			sfMusic_destroy(tmpSound->music);
		}
		else if (tmpSound->type == SOUNDFX) {
			sfSound_stop(tmpSound->sound);
			sfSound_destroy(tmpSound->sound);
			sfSoundBuffer_destroy(tmpSound->buffer);
		}

		Sound* toDelete = tmpSound;
		tmpSound = tmpSound->pNext;
		free(toDelete);
	}
	soundBegin = 0;
}

void RemoveAllSoundsButALL()
{
	Sound* tmpSound = soundBegin;
	while (tmpSound != NULL)
	{
		if (tmpSound->state != ALL)
		{
			if (tmpSound == soundBegin)
			{
				Sound* toDelete = tmpSound->pNext;
				soundBegin = toDelete;
				if (tmpSound->type == MUSIC) {
					sfMusic_destroy(tmpSound->music);
				}
				else if (tmpSound->type == SOUNDFX) {
					sfSound_destroy(tmpSound->sound);
					sfSoundBuffer_destroy(tmpSound->buffer);
				}
				free(tmpSound);
				tmpSound = toDelete;
			}
			else
			{
				Sound* toDelete = tmpSound;
				//tmpSound = tmpSound->pNext;
				if (tmpSound->type == MUSIC) {
					sfMusic_destroy(tmpSound->music);
				}
				else if (tmpSound->type == SOUNDFX) {
					sfSound_destroy(tmpSound->sound);
					sfSoundBuffer_destroy(tmpSound->buffer);
				}
				tmpSound = tmpSound->pNext;
				free(toDelete);
			}
		}
		else
			tmpSound = tmpSound->pNext;
	}
}

void PlayASound(char* _name, sfBool _loop)
{
	Sound* tmpSound = soundBegin;
	while (tmpSound != NULL)
	{
		if (strcmp(_name, tmpSound->name) == 0) {
			if (tmpSound->type == MUSIC) {
				sfMusic_setVolume(tmpSound->music, musicVolume);
				sfMusic_play(tmpSound->music);
				sfMusic_setLoop(tmpSound->music, _loop);
			}
			else if (tmpSound->type == SOUNDFX) {
				sfSound_setVolume(tmpSound->sound, SFXVolume);
				sfSound_play(tmpSound->sound);
				sfSound_setLoop(tmpSound->music, _loop);
			}
			return;
		}
		tmpSound = tmpSound->pNext;
	}
	printf("Unable to play the sound : %s , the name given might be wrong\n", _name);
}

void ChangeVolume(ressourceType _type, float _volume)
{
	Sound* tmpSound = soundBegin;
	while (tmpSound != NULL)
	{
		if (_type == MUSIC) {
			if (tmpSound->type == MUSIC) {
			sfMusic_setVolume(tmpSound->music, _volume);
			}
		}
		else if (_type == SOUNDFX) {
			if (tmpSound->type == SOUNDFX) {
			sfSound_setVolume(tmpSound->sound, _volume);
			}
		}
		tmpSound = tmpSound->pNext;
	}
}