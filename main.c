#include <stdio.h>
#include <inttypes.h>
#include <time.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "common.h"
#include "i2c.h"
#include "MPU9255.h"
#include "AK8963.h"
#include "MadgwickAHRS.h"

XYZData_us gyroDataRaw, accDataRaw, magnDataRaw;
dataXYZComputed gyroData, accData, magnData;

int fdMPU9255, fdAK8963;

void debugPrintXYZDATA(uint8_t *buf)
{
	for(int i = 0 ; i < 6 ; i ++)
		printf("%d ", *buf++);
	printf("\n");
}

void calAvgValue(uint8_t *pIndex, int16_t *pAvgBuffer, int16_t InVal, int32_t *pOutVal)
{	
	*(pAvgBuffer + ((*pIndex) ++)) = InVal;
  	*pIndex &= 0x07;
  	
  	*pOutVal = 0;
	
	// Unrolled the loop.
    *pOutVal += *(pAvgBuffer++);
	*pOutVal += *(pAvgBuffer++);
	*pOutVal += *(pAvgBuffer++);
	*pOutVal += *(pAvgBuffer++);
	*pOutVal += *(pAvgBuffer++);
	*pOutVal += *(pAvgBuffer++);
	*pOutVal += *(pAvgBuffer++);
	*pOutVal += *(pAvgBuffer);
  	
  	*pOutVal >>= 3;
}

int main (int argc, char *argv[])
{	
	wiringPiSetup() ;
	
	// Set up connection with MPU9255
	fdMPU9255 = wiringPiI2CSetup(MPU9255_DEV_ADDR);
	
	if(fdMPU9255 == -1)
	{
		printf("Can't establish connection with MPU9255\n");
		return -1;
	}
	
	MPU9255_init();
	
	// Set up connection with AK8963
	fdAK8963 = wiringPiI2CSetup(AK8963_DEV_ADDR);
	
	if(fdAK8963 == -1)
	{
		printf("Can't establish connection with AK8963\n");
		return -1;
	}
	
	AK8963_init();
	
	for ( ; ; )
	{
		clock_t begin = clock();
		
		getGyroData();
		getAccelData();
		getMagnData();

#if WEIRDO_PROTHESIS == 1
		MadgwickAHRSupdate((float)gyroData.X * 0.0175, (float)gyroData.Y * 0.0175,(float)gyroData.Z * 0.0175,
				(float)accData.X, (float)accData.Y,(float)accData.Z,
				(float)magnData.X, (float)magnData.Y,(float)magnData.Z);			
#else
		MadgwickAHRSupdate(TO_RAD((float)gyroData.X), TO_RAD((float)gyroData.Y),TO_RAD((float)gyroData.Z),
				NORM_ACC((float)accData.X), NORM_ACC((float)accData.Y), NORM_ACC((float)accData.Z),
					(float)magnData.X * MAGN_SCALE * magnXAdjustmentData, 
					(float)magnData.Y * MAGN_SCALE * magnYAdjustmentData,
					(float)magnData.Z * MAGN_SCALE * magnZAdjustmentData);				
#endif
		
		updateRPY();
		
		printf("%f %f %f\n", roll, pitch, yaw);
		
		clock_t end = clock();
		
		//printf("Execution time: %f\n", (double)(end - begin) / CLOCKS_PER_SEC );
	} 
	
	
	return 0;
}
