#pragma once
#include "tools.h"
#include "stateManager.h"

typedef enum EnemyType {
	E_GOOMBA,
	E_KOOPA
}EnemyType;

void initEnemies();
void updateEnemies(Window* _window);
void displayEnemies(Window* _window);

void createEnemy(EnemyType _type, sfVector2f _pos);
