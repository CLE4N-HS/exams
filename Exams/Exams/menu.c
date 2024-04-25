#include "menu.h"
#include "textureManager.h"
#include "gamepad.h"
#include "viewManager.h"
#include "dialogBox.h"
#include "fontManager.h"

sfFont* venture3D;

sfText* Play;
sfText* Editor;
sfText* Options;
sfText* Quit;

sfTexture* texture;
sfSprite* menuSprite;

int menuSelection;


void initMenu(Window* _window)
{
	Texture_Onload(MENU);
	Font_Onload(MENU);

	menuSprite = sfSprite_create();

	venture3D = getDefaultFont();

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

	float joystickYPos = getStickPos(0, sfTrue, sfFalse);

	if ((joystickYPos > 30.f || sfKeyboard_isKeyPressed(sfKeyUp)) && timer > 0.2f)
	{
		menuSelection--;
		if (menuSelection < 0)
			menuSelection = 2;
		timer = 0.0f;
	}
	if ((joystickYPos < -30.f || sfKeyboard_isKeyPressed(sfKeyDown)) && timer > 0.2f)
	{
		menuSelection++;
		if (menuSelection > 2)
			menuSelection = 0;
		timer = 0.0f;
	}
	if ((isButtonPressed(0, A) || sfKeyboard_isKeyPressed(sfKeyEnter)) && timer > 0.2f)
	{
		switch (menuSelection)
		{
		case 0:
			isEditor = sfFalse;
			changeState(_window, GAME);
			break;
		case 1:
			isEditor = sfTrue;
			changeState(_window, GAME);
			break;
		case 2:
			_window->isDone = sfTrue;
			//toggleOptions();
			break;
		case 3:
			//_window->isDone = sfTrue;
			//CreateDialogBox(ALERT, "Do you really want to quit ?", QUIT_TO_DESKTOP_DB);
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

	sfSprite_setTexture(menuSprite, GetTexture("menu"), sfTrue);
	sfSprite_setPosition(menuSprite, vector2f(0.f, 0.f));
	sfRenderTexture_drawSprite(_window->renderTexture, menuSprite, NULL);

	sfSprite_setTexture(menuSprite, GetTexture("select"), sfTrue);
	sfSprite_setPosition(menuSprite, vector2f(317.f, 641.f + 70.f * menuSelection));
	sfRenderTexture_drawSprite(_window->renderTexture, menuSprite, NULL);

	sfText_setCharacterSize(Quit, 34);
	sfText_setPosition(Quit, vector2f(550.f, 790.f));
	sfRenderTexture_drawText(_window->renderTexture, Quit, NULL);
}

void deinitMenu()
{
	
	//sfFont_destroy(venture3D);
	RemoveAllTextureButALL();
	sfText_destroy(Play);
	sfText_destroy(Editor);
	sfText_destroy(Options);
	sfText_destroy(Quit);
}