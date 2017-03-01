#include "i2c.h"
#include "wiringPi.h"
#include "wiringPiI2C.h"
#include "common.h"

#include <stdio.h>

void i2cGetXYZ(int fd, int regAddr, uint8_t *buf)
{
	for(int i = 0; i  < 6 ; i ++)
	{
		*buf = (uint8_t)wiringPiI2CReadReg8(fd, regAddr);
#if DEBUG_RAW_DATA == 1
		printf("%X ", *buf);
#endif
		buf++; regAddr++;
	}
#if DEBUG_RAW_DATA == 1
		printf("\n");
#endif
}

void i2cGetXYZInv(int fd, int regAddr, uint8_t *buf)
{
	for(int i = 0; i  < 6 ; i ++)
	{
		if(i % 2 == 0)
			buf[i + 1] = (uint8_t)wiringPiI2CReadReg8(fd, regAddr);
		else
			buf[i - 1] = (uint8_t)wiringPiI2CReadReg8(fd, regAddr);
		regAddr++;
	}
#if DEBUG_RAW_DATA == 1
	for(int i = 0 ; i < 6; i ++)
		printf("%X ", buf[i]);
	printf("\n");
#endif
}