#include "enemies.h"
#include "textureManager.h"
#include "List.h"
#include "CustomMath.h"
#include "map.h"
#include "player.h"
#include "fireballs.h"

#define GD_ENEMY STD_LIST_GETDATA(enemiesList, Enemies, i)
#define GOOMBAX_SPEED 100.f
#define GOOMBAY_SPEED 400.f

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



				// player Collisions
				for (int j = 0; j < 2; j++)
				{
					if (sfFloatRect_intersects(pgetPlayerBounds(j), &GD_ENEMY->bounds, NULL)) {
						if (getPlayerPos(j).y <= GD_ENEMY->pos.y + GD_ENEMY->bounds.height / 2.f && getPlayerPos(j).y <= GD_ENEMY->pos.y - 10.f) {
							GD_ENEMY->timer = 0.f;
							GD_ENEMY->state = E_DEAD;
							MakePlayerJump(j);
							//setPlayerVelocity(j, 0.f, -1000.f);
						}
						else
							DamagePlayer(j);
						break;
					}
				}

				// fireballs Collisions
				if (isFireballInBounds(&GD_ENEMY->bounds)) {
					GD_ENEMY->timer = 0.f;
					GD_ENEMY->state = E_DEAD;
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
		default:
			break;
		}

		sfVector2f viewPos = getGreatestViewPos();
		if (GD_ENEMY->pos.x < viewPos.x - 960.f - 8.f * GD_ENEMY->scale.x || GD_ENEMY->pos.y > 1080.f + 8.f * GD_ENEMY->scale.y) {
			enemiesList->erase(&enemiesList, i);
			continue;
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
		rect = IntRect(0, 16, 16, 16);
		velocity = vector2f(-200.f, 270.f);
		break;
	default:
		break;
	}

	addEnemy(_type, E_ALIVE, AddVectors(_pos, MultiplyVector(origin, BLOCK_SCALE)), origin, velocity, vector2f(BLOCK_SCALE, BLOCK_SCALE), rect, 0.f, isMovingLeft, FlRect(0.f, 0.f, 0.f, 0.f));
}
