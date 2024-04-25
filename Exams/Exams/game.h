#pragma once
#include "tools.h"
#include "windowManager.h"


typedef struct Leaderboard {
	int ldScore;
	char ldName[30];
}Leaderboard;
Leaderboard ld[5];

float gameTime;

/// <summary>
/// Initializes everything needed for the Game
/// </summary>
/// <param name="_window">The whole structure of the Window</param>
void initGame(Window* _window);

/// <summary>
/// Updates everything needed for the Game
/// </summary>
/// <param name="_window">The whole structure of the Window</param>
void updateGame(Window* _window);

/// <summary>
/// Displays everything needed for the Game
/// </summary>
/// <param name="_window">The whole structure of the Window</param>
void displayGame(Window* _window);

/// <summary>
/// Removes from the memory everything that was needed for the Game
/// </summary>
void deinitGame();

void addCoin();

void addLife();

void saveLeaderboard();

void loadLeaderboard();

void sortLeaderboard();
