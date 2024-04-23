#include "items.h"
#include "textureManager.h"
#include "List.h"
#include "CustomMath.h"
#include "map.h"
#include "player.h"

#define GD_ITEM STD_LIST_GETDATA(itemList, Items, i)
#define GD_ITEMJ STD_LIST_GETDATA(itemList, Items, j)

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
		velocity = vector2f(200.f, 270.f);
		isMovingLeft = sfFalse;
		break;
	case I_FIREFLOWER:
		rect = IntRect(16, 0, 16, 16);
		velocity = VECTOR2F_NULL;
		break;
	default:
		break;
	}

	addItem(_type, AddVectors(_pos, MultiplyVector(origin, BLOCK_SCALE)), origin, velocity, vector2f(BLOCK_SCALE, BLOCK_SCALE), rect, 0.f, isMovingLeft, FlRect(0.f, 0.f, 0.f, 0.f));
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
				if (isCollision2(GD_ITEM->bounds, sfFalse, sfFalse, vector2f(0.f, 0.f)))
					GD_ITEM->velocity.y = 0.f;
				else
					GD_ITEM->velocity.y = 270.f;

				if (GD_ITEM->velocity.x < 0.f) {
					if (isCollision2(GD_ITEM->bounds, sfTrue, sfTrue, vector2f(-200.f, 0.f))) {
						GD_ITEM->velocity.x = 200.f;
					}
				}
				else {
					if (isCollision2(GD_ITEM->bounds, sfTrue, sfFalse, vector2f(200.f, 0.f))) {
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
				if (isCollision2(GD_ITEM->bounds, sfFalse, sfFalse, vector2f(0.f, 0.f)))
					GD_ITEM->velocity.y = 0.f;
				else
					GD_ITEM->velocity.y = 270.f;

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
		default:
			break;
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
