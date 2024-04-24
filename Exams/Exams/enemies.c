#include "enemies.h"
#include "textureManager.h"
#include "List.h"
#include "CustomMath.h"
#include "map.h"
#include "player.h"
#include "fireballs.h"

#define GD_ENEMY STD_LIST_GETDATA(enemiesList, Enemies, i)
#define GD_ENEMYJ STD_LIST_GETDATA(enemiesList, Enemies, j)

#define GOOMBAX_SPEED 100.f
#define GOOMBAY_SPEED 400.f
#define SHELLX_SPEED 800.f
#define SHELLY_SPEED 600.f


typedef enum EnemyState {
	E_ALIVE,
	E_DEAD
}EnemyState;

typedef struct Enemies {
	EnemyType type;
	EnemyState state;
	sfVector2f pos;
	sfVector2f origin;
	sfVector2f velocity;
	sfVector2f scale;
	sfIntRect rect;
	float timer;
	sfBool isMovingLeft;
	sfFloatRect bounds;
}Enemies;

sfSprite* enemiesSprite;

stdList* enemiesList;

void addEnemy(EnemyType _type, EnemyState _state, sfVector2f _pos, sfVector2f _origin, sfVector2f _velocity, sfVector2f _scale, sfIntRect _rect, float _timer, sfBool _isMovingLeft, sfFloatRect _bounds)
{
	Enemies tmp;

	tmp.type = _type;
	tmp.state = _state;
	tmp.pos = _pos;
	tmp.origin = _origin;
	tmp.velocity = _velocity;
	tmp.scale = _scale;
	tmp.rect = _rect;
	tmp.timer = _timer;
	tmp.isMovingLeft = _isMovingLeft;
	tmp.bounds = _bounds;

	STD_LIST_PUSHBACK(enemiesList, tmp);
}

void initEnemies()
{
	enemiesSprite = sfSprite_create();
	sfSprite_setTexture(enemiesSprite, GetTexture("enemies"), sfFalse);

	enemiesList = STD_LIST_CREATE(Enemies, 0);
}

