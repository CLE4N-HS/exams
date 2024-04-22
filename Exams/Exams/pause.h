#pragma once
#include "tools.h"
#include "windowManager.h"

/// <summary>
/// Initializes everything needed for the Pause
/// </summary>
/// <param name="_window">The whole structure of the Window</param>
void initPause(Window* _window);

/// <summary>
/// Updates everything needed for the Pause
/// </summary>
/// <param name="_window">The whole structure of the Window</param>
void updatePause(Window* _window);

/// <summary>
/// Displays everything needed for the Pause
/// </summary>
/// <param name="_window">The whole structure of the Window</param>
void displayPause(Window* _window);

/// <summary>
/// Removes from the memory everything that was needed for the Pause
/// </summary>
void deinitPause();

