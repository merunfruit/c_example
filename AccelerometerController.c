#include "AccelerometerController.h"

void setupAccelerometer() {
	writeI2CReg(i2cFileDesc, ACCELEROMETER_ADDRESS_CTRL, 0x01);
}

accel_t readAccelerometerValue() {
	int size = 7;
	unsigned char *buff = malloc(size*sizeof(char));

	readI2CReg(i2cFileDesc, REG_XMSB, buff, size);

	int16_t x = ((buff[REG_XMSB] << 8) | (buff[REG_XLSB])) >> 4;
	int16_t y = ((buff[REG_YMSB] << 8) | (buff[REG_YLSB])) >> 4;
	int16_t z = ((buff[REG_ZMSB] << 8) | (buff[REG_ZLSB])) >> 4;

	accel_t res;
	res.x = x;
	res.y = y;
	res.z = z;

	free(buff);
	return res;
}
