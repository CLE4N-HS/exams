#pragma once
#include "tools.h"
#include "stateManager.h"

void initScore();
void updateScore(Window* _window);
void displayScore(Window* _window);

void createScore(int _score, sfVector2f _pos);
void eraseAllScore();
