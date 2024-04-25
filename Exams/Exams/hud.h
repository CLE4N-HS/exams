#pragma once
#include "tools.h"
#include "stateManager.h"

typedef struct Hud {
	char name[30];
	int score;
	int coins;
	int lives;
}Hud;
Hud hud[2];

void initHud();
void updateHud(Window* _window);
void displayHud(Window* _window);