#include "items.h"
#include "textureManager.h"
#include "List.h"
#include "CustomMath.h"
#include "map.h"
#include "player.h"
#include "fireballs.h"

#define GD_FIREBALL STD_LIST_GETDATA(fireballList, Fireballs, i)

typedef struct Fireballs {
	sfVector2f pos;
	sfVector2f origin;
	sfVector2f velocity;
	sfVector2f scale;
	sfIntRect rect;
	float timer;
	sfBool isMovingLeft;
	sfFloatRect bounds;
}Fireballs;

sfSprite* fireballSprite;

stdList* fireballList;

void addFireball(sfVector2f _pos, sfVector2f _origin, sfVector2f _velocity, sfVector2f _scale, sfIntRect _rect, float _timer, sfBool _isMovingLeft, sfFloatRect _bounds)
{
	Fireballs tmp;
	tmp.pos = _pos;
	tmp.origin = _origin;
	tmp.velocity = _velocity;
	tmp.scale = _scale;
	tmp.rect = _rect;
	tmp.timer = _timer;
	tmp.isMovingLeft = _isMovingLeft;
	tmp.bounds = _bounds;

	STD_LIST_PUSHBACK(fireballList, tmp);
}

void initFireballs()
{
	fireballSprite = sfSprite_create();
	sfSprite_setTexture(fireballSprite, GetTexture("fireball"), sfFalse);

	fireballList = STD_LIST_CREATE(Fireballs, 0);
}

void createFireball(sfVector2f _pos, sfBool _leftSide)
{
	sfIntRect rect = IntRect(0, 0, 16, 16);
	sfVector2f pos = vector2f(8.f * BLOCK_SCALE, -20.f * BLOCK_SCALE);
	sfVector2f velocity = vector2f(800.f, 250.f);
	if (_leftSide) {
		pos.x *= -1.f;
		velocity.x *= -1.f;
	}
	sfBool isMovingLeft = _leftSide;
	sfVector2f origin = vector2f(8.f, 8.f);

	addFireball(AddVectors(_pos, pos), origin, velocity, vector2f(2.f, 2.f), rect, 0.f, isMovingLeft, FlRect(0.f, 0.f, 0.f, 0.f));
}

int getNbFireballs()
{
	return fireballList->size(fireballList);
}

sfBool isFireballInBounds(sfFloatRect* _bounds)
{
	sfFloatRect bounds = *_bounds;
	for (int i = 0; i < fireballList->size(fireballList); i++)
	{
		if (sfFloatRect_intersects(&bounds, &GD_FIREBALL->bounds, NULL)) {
			fireballList->erase(&fireballList, i);
			return sfTrue;
		}
	}
	
	return sfFalse;
}

void updateFireballs(Window* _window)
{
	float dt = getDeltaTime();

	for (int i = 0; i < fireballList->size(fireballList); i++)
	{
		GD_FIREBALL->timer += dt;

		if (GD_FIREBALL->velocity.y < 500.f) {
			GD_FIREBALL->velocity.y += dt * 2000.f;
		}

		if (isCollision2(GD_FIREBALL->bounds, sfFalse, sfFalse, vector2f(0.f, 0.f), -1))
			GD_FIREBALL->velocity.y = -500.f;
		if (isCollision2(GD_FIREBALL->bounds, sfFalse, sfTrue, vector2f(0.f, 0.f), -1)) {
			fireballList->erase(&fireballList, i);
			continue;
		}

		if (GD_FIREBALL->velocity.x < 0.f) {
			if (isCollision2(GD_FIREBALL->bounds, sfTrue, sfTrue, vector2f(0.f, 0.f), -1)) {
				fireballList->erase(&fireballList, i);
				continue;
			}
		}
		else {
			if (isCollision2(GD_FIREBALL->bounds, sfTrue, sfFalse, vector2f(0.f, 0.f), -1)) {
				fireballList->erase(&fireballList, i);
				continue;
			}
		}

		GD_FIREBALL->pos = AddVectors(GD_FIREBALL->pos, MultiplyVector(GD_FIREBALL->velocity, dt));

		sfVector2f viewPos = getGreatestViewPos();
		// out of view
		if (GD_FIREBALL->pos.x < viewPos.x - 960.f - 8.f * GD_FIREBALL->scale.x || GD_FIREBALL->pos.y > 1080.f + 8.f * GD_FIREBALL->scale.y) {
			fireballList->erase(&fireballList, i);
			continue;
		}
	}
}

void displayFireballs(Window* _window)
{
	for (int i = 0; i < fireballList->size(fireballList); i++)
	{
		sfSprite_setTextureRect(fireballSprite, GD_FIREBALL->rect);
		sfSprite_setOrigin(fireballSprite, GD_FIREBALL->origin);
		sfSprite_setPosition(fireballSprite, GD_FIREBALL->pos);
		sfSprite_setRotation(fireballSprite, GD_FIREBALL->timer * 360.f);
		sfSprite_setScale(fireballSprite, GD_FIREBALL->scale);
		sfRenderTexture_drawSprite(_window->renderTexture, fireballSprite, NULL);

		GD_FIREBALL->bounds = sfSprite_getGlobalBounds(fireballSprite);
	}
}

void eraseAllFireballs()
{
	fireballList->clear(&fireballList);
}
