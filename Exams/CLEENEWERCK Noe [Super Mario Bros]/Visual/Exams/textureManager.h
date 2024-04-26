#pragma once
#include "ressourcesManager.h"

typedef struct Texture Texture;
struct Texture
{
	Texture* pNext;
	char name[30];
	ressourceType type;
	State state;
	sfTexture* sftexture;
	int compteur;
	sfIntRect spSheetRec;
	float frameTime;
	int NbFrame;
	int row;
};

Texture* textureBegin;


typedef enum 
{
	TEXNULL = -1, PATH, CONFIG
}StateTexture;



/// <summary>
/// Loads Textures needed in the current state based on there type
/// </summary>
/// <param name="_state">Current State</param>
void Texture_Onload(State _state);

/// <summary>
/// Gets a Texture in the linked list of all Textures
/// </summary>
/// <param name="_name">Name of the Texture</param>
/// <returns>The Texture or NULL if not found</returns>
sfTexture* GetTexture(char* _name);

/// <summary>
/// Gets the number of frame of a Texture in the liked list of all Textures
/// </summary>
/// <param name="_name">Name of the Texture</param>
/// <returns>The number of frame or 0 if not found</returns>
int GetNbFrame(char* _name);

/// <summary>
/// Gets the frame time of a Texture in the linked list of all Textures
/// </summary>
/// <param name="_name">Name of the Texture</param>
/// <returns>The frame time or 0.0f if not found</returns>
float GetFrameTime(char* _name);

/// <summary>
/// Gets the rect of a Texture in the linked list of all Textures
/// </summary>
/// <param name="_name">Name of the Texture</param>
/// <returns>The rect of the Texture or {0, 0, 0, 0} if not found</returns>
sfIntRect GetRect(char* _name);

/// <summary>
/// Adds a Texture in the linked list of all Textures
/// </summary>
/// <param name="_texture">The complete structure of the Texture</param>
void AddTexture(Texture* _texture);

/// <summary>
/// Removes a Texture from the linked list of all Textures to lighten the memory
/// </summary>
/// <param name="_texture">The structure of the Texture you want to remove</param>
/// <returns>The next Texture in the linked list</returns>
Texture* RemoveTexture(Texture* _texture);

/// <summary>
/// Removes all Textures from the linked list
/// </summary>
void RemoveAllTexture();

/// <summary>
/// Removes all Textures from the linked list but not those needed in all states
/// </summary>
void RemoveAllTextureButALL();
