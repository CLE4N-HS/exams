#include "map.h"
#include "editor.h"
#include "CustomMath.h"
#include "gamepadx.h"
#include "soundManager.h"
#include "items.h"

sfSprite* mapSprite;

sfTexture* tilesetTexture;

float changeMapTimer;

sfBool isMapF;

void initMap()
{
	mapSprite = sfSprite_create();

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



	changeMapTimer += dt;

	int key = 0;
	if (sfKeyboard_isKeyPressed(sfKeyNumpad1)) key = 1;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad2)) key = 2;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad3)) key = 3;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad4)) key = 4;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad5)) key = 5;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad6)) key = 6;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad7)) key = 7;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad8)) key = 8;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad9)) key = 9;

	if (key > 0 && changeMapTimer > 0.5f && !isMapF) {
		changeMapTimer = 0.f;
		loadMap(key);
		nbMap = key;
		setMapFinishedState(sfFalse);
	}
}

void displayMap(Window* _window)
{
	for (int j = 0; j < NB_BLOCKS_Y; j++)
	{
		for (int i = 0; i < NB_BLOCKS_X; i++)
		{
			sfSprite_setPosition(mapSprite, b[j][i].pos);
			sfSprite_setScale(mapSprite, vector2f(BLOCK_SCALE, BLOCK_SCALE));
			sfSprite_setColor(mapSprite, b[j][i].color);

			blockType tmpType = b[j][i].type;

			sfSprite_setTexture(mapSprite, tilesetTexture, sfFalse);

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
}

sfVector2i getPlayerBlockPos(sfVector2f _pos)
{
	sfVector2f fPos = MultiplyVector(_pos, 1.f / BLOCK_SIZE / BLOCK_SCALE);
	sfVector2i iPos = vector2i(0, 0);
	iPos.x = (int)fPos.x;
	iPos.y = (int)fPos.y;
	return iPos;
}

sfBool isGrounded(sfVector2f _pos, sfVector2f* _velocity, sfVector2f _origin, sfFloatRect _bounds)
{
	//sfVector2i blockPos = getPlayerBlockPos(vector2f(_pos.x - (_origin.x - 8.f) * BLOCK_SCALE, _pos.y + 1.f * BLOCK_SCALE - (_origin.y - 16.f) * BLOCK_SCALE)); // offset to not count the alpha 0
	//sfVector2i blockPos2 = getPlayerBlockPos(vector2f(_pos.x + (_origin.x - 8.f) * BLOCK_SCALE, _pos.y + 1.f * BLOCK_SCALE - (_origin.y - 16.f) * BLOCK_SCALE));

	sfVector2i blockPos = getPlayerBlockPos(vector2f(_pos.x - (_origin.x - 8.f) * BLOCK_SCALE, _pos.y + 1.f * BLOCK_SCALE - (_origin.y - _bounds.height / BLOCK_SCALE) * BLOCK_SCALE)); // offset to not count the alpha 0
	sfVector2i blockPos2 = getPlayerBlockPos(vector2f(_pos.x + (_origin.x - 8.f) * BLOCK_SCALE, _pos.y + 1.f * BLOCK_SCALE - (_origin.y - _bounds.height / BLOCK_SCALE) * BLOCK_SCALE));

	//sfVector2i blockPos = getPlayerBlockPos(vector2f(_pos.x - (_origin.x - 8.f) * BLOCK_SCALE * 2.f * BLOCK_SCALE, _pos.y + 1.f * BLOCK_SCALE - (_origin.y - 16.f) * BLOCK_SCALE)); // offset to not count the alpha 0
	//sfVector2i blockPos2 = getPlayerBlockPos(vector2f(_pos.x + (_origin.x - 8.f) * BLOCK_SCALE * 2.f * BLOCK_SCALE, _pos.y + 1.f * BLOCK_SCALE - (_origin.y - 16.f) * BLOCK_SCALE));
	
	//sfVector2i blockPos = getPlayerBlockPos(vector2f(_bounds.left, _bounds.top + _bounds.height / 2.f));
	//sfVector2i blockPos2 = getPlayerBlockPos(vector2f(_bounds.left + _bounds.width, _bounds.top + _bounds.height / 2.f));

	if (blockPos.y < 0 || blockPos.y >= NB_BLOCKS_Y || blockPos.x < 0 || blockPos.x >= NB_BLOCKS_X || blockPos2.y < 0 || blockPos2.y >= NB_BLOCKS_Y || blockPos2.x < 0 || blockPos2.x >= NB_BLOCKS_X) // out of array
		return sfFalse;

	tmpRect4 = FlRect(b[blockPos.y][blockPos.x].pos.x, b[blockPos.y][blockPos.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
	tmpRect5 = FlRect(b[blockPos2.y][blockPos2.x].pos.x, b[blockPos2.y][blockPos2.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);

	if (_velocity->y < -0.f) // prolly flying
		return sfFalse;


	if (b[blockPos.y][blockPos.x].isSolid || b[blockPos2.y][blockPos2.x].isSolid) {
		return sfTrue;
		_bounds.height += _velocity->y;
		sfFloatRect tmRect = FlRect(b[blockPos.y][blockPos.x].pos.x, b[blockPos.y][blockPos.x].pos.y, BLOCK_SCALE * BLOCK_SIZE, BLOCK_SCALE * BLOCK_SIZE);
		sfFloatRect tmppRect = FlRect(b[blockPos2.y][blockPos2.x].pos.x, b[blockPos2.y][blockPos2.x].pos.y, BLOCK_SCALE * BLOCK_SIZE, BLOCK_SCALE * BLOCK_SIZE);

		if (sfFloatRect_intersects(&_bounds, &tmRect, NULL) || sfFloatRect_intersects(&_bounds, &tmppRect, NULL)) {
			_velocity->y = 0.f;
			return sfTrue;
		}
	}

	return sfFalse;


}

sfBool isCollision2(sfFloatRect _rect, sfBool _XAxis, sfBool _UpOrLeft, sfVector2f _nextVelocity)
{
	float dt = getDeltaTime();
	float offset = 1.f;
	sfVector2f playerPos = VECTOR2F_NULL;
	sfVector2f playerPos2 = VECTOR2F_NULL;
	if (_XAxis) {
		if (_UpOrLeft) {
			playerPos = AddVectors(vector2f(_rect.left, _rect.top + BLOCK_SCALE), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
			playerPos2 = AddVectors(vector2f(_rect.left, _rect.top + _rect.height - BLOCK_SCALE * 2.f), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
			playerPos.x += BLOCK_SCALE * BLOCK_SIZE;
			playerPos2.x += BLOCK_SCALE * BLOCK_SIZE;
			//_rect.left += _nextVelocity.x * BLOCK_SCALE * offset;
		}
		else {
			playerPos = AddVectors(vector2f(_rect.left + _rect.width, _rect.top + BLOCK_SCALE), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
			playerPos2 = AddVectors(vector2f(_rect.left + _rect.width, _rect.top + _rect.height - BLOCK_SCALE * 2.f), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
			playerPos.x -= BLOCK_SCALE * BLOCK_SIZE;
			playerPos2.x -= BLOCK_SCALE * BLOCK_SIZE;
			//_rect.width += _nextVelocity.x * BLOCK_SCALE * offset;
		}
	}
	else {
		if (_UpOrLeft) {
			playerPos = AddVectors(vector2f(_rect.left, _rect.top), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
			playerPos2 = AddVectors(vector2f(_rect.left + _rect.width, _rect.top), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
			playerPos.y += BLOCK_SCALE * BLOCK_SIZE;
			playerPos2.y += BLOCK_SCALE * BLOCK_SIZE;
			//_rect.top += _nextVelocity.y * BLOCK_SCALE * dt * offset;
			//_rect.top -= BLOCK_SCALE * offset;
			//_rect.left += BLOCK_SCALE * offset;
			//_rect.width -= BLOCK_SCALE * 6.f;
		}
		else {
			playerPos = AddVectors(vector2f(_rect.left, _rect.top + _rect.height), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
			playerPos2 = AddVectors(vector2f(_rect.left + _rect.width, _rect.top + _rect.height), MultiplyVector(_nextVelocity, BLOCK_SCALE * dt * offset));
			playerPos.y -= BLOCK_SCALE * BLOCK_SIZE;
			playerPos2.y -= BLOCK_SCALE * BLOCK_SIZE;
			//_rect.height += _nextVelocity.y;
			//_rect.height -= BLOCK_SCALE;
		}
	}

	tmpRect3 = _rect;

	sfVector2i blockPos = getPlayerBlockPos(playerPos);
	sfVector2i blockPos2 = getPlayerBlockPos(playerPos2);


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
				return sfTrue;
				////sfFloatRect blockRect = FlRect(b[blockPos.y][blockPos.x - 1].pos.x, b[blockPos.y][blockPos.x - 1].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				////if (sfFloatRect_intersects(&_rect, &blockRect, NULL))
				//{
				//		return sfTrue;
				//}
			}
			if (b[blockPos2.y][blockPos2.x - 1].isSolid)
			{
				return sfTrue;
				////sfFloatRect blockRect2 = FlRect(b[blockPos2.y][blockPos2.x - 1].pos.x, b[blockPos2.y][blockPos2.x - 1].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				////if (sfFloatRect_intersects(&_rect, &blockRect2, NULL))
				//{
				//		return sfTrue;
				//}
			}
		}
		else if (!_UpOrLeft && blockPos.x < NB_BLOCKS_X - 1 && blockPos2.x < NB_BLOCKS_X - 1)
		{
			if (b[blockPos.y][blockPos.x + 1].isSolid)
			{
				return sfTrue;
				////sfFloatRect blockRect = FlRect(b[blockPos.y][blockPos.x + 1].pos.x, b[blockPos.y][blockPos.x + 1].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				////tmpRect = blockRect;
				////if (sfFloatRect_intersects(&_rect, &blockRect, NULL))
				//{
				//		return sfTrue;
				//}
			}
			if (b[blockPos2.y][blockPos2.x + 1].isSolid)
			{
				return sfTrue;
				////sfFloatRect blockRect2 = FlRect(b[blockPos2.y][blockPos2.x + 1].pos.x, b[blockPos2.y][blockPos2.x + 1].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				////tmpRect2 = blockRect2;
				////if (sfFloatRect_intersects(&_rect, &blockRect2, NULL))
				//{
				//		return sfTrue;
				//}
			}
		}
	}
	else
	{
		if (_UpOrLeft && blockPos.y > 0 && blockPos2.y > 0)
		{
			//tmpRect = FlRect(b[blockPos.y - 1][blockPos.x].pos.x, b[blockPos.y - 1][blockPos.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
			if (b[blockPos.y - 1][blockPos.x].isSolid)
			{
				switch (b[blockPos.y - 1][blockPos.x].type)
				{
				case T_QUESTION:
					if (b[blockPos.y - 1][blockPos.x].color.r > 200) {
						b[blockPos.y - 1][blockPos.x].color = color(100, 100, 100, 255);
						int random = rand() % 5;
						createItem(random, b[blockPos.y - 1][blockPos.x].pos);
					}
					break;
				default:
					break;
				}
				collide = sfTrue;
				////sfFloatRect blockRect = FlRect(b[blockPos.y - 1][blockPos.x].pos.x, b[blockPos.y - 1][blockPos.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				////if (sfFloatRect_intersects(&_rect, &blockRect, NULL))
				//{
				//		return sfTrue;
				//}
			}
			//tmpRect2 = FlRect(b[blockPos2.y - 1][blockPos2.x].pos.x, b[blockPos2.y - 1][blockPos2.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
			if (b[blockPos2.y - 1][blockPos2.x].isSolid)
			{
				switch (b[blockPos2.y - 1][blockPos2.x].type)
				{
				case T_QUESTION:
					if (b[blockPos2.y - 1][blockPos2.x].color.r > 200) {
						b[blockPos2.y - 1][blockPos2.x].color = color(100, 100, 100, 255);
						int random = rand() % 5;
						createItem(random, b[blockPos2.y - 1][blockPos2.x].pos);
					}
					break;
				default:
					break;
				}
				return sfTrue;
				////sfFloatRect blockRect2 = FlRect(b[blockPos2.y - 1][blockPos2.x].pos.x, b[blockPos2.y - 1][blockPos2.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				////if (sfFloatRect_intersects(&_rect, &blockRect2, NULL))
				//{
				//
				//		return sfTrue;
				//}
			}

			if (collide)
				return sfTrue;
		}
		else if (!_UpOrLeft && blockPos.y < NB_BLOCKS_Y - 1 && blockPos2.y < NB_BLOCKS_Y - 1)
		{
			//return sfFalse;
			_rect.height += _nextVelocity.y * BLOCK_SCALE;
			if (b[blockPos.y + 1][blockPos.x].isSolid)
			{
				return sfTrue;
				sfFloatRect blockRect = FlRect(b[blockPos.y + 1][blockPos.x].pos.x, b[blockPos.y + 1][blockPos.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				if (sfFloatRect_intersects(&_rect, &blockRect, NULL))
				{
						return sfTrue;
				}
			}
			if (b[blockPos2.y + 1][blockPos2.x].isSolid)
			{
				return sfTrue;
				sfFloatRect blockRect2 = FlRect(b[blockPos2.y + 1][blockPos2.x].pos.x, b[blockPos2.y + 1][blockPos2.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				if (sfFloatRect_intersects(&_rect, &blockRect2, NULL))
				{
						return sfTrue;
				}
			}
		}
	}

	return sfFalse;
}

sfBool isCollision3(sfFloatRect _rect, sfVector2f* _velocity)
{
	if (_velocity->x > EPSILON)
	{
		if (isCollision2(_rect, sfTrue, sfFalse, *_velocity))
		{
			_velocity->x = 0.f;
			return sfTrue;
		}
	}
	if (_velocity->x < -EPSILON)
	{
		if (isCollision2(_rect, sfTrue, sfTrue, *_velocity))
		{
			_velocity->x = 0.f;
			return sfTrue;
		}
	}
	if (_velocity->y > EPSILON)
	{
		if (isCollision2(_rect, sfFalse, sfFalse, *_velocity))
		{
			_velocity->y = 0.f;
			return sfTrue;
		}
	}
	if (_velocity->y < -EPSILON)
	{
		if (isCollision2(_rect, sfFalse, sfTrue, *_velocity))
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