#pragma once
#include "tools.h"
#include "gamepadx.h"
//#include "player.h"

#define MAX_PLAYER 2

typedef enum gamepadPS4 gamepadPS4;
enum gamepadPS4
{
	CROIX,
	ROND,
	CARRE,
	TRIANGLE,
	L1,
	R1,
	SHARE,
	OPTION,
	BSTICKL,
	BSTICKR,
	PS,
	BTOUCHPAD,
	L2,
	R2,
	NOTUSED,
	STICKLX,
	STICKLY,
	CROSSX,
	CROSSY,
	STICKRX,
	STICKRY,
	TRIGGER_R2,
	TRIGGER_L2
};

typedef enum {
	A_XBOX,
	B_XBOX,
	X_XBOX,
	Y_XBOX,
	LB_XBOX,
	RB_XBOX,
	SELECT_XBOX,
	START_XBOX,
	LSTICKBUTTON_XBOX,
	RSTICKBUTTON_XBOX,

	STICKLX_XBOX = 15,
	STICKLY_XBOX,
	CROSSX_XBOX,
	CROSSY_XBOX,
	STICKRX_XBOX,
	STICKRY_XBOX,
	TRIGGER_R2_XBOX,
	TRIGGER_L2_XBOX
}gamepadXBox;

typedef struct gamepad gamepad;
struct gamepad
{
	sfVector2f stickL;
	sfVector2f stickR;
	sfVector2f cross;
	float TriggerL;
	float TriggerR;
	sfBool Bouton[14];
};
gamepad Gamepad[/*8*/MAX_PLAYER];

int nbPlayer;


/// <summary>
/// Detects how many controllers are connected to know the number of players
/// </summary>
void GamepadDetection();

/// <summary>
/// Called every frame if there's an event on at least one controller, managages events and stocks the values in the gamepad structure
/// </summary>
/// <param name="_joystickId">The controller's ID</param>
/// <param name="event">The event detected</param>
void GamepadManager(int _joystickId, sfEvent event);

/// <summary>
/// Checks if a controller is currently pressing a button
/// </summary>
/// <param name="_joystickId">The controller's ID</param>
/// <param name="_button">The button's name</param>
/// <returns>sfTrue if the button is currently pressed, sfFalse otherwise</returns>
sfBool Gamepad_isButtonPressed(unsigned int _joystickId, gamepadPS4 _button);

/// <summary>
/// Checks if a controller is currently moving a stick
/// </summary>
/// <param name="_joystickId">The controller's ID</param>
/// <param name="_Axis">The stick and his axis</param>
/// <returns>A float between -100.f and 100.f or 0.f if the stick is not moving</returns>
float Gamepad_isJoystickMoved(unsigned int _joystickId, gamepadPS4 _Axis);

/// <summary>
/// Tells how many controllers are currently connected
/// </summary>
/// <returns>The number of connected controllers according to the maximum number of players</returns>
int NbConnectedControllers();

/// <summary>
/// Checks if a key or a button is currently pressed on all connected controllers
/// </summary>
/// <param name="_key">The key on the keyboard</param>
/// <param name="_button">The button on the controller</param>
/// <returns>sfTrue if at least one is pressed, sfFalse otherwise</returns>
sfBool isKeyboardOrControllerButtonPressed(sfKeyCode _key, gamepadXBox _button);

/// <summary>
/// Checks if a key is currently pressed or a stick is currently moved on all connected controllers. Automaticly checks the Cross too.
/// Recommented to use an offset if you'll be checking other axis later
/// </summary>
/// <param name="_key">The key on the keyboard</param>
/// <param name="_stick">The stick on the controller with his Axis</param>
/// <param name="_upOrLeft">sfTrue is you're checking the Up or Left Axis, sfFalse otherwise</param>
/// <param name="_offsetAxis">Should the Axis have an offset (between 0.f and 100.f) (default value for no offset -> 0.f)</param>
/// <returns>sfTrue if at least one is pressed or moved, sfFalse otherwise</returns>
sfBool isKeyboardOrControllerButtonMoved(sfKeyCode _key, gamepadXBox _stick, sfBool _upOrLeft, float _offsetAxis);

/// <summary>
/// Forces the released state of a button on all controllers
/// </summary>
/// <param name="_button">The button on the controller</param>
void forceReleasedButton(gamepadXBox _button);

