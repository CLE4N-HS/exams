#pragma once

#include "SFML/Graphics.h"


// Typedefs
typedef unsigned int    t_joyNum;
typedef unsigned short  t_buttonNum;

// Enums
enum {
    Count = 4       // Player 0-3
};

enum {
    DPAD_UP = 0x0001,
    DPAD_DOWN = 0x0002,
    DPAD_LEFT = 0x0004,
    DPAD_RIGHT = 0x0008,
    START = 0x0010,
    BACK = 0x0020,
    LEFT_THUMB = 0x0040,
    RIGHT_THUMB = 0x0080,
    LB = 0x0100,
    RB = 0x0200,
    A = 0x1000,
    B = 0x2000,
    X = 0x4000,
    Y = 0x8000,
};

/// <summary>
/// Checks if a controller is connected
/// </summary>
/// <param name="joyNum">The controller's ID</param>
/// <returns>sfTrue if the controller is connected, sfFalse otherwise</returns>
sfBool isConnected(t_joyNum joyNum);

/// <summary>
/// Gets the max number of buttons on a controller
/// </summary>
/// <param name="joyNum">The controller's ID</param>
/// <returns>The number of buttons on the controller</returns>
unsigned int getButtonCount(t_joyNum joyNum);

/// <summary>
/// Checks if button is pressed on a controller
/// </summary>
/// <param name="joyNum">The controller's ID</param>
/// <param name="buttonNum">The button's name</param>
/// <returns>sfTrue is the button is pressed, sfFalse otherwise</returns>
sfBool isButtonPressed(t_joyNum joyNum, t_buttonNum buttonNum);

/// <summary>
/// Checks if at least one of the 4 XBox controller is connected
/// </summary>
/// <returns>sfTrue if at least one of them is connected, sfFalse otherwise</returns>
sfBool isAnyXBox360ControllerConnected();

/// <summary>
/// Gets the values of the triggers and modifies them directly (between 0.f and 1.f)
/// </summary>
/// <param name="joyNum">The controller's ID</param>
/// <param name="left">Left trigger</param>
/// <param name="right">Right trigger</param>
void getTriggers(t_joyNum joyNum, float* left, float* right);

/// <summary>
/// Gets the values of the sticks and modfies them directly (between -100.f and 100.f)
/// </summary>
/// <param name="joyNum">The controller's ID</param>
/// <param name="left">Left stick</param>
/// <param name="right">Right stick</param>
void getSticksPosition(t_joyNum joyNum, sfVector2f* left, sfVector2f* right);

/// <summary>
/// Sets the vibration of a controller (between 0.f and 1.f) (dosn't go back to 0 automatically)
/// </summary>
/// <param name="joyNum">The controller's ID</param>
/// <param name="leftMotor">Vibration intensity for the Left motor</param>
/// <param name="rightMotor">Vibration intensity for the Right motor</param>
void setVibration(t_joyNum joyNum, float leftMotor, float rightMotor);



