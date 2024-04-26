#pragma once
#include "tools.h"
#include "stateManager.h"

#define FINISH_XPOS 14259.f


int playerTurn;
int wantedPlayerTurn;
sfBool isAtFinish;

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
sfVector2f getGreatestViewPos();
// means that the player could enter the pipe if they face the right direction
void setPlayerPossiblePipe(int _id, sfBool _canEnter, sfBool _YPipe);
sfBool isPlayerBig(int _id);
sfVector2f getPlayerPos(int _id);
void setPlayerPos(int _id, sfVector2f _pos);
void setPlayerVelocity(int _id, float _x, float _y);
// the player will make a small jump after boucing on an enemy
void MakePlayerJump(int _id);
void setPlayerStarPower(int _id);
sfBool PlayerHasStar(int _id);
// reset all infos about the player
void resetPlayer(int _id);
sfBool isPlayerAlive(int _id);
void setPlayerCanReplayMusic(int _id, sfBool _canHe);

