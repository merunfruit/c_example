#include "I2CController.h"

int initI2CBus(char* bus, int address) {
	// Initialize bus
	int i2cFileDesc = open(bus, O_RDWR);
	if (i2cFileDesc < 0) {
		printf("I2C: Unable to open bus for read/write (%s)\n", bus);
		perror("Error is:");
		exit(1);
	}

	int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
	if (result < 0) {
		perror("I2C: Unable to set I2C device to slave address");
		exit(1);
	}
	return i2cFileDesc;
}

void writeI2CReg(int i2cFileDesc, unsigned char regAddr, unsigned char value) {
	unsigned char buff[2] = {regAddr, value};
	int res = write(i2cFileDesc, buff, 2);
	if (res != 2) {
		perror("I2C: Unable to write to I2C register");
		exit(1);
	}
}

// Read multiple bytes
void readI2CReg(int i2cFileDesc, unsigned char regAddr, unsigned char* buff, int size) {
	// To read a register, must first write the address
	int res = write(i2cFileDesc, &regAddr, 1);
	if(res != 1) {
		perror("I2C: Unable to write to I2C register");
		exit(1);
	}

	// Now read the value and return it
	res = read(i2cFileDesc, buff, size*sizeof(char));
	if(res != size*sizeof(char)) {
		perror("I2C: Unable to read from I2C register");
		exit(1);
	}
}
