#include "Robot.h"

Robot::Robot() :
		driveBase(FrontLeftTalonPort, FrontRightTalonPort, RearLeftTalonPort,
				RearRightTalonPort, LeftEncoderA, LeftEncoderB, RightEncoderA,
				RightEncoderB, GyroIn), powerPad(GamePadIn, 0.05) {

}

void Robot::RobotInit() {
	printf ("Robot Init\n");
}

void Robot::AutonomousInit() {
	printf ("Auto Start\n");
}

void Robot::AutonomousPeriodic() {
	printf ("Auto Periodic\n");
}

void Robot::TeleopInit() {
	printf ("Teleop Start\n");
}

void Robot::TeleopPeriodic() {
	if (powerPad.GetButton(Gamepad::LeftShoulder) == true || powerPad.GetButton(Gamepad::RightShoulder) == true){
		driveBase.SetDrivePower(powerPad.GetAxis(Gamepad::LeftStickY) * .5, powerPad.GetAxis(Gamepad::RightStickY) * .5);
	}
	else if (powerPad.GetButton(Gamepad::LeftTrigger) == true || powerPad.GetButton(Gamepad::RightTrigger) == true){
		driveBase.SetDrivePower(powerPad.GetAxis(Gamepad::LeftStickY), powerPad.GetAxis(Gamepad::RightStickY));

	}
	else {
		driveBase.SetDrivePower(powerPad.GetAxis(Gamepad::LeftStickY) * .75, powerPad.GetAxis(Gamepad::RightStickY) * .75);
	}
}

void Robot::TestPeriodic() {

}

START_ROBOT_CLASS(Robot);
