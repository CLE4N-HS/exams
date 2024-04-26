#pragma once
#include "tools.h"
#include "stateManager.h"

typedef enum ItemType {
	I_MUSHROOM,
	I_FIREFLOWER,
	I_ONEUP,
	I_STAR,
	I_COIN
}ItemType;

void initItems();
void updateItem(Window* _window);
void displayItem(Window* _window);

void createItem(ItemType _type, sfVector2f _pos);
// return the ocrrect item that will go out of the block
ItemType getCorrectItem(int _x, int _y, sfBool _isBig);
void eraseAllItems();
