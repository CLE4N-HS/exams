#include "Gamepad.h"
#include "stateManager.h"
#include "dialogBox.h"

#define IS_BUTTON (_stick == LB_XBOX || _stick == RB_XBOX) || _stick >= STICKLX_XBOX && _stick < TRIGGER_R2_XBOX

void GamepadDetection()
{
	sfJoystick_update();
	int playernber = 0;
	for (unsigned int i = 0; i < /*8*/MAX_PLAYER; i++)
	{
		if (playernber >= MAX_PLAYER) break;
		if (sfJoystick_isConnected(i))
		{
			sfJoystickIdentification joyid = sfJoystick_getIdentification(i);
			if (joyid.productId == 654 || joyid.productId == 767 && joyid.vendorId == 1118) // || joyid.productId == 767 for my controller
			{
				//player[playernber].idGamepad = i;
				playernber++;
			}
		}
		else
		{
			//player[playernber].idGamepad = -1;
		}
	}
	nbPlayer = playernber;
}

void GamepadManager(int _joystickId, sfEvent event)
{

	//// to remove
	//sfJoystick_update();
	//
	//
	//setVibration(_joystickId, 0.f, 0.f);
	//
	//if (sfJoystick_isButtonPressed(_joystickId, 4)) {
	//	setVibration(_joystickId, 1.f, 0.f);
	//}
	//if (sfJoystick_isButtonPressed(_joystickId, 5)) {
	//	setVibration(_joystickId, 0.f, 1.f);
	//}
	//if (sfJoystick_isButtonPressed(_joystickId, 4) && sfJoystick_isButtonPressed(_joystickId, 5)) {
	//	setVibration(_joystickId, 1.f, 1.f);
	//}
	//
	//
	//if (sfJoystick_isButtonPressed(0, 1)) {
	//	printf("0");
	//}
	//if (sfJoystick_isButtonPressed(1, 1)) {
	//	printf("1");
	//}
	//if (sfJoystick_isButtonPressed(2, 1)) {
	//	printf("2");
	//}
	////

	if (event.type == sfEvtJoystickConnected)
	{
		printf("Gamepad is Connected id %d\n", event.joystickConnect.joystickId);
		GamepadDetection();
	}
	if (event.type == sfEvtJoystickDisconnected)
	{
		printf("Gamepad is Disconnected id %d\n", event.joystickConnect.joystickId);
		// TODO : Mettre en pause pour indiquer qu'un (ou plusieurs) Gamepad est déconnecté
		if (!isPaused && getState() == GAME) {
			togglePause();
			CreateDialogBox(INFO, "A controller was disconnected", 0);
		}
			
	}
	if (event.joystickButton.joystickId == _joystickId)
	{
		if (event.joystickMove.axis == sfJoystickX) // StickL 1 X
		{
			if (event.joystickMove.position > 10.0f || event.joystickMove.position < -10.0f)
			{
				Gamepad[_joystickId].stickL.x = event.joystickMove.position;
			}
			else
			{
				Gamepad[_joystickId].stickL.x = 0.0f;
			}
		}
		if (event.joystickMove.axis == sfJoystickY) // StickL 1 Y
		{
			if (event.joystickMove.position > 10.0f || event.joystickMove.position < -10.0f)
			{
				Gamepad[_joystickId].stickL.y = event.joystickMove.position;
			}
			else
			{
				Gamepad[_joystickId].stickL.y = 0.0f;
			}
		}
		if (event.joystickMove.axis == sfJoystickU) // StickR 2 X
		{
			if (event.joystickMove.position > 10.0f || event.joystickMove.position < -10.0f)
			{
				Gamepad[_joystickId].stickR.x = event.joystickMove.position;
			}
			else
			{
				Gamepad[_joystickId].stickR.x = 0.0f;
			}
		}
		if (event.joystickMove.axis == sfJoystickV) // StickR 2 Y
		{
			if (event.joystickMove.position > 10.0f || event.joystickMove.position < -10.0f)
			{
				Gamepad[_joystickId].stickR.y = event.joystickMove.position;
			}
			else
			{
				Gamepad[_joystickId].stickR.y = 0.0f;
			}
		}
		if (event.joystickMove.axis == sfJoystickPovX) // Cross X
		{
			if (event.joystickMove.position > 10.0f || event.joystickMove.position < -10.0f)
			{
				Gamepad[_joystickId].cross.x = event.joystickMove.position;
			}
			else
			{
				Gamepad[_joystickId].cross.x = 0.0f;
			}
		}
		if (event.joystickMove.axis == sfJoystickPovY) // Cross Y
		{
			if (event.joystickMove.position > 10.0f || event.joystickMove.position < -10.0f)
			{
				Gamepad[_joystickId].cross.y = event.joystickMove.position;
			}
			else
			{
				Gamepad[_joystickId].cross.y = 0.0f;
			}
		}

		if (event.joystickMove.axis == sfJoystickZ) // L2 et R2
		{
			if (event.joystickMove.position > 1.0f)
			{
				Gamepad[_joystickId].TriggerL = event.joystickMove.position;
			}
			else
			{
				Gamepad[_joystickId].TriggerL = 0.0f;
			}
			if (event.joystickMove.position < -1.0f)
			{
				Gamepad[_joystickId].TriggerR = -event.joystickMove.position;
			}
			else
			{
				Gamepad[_joystickId].TriggerR = 0.0f;
			}
		}

		if (event.type == sfEvtJoystickButtonPressed)
		{

			for (int i = 0; i < 14; i++)
			{
				if (event.joystickButton.button == i)
				{
					Gamepad[_joystickId].Bouton[i] = sfTrue;
				}
			}
		}

		if (event.type == sfEvtJoystickButtonReleased)
		{
			for (int i = 0; i < 14; i++)
			{
				if (event.joystickButton.button == i)
				{
					Gamepad[_joystickId].Bouton[i] = sfFalse;
				}
			}
		}

	}
}