void updateEnemies(Window* _window)
{
	float dt = getDeltaTime();

	for (int i = 0; i < enemiesList->size(enemiesList); i++)
	{
		sfVector2f viewPos = getGreatestViewPos();
		if (GD_ENEMY->pos.x < viewPos.x - 960.f - 8.f * GD_ENEMY->scale.x || GD_ENEMY->pos.y > 1080.f + 8.f * GD_ENEMY->scale.y) {
			enemiesList->erase(&enemiesList, i);
			continue;
		}

		if (GD_ENEMY->velocity.x < 0.f)
			GD_ENEMY->scale.x = BLOCK_SCALE;
		else
			GD_ENEMY->scale.x = -BLOCK_SCALE;

		switch (GD_ENEMY->type)
		{
		case E_GOOMBA:
			GD_ENEMY->timer += dt;

			if (GD_ENEMY->state == E_ALIVE)
			{
				if (isCollision2(GD_ENEMY->bounds, sfFalse, sfFalse, vector2f(0.f, 0.f), -1)) {
					GD_ENEMY->velocity.y = 0.f;
					Animator(&GD_ENEMY->rect, &GD_ENEMY->timer, 2, 1, 0.5f, 0.f);
				}
				else
					GD_ENEMY->velocity.y = GOOMBAY_SPEED;

				if (GD_ENEMY->velocity.x < 0.f) {
					if (isCollision2(GD_ENEMY->bounds, sfTrue, sfTrue, vector2f(-GOOMBAX_SPEED, 0.f), -1)) {
						GD_ENEMY->velocity.x = GOOMBAX_SPEED;
					}
				}
				else {
					if (isCollision2(GD_ENEMY->bounds, sfTrue, sfFalse, vector2f(GOOMBAX_SPEED, 0.f), -1)) {
						GD_ENEMY->velocity.x = -GOOMBAX_SPEED;
					}
				}

				GD_ENEMY->pos = AddVectors(GD_ENEMY->pos, MultiplyVector(GD_ENEMY->velocity, dt));

				// between them collisions
				for (int j = 0; j < enemiesList->size(enemiesList); j++)
				{
					if (i != j && sfFloatRect_intersects(&GD_ENEMY->bounds, &GD_ENEMYJ->bounds, NULL)) {
						//if (GD_ENEMY->velocity.x < 0.f) {
						//	GD_ENEMY->velocity.x = GOOMBAX_SPEED;
						//	GD_ENEMY->pos.x += GOOMBAX_SPEED * dt;
						//}
						//else {
						//	GD_ENEMY->velocity.x = -GOOMBAX_SPEED;
						//	GD_ENEMY->pos.x += -GOOMBAX_SPEED * dt;
						//}

						if (GD_ENEMYJ->velocity.x < 0.f) {
							GD_ENEMYJ->velocity.x = GOOMBAX_SPEED;
							GD_ENEMYJ->pos.x += GOOMBAX_SPEED * dt;
						}
						else {
							GD_ENEMYJ->velocity.x = -GOOMBAX_SPEED;
							GD_ENEMYJ->pos.x += -GOOMBAX_SPEED * dt;
						}
					}
				}



				// player Collisions
				sfBool shouldContinue = sfFalse;
				for (int j = 0; j < 2; j++)
				{
					if (sfFloatRect_intersects(pgetPlayerBounds(j), &GD_ENEMY->bounds, NULL)) {
						if (PlayerHasStar(j)) {
							enemiesList->erase(&enemiesList, i);
							shouldContinue = sfTrue;
							break;
						}
						else {
							if (getPlayerPos(j).y <= GD_ENEMY->pos.y + GD_ENEMY->bounds.height / 2.f && getPlayerPos(j).y <= GD_ENEMY->pos.y - 10.f) {
								GD_ENEMY->timer = 0.f;
								GD_ENEMY->state = E_DEAD;
								MakePlayerJump(j);
								break;
							}
							else
								DamagePlayer(j);
						}
					}
				}

				if (shouldContinue)
					continue;

				// fireballs Collisions
				if (isFireballInBounds(&GD_ENEMY->bounds)) {
					enemiesList->erase(&enemiesList, i);
					//GD_ENEMY->timer = 0.f;
					//GD_ENEMY->state = E_DEAD;
				}
			}
			else
			{
				GD_ENEMY->rect.left = 32;

				if (GD_ENEMY->timer > 0.5f) {
					enemiesList->erase(&enemiesList, i);
					continue;
				}
			}
			break;
		case E_KOOPA:
			GD_ENEMY->timer += dt;

			if (GD_ENEMY->state == E_ALIVE)
			{
				if (isCollision2(GD_ENEMY->bounds, sfFalse, sfFalse, vector2f(0.f, 0.f), -1)) {
					GD_ENEMY->velocity.y = 0.f;
					Animator(&GD_ENEMY->rect, &GD_ENEMY->timer, 2, 1, 0.5f, 0.f);
				}
				else
					GD_ENEMY->velocity.y = GOOMBAY_SPEED;

				if (GD_ENEMY->velocity.x < 0.f) {
					if (isCollision2(GD_ENEMY->bounds, sfTrue, sfTrue, vector2f(GOOMBAX_SPEED, 0.f), -1)) {
						GD_ENEMY->velocity.x = GOOMBAX_SPEED;
					}
				}
				else {
					if (isCollision2(GD_ENEMY->bounds, sfTrue, sfFalse, vector2f(-GOOMBAX_SPEED, 0.f), -1)) {
						GD_ENEMY->velocity.x = -GOOMBAX_SPEED;
					}
				}

				GD_ENEMY->pos = AddVectors(GD_ENEMY->pos, MultiplyVector(GD_ENEMY->velocity, dt));

				// between them collisions
				for (int j = 0; j < enemiesList->size(enemiesList); j++)
				{
					if (i != j && sfFloatRect_intersects(&GD_ENEMY->bounds, &GD_ENEMYJ->bounds, NULL)) {
						//if (GD_ENEMY->velocity.x < 0.f) {
						//	GD_ENEMY->velocity.x = GOOMBAX_SPEED;
						//	GD_ENEMY->pos.x += GOOMBAX_SPEED * dt;
						//}
						//else {
						//	GD_ENEMY->velocity.x = -GOOMBAX_SPEED;
						//	GD_ENEMY->pos.x += -GOOMBAX_SPEED * dt;
						//}

						if (GD_ENEMYJ->velocity.x < 0.f) {
							GD_ENEMYJ->velocity.x = GOOMBAX_SPEED;
							GD_ENEMYJ->pos.x += GOOMBAX_SPEED * dt;
						}
						else {
							GD_ENEMYJ->velocity.x = -GOOMBAX_SPEED;
							GD_ENEMYJ->pos.x += -GOOMBAX_SPEED * dt;
						}
					}
				}



				// player Collisions
				sfBool shouldContinue = sfFalse;
				for (int j = 0; j < 2; j++)
				{
					if (sfFloatRect_intersects(pgetPlayerBounds(j), &GD_ENEMY->bounds, NULL)) {
						if (PlayerHasStar(j)) {
							enemiesList->erase(&enemiesList, i);
							shouldContinue = sfTrue;
							break;
						}
						else {
							if (getPlayerPos(j).y <= GD_ENEMY->pos.y + GD_ENEMY->bounds.height / 2.f && getPlayerPos(j).y <= GD_ENEMY->pos.y - 10.f) {
								GD_ENEMY->timer = 0.f;
								GD_ENEMY->state = E_DEAD;
								GD_ENEMY->velocity = vector2f(0.f, SHELLY_SPEED);
								MakePlayerJump(j);
								break;
							}
							else
								DamagePlayer(j);
						}
					}
				}

				if (shouldContinue)
					continue;

				// fireballs Collisions
				if (isFireballInBounds(&GD_ENEMY->bounds)) {
					enemiesList->erase(&enemiesList, i);
					//GD_ENEMY->timer = 0.f;
					//GD_ENEMY->state = E_DEAD;
				}
			}
			else
			{
				GD_ENEMY->rect.left = 32;

				if (isCollision2(GD_ENEMY->bounds, sfFalse, sfFalse, vector2f(0.f, 0.f), -1)) {
					GD_ENEMY->velocity.y = 0.f;
				}
				else
					GD_ENEMY->velocity.y = SHELLY_SPEED;

				if (GD_ENEMY->timer > 0.2f) {
					// player Collisions
					sfBool shouldContinue = sfFalse;
					for (int j = 0; j < 2; j++)
					{
						if (sfFloatRect_intersects(pgetPlayerBounds(j), &GD_ENEMY->bounds, NULL)) {
							if (PlayerHasStar(j)) {
								enemiesList->erase(&enemiesList, i);
								shouldContinue = sfTrue;
								break;
							}
							else {
								if (getPlayerPos(j).y <= GD_ENEMY->pos.y + GD_ENEMY->bounds.height / 2.f && getPlayerPos(j).y <= GD_ENEMY->pos.y - 10.f) {
									GD_ENEMY->timer = 0.f;
									GD_ENEMY->state = E_DEAD;
									MakePlayerJump(j);
									break;
								}
								else
								{
									if (fabsf(GD_ENEMY->velocity.x) > GOOMBAX_SPEED - 10.f) { // TODO not hitting player
										DamagePlayer(j);
									}
									else {
										if (getPlayerPos(j).x > GD_ENEMY->pos.x) {
											GD_ENEMY->velocity.x = -SHELLX_SPEED;
										}
										else {
											GD_ENEMY->velocity.x = SHELLX_SPEED;
										}
									}
								}
							}
						}
					}

					if (shouldContinue)
						continue;

					if (GD_ENEMY->velocity.x < 0.f) {
						if (isCollision2(GD_ENEMY->bounds, sfTrue, sfTrue, vector2f(SHELLX_SPEED, 0.f), -1)) {
							GD_ENEMY->velocity.x = SHELLX_SPEED;
						}
					}
					else {
						if (isCollision2(GD_ENEMY->bounds, sfTrue, sfFalse, vector2f(-SHELLX_SPEED, 0.f), -1)) {
							GD_ENEMY->velocity.x = -SHELLX_SPEED;
						}
					}

					GD_ENEMY->pos = AddVectors(GD_ENEMY->pos, MultiplyVector(GD_ENEMY->velocity, dt));

					//between them collisions
					for (int j = 0; j < enemiesList->size(enemiesList); j++)
					{
						if (i != j && sfFloatRect_intersects(&GD_ENEMY->bounds, &GD_ENEMYJ->bounds, NULL)) {
							enemiesList->erase(&enemiesList, j);
							continue;
						}
					}

				}
			}
			break;
		default:
			break;
		}



	}
}

