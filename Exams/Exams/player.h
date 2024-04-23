#pragma once
#include "tools.h"
#include "stateManager.h"

typedef enum playerPower {
	P_DEAD,
	P_SMALL,
	P_BIG,
	P_FIRETHROWER
}playerPower;

void initPlayer();
void updatePlayer(Window* _window);
void displayPlayer(Window* _window);

sfFloatRect* pgetPlayerBounds(int _id);
void setPlayerPower(int _id, playerPower _power);
void DamagePlayer(int _id);
