#include "game.h"
#include "textureManager.h"
#include "soundManager.h"
#include "fontManager.h"
#include "viewManager.h"
#include "stateManager.h"
#include "Gamepad.h"
#include "map.h"
#include "player.h"
#include "items.h"
#include "enemies.h"
#include "fireballs.h"
#include "fontManager.h"
#include "menu.h"
#include "score.h"

#include <Windows.h>
#include "hud.h"

float timer;

sfText* gameText;
sfBool firstGame;
sfBool hasTimeWarning;

sfBool canPressKey;

void initGame(Window* _window)
{
	Texture_Onload(GAME);
	Sound_Onload(GAME);
	Font_Onload(GAME);

	SetViewPosition(mainView, vector2f(960.f, 540.f));

	timer = 0.f;

	gameText = sfText_create();
	sfText_setFont(gameText, getDefaultFont());
	sfText_setColor(gameText, sfWhite);
	sfText_setCharacterSize(gameText, 30);



	gameTime = 400.f;
	startTimer = 6.f;
	firstGame = sfTrue;
	hasTimeWarning = sfFalse;
	canPressKey = sfTrue;


	initEnemies();
	initItems();
	initFireballs();
	initScore();
	
	initMap();

	initPlayer();


	for (int i = 0; i < 2; i++)
	{
		hud[i].score = 0;
		hud[i].coins = 0;
		hud[i].lives = 3;
		hud[i].hasGameOver = sfFalse;
		hud[i].neverShowAgain = sfFalse;
		hud[i].canPlayGameOverMusic = sfTrue;
	}
	
	GamepadDetection();

	//Sleep(2000); // A enlever juste pour le test Thread

	w.state = sfTrue;

}

void updateGame(Window* _window)
{
	timer += getDeltaTime();
	
	if (sfKeyboard_isKeyPressed(sfKeyEscape) || isButtonPressed(0, START)) {
		changeState(_window, MENU);
	}

	float dt = getDeltaTime();

	static float timerr = 0.f;
	timerr += dt;

	if (getState() == GAME && isDebug && !isAtFinish && isPlayerAlive(playerTurn) && startTimer <= 0.f) {
		if (canPressKey) {
			if (sfKeyboard_isKeyPressed(sfKeyI)) {
				int randomItem = 0;
				randomItem = rand() % 4;
				createItem(randomItem, vector2f(getPlayerPos(playerTurn).x, getPlayerPos(playerTurn).y - 300.f));
				canPressKey = sfFalse;
			}
			else if (sfKeyboard_isKeyPressed(sfKeyF)) {
				setPlayerPos(playerTurn, vector2f(FINISH_XPOS - 100.f, 900.f));
				canPressKey = sfFalse;
			}

		}
		else if (!sfKeyboard_isKeyPressed(sfKeyI) && !sfKeyboard_isKeyPressed(sfKeyF)) {
			canPressKey = sfTrue;
		}

		if (sfKeyboard_isKeyPressed(sfKeyT)) {
			gameTime -= dt * 2.2f * 10.f;
		}
	}

	if (startTimer <= 0.f && !isAtFinish && isPlayerAlive(playerTurn)) {
		gameTime -= dt * 2.2f;
	}
	gameTime = max(gameTime, 0.f);
	if (gameTime < 101.f && !hasTimeWarning && !isAtFinish && startTimer <= 0.f) {
		hasTimeWarning = sfTrue;
		StopASound("gameMusic");
		StopASound("undergroundMusic");
		if (nbMap == 2) {
			PlayASound("lowUndergroundMusic", sfTrue);
		}
		else {
			PlayASound("lowTimerMusic", sfTrue);
		}
	}


	startTimer -= dt;
	if (startTimer > 0.f) {
		StopASound("gameMusic");
		StopASound("lowTimerMusic");
	}
	if (startTimer <= 0.f && (wantedPlayerTurn != playerTurn || firstGame)) {
		firstGame = sfFalse;
		nbMap = 1;
		loadMap(1);

		resetPlayer(wantedPlayerTurn);
		gameTime = 400.f;
		playerTurn = wantedPlayerTurn;
		StopASound("lowTimerMusic");
		StopASound("undergroundMusic");
		StopASound("lowUndergroundMusic");
		PlayASound("gameMusic", sfTrue);
		hasTimeWarning = sfFalse;
	}

	updateMap(_window);
	updatePlayer(_window);
	updateItem(_window);
	updateEnemies(_window);
	updateFireballs(_window);
	updateScore(_window);
	


	int shouldRetrunToMenu = 0;
	for (int nb = 0; nb < nbTotalPlayers; nb++)
	{
		if (hud[nb].neverShowAgain)
			shouldRetrunToMenu++;
	}
	if (shouldRetrunToMenu >= nbTotalPlayers) {
		changeState(_window, MENU);
	}

}

void displayGame(Window* _window)
{
	if (startTimer <= 0.f) {
		displayMap(_window);
		displayItem(_window);
		displayEnemies(_window);
		displayPlayer(_window);
		displayFireballs(_window);
		displayScore(_window);
	}

	displayHud(_window);
}

void addCoin()
{
	hud[playerTurn].coins++;
	PlayASound("coinSFX", sfFalse);
	if (hud[playerTurn].coins > 99) {
		addLife();
		hud[playerTurn].coins = 0;
	}
}

void addLife()
{
	if (hud[playerTurn].lives < 99)
		hud[playerTurn].lives++;
	PlayASound("oneUpSFX", sfFalse);
}

void deinitGame()
{
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
		fclose(file);

		return;
	}
	fread(&ld, sizeof(struct Hud), 5, file);
	fclose(file);
}