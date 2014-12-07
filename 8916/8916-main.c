#pragma config(Hubs,  S1, HTServo,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C2_1,     motorTest,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C1_1,    servoTest,            tServoStandard)
#pragma config(Servo,  srvo_S1_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_3,    tiltLeft,             tServoStandard)
#pragma config(Servo,  srvo_S1_C1_4,    tiltRight,            tServoStandard)
#pragma config(Servo,  srvo_S1_C1_5,    intakeLeft,           tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C1_6,    intakeRight,          tServoContinuousRotation)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "PowerScale.c"

long encoder = 0;

void setDrivePower(int left, int right) {
	motor[motorTest] = left;
}

void setIntakePower(int power) {
	power -= 1;
	int scaled = power * 254 / 128 + 127;

	servo[intakeLeft]  = scaled;
	servo[intakeRight] = 255 - scaled;
}

void setIntakeTilt(int angle) {
  angle -= 1;
	int scaled = angle * 254 / 128 + 127;

	servo[tiltLeft]  = scaled;
	servo[tiltRight] = 255 - scaled;
}

task main() {

	while(true) {
		getJoystickSettings(joystick);
		setDrivePower(getScaledPower(joystick.joy1_y1), getScaledPower(joystick.joy1_y2));
		setIntakePower(joystick.joy1_y2);
		setIntakeTilt(joystick.joy1_x2);

		encoder = nMotorEncoder[motorTest];

		//_previousLatchInput = joy1Btn(8);
/*		if(joy1Btn(6) == 1) motor_six_bar = 100;
		else if(joy1Btn(8) == 1) motor_six_bar = -10;
		else motor_six_bar = 0;

		if(joy1Btn(5) == 1) motor_intake = 100;
		else if(joy1Btn(7) == 1) motor_intake = -100;
		else motor_intake = 0;

		if(joy1Btn(4) == 1 && servo_hopper < 255) servo_hopper += 5;
		else if(joy1Btn(2) == 1 && servo_hopper > 0) servo_hopper -= 5;*/
	}
}