#pragma once
#include "tools.h"
#include "windowManager.h"


typedef enum 
{
	RTNULL = -1, 
	ALL,
	INTRO,
	MENU,
	GAME,
	EDITOR,
	END
}State;


typedef struct 
{
	sfBool state;
	sfMutex* mutex;
}watcher;

watcher w;

static State state = MENU;

static sfBool onePass = sfFalse;
static sfBool isPaused = sfFalse;
static sfBool isOption = sfFalse;

sfThread* loadingThread;
sfSprite* spLoading;

/// <summary>
/// Called every frame, at first inits all necessary things, then inits the state you'll be in
/// </summary>
/// <param name="_window">Current window</param>
void stateInit(Window* _window);

/// <summary>
/// Called every frame if there's an event
/// </summary>
/// <param name="_window">Current window</param>
/// <param name="_event">Event</param>
void stateEventUpdate(Window* _window, sfEvent* _event);

/// <summary>
/// Called every frame, only updates the state that you're in, or updates the loading screen
/// </summary>
/// <param name="_window">Current window</param>
void stateUpdate(Window* _window);

/// <summary>
/// Called every frame, only displays the state that you're in, or displays the loading screen
/// </summary>
/// <param name="_window"></param>
void stateDisplay(Window* _window);

/// <summary>
/// Deinits the state that you were in so that it dosn't stay in the memory
/// </summary>
/// <param name="_window">Current window</param>
void stateDeinit(Window* _window);

/// <summary>
/// Call this function when you want to change state, it will deinits the current state and inits the wanted state
/// </summary>
/// <param name="_window">Current window</param>
/// <param name="_state">Wanted state</param>
void changeState(Window* _window, State _state);

State getState();

/// <summary>
/// Pauses the game, or restarts the game if it was already paused
/// </summary>
void togglePause();

/// <summary>
/// Activates the options, or deactivates them if they were already present
/// </summary>
void toggleOptions();
