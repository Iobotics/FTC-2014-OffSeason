/**
 * Common power scaling functions
 *
 *  To use:
 *   1. include "PowerScale.c" in main program
 *   2. add directory containing PowerScale.c in the compiler include path
 *      View -> Preferences -> Detailed Preferences... -> Compiler (tab) -> Include Directories (tab)
 *			  add to User Include Files (Common) list
 *   3. call getScaledPower(int) in code
 **/

#define MOTOR_MAX   100
#define MOTOR_MIN  -100
#define EXP_FACTOR  1.037003976

int getScaledPower(int value) {
	int res = 0;

	if(value >= 0) {
		res = pow(EXP_FACTOR, value) - 1;
	} else {
		res = -1 * (pow(EXP_FACTOR, -1 * value) - 1);
	}

	if (res > MOTOR_MAX) { res = MOTOR_MAX; }
	if (res < MOTOR_MIN) { res = MOTOR_MIN; }
	return res;
}
