#include "JoystickController.h"

int buffer[JOYSTICK_SIZE] = { JOYSTICK_UP_GPIO, JOYSTICK_DOWN_GPIO, JOYSTICK_LEFT_GPIO, JOYSTICK_RIGHT_GPIO, JOYSTICK_PUSH_GPIO };

void setupGPIO() {
	writeGPIOExport();
	setPinInputs();
}

_Bool checkJoystickUpValues() {
	return checkJoystickValues(JOYSTICK_UP_GPIO);
}

_Bool checkJoystickDownValues() {
	return checkJoystickValues(JOYSTICK_DOWN_GPIO);
}

_Bool checkJoystickLeftValues() {
	return checkJoystickValues(JOYSTICK_LEFT_GPIO);
}

_Bool checkJoystickRightValues() {
	return checkJoystickValues(JOYSTICK_RIGHT_GPIO);
}

_Bool checkJoystickPushValues() {
	return checkJoystickValues(JOYSTICK_PUSH_GPIO);
}

_Bool checkJoystickValues(int pin) {
	char *str = (char *) malloc(sizeof(char) * STR_LENGTH);
	sprintf(str, GPIO_PIN_FILEPATH "%d/value", pin);

	FILE *pfile = fopen(str, "r");
	if (pfile == NULL) {
		printf("GPIO: Unable to open GPIO%d value file.\n", pin);
	}

	//Read file
	int buff = 0;
	fscanf(pfile, "%d", &buff);
	fclose(pfile);

	// 1 for not press, 0 for pressed
	_Bool value = buff;
	free(str);
	return !value;
}

void writeGPIOExport() {
	for(int i = 0; i < JOYSTICK_SIZE; i++) {
		FILE *pfile = fopen(EXPORT_FILE, "w");
		if (pfile == NULL) {
			printf("GPIO: Unable to open export file.\n");
		}
		fprintf(pfile, "%d", buffer[i]);
		fclose(pfile);
	}
}

void setPinInputs() {
	for(int i = 0; i < JOYSTICK_SIZE; i++) {
		char *str = (char *) malloc(sizeof(char) * STR_LENGTH);
		sprintf(str, GPIO_PIN_FILEPATH "%d/direction", buffer[i]);

		FILE *pfile = fopen(str, "w");
		if (pfile == NULL) {
			printf("GPIO: Unable to open GPIO%d direction file.\n", buffer[i]);
		}
		fprintf(pfile, "in");
		fclose(pfile);
		free(str);
	}
}
