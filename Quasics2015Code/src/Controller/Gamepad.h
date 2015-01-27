/*
 * Gamepad.h
 *
 *  Created on: Jan 23, 2015
 *      Author: ray
 */

#ifndef SRC_CONTROLLER_GAMEPAD_H_
#define SRC_CONTROLLER_GAMEPAD_H_

#include "WPILib.h"
#include <math.h>

class Gamepad {
public:
	Gamepad(int input, float DeadbandWidth);

	enum AxisType {
		LeftStickX, LeftStickY, RightStickX, RightStickY, Trigger, DPadX
	};
	enum ButtonType {
		A,
		B,
		X,
		Y,
		LeftShoulder,
		RightShoulder,
		LeftStick,
		RightStick,
		Back,
		Start
	};

	void SetDeadband(float NewDeadbandWidth);
	float GetAxis(AxisType axisGet);
	bool GetButton(ButtonType buttonGet);

private:
	Joystick stick;

	float deadbandWidth;
};

#endif /* SRC_CONTROLLER_GAMEPAD_H_ */