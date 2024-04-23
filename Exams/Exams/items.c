#include "items.h"
#include "textureManager.h"
#include "List.h"
#include "CustomMath.h"
#include "map.h"

#define GD_ITEM STD_LIST_GETDATA(itemList, Items, i)

typedef struct Items {
	ItemType type;
	sfVector2f pos;
	sfVector2f velocity;
	sfVector2f scale;
	sfIntRect rect;
	float timer;
	sfBool isMovingLeft;
}Items;

sfSprite* itemSprite;

stdList* itemList;

void addItem(ItemType _type, sfVector2f _pos, sfVector2f _velocity, sfVector2f _scale, sfIntRect _rect, float _timer, sfBool _isMovingLeft)
{
	Items tmp;
	tmp.type = _type;
	tmp.pos = _pos;
	tmp.velocity = _velocity;
	tmp.scale = _scale;
	tmp.rect = _rect;
	tmp.timer = _timer;
	tmp.isMovingLeft = _isMovingLeft;

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

	switch (_type)
	{
	case I_MUSHROOM:
		rect = IntRect(0, 0, 16, 16);
		velocity = vector2f(200.f, 270.f);
		isMovingLeft = sfFalse;
		break;
	default:
		break;
	}

	addItem(_type, _pos, velocity, vector2f(BLOCK_SCALE, BLOCK_SCALE), rect, 0.f, isMovingLeft);
}

void updateItem(Window* _window)
{
	float dt = getDeltaTime();

	for (int i = 0; i < itemList->size(itemList); i++)
	{
		GD_ITEM->timer += dt;

		if (GD_ITEM->timer < 1.f) {
			GD_ITEM->pos.y -= 100.f * dt;
		}
		else {
			if (isGrounded(GD_ITEM->pos, &GD_ITEM->velocity, VECTOR2F_NULL))
				GD_ITEM->velocity.y = 0.f;
			else
				GD_ITEM->velocity.y = 270.f;

			GD_ITEM->pos = AddVectors(GD_ITEM->pos, MultiplyVector(GD_ITEM->velocity, dt));
		}
	}
}

void displayItem(Window* _window)
{
	for (int i = 0; i < itemList->size(itemList); i++)
	{
		sfSprite_setTextureRect(itemSprite, GD_ITEM->rect);
		sfSprite_setPosition(itemSprite, GD_ITEM->pos);
		sfSprite_setScale(itemSprite, GD_ITEM->scale);
		sfRenderTexture_drawSprite(_window->renderTexture, itemSprite, NULL);
	}
}
