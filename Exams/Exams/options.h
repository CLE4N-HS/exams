#pragma once
#include "tools.h"
#include "windowManager.h"



/// <summary>
/// Initializes everything needed for the Options
/// </summary>
/// <param name="_window">The whole structure of the Window</param>
void initOptions(Window* _window);

/// <summary>
/// Updates everything needed for the Options
/// </summary>
/// <param name="_window">The whole structure of the Window</param>
void updateOptions(Window* _window);

/// <summary>
/// Displays everything needed for the Options
/// </summary>
/// <param name="_window">The whole structure of the Window</param>
void displayOptions(Window* _window);

/// <summary>
/// Removes from the memory everything that was needed for the Options
/// </summary>
void deinitOptions();
