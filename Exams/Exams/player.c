#include "player.h"
#include "textureManager.h"
#include "map.h"
#include "CustomMath.h"

typedef struct Player {
	sfVector2f pos;
	sfVector2f origin;
	sfVector2f scale;
	sfIntRect rect;
	sfVector2f velocity;
}Player;
Player p[2];

sfSprite* playerSprite;

void initPlayer()
{
	playerSprite = sfSprite_create();
	sfSprite_setTexture(playerSprite, GetTexture("mario"), sfFalse);

	for (int i = 0; i < 2; i++)
	{
		p[i].rect = IntRect(0, 32, 18, 16);
		p[i].origin = vector2f(11.f, 16.f);
		p[i].scale = vector2f(BLOCK_SCALE, BLOCK_SCALE);
		p[i].pos = vector2f(100.f, 800.f);
		p[i].velocity = VECTOR2F_NULL;
	}
}

void updatePlayer(Window* _window)
{
	for (int i = 0; i < 2; i++)
	{

	}
}

void displayPlayer(Window* _window)
{
	for (int i = 0; i < 2; i++)
	{
		sfSprite_setTextureRect(playerSprite, p[i].rect);
		sfSprite_setPosition(playerSprite, p[i].pos);
		sfSprite_setScale(playerSprite, p[i].scale);
		sfRenderTexture_drawSprite(_window->renderTexture, playerSprite, NULL);
	}
}
