#pragma once
#include "tools.h"
#include "stateManager.h"

typedef enum ItemType {
	I_MUSHROOM,
	I_FIREFLOWER,
	I_ONEUP
}ItemType;

void initItems();
void updateItem(Window* _window);
void displayItem(Window* _window);

void createItem(ItemType _type, sfVector2f _pos);