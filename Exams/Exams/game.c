#include "game.h"
#include "textureManager.h"
#include "soundManager.h"
#include "fontManager.h"
#include "viewManager.h"
#include "stateManager.h"
#include "Gamepad.h"
#include "pause.h"
#include "map.h"
#include "editor.h"
#include "player.h"
#include "items.h"
#include "enemies.h"
#include "fireballs.h"
#include "fontManager.h"

#include <Windows.h>

float timer;

sfText* gameText;
char buffer[30];

sfSprite* gameSprite;

int gameScore;
float gameTime;

int nbPlayerMode;

void initGame(Window* _window)
{
	Texture_Onload(GAME);
	Sound_Onload(GAME);
	Font_Onload(GAME);

	SetViewPosition(mainView, vector2f(960.f, 540.f));

	timer = 0.f;

	gameText = sfText_create();
	//sfText_setFont(gameText, GetFont("marioFont"));
	sfText_setFont(gameText, getDefaultFont());
	sfText_setColor(gameText, sfWhite);
	sfText_setCharacterSize(gameText, 30);

	gameSprite = sfSprite_create();
	sfSprite_setTexture(gameSprite, GetTexture("items"), sfFalse);
	sfSprite_setTextureRect(gameSprite, IntRect(0, 64, 16, 16));
	sfSprite_setPosition(gameSprite, vector2f(500.f, 50.f));
	sfSprite_setScale(gameSprite, vector2f(3.f, 3.f));

	gameScore = 0;
	gameTime = 400.f;

	if (isEditor) {
	}
	else {
		initEnemies();
	}
	
	initMap();
	if (isEditor) {
		initEditor();
	}
	else {
		initPlayer();
		initItems();
		//initEnemies();
		initFireballs();
	}

	for (int i = 0; i < 2; i++)
	{
		hud[i].score = 0;
		hud[i].nbCoins = 0;
		hud[i].nbLifes = 0;
	}
	
	GamepadDetection();

	//Sleep(2000); // A enlever juste pour le test Thread

	w.state = sfTrue;

}

void updateGame(Window* _window)
{
	timer += getDeltaTime();
	
	for (int i = 0; i < /*8*/nbPlayer; i++)
	{
		if (Gamepad_isButtonPressed(i, OPTION) && timer > 0.2f)
		{
			togglePause();
			timer = 0.0f;
		}
	}

	if (isKeyboardOrControllerButtonPressed(sfKeyEscape, SELECT_XBOX) && timer > 0.2f)
	{
		togglePause();
		timer = 0.0f;
	}

	float dt = getDeltaTime();

	static float timerr = 0.f;
	timerr += dt;

	if (isButtonPressed(0, X) && timerr > 0.5f) { // TODO remove
		createEnemy(E_GOOMBA, vector2f(getPlayerPos(0).x + 500.f, 200.f));
		createItem(I_FIREFLOWER, vector2f(getPlayerPos(0).x, getPlayerPos(0).y - 100.f));
		timerr = 0.f;
	}
	if (isButtonPressed(0, Y) && timerr > 0.5f) { // TODO remove
		createEnemy(E_KOOPA, vector2f(getPlayerPos(0).x + 500.f, 200.f));
		timerr = 0.f;
	}

	gameTime -= dt * 2.2f;
	gameTime = max(gameTime, 0.f);
	
	updateMap(_window);
	if (isEditor) {
		updateEditor(_window);
	}
	else {
		updatePlayer(_window);
		updateItem(_window);
		updateEnemies(_window);
		updateFireballs(_window);
	}

	gameScore = min(gameScore, 999999);
}

void displayGame(Window* _window)
{
	displayMap(_window);
	if (isEditor) {
		displayEditor(_window);
	}
	else {
		displayItem(_window);
		displayEnemies(_window);
		displayPlayer(_window);
		displayFireballs(_window);

		sfRenderTexture_setView(_window->renderTexture, sfRenderTexture_getDefaultView(_window->renderTexture));

		sprintf(buffer, "%s\n%06d", hud[playerTurn].name, hud[playerTurn].score);
		sfText_setString(gameText, buffer);
		sfText_setPosition(gameText, vector2f(100.f, 20.f));
		sfRenderTexture_drawText(_window->renderTexture, gameText, NULL);

		sfRenderTexture_drawSprite(_window->renderTexture, gameSprite, NULL);

		sprintf(buffer, "x%02d", hud[playerTurn].nbCoins);
		sfText_setString(gameText, buffer);
		sfText_setPosition(gameText, vector2f(570.f, 60.f));
		sfRenderTexture_drawText(_window->renderTexture, gameText, NULL);

		sprintf(buffer, "WORLD\n 1-1");
		sfText_setString(gameText, buffer);
		sfText_setPosition(gameText, vector2f(1000.f, 20.f));
		sfRenderTexture_drawText(_window->renderTexture, gameText, NULL);

		sprintf(buffer, "TIME\n %03.f", gameTime);
		sfText_setString(gameText, buffer);
		sfText_setPosition(gameText, vector2f(1500.f, 20.f));
		sfRenderTexture_drawText(_window->renderTexture, gameText, NULL);


		sfRenderTexture_setView(_window->renderTexture, mainView->view);
	}

}

void deinitGame()
{
	deinitPause();
	RemoveAllTextureButALL();
}