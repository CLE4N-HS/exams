#pragma once
#include "tools.h"


// Variables
typedef struct 
{
	sfRenderWindow* renderWindow;
	sfRenderTexture* renderTexture;
	sfBool isDone;
	sfBool isFullscreen;
	char* windowTitle;
	sfVideoMode videoMode;
	sfVector2i defaultVideoMode;
	sfEvent event;
}Window;


// Prototypes

/// <summary>
/// Checks whether the window should continue to work
/// </summary>
/// <param name="This">Current Window</param>
/// <returns>sfTrue if needed to stop, sfFalse otherwise</returns>
sfBool IsDone(Window* This);

/// <summary>
/// Checks if the Window is currently in Full Screen
/// </summary>
/// <param name="This">Current Window</param>
/// <returns>sfTrue if Full Screen, sfFalse otherwise</returns>
sfBool IsFullscreen(Window* This);

/// <summary>
/// Called every frame, clears the Window
/// </summary>
/// <param name="This">Current Window</param>
void BeginDraw(Window* This); // Clear the window.

/// <summary>
/// Called every frame, displays what is needed on the Window
/// </summary>
/// <param name="This">Current Window</param>
void EndDraw(Window* This); // Display the changes.

/// <summary>
/// Setups a Window
/// </summary>
/// <param name="_Title">Title of the Window</param>
/// <param name="_defaultDesktopMode">set a default Desktop mode, default (covers the entire screen) -> {1920, 1080}</param>
/// <returns>The whole structure of the Window</returns>
Window* windowSetup(const char* _Title, sfVector2i _defaultVideoMode);

/// <summary>
/// Destroys a Window
/// </summary>
/// <param name="This">The whole structure of the Window</param>
void windowDestroy(Window* This);

/// <summary>
/// Creates a Window
/// </summary>
/// <param name="This">The whole structure of the Window</param>
void windowCreate(Window* This);

/// <summary>
/// Activates Full Screen mode, if it already Full Screen activates Default Style
/// </summary>
/// <param name="This">The whole structure of the Window</param>
void ToggleFullscreen(Window* This);

/// <summary>
/// Called every frame, initializes everything needed
/// </summary>
/// <param name="This">The whole structure of the Window</param>
void windowInit(Window* This);

/// <summary>
/// Called every frame, updates everything needed
/// </summary>
/// <param name="This">The whole structure of the Window</param>
void windowUpdate(Window* This);

/// <summary>
/// Called every frame, displays evertything needed
/// </summary>
/// <param name="This">The whole structure of the Window</param>
void windowDraw(Window* This);
