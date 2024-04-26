#include "hud.h"
#include "viewManager.h"
#include "game.h"
#include "player.h"
#include "textureManager.h"
#include "menu.h"
#include "soundManager.h"

char buffer[30];

sfText* hudText;
sfSprite* hudSprite;
sfRectangleShape* gameRectangle;

void initHud()
{
	hudText = sfText_create();
	sfText_setFont(hudText, getDefaultFont());
	sfText_setColor(hudText, sfWhite);
	sfText_setCharacterSize(hudText, 30);

	hudSprite = sfSprite_create();

	gameRectangle = sfRectangleShape_create();
	sfRectangleShape_setPosition(gameRectangle, vector2f(0.f, 0.f));
	sfRectangleShape_setFillColor(gameRectangle, sfBlack);
	sfRectangleShape_setSize(gameRectangle, vector2f(1920.f, 1080.f));
}

void displayHud(Window* _window)
{
	sfRenderTexture_setView(_window->renderTexture, sfRenderTexture_getDefaultView(_window->renderTexture));

	int rightPlayer = wantedPlayerTurn;
	for (int i = 0; i < 2; i++)
	{
		if (!hud[i].neverShowAgain && hud[i].lives <= 0) {
			rightPlayer = i;
		}
	}
	if (nbTotalPlayers > 1) {
		if ((hud[0].lives <= 0 && hud[1].lives > 0 && hud[0].neverShowAgain) || (hud[1].lives <= 0 && hud[0].lives > 0 && hud[1].neverShowAgain))
			rightPlayer = wantedPlayerTurn;
	}
	if (startTimer > 0.f && getState() == GAME) {

		sfRenderTexture_drawRectangleShape(_window->renderTexture, gameRectangle, NULL);

		sfFloatRect tmpRect = FlRect(0.f, 0.f, 0.f, 0.f);
		if (hud[rightPlayer].lives <= 0) {
			hud[rightPlayer].hasGameOver = sfTrue;

			if (hud[rightPlayer].canPlayGameOverMusic) {
				hud[rightPlayer].canPlayGameOverMusic = sfFalse;
				PlayASound("gameOverMusic", sfFalse);
			}
			//rightPlayer = playerTurn;

			sprintf(buffer, "%s", hud[rightPlayer].name);
			sfText_setString(hudText, buffer);
			sfText_setPosition(hudText, vector2f(960.f, 450.f));
			tmpRect = sfText_getLocalBounds(hudText);
			sfText_setOrigin(hudText, TEXT_ORIGIN);
			sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

			sfText_setString(hudText, "GAME OVER");
			sfText_setPosition(hudText, vector2f(960.f, 600.f));
			tmpRect = sfText_getLocalBounds(hudText);
			sfText_setOrigin(hudText, TEXT_ORIGIN);
			sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);
		}
		else {
			sfText_setString(hudText, "WORLD 1-1");
			sfText_setPosition(hudText, vector2f(960.f, 450.f));
			tmpRect = sfText_getLocalBounds(hudText);
			sfText_setOrigin(hudText, TEXT_ORIGIN);
			sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

			sfSprite_setTexture(hudSprite, GetTexture("mario"), sfFalse);
			if (rightPlayer == 1) sfSprite_setTextureRect(hudSprite, IntRect(0, 140, 16, 16));
			else sfSprite_setTextureRect(hudSprite, IntRect(0, 32, 16, 16));
			sfSprite_setPosition(hudSprite, vector2f(870.f, 586.f));
			sfSprite_setScale(hudSprite, vector2f(3.f, 3.f));
			sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);

			sfText_setOrigin(hudText, vector2f(0.f, 0.f));

			sprintf(buffer, "x  %d", hud[rightPlayer].lives);
			sfText_setString(hudText, buffer);
			sfText_setPosition(hudText, vector2f(960.f, 600.f));
			sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);
		}
	}
	else {
		if (hud[rightPlayer].hasGameOver)
			hud[rightPlayer].neverShowAgain = sfTrue;
	}

	sfText_setOrigin(hudText, vector2f(0.f, 0.f));

	hud[rightPlayer].score = min(hud[rightPlayer].score, 999999);

	if (getState() == GAME) sprintf(buffer, "%s\n%06d", hud[rightPlayer].name, hud[rightPlayer].score);
	else sprintf(buffer, "MARIO\n000000");
	sfText_setString(hudText, buffer);
	sfText_setPosition(hudText, vector2f(100.f, 20.f));
	sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

	sfSprite_setTexture(hudSprite, GetTexture("items"), sfFalse);
	sfSprite_setTextureRect(hudSprite, IntRect(0, 64, 16, 16));
	sfSprite_setPosition(hudSprite, vector2f(500.f, 50.f));
	sfSprite_setScale(hudSprite, vector2f(3.f, 3.f));
	sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);

	if (getState() == GAME) sprintf(buffer, "x%02d", hud[rightPlayer].coins);
	else sprintf(buffer, "x00");
	sfText_setString(hudText, buffer);
	sfText_setPosition(hudText, vector2f(570.f, 60.f));
	sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

	sprintf(buffer, "WORLD\n 1-1");
	sfText_setString(hudText, buffer);
	sfText_setPosition(hudText, vector2f(1000.f, 20.f));
	sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

	if (getState() == GAME && startTimer <= 0.f) sprintf(buffer, "TIME\n %03.f", gameTime);
	else sprintf(buffer, "TIME\n");
	sfText_setString(hudText, buffer);
	sfText_setPosition(hudText, vector2f(1500.f, 20.f));
	sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);


	if (getState() == GAME && isDebug && startTimer <= 0.f) {
		sfText_setCharacterSize(hudText, 15);
		sfText_setString(hudText, "Debug mode :\nPress I to spawn items\nPress F to get close to finish\nHold T to lower the timer");
		sfText_setPosition(hudText, vector2f(50.f, 150.f));
		sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

		sfText_setCharacterSize(hudText, 30);
	}



	sfRenderTexture_setView(_window->renderTexture, mainView->view);
}
