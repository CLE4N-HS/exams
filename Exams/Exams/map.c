#include "map.h"


tile* tileBegin;


void setTile(int _tileSelected, char _layer, sfVector2i _pos)
{
	tile* tempTile = (tile*)calloc(1, sizeof(tile));
	tempTile->tileSelected = _tileSelected;
	tempTile->layer = _layer;
	tempTile->pos = _pos;
	//tempTile->type = _type;
	tempTile->animTimer = 0.0f;

	addTile(tempTile);
}

void addTile(tile* _Tile)
{
	_Tile->pNext = tileBegin;
	tileBegin = _Tile;
}

void loadMap(char* _path)
{

}

void saveMap(char* _path)
{

}
