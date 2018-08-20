#ifndef I2CCONTROLLER_H_
#define I2CCONTROLLER_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#define BONE_CAPE_SLOTS "/sys/devices/platform/bone_capemgr/slots"
#define I2C_1 "BB-I2C1"
#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

int initI2CBus(char* bus, int address);
void writeI2CReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);
void readI2CReg(int i2cFileDesc, unsigned char regAddr, unsigned char* buff, int size);

#endif /* I2CCONTROLLER_H_ */
