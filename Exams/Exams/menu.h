#pragma once
#include "tools.h"
#include "stateManager.h"
#include "windowManager.h"

sfFont* fontScore;

/// <summary>
/// Initializes everything needed for the Menu
/// </summary>
/// <param name="_window">The whole structure of the Window</param>
void initMenu(Window* _window);

/// <summary>
/// Udpates everything needed for the Menu
/// </summary>
/// <param name="_window">The whole structure of the Window</param>
void updateMenu(Window* _window);

/// <summary>
/// Displays everything needed for the Menu
/// </summary>
/// <param name="_window">The whole structure of the Window</param>
void displayMenu(Window* _window);

/// <summary>
/// Removes from the memory everything that was needed for the Menu
/// </summary>
void deinitMenu();

