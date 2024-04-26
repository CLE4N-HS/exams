#include "stateManager.h"
#include "ressourcesManager.h"
#include "textureManager.h"
#include "soundManager.h"
#include "fontManager.h"
#include "menu.h"
#include "game.h"
#include "gamepad.h"
#include "viewManager.h"
#include "map.h"
#include "hud.h"


void stateInit(Window* _window)
{
	static sfBool firstload = sfFalse;
	if (!firstload)
	{
		OnLoad("../Ressources/ressources.load");
		Texture_Onload(ALL);
		Sound_Onload(ALL);
		Font_Onload(ALL);
		spLoading = sfSprite_create();
		sfSprite_setTexture(spLoading, GetTexture("loading"), sfTrue);
		sfIntRect AnimRect = { 0, 0, 128, 128 };
		sfSprite_setOrigin(spLoading, vector2f(64.0f, 64.0f));
		sfSprite_setPosition(spLoading, vector2f(mainView->PosView.x, mainView->PosView.y));
		sfSprite_setTextureRect(spLoading, AnimRect);

		SFXVolume = 50.0f;
		musicVolume = 50.0f;
		timeScale = 1.f;

		nbPlayer = NbConnectedControllers();
		
		firstload = sfTrue;
		initHud();
		loadLeaderboard();

		w.state = sfTrue;
	}

	if (!onePass)
	{
		onePass = sfTrue;
		if (state == INTRO)
		{
			
		}
		if (state == MENU)
		{
			initMenu(_window);
		}
		if (state == GAME)
		{
			w.mutex = sfMutex_create();
			w.state = sfFalse;
			loadingThread = sfThread_create(&initGame, _window);
			sfThread_launch(loadingThread);
		}
		if (state == END)
		{
			
		}
	}
}

void stateEventUpdate(Window* _window, sfEvent* _event)
{
	if (w.state)
	{
			if (state == INTRO)
			{

			}
			else if (state == MENU)
			{
				
			}
			else if (state == GAME)
			{
				
			}
			else if (state == END)
			{

			}
		
	}
}

void stateUpdate(Window* _window)
{
	if (sfMouse_isButtonPressed(sfMouseMiddle) && isDebug) {
		_window->isDone = sfTrue;
	}
	if (w.state)
	{
		if (state == INTRO)
		{

		}
		else if (state == MENU)
		{
			updateMenu(_window);
		}
		else if (state == GAME)
		{
			updateGame(_window);
		}
		else if (state == END)
		{

		}
	}
}

void stateDisplay(Window* _window)
{
	if (w.state)
	{
		if (state == INTRO)
		{

		}
		if (state == MENU)
		{
			displayMenu(_window);
		}
		if (state == GAME)
		{
			displayGame(_window);
		}
		if (state == END)
		{

		}
	}
}

void stateDeinit(Window* _window)
{
	RemoveAllTextureButALL();
	RemoveAllSoundsButALL();
	RemoveAllFontsButALL();
	if (state == INTRO)
	{

	}
	if (state == MENU)
	{
		deinitMenu();
	}
	if (state == GAME)
	{
		deinitGame();
	}
	if (state == END)
	{

	}
}


void changeState(Window* _window, State _state)
{
	stateDeinit(_window);
	state = _state;
	onePass = sfFalse;
	stateInit(_window);
}

State getState()
{
	return state;
}
