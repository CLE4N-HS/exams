#include "menu.h"
#include "textureManager.h"
#include "gamepad.h"
#include "viewManager.h"
#include "dialogBox.h"
#include "fontManager.h"
#include "game.h"
#include "hud.h"

sfFont* venture3D;

sfText* menuText;

sfTexture* texture;
sfSprite* menuSprite;

int menuSelection;

int nbNamesToChoose;

float menuTimer;
char nameChoice[30];
int index;
sfEvent event;
sfKeyCode lastKey;
sfBool canPressKey;
char buffer[30];

void initMenu(Window* _window)
{
	Texture_Onload(MENU);
	Font_Onload(MENU);

	menuSprite = sfSprite_create();

	venture3D = getDefaultFont();

	SetViewPosition(mainView, vector2f(mainView->defaultVideoMode.x / 2.0f, mainView->defaultVideoMode.y / 2.0f));

	menuText = sfText_create();
	sfText_setFont(menuText, venture3D);

	GamepadDetection();
	menuSelection = 0;
	nbNamesToChoose = 0;
	menuTimer = 0.f;
	index = 0;
	lastKey = -1;
	canPressKey = sfTrue;
}

void updateMenu(Window* _window)
{
	float dt = getDeltaTime();
	menuTimer += dt;

	if (nbNamesToChoose > 0) {
		
		if (sfKeyboard_isKeyPressed(sfKeyEscape)) {
			nbNamesToChoose = 0;
		}

		if (!sfKeyboard_isKeyPressed(lastKey)) {
			canPressKey = sfTrue;
		}
		int wantedKey = -1;
		int nbKeyPressed = 0;
		if (index < 11) {
			for (int i = 0; i < 26; i++)
			{
				if (sfKeyboard_isKeyPressed(i)) {
					wantedKey = i;
					nbKeyPressed++;
				}
			}
			if (sfKeyboard_isKeyPressed(sfKeyNum8) && canPressKey && index > 1) {
				lastKey = sfKeyNum8;
				canPressKey = sfFalse;
				if (nameChoice[index - 1] != '_') {
					nameChoice[index] = '_';
					index++;
				}
			}
		}
		if (index < 12 && index > 0 && canPressKey) {
			if (sfKeyboard_isKeyPressed(sfKeyBackspace)) {
				lastKey = sfKeyBackspace;
				canPressKey = sfFalse;
				nameChoice[index - 1] = '\0';
				if (index > 0)
					index--;
			}
		}
		if (wantedKey >= 0 && nbKeyPressed == 1 && canPressKey) {
			lastKey = wantedKey;
			nameChoice[index] = 'A' + wantedKey;
			index++;
			canPressKey = sfFalse;
		}
		if (sfKeyboard_isKeyPressed(sfKeyEnter) && index > 0) {
			lastKey = sfKeyEnter;
			canPressKey = sfFalse;

			if (nbTotalPlayers == 2) {

				if (nbNamesToChoose == 2) {
					for (int i = 0; i < 30; i++)
					{
						hud[0].name[i] = nameChoice[i];
						nameChoice[i] = '\0';
					}
				}
				else if (nbNamesToChoose == 1) {
					for (int i = 0; i < 30; i++)
					{
						hud[1].name[i] = nameChoice[i];
						nameChoice[i] = '\0';
					}
				}
			}
			else if (nbTotalPlayers == 1) {
				for (int i = 0; i < 30; i++)
				{
					hud[0].name[i] = nameChoice[i];
					nameChoice[i] = '\0';
				}
			}

			index = 0;

			nbNamesToChoose--;

			if (nbNamesToChoose <= 0) {
				changeState(_window, GAME);
			}
		}
	}
	else {
		float joystickYPos = getStickPos(0, sfTrue, sfFalse);

		if ((joystickYPos > 30.f || sfKeyboard_isKeyPressed(sfKeyUp)) && menuTimer > 0.2f)
		{
			menuSelection--;
			if (menuSelection < 0)
				menuSelection = 2;
			menuTimer = 0.0f;
		}
		if ((joystickYPos < -30.f || sfKeyboard_isKeyPressed(sfKeyDown)) && menuTimer > 0.2f)
		{
			menuSelection++;
			if (menuSelection > 2)
				menuSelection = 0;
			menuTimer = 0.0f;
		}
		if ((isButtonPressed(0, A) || sfKeyboard_isKeyPressed(sfKeyEnter)) && menuTimer > 0.2f)
		{
			switch (menuSelection)
			{
			case 0:
				isEditor = sfFalse;
				nbNamesToChoose = 1;
				menuTimer = 0.f;
				nbTotalPlayers = 1;
				break;
			case 1:
				isEditor = sfFalse;
				nbNamesToChoose = 2;
				menuTimer = 0.f;
				nbTotalPlayers = 2;
				break;
			case 2:
				_window->isDone = sfTrue;
				break;
			default:
				break;
			}
		}
	}

}

