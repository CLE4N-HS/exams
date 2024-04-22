#pragma once
#include "tools.h"

enum TILETYPE{ SINGLE, ANIME };


typedef struct tile tile;
struct tile
{
	tile* pNext;
	int tileSelected;
	char layer;
	sfVector2i pos;
	enum TILETYPE type;
	int frame;
	float animTimer;
	float timing;
};


void setTile(int _tileSelected, char _layer, sfVector2i _pos);
void addTile(tile* _Tile);

void loadMap(char* _path);
void saveMap(char* _path);