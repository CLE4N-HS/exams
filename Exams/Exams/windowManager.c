#include "windowManager.h"
#include "gamepad.h"
#include "viewManager.h"
#include "stateManager.h"


sfTexture* allTextures;
sfSprite* allSprites;



sfBool IsDone(Window* This)
{
	return This->isDone;
}

sfBool IsFullscreen(Window* This)
{
	return This->isFullscreen;
}

void BeginDraw(Window* This)
{
	sfRenderTexture_clear(This->renderTexture, sfBlack);
	//sfRenderWindow_clear(This->renderWindow, sfBlack);
}

void EndDraw(Window* This)
{
	sfRenderWindow_display(This->renderWindow);
}

Window* windowSetup(const char* _Title, sfVector2i _defaultVideoMode)
{
	Window* This = malloc(sizeof(Window));
	if (This == NULL)
	{
		printf("Allocation pour la window echoue");
	}
	This->windowTitle = malloc(strlen(_Title)*sizeof(char));
	if (This->windowTitle == NULL)
	{
		printf("Allocation pour Title de la window echoue");
	}

	strcpy(This->windowTitle, _Title);
	
	This->isFullscreen = sfFalse;
	This->isDone = sfFalse;
	This->videoMode = sfVideoMode_getDesktopMode();
	This->defaultVideoMode = _defaultVideoMode;
	
	windowCreate(This);

	// MainView
	sfFloatRect rectViewPort = { 0.0f, 0.0f, 1.0f, 1.0f };
	mainView = setupView(_defaultVideoMode, rectViewPort, This->videoMode);

	// Render Texture
	This->renderTexture = sfRenderTexture_create(_defaultVideoMode.x, _defaultVideoMode.y, sfFalse);
	//allTextures = sfTexture_create(_defaultVideoMode.x, _defaultVideoMode.y);
	allSprites = sfSprite_create();

	return This;
}

void windowDestroy(Window* This)
{
	sfRenderWindow_close(This->renderWindow);
}

void windowCreate(Window* This)
{
	sfWindowStyle style = (This->isFullscreen ? sfFullscreen : sfDefaultStyle);

	This->renderWindow = sfRenderWindow_create(This->videoMode, This->windowTitle, style, NULL);
}

void ToggleFullscreen(Window* This)
{
	This->isFullscreen = !This->isFullscreen;
	windowDestroy(This);
	windowCreate(This);
}

void windowInit(Window* This) // useless because stateInit() is called in changeState()
{
	stateInit(This);
}

void windowUpdate(Window* This)
{
	static sfBool first = sfFalse;
	if (!first) {
		stateInit(This);
		first = sfTrue;
	}

	restartClock();
	while (sfRenderWindow_pollEvent(This->renderWindow, &This->event))
	{
		if (This->event.type == sfEvtClosed)
			This->isDone = sfTrue;

		for (int i = 0; i < /*8*//*nbPlayer*/MAX_PLAYER; i++)
		{
			GamepadManager(i, This->event);
		}
		stateEventUpdate(This, &This->event);
	}
	if (sfKeyboard_isKeyPressed(sfKeyF11))
	{
		ToggleFullscreen(This);
	}
	if (sfKeyboard_isKeyPressed(sfKeyF10))
	{
		screenshot(This->renderWindow);
	}

	// MAINVIEW 
	sfView_setCenter(mainView->view, mainView->PosView);
	
	stateUpdate(This);
}

void windowDraw(Window* This)
{
	sfRenderWindow_setView(This->renderWindow, mainView->view);
	stateDisplay(This);

	sfRenderTexture_display(This->renderTexture);
	allTextures = sfRenderTexture_getTexture(This->renderTexture);
	sfSprite_setTexture(allSprites, allTextures, sfFalse);

	sfRenderWindow_clear(This->renderWindow, sfBlack);
	sfRenderWindow_drawSprite(This->renderWindow, allSprites, NULL);
}