#pragma once
#include "tools.h"
#include "stateManager.h"


typedef enum 
{
	SINGLE, SPRITESHEET, ANIMATION, SOUNDFX, MUSIC, FONT, VIDEO
}ressourceType;

typedef struct ressources ressources;
struct ressources
{
	ressources* pNext;
	State state;
	ressourceType type;
	char name[30];
	char path[200];
};

ressources* ResBegin;

/// <summary>
/// Reads a file in which all ressources are specified, then loads all ressources
/// </summary>
/// <param name="_path">Path of the file</param>
void OnLoad(char* _path);

/// <summary>
/// Adds Ressources in the linked list
/// </summary>
/// <param name="_ressources">Ressource</param>
void AddRessources(ressources* _ressources);

/// <summary>
/// Removes Ressources from the linked list
/// </summary>
/// <param name="_ressources">Ressouce</param>
/// <returns>The next Ressouce in the linked list</returns>
ressources* RemoveRessources(ressources* _ressources);
