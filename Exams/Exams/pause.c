#include "pause.h"
#include "stateManager.h"
#include "fontManager.h"
#include "textureManager.h"
#include "viewManager.h"
#include "gamepad.h"
#include "dialogBox.h"

sfRectangleShape* rshape;

sfText* txtPause;
sfText* txtReturnGame;
sfText* txtReturnMenu;
sfText* txtSoundVolume;

sfTexture* texCursor;
sfSprite* spCursor;

int pauseSelect;

void initPause(Window* _window)
{
	rshape = sfRectangleShape_create();
	sfRectangleShape_setSize(rshape, (sfVector2f) { (float)mainView->defaultVideoMode.x, (float)mainView->defaultVideoMode.y});
	sfRectangleShape_setFillColor(rshape, (sfColor) {0,0,0,150});
	txtPause = sfText_create();
	txtReturnGame = sfText_create();
	txtReturnMenu = sfText_create();
	txtSoundVolume = sfText_create();
	sfText_setFont(txtPause, getDefaultFont());
	sfText_setColor(txtPause, sfRed);
	sfText_setString(txtPause, "PAUSE");
	sfText_setFont(txtReturnGame, getDefaultFont());
	sfText_setCharacterSize(txtReturnGame, 20);
	sfText_setColor(txtReturnGame, sfWhite);
	sfText_setString(txtReturnGame, "Return to Game");
	sfText_setFont(txtReturnMenu, getDefaultFont());
	sfText_setCharacterSize(txtReturnMenu, 20);
	sfText_setColor(txtReturnMenu, sfWhite);
	sfText_setString(txtReturnMenu, "Exit to Menu");
	sfText_setFont(txtSoundVolume, getDefaultFont());
	sfText_setCharacterSize(txtSoundVolume, 20);
	sfText_setColor(txtSoundVolume, sfWhite);
	sfText_setString(txtSoundVolume, "Options");

	texCursor = sfTexture_createFromFile("../Ressources/Textures/cursor.png", NULL);
	spCursor = sfSprite_create();
	sfSprite_setTexture(spCursor, texCursor, sfTrue);
	pauseSelect = 0;
}

void updatePause(Window* _window)
{
	static float timer = 0.0f;
	timer += getDeltaTime();

	for (int i = 0; i < /*8*/nbPlayer; i++)
	{
		if (Gamepad_isButtonPressed(i, OPTION) && timer > 0.2f)
		{
			togglePause();
			timer = 0.0f;
		}


		if (Gamepad_isJoystickMoved(i, CROSSY) > 0 && timer > 0.2f)
		{
			pauseSelect--;
			if (pauseSelect < 0)
				pauseSelect = 2;
			timer = 0.0f;
		}
		if (Gamepad_isJoystickMoved(i, CROSSY) < 0 && timer > 0.2f)
		{
			pauseSelect++;
			if (pauseSelect > 2)
				pauseSelect = 0;
			timer = 0.0f;
		}

		if (Gamepad_isButtonPressed(i, CROIX) && timer > 0.2f)
		{
			if (pauseSelect == 0)
			{
				togglePause();
			}
			else if (pauseSelect == 2)
			{
				CreateDialogBox(ALERT, "Voulez vous retourner au menu?\n", 1);
			}

			timer = 0.0f;
		}
	}
	if (sfKeyboard_isKeyPressed(sfKeyEscape) && timer > 0.2f)
	{
		togglePause();
		timer = 0.0f;
	}
	if (sfKeyboard_isKeyPressed(sfKeyDown) && timer > 0.2f)
	{
		pauseSelect++;
		if (pauseSelect > 2)
			pauseSelect = 0;
		timer = 0.0f;
	}
	if (sfKeyboard_isKeyPressed(sfKeyUp) && timer > 0.2f)
	{
		pauseSelect--;
		if (pauseSelect < 0)
			pauseSelect = 2;
		timer = 0.0f;
	}
	if (sfKeyboard_isKeyPressed(sfKeyEnter) && timer > 0.2f)
	{
		
		if (pauseSelect == 0)
		{
			togglePause();
		}
		else if (pauseSelect == 1)
		{
			toggleOptions();
		}
		else if (pauseSelect == 2)
		{
			CreateDialogBox(ALERT, "Voulez vous retourner au menu?\n", 1);
		}

		timer = 0.0f;
	}
	//remove
	if (sfKeyboard_isKeyPressed(sfKeyA) && timer > 0.2f) {
		CreateDialogBox(INFO, "SALUT", 1);
		timer = 0.f;
	}

	if (sfKeyboard_isKeyPressed(sfKeyP) && timer > 0.2f) {
		sfText_setFont(txtReturnGame, GetFont("tmpFont"));
		timer = 0.f;
	}

	//

	sfRectangleShape_setPosition(rshape, (sfVector2f) { mainView->PosView.x - mainView->defaultVideoMode.x/2, mainView->PosView.y - mainView->defaultVideoMode.y / 2});
	
	sfText_setPosition(txtPause, vector2f(GetViewPosition(mainView).x, GetViewPosition(mainView).y - 250.0f));
	sfText_setPosition(txtReturnGame, vector2f(GetViewPosition(mainView).x - 100.0f, GetViewPosition(mainView).y - 200.0f));
	sfText_setPosition(txtReturnMenu, vector2f(GetViewPosition(mainView).x - 100.0f, GetViewPosition(mainView).y - 100.0f));
	sfText_setPosition(txtSoundVolume, vector2f(GetViewPosition(mainView).x - 100.0f, GetViewPosition(mainView).y - 150.0f));

	if (pauseSelect == 0)
	{
		sfText_setPosition(txtReturnGame, vector2f(GetViewPosition(mainView).x - 80.0f, GetViewPosition(mainView).y - 200.0f));
		sfSprite_setPosition(spCursor, vector2f(GetViewPosition(mainView).x - 100.0f, GetViewPosition(mainView).y - 195.0f));
	}
	else if(pauseSelect == 1)
	{
		sfText_setPosition(txtSoundVolume, vector2f(GetViewPosition(mainView).x - 80.0f, GetViewPosition(mainView).y - 150.0f));
		sfSprite_setPosition(spCursor, vector2f(GetViewPosition(mainView).x - 100.0f, GetViewPosition(mainView).y - 145.0f));
	}
	else if (pauseSelect == 2)
	{
		sfText_setPosition(txtReturnMenu, vector2f(GetViewPosition(mainView).x - 80.0f, GetViewPosition(mainView).y - 100.0f));
		sfSprite_setPosition(spCursor, vector2f(GetViewPosition(mainView).x - 100.0f, GetViewPosition(mainView).y - 95.0f));
	}
}

void displayPause(Window* _window)
{
	sfRenderTexture_setView(_window->renderTexture, mainView->view);
	sfRenderTexture_drawRectangleShape(_window->renderTexture, rshape, NULL);
	
	sfRenderTexture_drawSprite(_window->renderTexture, spCursor, NULL);
	
	sfRenderTexture_drawText(_window->renderTexture, txtPause, NULL);
	sfRenderTexture_drawText(_window->renderTexture, txtReturnGame, NULL);
	sfRenderTexture_drawText(_window->renderTexture, txtReturnMenu, NULL);
	sfRenderTexture_drawText(_window->renderTexture, txtSoundVolume, NULL);
}

void deinitPause()
{
	sfRectangleShape_destroy(rshape);
	sfText_destroy(txtPause);
	sfText_destroy(txtReturnGame);
	sfText_destroy(txtReturnMenu);
	sfText_destroy(txtSoundVolume);
	sfSprite_destroy(spCursor);
	sfTexture_destroy(texCursor);
}