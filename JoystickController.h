#ifndef JOYSTICKCONTROLLER_H_
#define JOYSTICKCONTROLLER_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define EXPORT_FILE "/sys/class/gpio/export"
#define GPIO_PIN_FILEPATH "/sys/class/gpio/gpio"
#define JOYSTICK_UP_GPIO 26
#define JOYSTICK_DOWN_GPIO 46
#define JOYSTICK_RIGHT_GPIO 47
#define JOYSTICK_LEFT_GPIO 65
#define JOYSTICK_PUSH_GPIO 27
#define JOYSTICK_SIZE 5
#define STR_LENGTH 256

// Setup connection to gpio pins
void setupGPIO();

// Check values of joystick
_Bool checkJoystickUpValues();
_Bool checkJoystickDownValues();
_Bool checkJoystickLeftValues();
_Bool checkJoystickRightValues();
_Bool checkJoystickPushValues();

// Open value file and retrieve value
_Bool checkJoystickValues(int pin);

// Write pin number to export file
void writeGPIOExport();

// Write direction to gpio pins
void setPinInputs();

#endif /* JOYSTICKCONTROLLER_H_ */
