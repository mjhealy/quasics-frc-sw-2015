/*
 * DriveTrain.cpp
 *
 *  Created on: Jan 24, 2015
 *      Author: raymond healy
 */

#include "DriveTrain.h"

DriveTrain::DriveTrain(int fLPort, int fRPort, int rLPort, int rRPort,
		int lEncoderPortA, int lEncoderPortB, int rEncoderPortA,
		int rEncoderPortB, int gyroPort) :
		AutoStatus(Disabled),TargetDegrees(0), TargetDistanceIn(0),

		leftFront(fLPort), leftRear(rLPort), rightFront(fRPort), rightRear(
				rRPort),

				leftDist(lEncoderPortA), rightDist(rEncoderPortA), leftTrim(lEncoderPortB), rightTrim(rEncoderPortB),

		gyro(gyroPort)

{

}

//Functions
//Teleop Power Setting
/*Tank Drive
 * Set the power to both sides manually
 */
void DriveTrain::SetDrivePower(float leftDrivePower, float rightDrivePower) {
	if (leftTrim.Get() >= rightTrim.Get()){
		leftFront.Set(leftDrivePower * (rightTrim.Get() / leftTrim.Get()));
		leftRear.Set(leftDrivePower * (rightTrim.Get() / leftTrim.Get()));
		rightFront.Set(-rightDrivePower);
		rightRear.Set(-rightDrivePower);
	}
	else{
		leftFront.Set(leftDrivePower);
		leftRear.Set(leftDrivePower);
		rightFront.Set(-rightDrivePower * (leftTrim.Get() / rightTrim.Get()));
		rightRear.Set(-rightDrivePower * (leftTrim.Get() / rightTrim.Get()));
	}

}
/*FPS Drive
 * Use 2 Axes, One for Throttle and one for Yaw Control
 *
 * Yaw modifier modeled off of the equation Y = -|2X| + 1, converted, in this case to
 * left_scale = 2(rudder_axis) + upper_bound and right_scale = 2(rudder_axis) + upper_bound
 * where upper_bound = 1
 */
void DriveTrain::FPSDrive(float throttlePower, float sideScale){
	float leftScale = 1;
	float rightScale = 1;

	if (sideScale >= 0){
		rightScale = -2 * sideScale + 1;
	}
	else {
		leftScale = 2 * sideScale + 1;
	}
	leftFront.Set (throttlePower * leftScale);
	leftRear.Set (throttlePower* leftScale);
	rightFront.Set (-throttlePower * rightScale);
	rightRear.Set (-throttlePower * rightScale);
	//SetDrivePower (throttlePower * leftScale, throttlePower * rightScale);
}

//Auto mode Power Setting
void DriveTrain::AutoDriveStart(float distanceIn) {
	leftDist.Reset();
	rightDist.Reset();
	TargetDistanceIn = distanceIn;
	AutoStatus = Driving;
}
void DriveTrain::AutoTurnStart(float degrees) {
	gyro.Reset();
	TargetDegrees = degrees;
	AutoStatus = Turning;
}
void DriveTrain::AutoProcess() {
	switch (AutoStatus){
	case Driving:
		if (leftDist.Get() * InPerTick >= TargetDistanceIn){
			leftFront.Set (0);
			leftRear.Set (0);
		}
		else if (leftDist.Get() * InPerTick > TargetDistanceIn - .5 && leftDist.Get() * InPerTick < TargetDistanceIn){
			leftFront.Set (.25);
			leftRear.Set (.25);
		}
		else{
			leftFront.Set (.75);
			leftRear.Set (.75);
		}

		if (rightDist.Get() * InPerTick >= TargetDistanceIn){
				rightFront.Set (0);
				rightRear.Set (0);
				TargetDistanceIn = 0;
				AutoStatus = Ready;
			}
			else if (leftDist.Get() * InPerTick > TargetDistanceIn - .5 && leftDist.Get() * InPerTick < TargetDistanceIn){
				rightFront.Set (-.25);
				rightRear.Set (-.25);
			}
			else{
				rightFront.Set (-.75);
				rightRear.Set (-.75);
			}
		break;

	case Turning:
		if (TargetDegrees > 0 && TargetDegrees <= 180){
			if (gyro.GetAngle() <= TargetDegrees - 2.5){
				leftFront.Set (.75);
				leftRear.Set (.75);
				rightFront.Set (.75);
				rightRear.Set (.75);
			}
			else if (gyro.GetAngle() > TargetDegrees - 2.5 && gyro.GetAngle() < TargetDegrees){
				leftFront.Set (.25);
				leftRear.Set (.25);
				rightFront.Set (.25);
				rightRear.Set (.25);
			}
			else
			{
				leftFront.Set (0);
				leftRear.Set (0);
				rightFront.Set (0);
				rightRear.Set (0);
				AutoStatus = Ready;
				TargetDegrees = 0;
			}
		}
		else {
			if (gyro.GetAngle() >= TargetDegrees + 2.5){
				leftFront.Set (-.75);
				leftRear.Set (-.75);
				rightFront.Set (-.75);
				rightRear.Set (-.75);
			}
			else if (gyro.GetAngle() < TargetDegrees + 2.5 && gyro.GetAngle() > TargetDegrees){
				leftFront.Set (-.25);
				leftRear.Set (-.25);
				rightFront.Set (-.25);
				rightRear.Set (-.25);
			}
			else {
				leftFront.Set (0);
				leftRear.Set (0);
				rightFront.Set (0);
				rightRear.Set (0);
				AutoStatus = Ready;
				TargetDegrees = 0;
			}
		}
		break;
	default:
		break;

	}
}

//Sensors
void DriveTrain::ResetSensor(driveSensor whichSensor) {
	switch (whichSensor){
	case LeftEncoder:
		leftDist.Reset();
		break;
	case RightEncoder:
		rightDist.Reset();
		break;
	case Gyroscope:
		gyro.Reset();
		break;
	}
}
float DriveTrain::GetSensorValue(driveSensor whichSensor) {
	switch (whichSensor) {
	case LeftEncoder:
		return leftDist.Get();
	case RightEncoder:
		return rightDist.Get();
	case Gyroscope:
		return gyro.GetAngle();
	default:
		return 0;
	}
}
float DriveTrain::GetSpeed(driveSide whichSide, speedUnit whichSpeed) {
	return 0;//leave for end
}
float DriveTrain::GetLeftDistanceIn (){
	return (leftDist.Get() * InPerTick);
}
float DriveTrain::GetRightDistanceIn (){
	return (rightDist.Get() * InPerTick);
}

//Misc
bool DriveTrain::AutoTurning() {
		if (AutoStatus == Turning){
			return true;
		}
		else{
			return false;
		}
}
bool DriveTrain::AutoDriving() {
	if (AutoStatus == Driving){
				return true;
			}
			else{
				return false;
			}
}
void DriveTrain::EndDriveAuto() {
	AutoStatus = Disabled;
}
