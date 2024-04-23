#pragma once
#include "tools.h"
#include "stateManager.h"

void initFireballs();
void updateFireballs(Window* _window);
void displayFireballs(Window* _window);

void createFireball(sfVector2f _pos, sfBool _leftSide);
int getNbFireballs();
sfBool isFireballInBounds(sfFloatRect* _bounds);
