#include "items.h"
#include "textureManager.h"
#include "List.h"
#include "CustomMath.h"
#include "map.h"
#include "player.h"

#define GD_ITEM STD_LIST_GETDATA(itemList, Items, i)
#define GD_ITEMJ STD_LIST_GETDATA(itemList, Items, j)

#define MUSHROOM_YSPEED 500.f
#define STAR_YSPEED 500.f

typedef struct Items {
	ItemType type;
	sfVector2f pos;
	sfVector2f origin;
	sfVector2f velocity;
	sfVector2f scale;
	sfIntRect rect;
	float timer;
	sfBool isMovingLeft;
	sfFloatRect bounds;
}Items;

sfSprite* itemSprite;

stdList* itemList;

void addItem(ItemType _type, sfVector2f _pos, sfVector2f _origin, sfVector2f _velocity, sfVector2f _scale, sfIntRect _rect, float _timer, sfBool _isMovingLeft, sfFloatRect _bounds)
{
	Items tmp;
	tmp.type = _type;
	tmp.pos = _pos;
	tmp.origin = _origin;
	tmp.velocity = _velocity;
	tmp.scale = _scale;
	tmp.rect = _rect;
	tmp.timer = _timer;
	tmp.isMovingLeft = _isMovingLeft;
	tmp.bounds = _bounds;

	STD_LIST_PUSHBACK(itemList, tmp);
}

void initItems()
{
	itemSprite = sfSprite_create();
	sfSprite_setTexture(itemSprite, GetTexture("items"), sfFalse);

	itemList = STD_LIST_CREATE(Items, 0);
}

void createItem(ItemType _type, sfVector2f _pos)
{
	sfIntRect rect = IntRect(0, 0, 0, 0);
	sfVector2f velocity = VECTOR2F_NULL;
	sfBool isMovingLeft = sfTrue;
	sfVector2f origin = vector2f(8.f, 16.f);

	switch (_type)
	{
	case I_MUSHROOM:
		rect = IntRect(0, 0, 16, 16);
		velocity = vector2f(200.f, MUSHROOM_YSPEED);
		isMovingLeft = sfFalse;
		break;
	case I_FIREFLOWER:
		rect = IntRect(0, 16, 16, 16);
		velocity = VECTOR2F_NULL;
		break;
	case I_ONEUP:
		rect = IntRect(0, 32, 16, 16);
		velocity = vector2f(200.f, MUSHROOM_YSPEED);
		isMovingLeft = sfFalse;
		break;
	case I_STAR:
		rect = IntRect(0, 48, 16, 16);
		velocity = vector2f(200.f, -STAR_YSPEED);
		isMovingLeft = sfFalse;
		break;
	case I_COIN:
		rect = IntRect(0, 64, 16, 16);
		velocity = vector2f(0.f, -270.f);
		isMovingLeft = sfFalse;
		addCoin();
		break;
	default:
		break;
	}

	addItem(_type, AddVectors(_pos, MultiplyVector(origin, BLOCK_SCALE)), origin, velocity, vector2f(BLOCK_SCALE, BLOCK_SCALE), rect, 0.f, isMovingLeft, FlRect(0.f, 0.f, 0.f, 0.f));
}

ItemType getCorrectItem(int _x, int _y, sfBool _isBig)
{
	if (_x == 21 && _y == 9) {
		if (_isBig) return I_FIREFLOWER;
		else return I_MUSHROOM;
	}
	if (_x == 78 && _y == 9) {
		if (_isBig) return I_FIREFLOWER;
		else return I_MUSHROOM;
	}
	if (_x == 109 && _y == 5) {
		if (_isBig) return I_FIREFLOWER;
		else return I_MUSHROOM;
	}

	return I_COIN;
}

