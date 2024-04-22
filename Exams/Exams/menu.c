#include "menu.h"
#include "textureManager.h"
#include "gamepad.h"
#include "viewManager.h"
#include "dialogBox.h"

sfFont* venture3D;

sfText* Play;
sfText* Editor;
sfText* Options;
sfText* Quit;

sfTexture* texture;
sfSprite* sprite;

int menuSelection;


void initMenu(Window* _window)
{
	Texture_Onload(MENU);

	venture3D = sfFont_createFromFile("../Ressources/Fonts/3Dventure.ttf");

	SetViewPosition(mainView, vector2f(mainView->defaultVideoMode.x / 2.0f, mainView->defaultVideoMode.y / 2.0f));

	Play = sfText_create();
	Editor = sfText_create();
	Options = sfText_create();
	Quit = sfText_create();
	sfText_setFont(Play, venture3D);
	sfText_setFont(Editor, venture3D);
	sfText_setFont(Options, venture3D);
	sfText_setFont(Quit, venture3D);
	sfText_setString(Play, "Play");
	sfText_setString(Editor, "Editor");
	sfText_setString(Options, "Options");
	sfText_setString(Quit, "Quit");
	sfText_setCharacterSize(Play, 72);
	sfText_setCharacterSize(Editor, 72);
	sfText_setCharacterSize(Options, 72);
	sfText_setCharacterSize(Quit, 72);
	sfText_setPosition(Play, vector2f(mainView->PosView.x + 100.0f ,mainView->PosView.y - 100.0f));
	sfText_setPosition(Editor, vector2f(mainView->PosView.x + 200.0f, mainView->PosView.y - 0.0f));
	sfText_setPosition(Options, vector2f(mainView->PosView.x + 300.0f, mainView->PosView.y + 100.0f));
	sfText_setPosition(Quit, vector2f(mainView->PosView.x + 200.0f, mainView->PosView.y + 200.0f));

	GamepadDetection();
	menuSelection = 0;
}

void updateMenu(Window* _window)
{
	static float timer = 0.0f;
	timer += getDeltaTime();

	switch (menuSelection)
	{
	case 0:
		sfText_setColor(Play, sfRed);
		sfText_setColor(Editor, sfWhite);
		sfText_setColor(Options, sfWhite);
		sfText_setColor(Quit, sfWhite);
		break;
	case 1:
		sfText_setColor(Play, sfWhite);
		sfText_setColor(Editor, sfRed);
		sfText_setColor(Options, sfWhite);
		sfText_setColor(Quit, sfWhite);
		break;
	case 2:
		sfText_setColor(Play, sfWhite);
		sfText_setColor(Editor, sfWhite);
		sfText_setColor(Options, sfRed);
		sfText_setColor(Quit, sfWhite);
		break;
	case 3:
		sfText_setColor(Play, sfWhite);
		sfText_setColor(Editor, sfWhite);
		sfText_setColor(Options, sfWhite);
		sfText_setColor(Quit, sfRed);
		break;
	default:
		break;
	}

	for (int i = 0; i < /*8*/nbPlayer; i++)
	{
		if (Gamepad_isButtonPressed(i, CROIX) && timer > 0.2f)
		{
			switch (menuSelection)
			{
			case 0:
				changeState(_window, GAME);
				break;
			case 1:
				changeState(_window, EDITOR);
				break;
			case 2:
				toggleOptions();
				break;
			case 3:
				_window->isDone = sfTrue;
				break;
			default:
				break;
			}
			timer = 0.0f;
		}
		if (Gamepad_isJoystickMoved(i, CROSSY) > 0 && timer > 0.2f)
		{
			menuSelection--;
			if (menuSelection < 0)
				menuSelection = 3;
			timer = 0.0f;
		}
		if (Gamepad_isJoystickMoved(i, CROSSY) < 0 && timer > 0.2f)
		{
			menuSelection++;
			if (menuSelection > 3)
				menuSelection = 0;
			timer = 0.0f;
		}
		if (Gamepad_isJoystickMoved(i, STICKLY) < 0 && timer > 0.5f)
		{
			menuSelection--;
			if (menuSelection < 0)
				menuSelection = 3;
			timer = 0.0f;
		}
		if (Gamepad_isJoystickMoved(i, STICKLY) > 0 && timer > 0.5f)
		{
			menuSelection++;
			if (menuSelection > 3)
				menuSelection = 0;
			timer = 0.0f;
		}
	}

	if (sfKeyboard_isKeyPressed(sfKeyUp) && timer > 0.2f)
	{
		menuSelection--;
		if (menuSelection < 0)
			menuSelection = 3;
		timer = 0.0f;
	}
	if (sfKeyboard_isKeyPressed(sfKeyDown) && timer > 0.2f)
	{
		menuSelection++;
		if (menuSelection > 3)
			menuSelection = 0;
		timer = 0.0f;
	}
	if (sfKeyboard_isKeyPressed(sfKeyEnter) && timer > 0.2f)
	{
		switch (menuSelection)
		{
		case 0:
			changeState(_window, GAME);
			break;
		case 1:
			//changeState(_window, EDITOR);
			break;
		case 2:
			toggleOptions();
			break;
		case 3:
			//_window->isDone = sfTrue;
			CreateDialogBox(ALERT, "Do you really want to quit ?", QUIT_TO_DESKTOP_DB);
			break;
		default:
			break;
		}
		timer = 0.0f;	
	}

}

void displayMenu(Window* _window)
{
	
	sfRenderTexture_drawText(_window->renderTexture, Play, NULL);
	sfRenderTexture_drawText(_window->renderTexture, Editor, NULL);
	sfRenderTexture_drawText(_window->renderTexture, Options, NULL);
	sfRenderTexture_drawText(_window->renderTexture, Quit, NULL);
}

void deinitMenu()
{
	
	sfFont_destroy(venture3D);
	RemoveAllTextureButALL();
	sfText_destroy(Play);
	sfText_destroy(Editor);
	sfText_destroy(Options);
	sfText_destroy(Quit);
}