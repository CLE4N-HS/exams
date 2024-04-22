#pragma once
#include "tools.h"
#include "stateManager.h"
#include "map.h"
#include "textureManager.h"

#define SAVE_PATH "../Ressources/Saves/"
#define NB_MAX_TILES 31

void initEditor();
void updateEditor(Window* _window);
void displayEditor(Window* _window);

// save map into SAVE_PATH..
void saveMap(int _nbMap);

// CTRL C CTRL V a map
void copyMap(int _MapToCopy);

void deinitEditor();
