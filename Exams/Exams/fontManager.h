#pragma once
#include "tools.h"
#include "ressourcesManager.h"

typedef struct Font Font;
struct Font {
	Font* pNext;
	sfFont* font;
	char name[200];
	ressourceType type;
	State state;

};

Font* FontBegin;

/// <summary>
/// Loads Fonts needed in the current state based on there type
/// </summary>
/// <param name="_state">Current State</param>
void Font_Onload(State _state);

/// <summary>
/// Adds a Font in the linked list of all Fonts
/// </summary>
/// <param name="_font">The complete structure of the Fonts</param>
void AddFont(Font* _font);

/// <summary>
/// Removes a Font from the linked list of all Fonts to lighten the memory
/// </summary>
/// <param name="_font">The structure of the Font you want to remove</param>
/// <returns>The next Font in the linked list</returns>
Font* RemoveFont(Font* _font);

/// <summary>
/// Removes all Fonts from the linked list
/// </summary>
void RemoveAllFonts();

/// <summary>
/// Removes all Fonts from the linked list but not those needed in all states
/// </summary>
void RemoveAllFontsButALL();

/// <summary>
/// Gets a Font in the linked list of all Fonts
/// </summary>
/// <param name="_name">Name of the Font</param>
/// <returns>The Font or the defaultFont if not found</returns>
sfFont* GetFont(char* _name);
