#pragma once
#include "tools.h"
#include "stateManager.h"

#define BLOCK_SIZE 16

#define BLOCK_SCALE 4.821428f

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
	T_L_WINDOWCASTLE

}blockType;

typedef struct {
	blockType type;
	sfVector2f pos;
	sfIntRect rect;
	sfBool isSolid;
	float timer;
}Blocks;
Blocks b[NB_BLOCKS_Y][NB_BLOCKS_X];

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
sfBool isGrounded(sfVector2f _pos, sfVector2f* _velocity, sfVector2f* _drag);

// global collisions
sfBool isCollision3(sfFloatRect _rect, sfVector2f* _velocity);

// collisions on a certain axis
sfBool isCollision2(sfFloatRect _rect, sfBool _XAxis, sfBool _UpOrLeft);

sfBool isMapFinished();

void setMapFinishedState(sfBool _isMapF);

void deinitMap();
