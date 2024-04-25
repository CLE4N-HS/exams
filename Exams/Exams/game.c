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
#include "hud.h"

float timer;

sfText* gameText;

int gameScore;

int nbPlayerMode;

sfRectangleShape* gameRectangle;

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

	gameRectangle = sfRectangleShape_create();
	sfRectangleShape_setPosition(gameRectangle, vector2f(0.f, 0.f));
	sfRectangleShape_setFillColor(gameRectangle, sfBlack);
	sfRectangleShape_setSize(gameRectangle, vector2f(1920.f, 1080.f));

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
		hud[i].coins = 0;
		hud[i].lives = 0;
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
		displayHud(_window);
	}

}

void addCoin()
{
	hud[playerTurn].coins++;
	if (hud[playerTurn].coins > 99) {
		addLife();
		hud[playerTurn].coins = 0;
	}
}

void addLife()
{
	if (hud[playerTurn].lives < 99)
		hud[playerTurn].lives;
}

void deinitGame()
{
	deinitPause();
	RemoveAllTextureButALL();
}

void sortLeaderboard()
{
	// create a copy of the leaderboard
	int tmpCurrentldScore = hud[playerTurn].score;
	char tmpCurrentldName[30];
	strcpy(tmpCurrentldName, hud[playerTurn].name);
	Leaderboard tmpLd[5];

	for (int i = 0; i < 5; i++)
	{
		tmpLd[i].ldScore = ld[i].ldScore;
		strcpy(tmpLd[i].ldName, ld[i].ldName);
	}

	// sort the copy
	if (tmpCurrentldScore >= tmpLd[4].ldScore) {
		tmpLd[4].ldScore = tmpCurrentldScore;
		strcpy(tmpLd[4].ldName, tmpCurrentldName);
	}
	if (tmpCurrentldScore >= tmpLd[3].ldScore) {
		int tmpldScore3 = tmpLd[3].ldScore;
		char tmpldName4[30];
		strcpy(tmpldName4, tmpLd[3].ldName);
		tmpLd[3].ldScore = tmpCurrentldScore;
		tmpLd[4].ldScore = tmpldScore3;
		strcpy(tmpLd[3].ldName, tmpCurrentldName);
		strcpy(tmpLd[4].ldName, tmpldName4);
	}
	if (tmpCurrentldScore >= tmpLd[2].ldScore) {
		int tmpldScore2 = tmpLd[2].ldScore;
		char tmpldName2[30];
		strcpy(tmpldName2, tmpLd[2].ldName);
		tmpLd[2].ldScore = tmpCurrentldScore;
		tmpLd[3].ldScore = tmpldScore2;
		strcpy(tmpLd[2].ldName, tmpCurrentldName);
		strcpy(tmpLd[3].ldName, tmpldName2);
	}
	if (tmpCurrentldScore >= tmpLd[1].ldScore) {
		int tmpldScore1 = tmpLd[1].ldScore;
		char tmpldName1[30];
		strcpy(tmpldName1, tmpLd[1].ldName);
		tmpLd[1].ldScore = tmpCurrentldScore;
		tmpLd[2].ldScore = tmpldScore1;
		strcpy(tmpLd[1].ldName, tmpCurrentldName);
		strcpy(tmpLd[2].ldName, tmpldName1);
	}
	if (tmpCurrentldScore >= tmpLd[0].ldScore) {
		int tmpldScore0 = tmpLd[0].ldScore;
		char tmpldName0[30];
		strcpy(tmpldName0, tmpLd[0].ldName);
		tmpLd[0].ldScore = tmpCurrentldScore;
		tmpLd[1].ldScore = tmpldScore0;
		strcpy(tmpLd[0].ldName, tmpCurrentldName);
		strcpy(tmpLd[1].ldName, tmpldName0);
	}

	// transfer the copy into the real Leaderboard
	for (int i = 0; i < 5; i++)
	{
		ld[i].ldScore = tmpLd[i].ldScore;
		strcpy(ld[i].ldName, tmpLd[i].ldName);

	}
}

void saveLeaderboard()
{
	sortLeaderboard();

	// store it in a file
	FILE* file;
	file = fopen(SAVE_PATH"leaderboard.bin", "wb");
	fwrite(&ld, sizeof(struct Leaderboard), 5, file);
	fclose(file);
}

void loadLeaderboard()
{
	FILE* file;
	file = fopen(SAVE_PATH"leaderboard.bin", "rb");
	if (file == NULL) {

		for (int i = 0; i < 5; i++)
		{
			ld[i].ldScore = -1;
			strcpy(ld[i].ldName, "???");
		}

		file = fopen(SAVE_PATH"leaderboard.bin", "wb");
		fwrite(&ld, sizeof(struct Leaderboard), 5, file);
		file = fclose(file);

		return;
	}
	fread(&ld, sizeof(struct Hud), 5, file);
	fclose(file);
}