void displayMenu(Window* _window)
{
	sfRenderTexture_setView(_window->renderTexture, sfRenderTexture_getDefaultView(_window->renderTexture));
	sfSprite_setTexture(menuSprite, GetTexture("menu"), sfTrue);
	sfSprite_setPosition(menuSprite, vector2f(0.f, 0.f));
	sfRenderTexture_drawSprite(_window->renderTexture, menuSprite, NULL);

	sfSprite_setTexture(menuSprite, GetTexture("select"), sfTrue);
	sfSprite_setPosition(menuSprite, vector2f(317.f, 641.f + 70.f * menuSelection));
	sfRenderTexture_drawSprite(_window->renderTexture, menuSprite, NULL);

	sfText_setFillColor(menuText, sfWhite);
	sfText_setOrigin(menuText, vector2f(0.f, 0.f));
	sfText_setCharacterSize(menuText, 34);
	sfText_setString(menuText, "Quit");
	sfText_setPosition(menuText, vector2f(550.f, 790.f));
	sfRenderTexture_drawText(_window->renderTexture, menuText, NULL);

	if (nbNamesToChoose > 0) {
		sfText_setCharacterSize(menuText, 40);
		if (nbNamesToChoose == 1 && nbTotalPlayers == 2) sfText_setString(menuText, "Player 2");
		else sfText_setString(menuText, "Player 1");
		sfText_setPosition(menuText, vector2f(1160.f, 150.f));
		sfRenderTexture_drawText(_window->renderTexture, menuText, NULL);

		sfText_setCharacterSize(menuText, 30);
		sfText_setLineSpacing(menuText, 2.f);
		sfText_setString(menuText, "ENTER YOUR NAME :\n'A -> Z' + '_'\n(back to erase)\nenter to confirm");
		sfText_setPosition(menuText, vector2f(1160.f, 300.f));
		sfRenderTexture_drawText(_window->renderTexture, menuText, NULL);

		sfText_setLineSpacing(menuText, 1.f);

		sfText_setCharacterSize(menuText, 50);
		sfText_setString(menuText, nameChoice);
		sfText_setPosition(menuText, vector2f(1160.f, 600.f));
		sfRenderTexture_drawText(_window->renderTexture, menuText, NULL);
	}
	else {
		sfText_setCharacterSize(menuText, 40);
		sfText_setString(menuText, "LEADERBOARD :");
		sfText_setPosition(menuText, vector2f(1160.f, 150.f));
		sfRenderTexture_drawText(_window->renderTexture, menuText, NULL);
		for (int i = 0; i < 5; i++)
		{
			if (ld[i].ldScore >= 0) {
				sfText_setCharacterSize(menuText, 30);
				sfText_setString(menuText, ld[i].ldName);
				sfText_setPosition(menuText, vector2f(1160.f, 300.f + i * 100.f));
				sfRenderTexture_drawText(_window->renderTexture, menuText, NULL);

				sfText_setCharacterSize(menuText, 30);
				sprintf(buffer, "%06d", ld[i].ldScore);
				sfText_setString(menuText, buffer);
				sfText_setPosition(menuText, vector2f(1600.f, 300.f + i * 100.f));
				sfRenderTexture_drawText(_window->renderTexture, menuText, NULL);
			}
		}
	}
	sfRenderTexture_setView(_window->renderTexture, mainView->view);

	displayHud(_window);
}

void deinitMenu()
{
	
	//sfFont_destroy(venture3D);
	RemoveAllTextureButALL();
	sfText_destroy(menuText);
}