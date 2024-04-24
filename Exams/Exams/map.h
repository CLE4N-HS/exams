#pragma once
#include "tools.h"
#include "stateManager.h"

#define BLOCK_SIZE 16

#define BLOCK_SCALE /*4.5f*/ 4.821428f /*5.f*/

#define NB_BLOCKS_X 212
#define NB_BLOCKS_Y 14

int nbMap;

typedef enum {
	T_STAIR,
	T_CASTLEBLOCK,
	T_GROUND,
	T_L_BUSH,
	T_BLOCK,
	T_M_BUSH,
	T_QUESTION,
	T_R_BUSH,
	T_UL_CLOUD,
	T_BL_CLOUD,
	T_UM_CLOUD,
	T_BM_CLOUD,
	T_UR_CLOUD,
	T_BR_CLOUD,
	T_BLUE,
	T_BL_HILL,
	T_UM_HILL,
	T_BM_HILL,
	T_GREEN,
	T_BR_HILL,
	T_UL_PIPE,
	T_BL_PIPE,
	T_UR_PIPE,
	T_BR_PIPE,
	T_U_FLAG,
	T_B_FLAG,
	T_U_EMPTYCASTLE,
	T_R_WINDOWCASTLE,
	T_U_DOORCASTLE,
	T_B_DOORCASTLE,
	T_U_CASTLE,
	T_L_WINDOWCASTLE,
	T_UL_DARK_PIPE,
	T_BL_DARK_PIPE,
	T_UR_DARK_PIPE,
	T_BR_DARK_PIPE,
	T_DARK_SKY,
	T_LSIDE_DARK_PIPE,
	T_DARK_COIN,
	T_RSIDE_DARK_PIPE,
	T_DARK_BLOCK,
	T_DARK_GROUND
}blockType;

typedef struct {
	blockType type;
	sfVector2f pos;
	sfIntRect rect;
	sfBool isSolid;
	float timer;
	sfColor color;
}Blocks;
Blocks b[NB_BLOCKS_Y][NB_BLOCKS_X];

sfFloatRect tmpRect;
sfFloatRect tmpRect2;
sfFloatRect tmpRect3;
sfFloatRect tmpRect4;
sfFloatRect tmpRect5;

sfVector2f tmpcrpos;
sfVector2f tmpcrpos2;

void initMap();

void updateMap(Window* _window);

void displayMap(Window* _window);

// make a default map in case of no file and create it
void defaultMap();

// load a map in the folders and update blocks
void loadMap(int _nbMap);

// return the block index of the player position
sfVector2i getPlayerBlockPos(sfVector2f _pos);

// collisions for the ground
sfBool isGrounded(sfVector2f _pos, sfVector2f* _velocity, sfVector2f _origin, sfFloatRect _bounds);

// global collisions
sfBool isCollision3(sfFloatRect _rect, sfVector2f* _velocity, int _playerId);

// collisions on a certain axis
sfBool isCollision2(sfFloatRect _rect, sfBool _XAxis, sfBool _UpOrLeft, sfVector2f _nextVelocity, int _playerId);

sfBool isMapFinished();

void setMapFinishedState(sfBool _isMapF);

void deinitMap();
