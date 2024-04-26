#include "map.h"
#include "editor.h"
#include "CustomMath.h"
#include "gamepadx.h"
#include "soundManager.h"
#include "items.h"
#include "player.h"
#include "enemies.h"
#include "fireballs.h"
#include "game.h"
#include "score.h"
#include "hud.h"
#include "viewManager.h"

#define UP_BLOCK_TIMER 0.2f

#define SPECIAL_BLOC_RESET_TIMER 28.f

sfSprite* mapSprite;

sfTexture* tilesetTexture;

sfRectangleShape* mapRectangle;

float changeMapTimer;

sfBool isMapF;

sfBool couldGoInYPipe;
sfBool couldGoInXPipe;

float specialBlocTimer;

void initMap()
{
	mapSprite = sfSprite_create();


	mapRectangle = sfRectangleShape_create();
	sfRectangleShape_setFillColor(mapRectangle, color(92, 148, 252, 255));
	sfRectangleShape_setPosition(mapRectangle, vector2f(0.f, 0.f));
	sfRectangleShape_setSize(mapRectangle, vector2f(1920.f, 1080.f));

	nbMap = 1;
	loadMap(nbMap);
	changeMapTimer = 0.f;

	isMapF = sfFalse;

	tilesetTexture = GetTexture("tileset");


	sfSprite_setTexture(mapSprite, tilesetTexture, sfFalse);

}

void updateMap(Window* _window)
{
	float dt = getDeltaTime();
}

void displayMap(Window* _window)
{
	float dt = getDeltaTime();

	sfRenderTexture_setView(_window->renderTexture, sfRenderTexture_getDefaultView(_window->renderTexture)); // blue or black background
	if (nbMap == 2) sfRectangleShape_setFillColor(mapRectangle, sfBlack);
	else sfRectangleShape_setFillColor(mapRectangle, color(92, 148, 252, 255));
	sfRenderTexture_drawRectangleShape(_window->renderTexture, mapRectangle, NULL);
	sfRenderTexture_setView(_window->renderTexture, mainView->view);

	for (int j = 0; j < NB_BLOCKS_Y; j++)
	{
		for (int i = 0; i < NB_BLOCKS_X; i++)
		{
			if (j == 9 && i == 94 && specialBlocTimer > 0.5f && specialBlocTimer < SPECIAL_BLOC_RESET_TIMER - 0.5f) {
				specialBlocTimer -= dt * 5.f;
				printf("%f\n", specialBlocTimer);
			}

			if (b[j][i].timer > 0.f) { // should be in update but it's more optimized this way
				if (b[j][i].timer >= 0.1f)
					b[j][i].pos.y -= 120.f * dt;
				else
					b[j][i].pos.y += 120.f * dt;
				b[j][i].timer -= dt;
				max(b[j][i].timer, 0.f);
			}
			else {
				b[j][i].pos.y = j * BLOCK_SCALE * BLOCK_SIZE;
			}


			sfSprite_setPosition(mapSprite, b[j][i].pos);
			sfSprite_setScale(mapSprite, vector2f(BLOCK_SCALE, BLOCK_SCALE));
			sfSprite_setColor(mapSprite, b[j][i].color);

			sfSprite_setTextureRect(mapSprite, b[j][i].rect);
			sfRenderTexture_drawSprite(_window->renderTexture, mapSprite, NULL);
		}
	}

}

