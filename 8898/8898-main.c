#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     motorRF,       tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     motorRM,       tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     motorRR,       tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     motorLF,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     motorLM,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorLR,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     motorIntake,   tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_2,     motorArm,      tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S2_C1_1,    servoHopper,          tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "lib\PowerScale.c"
#include "lib\4211Lib_PID.h"

#define MOTOR_TICKS_PER_REV		1120

// ARM constants and state //
#define ARM_GEAR_REDUCTION		8
#define ARM_TICKS_TO_DEGREES 	(360.0 / (MOTOR_TICKS_PER_REV * ARM_GEAR_REDUCTION))
#define ARM_MAX_DEGREES				118
#define ARM_MIN_DEGREES				0
#define ARM_MAX_POWER_UP			100
#define ARM_MAX_POWER_DOWN    -10
#define ARM_KP 	5.0
#define ARM_KI 	0.0
#define ARM_KD 	0.0

float armDegrees, armPIDOutput, armPower;
PIDRefrence armPID;

// HOPPER constants //
#define HOPPER_SERVO_DEGREES	180
#define HOPPER_MAX_DEGREES 		90
#define HOPPER_MIN_DEGREES 		0

// control function prototypes //
void Drive_setPower(int left, int right);
void Arm_setPosition(int degrees);
void Intake_setPower(int power);
void Hopper_setTilt(int degrees);

void Robot_initialize() {
	// initialize the arm //
	nMotorEncoder[motorArm] = 0;
	armPID = addNewPID(ARM_KP, ARM_KI, ARM_KD, &armDegrees, &armPIDOutput);

  // start PID //
	setPIDTaskSettings(Hz_200, T3);
	startTask(pidHandler);
	while(!isPIDTaskReady) { } // wait for PID task to start //
}

task main() {
	Robot_initialize();
	//waitForStart();

	while(true) {
		getJoystickSettings(joystick);
		Drive_setPower(getScaledPower(joystick.joy1_y1), getScaledPower(joystick.joy1_y2));

		armDegrees = - nMotorEncoder[motorArm] * ARM_TICKS_TO_DEGREES;
		armPower   = trim(armPIDOutput, ARM_MAX_POWER_UP, ARM_MAX_POWER_DOWN);
		motor[motorArm] = armPower;

		if(joy1Btn(6)) {
			Arm_setPosition(60);
		} else if(joy1Btn(5)) {
			Arm_setPosition(30);
		} else if(joy1Btn(7)) {
			Arm_setPosition(120)
	  } else if(joy1Btn(8)) {
	  	Arm_setPosition(0);
		}

		/*if(joy1Btn(5)) {
			Intake_setPower(100);
		} else if(joy1Btn(7)) {
			Intake_setPower(-100);
		} else {
			Intake_setPower(0);
		}*/

		if(joy1Btn(4)) {
			Hopper_setTilt(20);
		} else {
			Hopper_setTilt(0);
		}
	}
}

void Drive_setPower(int left, int right) {
	left  = trim(left,  100, -100);
	right = trim(right, 100, -100);
	motor[motorLF] = left;
	motor[motorLM] = left;
	motor[motorLR] = left;
	motor[motorRF] = right;
	motor[motorRM] = right;
	motor[motorRR] = right;
}

void Arm_setPosition(int degrees) {
	degrees = trim(degrees, ARM_MAX_DEGREES, ARM_MIN_DEGREES);
	setPIDTarget(armPID, degrees);
}

void Intake_setPower(int power) {
	power = trim(power, 100, -100);
	motor[motorIntake] = power;
}

void Hopper_setTilt(int degrees) {
	degrees = trim(degrees, HOPPER_MAX_DEGREES, HOPPER_MIN_DEGREES);
	servo[servoHopper] = degrees * 255 / HOPPER_SERVO_DEGREES;
}
