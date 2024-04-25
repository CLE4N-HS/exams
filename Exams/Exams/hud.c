#include "hud.h"
#include "viewManager.h"
#include "game.h"
#include "player.h"

char buffer[30];

sfText* hudText;
sfSprite* hudSprite;

void initHud()
{
	hudText = sfText_create();
	sfText_setFont(hudText, getDefaultFont());
	sfText_setColor(hudText, sfWhite);
	sfText_setCharacterSize(hudText, 30);

	hudSprite = sfSprite_create();
	sfSprite_setTexture(hudSprite, GetTexture("items"), sfFalse);
	sfSprite_setTextureRect(hudSprite, IntRect(0, 64, 16, 16));
	sfSprite_setPosition(hudSprite, vector2f(500.f, 50.f));
	sfSprite_setScale(hudSprite, vector2f(3.f, 3.f));
}

void displayHud(Window* _window)
{
	sfRenderTexture_setView(_window->renderTexture, sfRenderTexture_getDefaultView(_window->renderTexture));

	if (getState() == GAME) sprintf(buffer, "%s\n%06d", hud[playerTurn].name, hud[playerTurn].score);
	else sprintf(buffer, "MARIO\n000000");
	sfText_setString(hudText, buffer);
	sfText_setPosition(hudText, vector2f(100.f, 20.f));
	sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

	sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);

	if (getState() == GAME) sprintf(buffer, "x%02d", hud[playerTurn].coins);
	else sprintf(buffer, "x00");
	sfText_setString(hudText, buffer);
	sfText_setPosition(hudText, vector2f(570.f, 60.f));
	sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

	sprintf(buffer, "WORLD\n 1-1");
	sfText_setString(hudText, buffer);
	sfText_setPosition(hudText, vector2f(1000.f, 20.f));
	sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

	if (getState() == GAME) sprintf(buffer, "TIME\n %03.f", gameTime);
	else sprintf(buffer, "TIME\n 000");
	sfText_setString(hudText, buffer);
	sfText_setPosition(hudText, vector2f(1500.f, 20.f));
	sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);



	sfRenderTexture_setView(_window->renderTexture, mainView->view);
}
