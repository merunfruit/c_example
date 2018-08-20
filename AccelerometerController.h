#ifndef ACCELEROMETERCONTROLLER_H_
#define ACCELEROMETERCONTROLLER_H_

#include <stdlib.h>
#include <stdint.h>
#include "I2CController.h"

#define ACCELEROMETER_ADDRESS 0x1C
#define ACCELEROMETER_ADDRESS_CTRL 0x2A
#define REG_CTRL1 0x2A
#define REG_STATUS 0x00
#define REG_XMSB 0x01
#define REG_XLSB 0x02
#define REG_YMSB 0x03
#define REG_YLSB 0x04
#define REG_ZMSB 0x05
#define REG_ZLSB 0x06

typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} accel_t;

void setupAccelerometer();
accel_t readAccelerometerValue();

extern int i2cFileDesc;

#endif /* ACCELEROMETERCONTROLLER_H_ */