sfBool Gamepad_isButtonPressed(unsigned int _joystickId, gamepadPS4 _button)
{
	return Gamepad[_joystickId].Bouton[_button];
}

float Gamepad_isJoystickMoved(unsigned int _joystickId, gamepadPS4 _Axis)
{
	switch (_Axis)
	{
	case STICKLX:
		return Gamepad[_joystickId].stickL.x;
		break;
	case STICKLY:
		return Gamepad[_joystickId].stickL.y;
		break;
	case CROSSX:
		return Gamepad[_joystickId].cross.x;
		break;
	case CROSSY:
		return Gamepad[_joystickId].cross.y;
		break;
	case STICKRX:
		return Gamepad[_joystickId].stickR.x;
		break;
	case STICKRY:
		return Gamepad[_joystickId].stickR.y;
		break;
	case TRIGGER_R2:
		return Gamepad[_joystickId].TriggerR;
		break;
	case TRIGGER_L2:
		return Gamepad[_joystickId].TriggerL;
		break;
	default:
		return 0.0f;
		break;
	}
}

int NbConnectedControllers()
{
	int nb = 0;
	for (unsigned int i = 0; i < MAX_PLAYER; i++) {
		if (sfJoystick_isConnected(i))
			nb++;
	}
	return nb;
}

sfBool isKeyboardOrControllerButtonPressed(sfKeyCode _key, gamepadXBox _button)
{
	if (sfKeyboard_isKeyPressed(_key))
		return sfTrue;

	for (int i = 0; i < nbPlayer; i++)
	{
		if (_button == LB_XBOX || _button == RB_XBOX || _button >= STICKLX_XBOX) {
			printf("isKeyboardOrControllerButtonPressed() cannot support sticks or triggers\n");
			return sfFalse;
		}
		else {
			if (Gamepad_isButtonPressed(i, _button))
				return sfTrue;
		}

	}

	return sfFalse;
}

sfBool isKeyboardOrControllerButtonMoved(sfKeyCode _key, gamepadXBox _stick, sfBool _upOrLeft, float _offsetAxis)
{
	if (sfKeyboard_isKeyPressed(_key))
		return sfTrue;

	for (int i = 0; i < nbPlayer; i++) {
		if (IS_BUTTON) {

			if (_upOrLeft) {
				if (Gamepad_isJoystickMoved(i, _stick) < -_offsetAxis)
					return sfTrue;

				// Only for left stick

				if (_stick == STICKLX_XBOX) {
					if (Gamepad_isJoystickMoved(i, CROSSX_XBOX) > 0) // Left Cross
						return sfTrue;
				}
				else if (_stick == STICKLY_XBOX) {
					if (Gamepad_isJoystickMoved(i, CROSSY_XBOX) > 0) // Up Cross
						return sfTrue;
				}

			}
			else {
				if (Gamepad_isJoystickMoved(i, _stick) > _offsetAxis)
					return sfTrue;

				// Only for left stick

				if (_stick == STICKLX_XBOX) {
					if (Gamepad_isJoystickMoved(i, CROSSX_XBOX) < 0) // Right Cross
						return sfTrue;
				}
				else if (_stick == STICKLY_XBOX) {
					if (Gamepad_isJoystickMoved(i, CROSSY_XBOX) < 0) // Down Cross
						return sfTrue;
				}
			}

		}
		else {
			printf("isKeyboardOrControllerButtonMoved() cannot support buttons or triggers\n");
			return sfFalse;
		}

	}

	return sfFalse;
}

void forceReleasedButton(gamepadXBox _button)
{
	for (int i = 0; i < nbPlayer; i++) {
		Gamepad[i].Bouton[_button] = sfFalse;
	}
}