void displayEnemies(Window* _window)
{
	for (int i = 0; i < enemiesList->size(enemiesList); i++)
	{
		sfSprite_setTextureRect(enemiesSprite, GD_ENEMY->rect);
		sfSprite_setPosition(enemiesSprite, GD_ENEMY->pos);
		sfSprite_setOrigin(enemiesSprite, GD_ENEMY->origin);
		sfSprite_setScale(enemiesSprite, GD_ENEMY->scale);
		sfRenderTexture_drawSprite(_window->renderTexture, enemiesSprite, NULL);

		GD_ENEMY->bounds = sfSprite_getGlobalBounds(enemiesSprite);
	}
}

void createEnemy(EnemyType _type, sfVector2f _pos)
{
	sfIntRect rect = IntRect(0, 0, 0, 0);
	sfVector2f velocity = VECTOR2F_NULL;
	sfBool isMovingLeft = sfTrue;
	sfVector2f origin = vector2f(8.f, 16.f);

	switch (_type)
	{
	case E_GOOMBA:
		rect = IntRect(0, 0, 16, 16);
		velocity = vector2f(-GOOMBAX_SPEED, GOOMBAY_SPEED);
		break;
	case E_KOOPA:
		rect = IntRect(0, 16, 16, 24);
		velocity = vector2f(-GOOMBAX_SPEED, GOOMBAY_SPEED);
		break;
	default:
		break;
	}

	addEnemy(_type, E_ALIVE, AddVectors(_pos, MultiplyVector(origin, BLOCK_SCALE)), origin, velocity, vector2f(BLOCK_SCALE, BLOCK_SCALE), rect, 0.f, isMovingLeft, FlRect(0.f, 0.f, 0.f, 0.f));
}
