#include <stdint.h>
#include <stdio.h>

#include "common.h"
#include "i2c.h"
#include "MPU9255.h"
#include "wiringPi.h"
#include "wiringPiI2C.h"

avgDigFilter gyroXAvgDigFilter, gyroYAvgDigFilter, gyroZAvgDigFilter;
avgDigFilter accelXAvgDigFilter, accelYAvgDigFilter, accelZAvgDigFilter;

struct XYZData gyroOffset;
struct XYZData accOffset;

void MPU9255_init()
{
	// Enable bypass mode to get data from magnetometer (AK8963). 
	wiringPiI2CWriteReg8(fdMPU9255, MPU9255_R_BYPASS_ENABLE, MPU9255_B_BYPASS_ENABLE);
	
	// Run all.
	wiringPiI2CWriteReg8(fdMPU9255, MPU9255_R_PWR_MGMT1, MPU9255_B_RUN_ALL);
	
	// Sample rate dividing factor set.
	wiringPiI2CWriteReg8(fdMPU9255, MPU9255_R_SMPLRT_DIV, MPU9255_SAMPLE_RATE_DIV_IN_REG);
	
	// Enable DLPF
	wiringPiI2CWriteReg8(fdMPU9255, MPU9255_R_CONFIG, MPU9255_B_BAND50HZ);
	
	// Configure gyrscope.
	wiringPiI2CWriteReg8(fdMPU9255, MPU9255_R_GYRO_CONFIG, MPU9255_B_FULL_SC_1000);
	
	// Configure accelerometer.
	wiringPiI2CWriteReg8(fdMPU9255, MPU9255_R_ACCEL_CONFIG, MPU9255_B_FULL_SC_2G);
	
	// Get accelerometer offset
	getAccOffset();
	
	// Get gyroscope offset
	getGyroOffset();
	
}

void getAccOffset()
{
	int32_t tempAX = 0, tempAY = 0, tempAZ = 0;

	for(int i = 0 ; i < 32; i ++)
	{
		getAccelData();
		
		tempAX += accDataRaw.XYZByStruct.X;
		tempAY += accDataRaw.XYZByStruct.Y;
		tempAZ += accDataRaw.XYZByStruct.Z;
		
		delayMicroseconds(100);
	}
	
	accOffset.X = tempAX >> 5;
	accOffset.Y = tempAY >> 5;
	accOffset.Z = (tempAZ >> 5) - 16384;
}

void getGyroOffset()
{
	int32_t tempGX = 0, tempGY = 0, tempGZ = 0;

	for(int i = 0 ; i < 32; i ++)
	{
		getGyroData();
		
		tempGX += gyroDataRaw.XYZByStruct.X;
		tempGY += gyroDataRaw.XYZByStruct.Y;
		tempGZ += gyroDataRaw.XYZByStruct.Z;
		
		delayMicroseconds(100);
	}
	
	gyroOffset.X = tempGX >> 5;
	gyroOffset.Y = tempGY >> 5;
	gyroOffset.Z = tempGZ >> 5;
	
}

void getGyroData()
{
	int32_t filterResultX, filterResultY, filterResultZ;
	
	i2cGetXYZInv(fdMPU9255, MPU9255_R_GYRO_DATA, gyroDataRaw.XYZByArray);
#if DEBUG_RAW_DATA == 1
	printf("%X %X %X\n", gyroDataRaw.XYZByStruct.X, gyroDataRaw.XYZByStruct.Y, gyroDataRaw.XYZByStruct.Z);
#endif
	calAvgValue(&gyroXAvgDigFilter.index, gyroXAvgDigFilter.avgBuffer, gyroDataRaw.XYZByStruct.X, &filterResultX);
	calAvgValue(&gyroYAvgDigFilter.index, gyroYAvgDigFilter.avgBuffer, gyroDataRaw.XYZByStruct.Y, &filterResultY);
	calAvgValue(&gyroZAvgDigFilter.index, gyroZAvgDigFilter.avgBuffer, gyroDataRaw.XYZByStruct.Z, &filterResultZ);
	
	gyroData.X = filterResultX - gyroOffset.X;
	gyroData.Y = filterResultY - gyroOffset.Y;
	gyroData.Z = filterResultZ - gyroOffset.Z;

#if WEIRDO_PROTHESIS == 1
	gyroData.X /= 33;
	gyroData.Y /= 33;
	gyroData.Z /= 33;
#endif
	
#if DEBUG_AVG_DATA == 1
	printf("%d %d %d\n", gyroData.X, gyroData.Y, gyroData.Z); 
#endif
	
}

void getAccelData()
{
	int32_t filterResultX, filterResultY, filterResultZ;
	
	i2cGetXYZInv(fdMPU9255, MPU9255_R_ACCEL_DATA, accDataRaw.XYZByArray);
#if DEBUG_RAW_ACC_DATA == 1
	printf("%X %X %X\n", accDataRaw.XYZByStruct.X, accDataRaw.XYZByStruct.Y, accDataRaw.XYZByStruct.Z);
#endif
	calAvgValue(&accelXAvgDigFilter.index, accelXAvgDigFilter.avgBuffer, accDataRaw.XYZByStruct.X, &filterResultX);
	calAvgValue(&accelYAvgDigFilter.index, accelYAvgDigFilter.avgBuffer, accDataRaw.XYZByStruct.Y, &filterResultY);
	calAvgValue(&accelZAvgDigFilter.index, accelZAvgDigFilter.avgBuffer, accDataRaw.XYZByStruct.Z, &filterResultZ);
	
	accData.X = filterResultX - (int32_t)accOffset.X;
	accData.Y = filterResultY - (int32_t)accOffset.Y;
	accData.Z = filterResultZ - (int32_t)accOffset.Z;
	
#if DEBUG_AVG_DATA == 1
	printf("%d %d %d\n", accData.X, accData.Y, accData.Z); 
#endif
}