void updateItem(Window* _window)
{
	float dt = getDeltaTime();

	for (int i = 0; i < itemList->size(itemList); i++)
	{
		switch (GD_ITEM->type)
		{
		case I_MUSHROOM:
			GD_ITEM->timer += dt;

			if (GD_ITEM->timer < 1.f) {
				GD_ITEM->pos.y -= 100.f * dt;
			}
			else {
				//if (isGrounded(GD_ITEM->pos, &GD_ITEM->velocity, GD_ITEM->origin))
				//if (isCollision2(FlRect(GD_ITEM->pos.x, GD_ITEM->pos.y - BLOCK_SCALE * BLOCK_SIZE, GD_ITEM->rect.width * BLOCK_SCALE, GD_ITEM->rect.height * BLOCK_SCALE), sfFalse, sfFalse, vector2f(0.f, 0.f)))
				if (isCollision2(GD_ITEM->bounds, sfFalse, sfFalse, vector2f(0.f, 0.f), -1))
					GD_ITEM->velocity.y = 0.f;
				else
					GD_ITEM->velocity.y = MUSHROOM_YSPEED;

				if (GD_ITEM->velocity.x < 0.f) {
					if (isCollision2(GD_ITEM->bounds, sfTrue, sfTrue, vector2f(-200.f, 0.f), -1)) {
						GD_ITEM->velocity.x = 200.f;
					}
				}
				else {
					if (isCollision2(GD_ITEM->bounds, sfTrue, sfFalse, vector2f(200.f, 0.f), -1)) {
						GD_ITEM->velocity.x = -200.f;
					}
				}

				GD_ITEM->pos = AddVectors(GD_ITEM->pos, MultiplyVector(GD_ITEM->velocity, dt));


				sfBool shouldContinue = sfFalse;
				for (int j = 0; j < 2; j++)
				{
					if (sfFloatRect_intersects(pgetPlayerBounds(j), &GD_ITEM->bounds, NULL)) {
						itemList->erase(&itemList, i);
						setPlayerPower(j, P_BIG);
						shouldContinue = sfTrue;
						break;
					}
				}
				if (shouldContinue)
					continue;
			}
			break;
		case I_FIREFLOWER:
			GD_ITEM->timer += dt;

			if (GD_ITEM->timer < 1.f) {
				GD_ITEM->pos.y -= 100.f * dt;
			}
			else {
				if (isCollision2(GD_ITEM->bounds, sfFalse, sfFalse, vector2f(0.f, 0.f), -1))
					GD_ITEM->velocity.y = 0.f;
				else
					GD_ITEM->velocity.y = MUSHROOM_YSPEED;

				GD_ITEM->pos = AddVectors(GD_ITEM->pos, MultiplyVector(GD_ITEM->velocity, dt));


				sfBool shouldContinue = sfFalse;
				for (int j = 0; j < 2; j++)
				{
					if (sfFloatRect_intersects(pgetPlayerBounds(j), &GD_ITEM->bounds, NULL)) {
						itemList->erase(&itemList, i);
						setPlayerPower(j, P_FIRETHROWER);
						shouldContinue = sfTrue;
						break;
					}
				}
				if (shouldContinue)
					continue;
			}
			break;
		case I_ONEUP:
			GD_ITEM->timer += dt;

			if (GD_ITEM->timer < 1.f) {
				GD_ITEM->pos.y -= 100.f * dt;
			}
			else {
				if (isCollision2(GD_ITEM->bounds, sfFalse, sfFalse, vector2f(0.f, 0.f), -1))
					GD_ITEM->velocity.y = 0.f;
				else
					GD_ITEM->velocity.y = MUSHROOM_YSPEED;

				if (GD_ITEM->velocity.x < 0.f) {
					if (isCollision2(GD_ITEM->bounds, sfTrue, sfTrue, vector2f(-200.f, 0.f), -1)) {
						GD_ITEM->velocity.x = 200.f;
					}
				}
				else {
					if (isCollision2(GD_ITEM->bounds, sfTrue, sfFalse, vector2f(200.f, 0.f), -1)) {
						GD_ITEM->velocity.x = -200.f;
					}
				}

				GD_ITEM->pos = AddVectors(GD_ITEM->pos, MultiplyVector(GD_ITEM->velocity, dt));


				sfBool shouldContinue = sfFalse;
				for (int j = 0; j < 2; j++)
				{
					if (sfFloatRect_intersects(pgetPlayerBounds(j), &GD_ITEM->bounds, NULL)) {
						itemList->erase(&itemList, i);
						// oneUp
						shouldContinue = sfTrue;
						break;
					}
				}
				if (shouldContinue)
					continue;
			}
			break;
		case I_STAR:
			GD_ITEM->timer += dt;

			if (GD_ITEM->timer < 1.f) {
				GD_ITEM->pos.y -= 100.f * dt;
			}
			else {

				if (GD_ITEM->velocity.y < STAR_YSPEED) {
					GD_ITEM->velocity.y += 1000.f * dt;
				}


				if (isCollision2(GD_ITEM->bounds, sfFalse, sfFalse, vector2f(0.f, 0.f), -1))
					GD_ITEM->velocity.y = -STAR_YSPEED;

				if (isCollision2(GD_ITEM->bounds, sfFalse, sfTrue, vector2f(0.f, 0.f), -1))
					GD_ITEM->velocity.y = STAR_YSPEED;

				if (GD_ITEM->velocity.x < 0.f) {
					if (isCollision2(GD_ITEM->bounds, sfTrue, sfTrue, vector2f(-200.f, 0.f), -1)) {
						GD_ITEM->velocity.x = 200.f;
					}
				}
				else {
					if (isCollision2(GD_ITEM->bounds, sfTrue, sfFalse, vector2f(200.f, 0.f), -1)) {
						GD_ITEM->velocity.x = -200.f;
					}
				}

				GD_ITEM->pos = AddVectors(GD_ITEM->pos, MultiplyVector(GD_ITEM->velocity, dt));


				sfBool shouldContinue = sfFalse;
				for (int j = 0; j < 2; j++)
				{
					if (sfFloatRect_intersects(pgetPlayerBounds(j), &GD_ITEM->bounds, NULL)) {
						itemList->erase(&itemList, i);
						setPlayerStarPower(j);
						shouldContinue = sfTrue;
						break;
					}
				}
				if (shouldContinue)
					continue;
			}
			break;
		case I_COIN:
			GD_ITEM->timer += dt * 6.f;

			GD_ITEM->pos.y -= sin(GD_ITEM->timer) * 600.f * dt;

			if (GD_ITEM->timer > 5.f) {
				itemList->erase(&itemList, i);
				continue;
			}

			break;
		default:
			break;
		}

		sfVector2f viewPos = getGreatestViewPos();
		// out of view
		if (GD_ITEM->pos.x < viewPos.x - 960.f - 8.f * GD_ITEM->scale.x || GD_ITEM->pos.y > 1080.f + 8.f * GD_ITEM->scale.y) {
			itemList->erase(&itemList, i);
			continue;
		}
		


	}
}

void displayItem(Window* _window)
{
	for (int i = 0; i < itemList->size(itemList); i++)
	{
		sfSprite_setTextureRect(itemSprite, GD_ITEM->rect);
		sfSprite_setOrigin(itemSprite, GD_ITEM->origin);
		sfSprite_setPosition(itemSprite, GD_ITEM->pos);
		sfSprite_setScale(itemSprite, GD_ITEM->scale);
		sfRenderTexture_drawSprite(_window->renderTexture, itemSprite, NULL);

		GD_ITEM->bounds = sfSprite_getGlobalBounds(itemSprite);
	}
}