void defaultMap()
{
	for (int j = 0; j < NB_BLOCKS_Y; j++)
	{
		for (int i = 0; i < NB_BLOCKS_X; i++)
		{
			//if (j > 3) {
			//	b[j][i].type = T_TLIGHTWALL;
			//	b[j][i].isSolid = sfTrue;
			//	b[j][i].rect = IntRect(1 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
			//}
			//else {
			//	b[j][i].type = T_DARKWALL2;
			//	b[j][i].isSolid = sfFalse;
			//	b[j][i].rect = IntRect(4 * BLOCK_SIZE, 5 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
			//}
			//if (i <= 1)
			//{
			//	b[j][i].isSolid = sfTrue;
			//}
			b[j][i].isSolid = sfFalse;
			b[j][i].type = T_BLUE;
			b[j][i].rect = IntRect(7 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
			b[j][i].pos = vector2f(i * BLOCK_SIZE * BLOCK_SCALE, j * BLOCK_SIZE * BLOCK_SCALE);
			b[j][i].timer = 0.f;
			b[j][i].color = color(255, 255, 255, 255);

		}
	}
}

void loadMap(int _nbMap)
{

	FILE* file;
	switch (_nbMap)
	{
	case 1:
		file = fopen(SAVE_PATH"map1.bin", "rb");
		if (file == NULL) {
			defaultMap();
			file = fopen(SAVE_PATH"map1.bin", "wb");
			fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		else {
			fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		break;
	case 2:
		file = fopen(SAVE_PATH"map2.bin", "rb");
		if (file == NULL) {
			defaultMap();
			file = fopen(SAVE_PATH"map2.bin", "wb");
			fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		else {
			fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		break;
	case 3:
		file = fopen(SAVE_PATH"map3.bin", "rb");
		if (file == NULL) {
			defaultMap();
			file = fopen(SAVE_PATH"map3.bin", "wb");
			fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		else {
			fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		break;
	case 4:
		file = fopen(SAVE_PATH"map4.bin", "rb");
		if (file == NULL) {
			defaultMap();
			file = fopen(SAVE_PATH"map4.bin", "wb");
			fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		else {
			fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		break;
	case 5:
		file = fopen(SAVE_PATH"map5.bin", "rb");
		if (file == NULL) {
			defaultMap();
			file = fopen(SAVE_PATH"map5.bin", "wb");
			fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		else {
			fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		break;
	case 6:
		file = fopen(SAVE_PATH"map6.bin", "rb");
		if (file == NULL) {
			defaultMap();
			file = fopen(SAVE_PATH"map6.bin", "wb");
			fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		else {
			fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		break;
	case 7:
		file = fopen(SAVE_PATH"map7.bin", "rb");
		if (file == NULL) {
			defaultMap();
			file = fopen(SAVE_PATH"map7.bin", "wb");
			fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		else {
			fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		break;
	case 8:
		file = fopen(SAVE_PATH"map8.bin", "rb");
		if (file == NULL) {
			defaultMap();
			file = fopen(SAVE_PATH"map8.bin", "wb");
			fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		else {
			fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		break;
	case 9:
		file = fopen(SAVE_PATH"map9.bin", "rb");
		if (file == NULL) {
			defaultMap();
			file = fopen(SAVE_PATH"map9.bin", "wb");
			fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		else {
			fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		break;
	default:
		break;
	}

	eraseAllFireballs();
	eraseAllEnemies();
	eraseAllItems();
	eraseAllScore();
	if (nbMap == 1) {
		loadAllMapOneEnnemies();
		specialBlocTimer = SPECIAL_BLOC_RESET_TIMER;
	}
}

sfVector2i getPlayerBlockPos(sfVector2f _pos)
{
	sfVector2f fPos = MultiplyVector(_pos, 1.f / BLOCK_SIZE / BLOCK_SCALE);
	sfVector2i iPos = vector2i(0, 0);
	iPos.x = (int)fPos.x;
	iPos.y = (int)fPos.y;
	return iPos;
}

void BecomeBlueSky(int _y, int _x)
{
	b[_y][_x].type = T_BLUE;
	b[_y][_x].rect = IntRect(7 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
	b[_y][_x].isSolid = sfFalse;
}

void BecomeDarkSky(int _y, int _x)
{
	b[_y][_x].type = T_DARK_SKY;
	b[_y][_x].rect = IntRect(18 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
	b[_y][_x].isSolid = sfFalse;
}

void BecomeHitBlock(int _y, int _x)
{
	b[_y][_x].type = T_HIT_BLOCK;
	b[_y][_x].rect = IntRect(21 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
	b[_y][_x].isSolid = sfTrue;
}

sfBool isGrounded(sfVector2f _pos, sfVector2f* _velocity, sfVector2f _origin, sfFloatRect _bounds)
{
	float dt = getDeltaTime();

	sfVector2i blockPos = getPlayerBlockPos(AddVectors(vector2f(_pos.x - 8.f * BLOCK_SCALE + BLOCK_SCALE, _pos.y + 1.f * BLOCK_SCALE - (_origin.y - _bounds.height / BLOCK_SCALE) * BLOCK_SCALE), MultiplyVector(*_velocity, dt))); // offset to not count the alpha 0
	sfVector2i blockPos2 = getPlayerBlockPos(AddVectors(vector2f(_pos.x + 8.f * BLOCK_SCALE - BLOCK_SCALE, _pos.y + 1.f * BLOCK_SCALE - (_origin.y - _bounds.height / BLOCK_SCALE) * BLOCK_SCALE), MultiplyVector(*_velocity, dt)));

	if (blockPos.y < 0 || blockPos.y >= NB_BLOCKS_Y || blockPos.x < 0 || blockPos.x >= NB_BLOCKS_X || blockPos2.y < 0 || blockPos2.y >= NB_BLOCKS_Y || blockPos2.x < 0 || blockPos2.x >= NB_BLOCKS_X) // out of array
		return sfFalse;

	tmpRect4 = FlRect(b[blockPos.y][blockPos.x].pos.x, b[blockPos.y][blockPos.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
	tmpRect5 = FlRect(b[blockPos2.y][blockPos2.x].pos.x, b[blockPos2.y][blockPos2.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);

	if (_velocity->y < -0.f) // prolly flying
		return sfFalse;


	if (b[blockPos.y][blockPos.x].isSolid || b[blockPos2.y][blockPos2.x].isSolid) {
		if (b[blockPos.y][blockPos.x].type == T_HIDDEN_BLOCK || b[blockPos2.y][blockPos2.x].type == T_HIDDEN_BLOCK)
			return sfFalse;

		return sfTrue;
	}

	return sfFalse;


}

sfBool isCollision2(sfFloatRect _rect, sfBool _XAxis, sfBool _UpOrLeft, sfVector2f _nextVelocity, int _playerId)
{
	float dt = getDeltaTime();
	float offset = 1.f;
	sfVector2f playerPos = VECTOR2F_NULL;
	sfVector2f playerPos2 = VECTOR2F_NULL;
	sfVector2f playerPos3 = VECTOR2F_NULL;
	sfBool isBig = sfFalse;
	if (_playerId >= 0) {
		isBig = isPlayerBig(_playerId);
	}

	if (_XAxis) {
		if (_playerId >= 0) {
			_rect.height -= BLOCK_SCALE;
			_rect.top -= BLOCK_SCALE;
		}
		if (_UpOrLeft) {
			playerPos = AddVectors(vector2f(_rect.left, _rect.top + BLOCK_SCALE), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
			playerPos2 = AddVectors(vector2f(_rect.left, _rect.top + _rect.height - BLOCK_SCALE * 2.f), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
			playerPos.x += BLOCK_SCALE * BLOCK_SIZE;
			playerPos2.x += BLOCK_SCALE * BLOCK_SIZE;
			if (isBig) {
				playerPos3 = AddVectors(vector2f(_rect.left, _rect.top + _rect.height - BLOCK_SCALE * 1.f), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
				playerPos3.x += BLOCK_SCALE * BLOCK_SIZE;
			}
		}
		else {
			playerPos = AddVectors(vector2f(_rect.left + _rect.width, _rect.top + BLOCK_SCALE), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
			playerPos2 = AddVectors(vector2f(_rect.left + _rect.width, _rect.top + _rect.height - BLOCK_SCALE * 2.f), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
			playerPos.x -= BLOCK_SCALE * BLOCK_SIZE;
			playerPos2.x -= BLOCK_SCALE * BLOCK_SIZE;
			if (isBig) {
				playerPos3 = AddVectors(vector2f(_rect.left + _rect.width, _rect.top + _rect.height - BLOCK_SCALE * 1.f), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
				playerPos3.x -= BLOCK_SCALE * BLOCK_SIZE;
			}
		}
	}
	else {
		if (_UpOrLeft) {
			playerPos = AddVectors(vector2f(_rect.left + BLOCK_SCALE, _rect.top), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
			playerPos2 = AddVectors(vector2f(_rect.left + _rect.width - BLOCK_SCALE, _rect.top), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
			playerPos.y += BLOCK_SCALE * BLOCK_SIZE;
			playerPos2.y += BLOCK_SCALE * BLOCK_SIZE;
			playerPos3 = AddVectors(vector2f(_rect.left + _rect.width / 2.f, _rect.top), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
			playerPos3.y += BLOCK_SCALE * BLOCK_SIZE;
		}
		else {
			playerPos = AddVectors(vector2f(_rect.left + BLOCK_SCALE, _rect.top + _rect.height), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
			playerPos2 = AddVectors(vector2f(_rect.left + _rect.width - BLOCK_SCALE, _rect.top + _rect.height), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
			playerPos.y -= BLOCK_SCALE * BLOCK_SIZE;
			playerPos2.y -= BLOCK_SCALE * BLOCK_SIZE;
			playerPos3 = AddVectors(vector2f(_rect.left + _rect.width / 2.f, _rect.top + _rect.height), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
			playerPos3.y -= BLOCK_SCALE * BLOCK_SIZE;
		}
	}

	tmpRect3 = _rect;

	sfVector2i blockPos = getPlayerBlockPos(playerPos);
	sfVector2i blockPos2 = getPlayerBlockPos(playerPos2);
	sfVector2i blockPos3 = getPlayerBlockPos(playerPos3);


	tmpRect = FlRect(blockPos.x * BLOCK_SCALE * BLOCK_SIZE, blockPos.y * BLOCK_SCALE * BLOCK_SIZE, BLOCK_SCALE * BLOCK_SIZE, BLOCK_SCALE * BLOCK_SIZE);
	tmpRect2 = FlRect(blockPos2.x * BLOCK_SCALE * BLOCK_SIZE, blockPos2.y * BLOCK_SCALE * BLOCK_SIZE, BLOCK_SCALE * BLOCK_SIZE, BLOCK_SCALE * BLOCK_SIZE);

	if (blockPos.y < 0 || blockPos.y >= NB_BLOCKS_Y || blockPos.x < 0 || blockPos.x >= NB_BLOCKS_X || blockPos2.y < 0 || blockPos2.y >= NB_BLOCKS_Y || blockPos2.x < 0 || blockPos2.x >= NB_BLOCKS_X) // out of array
		return sfFalse;

	sfBool collide = sfFalse;


	if (_XAxis)
	{
		if (_UpOrLeft && blockPos.x > 0 && blockPos2.x > 0)
		{
			if (b[blockPos.y][blockPos.x - 1].isSolid)
			{
				if (_playerId >= 0) {
					switch (b[blockPos.y][blockPos.x - 1].type)
					{
					case T_DARK_COIN:
						BecomeDarkSky(blockPos.y, blockPos.x - 1);
						hud[playerTurn].score += 200;
						addCoin();
						break;
					case T_HIDDEN_BLOCK:
						break;
					default:
						return sfTrue;
						break;
					}
				}
				else if (b[blockPos.y][blockPos.x - 1].type != T_DARK_COIN)
					return sfTrue;
			}
			if (b[blockPos2.y][blockPos2.x - 1].isSolid)
			{
				if (_playerId >= 0) {
					switch (b[blockPos2.y][blockPos2.x - 1].type)
					{
					case T_DARK_COIN:
						BecomeDarkSky(blockPos2.y, blockPos2.x - 1);
						hud[playerTurn].score += 200;
						addCoin();
						break;
					case T_HIDDEN_BLOCK:
						break;
					default:
						return sfTrue;
						break;
					}
				}
				else if (b[blockPos2.y][blockPos2.x - 1].type != T_DARK_COIN)
					return sfTrue;
			}
			if (isBig && b[blockPos3.y][blockPos3.x - 1].isSolid)
			{
				switch (b[blockPos3.y][blockPos3.x - 1].type)
				{
				case T_DARK_COIN:
					BecomeDarkSky(blockPos3.y, blockPos3.x - 1);
					hud[playerTurn].score += 200;
					addCoin();
					break;
				case T_HIDDEN_BLOCK:
					break;
				default:
					return sfTrue;
					break;
				}
			}
		}
		else if (!_UpOrLeft && blockPos.x < NB_BLOCKS_X - 1 && blockPos2.x < NB_BLOCKS_X - 1)
		{
			if (b[blockPos.y][blockPos.x + 1].isSolid)
			{
				if (_playerId >= 0) {
					switch (b[blockPos.y][blockPos.x + 1].type)
					{
					case T_DARK_COIN:
						BecomeDarkSky(blockPos.y, blockPos.x + 1);
						hud[playerTurn].score += 200;
						addCoin();
						break;
					case T_BL_DARK_PIPE:
						setPlayerPossiblePipe(_playerId, sfTrue, sfFalse);
						return sfTrue;
						break;
					case T_UL_DARK_PIPE:
						if (isBig) {
							setPlayerPossiblePipe(_playerId, sfTrue, sfFalse);
							return sfTrue;
						}
						break;
					case T_HIDDEN_BLOCK:
						break;
					default:
						return sfTrue;
						break;
					}
				}
				else if (b[blockPos.y][blockPos.x + 1].type != T_DARK_COIN)
					return sfTrue;
			}
			if (b[blockPos2.y][blockPos2.x + 1].isSolid)
			{
				if (_playerId >= 0) {
					switch (b[blockPos2.y][blockPos2.x + 1].type)
					{
					case T_DARK_COIN:
						BecomeDarkSky(blockPos2.y, blockPos2.x + 1);
						hud[playerTurn].score += 200;
						addCoin();
						break;
					case T_BL_DARK_PIPE:
						setPlayerPossiblePipe(_playerId, sfTrue, sfFalse);
						return sfTrue;
						break;
					case T_HIDDEN_BLOCK:
						break;
					default:
						return sfTrue;
						break;
					}
				}
				else if (b[blockPos2.y][blockPos2.x + 1].type != T_DARK_COIN)
					return sfTrue;
			}
			if (isBig && b[blockPos3.y][blockPos3.x + 1].isSolid)
			{
				switch (b[blockPos3.y][blockPos3.x + 1].type)
				{
				case T_DARK_COIN:
					BecomeDarkSky(blockPos3.y, blockPos3.x + 1);
					hud[playerTurn].score += 200;
					addCoin();
					break;
				case T_BL_DARK_PIPE:
					setPlayerPossiblePipe(_playerId, sfTrue, sfFalse);
					return sfTrue;
					break;
				case T_HIDDEN_BLOCK:
					break;
				default:
					return sfTrue;
					break;
				}
			}
		}
	}
	else
	{
		if (_UpOrLeft && blockPos.y > 0 && blockPos2.y > 0)
		{
			sfBool isHittingSpecialBloc = sfFalse;
			if (b[blockPos.y - 1][blockPos.x].isSolid)
			{
				if (_playerId >= 0) {
					switch (b[blockPos.y - 1][blockPos.x].type)
					{
					case T_BLOCK:
						if (blockPos.y - 1 == 9 && blockPos.x == 94 && !isHittingSpecialBloc) {
							isHittingSpecialBloc = sfTrue;
							if (specialBlocTimer < 1.f)
								BecomeHitBlock(blockPos.y - 1, blockPos.x);
							else
								b[blockPos.y - 1][blockPos.x].timer = UP_BLOCK_TIMER;
							createItem(I_COIN, b[blockPos.y - 1][blockPos.x].pos);
							specialBlocTimer -= 1.f;
						}
						else if (blockPos.y - 1 == 9 && blockPos.x == 101) {
							BecomeHitBlock(blockPos.y - 1, blockPos.x);
							createItem(I_STAR, b[blockPos.y - 1][blockPos.x].pos);
						}
						else if (isBig && !isHittingSpecialBloc) {
							BecomeBlueSky(blockPos.y - 1, blockPos.x);
							hud[playerTurn].score += 50;
						}
						if (!isBig)
							b[blockPos.y - 1][blockPos.x].timer = UP_BLOCK_TIMER;
						collide = sfTrue;
						break;
					case T_QUESTION:
						BecomeHitBlock(blockPos.y - 1, blockPos.x);
						createItem(getCorrectItem(blockPos.x, blockPos.y - 1, isBig), b[blockPos.y - 1][blockPos.x].pos);
						b[blockPos.y - 1][blockPos.x].timer = UP_BLOCK_TIMER;
						collide = sfTrue;
						break;
					case T_DARK_COIN:
						BecomeDarkSky(blockPos.y - 1, blockPos.x);
						hud[playerTurn].score += 200;
						addCoin();
						break;
					case T_DARK_BLOCK:
						if (isBig) {
							BecomeDarkSky(blockPos.y - 1, blockPos.x);
							hud[playerTurn].score += 50;
						}
						if (!isBig)
							b[blockPos.y - 1][blockPos.x].timer = UP_BLOCK_TIMER;
						collide = sfTrue;
						break;
					case T_HIDDEN_BLOCK:
						BecomeHitBlock(blockPos.y - 1, blockPos.x);
						createItem(I_ONEUP, b[blockPos.y - 1][blockPos.x].pos);
						b[blockPos.y - 1][blockPos.x].timer = UP_BLOCK_TIMER;
						collide = sfTrue;
						break;
					default:
						collide = sfTrue;
						break;
					}
				}
				else if (b[blockPos.y - 1][blockPos.x].type != T_DARK_COIN)
					return sfTrue;
			}
			if (b[blockPos2.y - 1][blockPos2.x].isSolid)
			{
				if (_playerId >= 0) {
					switch (b[blockPos2.y - 1][blockPos2.x].type)
					{
					case T_BLOCK:
						if (blockPos2.y - 1 == 9 && blockPos2.x == 94 && !isHittingSpecialBloc) {
							isHittingSpecialBloc = sfTrue;
							if (specialBlocTimer < 1.f)
								BecomeHitBlock(blockPos2.y - 1, blockPos2.x);
							else
								b[blockPos2.y - 1][blockPos2.x].timer = UP_BLOCK_TIMER;
							createItem(I_COIN, b[blockPos2.y - 1][blockPos2.x].pos);
							specialBlocTimer -= 1.f;
						}
						else if (blockPos2.y - 1 == 9 && blockPos2.x == 101) {
							BecomeHitBlock(blockPos2.y - 1, blockPos2.x);
							createItem(I_STAR, b[blockPos2.y - 1][blockPos2.x].pos);
						}
						else if (isBig && !isHittingSpecialBloc) {
							BecomeBlueSky(blockPos2.y - 1, blockPos2.x);
							hud[playerTurn].score += 50;
						}
						if (!isBig)
							b[blockPos2.y - 1][blockPos2.x].timer = UP_BLOCK_TIMER;
						collide = sfTrue;
						break;
					case T_QUESTION:
						BecomeHitBlock(blockPos2.y - 1, blockPos2.x);
						createItem(getCorrectItem(blockPos2.x, blockPos2.y - 1, isBig), b[blockPos2.y - 1][blockPos2.x].pos);
						b[blockPos2.y - 1][blockPos2.x].timer = UP_BLOCK_TIMER;
						collide = sfTrue;
						break;
					case T_DARK_COIN:
						BecomeDarkSky(blockPos2.y - 1, blockPos2.x);
						hud[playerTurn].score += 200;
						addCoin();
						break;
					case T_DARK_BLOCK:
						if (isBig) {
							BecomeDarkSky(blockPos2.y - 1, blockPos2.x);
							hud[playerTurn].score += 50;
						}
						if (!isBig)
							b[blockPos2.y - 1][blockPos2.x].timer = UP_BLOCK_TIMER;
						collide = sfTrue;
						break;
					case T_HIDDEN_BLOCK:
						BecomeHitBlock(blockPos2.y - 1, blockPos2.x);
						createItem(I_ONEUP, b[blockPos2.y - 1][blockPos2.x].pos);
						b[blockPos2.y - 1][blockPos2.x].timer = UP_BLOCK_TIMER;
						collide = sfTrue;
						break;
					default:
						collide = sfTrue;
						break;
					}
				}
				else if (b[blockPos2.y - 1][blockPos2.x].type != T_DARK_COIN)
					return sfTrue;
			}
			if (b[blockPos3.y - 1][blockPos3.x].isSolid)
			{
				if (_playerId >= 0) {
					switch (b[blockPos3.y - 1][blockPos3.x].type)
					{
					case T_BLOCK:
						if (blockPos3.y - 1 == 9 && blockPos3.x == 94 && !isHittingSpecialBloc) {
							isHittingSpecialBloc = sfTrue;
							if (specialBlocTimer < 1.f)
								BecomeHitBlock(blockPos3.y - 1, blockPos3.x);
							else
								b[blockPos3.y - 1][blockPos3.x].timer = UP_BLOCK_TIMER;
							createItem(I_COIN, b[blockPos3.y - 1][blockPos3.x].pos);
							specialBlocTimer -= 1.f;
						}
						else if (blockPos3.y - 1 == 9 && blockPos3.x == 101) {
							BecomeHitBlock(blockPos3.y - 1, blockPos3.x);
							createItem(I_STAR, b[blockPos3.y - 1][blockPos3.x].pos);
						}
						else if (isBig && !isHittingSpecialBloc) {
							BecomeBlueSky(blockPos3.y - 1, blockPos3.x);
							hud[playerTurn].score += 50;
						}
						if (!isBig)
							b[blockPos3.y - 1][blockPos3.x].timer = UP_BLOCK_TIMER;
						collide = sfTrue;
						break;
					case T_QUESTION:
						BecomeHitBlock(blockPos3.y - 1, blockPos3.x);
						createItem(getCorrectItem(blockPos3.x, blockPos3.y - 1, isBig), b[blockPos3.y - 1][blockPos3.x].pos);
						b[blockPos3.y - 1][blockPos3.x].timer = UP_BLOCK_TIMER;
						collide = sfTrue;
						break;
					case T_DARK_COIN:
						BecomeDarkSky(blockPos3.y - 1, blockPos3.x);
						hud[playerTurn].score += 200;
						addCoin();
						break;
					case T_DARK_BLOCK:
						if (isBig) {
							BecomeDarkSky(blockPos3.y - 1, blockPos3.x);
							hud[playerTurn].score += 50;
						}
						if (!isBig)
							b[blockPos3.y - 1][blockPos3.x].timer = UP_BLOCK_TIMER;
						collide = sfTrue;
						break;
					case T_HIDDEN_BLOCK:
						BecomeHitBlock(blockPos3.y - 1, blockPos3.x);
						createItem(I_ONEUP, b[blockPos3.y - 1][blockPos3.x].pos);
						b[blockPos3.y - 1][blockPos3.x].timer = UP_BLOCK_TIMER;
						collide = sfTrue;
						break;
					default:
						return sfTrue;
						break;
					}
				}
				else if (b[blockPos3.y - 1][blockPos3.x].type != T_DARK_COIN)
					return sfTrue;
			}


			if (collide)
				return sfTrue;
		}
		else if (!_UpOrLeft && blockPos.y < NB_BLOCKS_Y - 1 && blockPos2.y < NB_BLOCKS_Y - 1)
		{
			sfBool leftPipe = sfFalse;
			if (b[blockPos.y + 1][blockPos.x].isSolid)
			{
				if (_playerId >= 0) {
					switch (b[blockPos.y + 1][blockPos.x].type)
					{
					case T_DARK_COIN:
						BecomeDarkSky(blockPos.y + 1, blockPos.x);
						hud[playerTurn].score += 200;
						addCoin();
						break;
					case T_UL_PIPE:
						leftPipe = sfTrue;
						collide = sfTrue;
						break;
					case T_HIDDEN_BLOCK:
						break;
					default:
						return sfTrue;
						break;
					}
				}
				else if (b[blockPos.y + 1][blockPos.x].type != T_DARK_COIN)
					return sfTrue;
			}
			if (b[blockPos2.y + 1][blockPos2.x].isSolid)
			{
				if (_playerId >= 0) {
					switch (b[blockPos2.y + 1][blockPos2.x].type)
					{
					case T_DARK_COIN:
						BecomeDarkSky(blockPos2.y + 1, blockPos2.x);
						hud[playerTurn].score += 200;
						addCoin();
						break;
					case T_UR_PIPE:
						if (leftPipe)
							setPlayerPossiblePipe(_playerId, sfTrue, sfTrue);
						return sfTrue;
						break;
					case T_HIDDEN_BLOCK:
						break;
					default:
						return sfTrue;
						break;
					}
				}
				else if (b[blockPos2.y + 1][blockPos2.x].type != T_DARK_COIN)
					return sfTrue;
			}
			if (b[blockPos3.y + 1][blockPos3.x].isSolid)
			{
				if (_playerId >= 0) {
					switch (b[blockPos3.y + 1][blockPos3.x].type)
					{
					case T_DARK_COIN:
						BecomeDarkSky(blockPos3.y + 1, blockPos3.x);
						hud[playerTurn].score += 200;
						addCoin();
						break;
					case T_UR_PIPE:
						if (leftPipe)
							setPlayerPossiblePipe(_playerId, sfTrue, sfTrue);
						return sfTrue;
						break;
					case T_HIDDEN_BLOCK:
						break;
					default:
						return sfTrue;
						break;
					}
				}
				else if (b[blockPos3.y + 1][blockPos3.x].type != T_DARK_COIN)
					return sfTrue;
			}

			if (collide)
				return sfTrue;
		}
	}

	return sfFalse;
}

sfBool isCollision3(sfFloatRect _rect, sfVector2f* _velocity, int _playerId)
{
	if (_velocity->x > EPSILON)
	{
		if (isCollision2(_rect, sfTrue, sfFalse, *_velocity, _playerId))
		{
			_velocity->x = 0.f;
			return sfTrue;
		}
	}
	if (_velocity->x < -EPSILON)
	{
		if (isCollision2(_rect, sfTrue, sfTrue, *_velocity, _playerId))
		{
			_velocity->x = 0.f;
			return sfTrue;
		}
	}
	if (_velocity->y > EPSILON)
	{
		if (isCollision2(_rect, sfFalse, sfFalse, *_velocity, _playerId))
		{
			_velocity->y = 0.f;
			return sfTrue;
		}
	}
	if (_velocity->y < -EPSILON)
	{
		if (isCollision2(_rect, sfFalse, sfTrue, *_velocity, _playerId))
		{
			_velocity->y = 0.f;
			return sfTrue;
		}
	}
	return sfFalse;
}


sfBool isMapFinished()
{
	return isMapF;
}

void setMapFinishedState(sfBool _isMapF)
{
	isMapF = _isMapF;
}

void deinitMap()
{
	sfSprite_destroy(mapSprite);
}