#pragma once
#include "tools.h"
#include "stateManager.h"

typedef enum dialBox dialBox;
enum dialBox
{
	INFO,
	ALERT
};

typedef enum {
	PASS_DB,
	BACK_TO_MENU_DB,
	QUIT_TO_DESKTOP_DB
}DBcode;
DBcode DialogBoxCode;

typedef struct stDialogBox stDialogBox;
struct stDialogBox
{
	sfVector2f position;
	sfVector2f size;
	dialBox type;
	sfRectangleShape* dialogshape;
	sfRectangleShape* dialogshapeNo;
	sfRectangleShape* dialogshapeYes;
	char* message;
	sfText* txt;
	sfText* txtYes;
	sfText* txtNo;
};

stDialogBox* currentDialogBox;
sfBool isDialogBox;
sfBool DialogBoxChoice;

/// <summary>
/// Creates a Dialog Box, one at a time ideally
/// </summary>
/// <param name="_type">Predefine the type of the DB</param>
/// <param name="_message">DB message, there's only one lign so you can use "\n" if you need to</param>
/// <param name="_DialogBoxCode">Predefine the purpose of the DB</param>
void CreateDialogBox(dialBox _type, char* _message, DBcode _DialogBoxCode);

/// <summary>
/// Updates DB, interactions and navigation between buttons
/// </summary>
/// <param name="_window">Current window</param>
void dialogBoxUpdate(Window* _window);

/// <summary>
/// Displays DB
/// </summary>
/// <param name="_window">Current window</param>
void dialogBoxDisplay(Window* _window);

/// <summary>
/// Destroys and free DB
/// </summary>
void dialogBoxDestroy();

/// <summary>
/// Manages the outcome of the DB
/// </summary>
/// <param name="_window">Current window</param>
/// <param name="_code">The outcome, decided at the creation (-> dialogBoxCreate())</param>
void DialogBoxManager(Window* _window, DBcode _code);
