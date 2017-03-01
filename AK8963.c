#include <stdint.h>
#include <stdio.h>

#include "common.h"
#include "i2c.h"
#include "AK8963.h"
#include "wiringPi.h"
#include "wiringPiI2C.h"

avgDigFilter magnXAvgDigFilter, magnYAvgDigFilter, magnZAvgDigFilter;
float magnXAdjustmentData, magnYAdjustmentData, magnZAdjustmentData;

void AK8963_init()
{
	uint8_t magnXAdjDataTemp, magnYAdjDataTemp, magnZAdjDataTemp;
	
	// Power down magnetometer
	wiringPiI2CWriteReg8(fdAK8963, AK8963_R_CONTROL1, AK8963_B_PWRDOWN);
	delay(10);
	
	// Enter Fuse ROM to get callibration data.
	wiringPiI2CWriteReg8(fdAK8963, AK8963_R_CONTROL1, AK8963_B_ENTER_FUSE_ROM);
	delay(10);
	
	// Read calibration data.
	magnXAdjDataTemp = wiringPiI2CReadReg8(fdAK8963, AK8963_R_ASAX);
	magnYAdjDataTemp = wiringPiI2CReadReg8(fdAK8963, AK8963_R_ASAY);
	magnZAdjDataTemp = wiringPiI2CReadReg8(fdAK8963, AK8963_R_ASAZ);
	
	magnXAdjustmentData = ((float)(magnXAdjDataTemp - 128))/256. + 1.;
	magnYAdjustmentData = ((float)(magnYAdjDataTemp - 128))/256. + 1.;
	magnZAdjustmentData = ((float)(magnZAdjDataTemp - 128))/256. + 1.;
	
	// Again power down magnetometer
	wiringPiI2CWriteReg8(fdAK8963, AK8963_R_CONTROL1, AK8963_B_PWRDOWN);
	delay(10);
	
	// Set continuous measurement mode 2 (100 sps) and 16 bit resolution
	wiringPiI2CWriteReg8(fdAK8963, AK8963_R_CONTROL1, AK8963_B_CNTNS_MODE_2_100HZ | AK8963_B_16BIT);
}
	
void getMagnData()
{
	int32_t filterResultX, filterResultY, filterResultZ;
	
	i2cGetXYZ(fdAK8963, AK8963_R_MAGN_DATA, magnDataRaw.XYZByArray);
#if DEBUG_RAW_MAGN_DATA == 1
	printf("%X %X %X\n", magnDataRaw.XYZByStruct.X, magnDataRaw.XYZByStruct.Y, magnDataRaw.XYZByStruct.Z);
#endif
	calAvgValue(&magnXAvgDigFilter.index, magnXAvgDigFilter.avgBuffer, magnDataRaw.XYZByStruct.X, &filterResultX);
	calAvgValue(&magnYAvgDigFilter.index, magnYAvgDigFilter.avgBuffer, magnDataRaw.XYZByStruct.Y, &filterResultY);
	calAvgValue(&magnZAvgDigFilter.index, magnZAvgDigFilter.avgBuffer, magnDataRaw.XYZByStruct.Z, &filterResultZ);
	
	magnData.X = filterResultX;
	magnData.Y = filterResultY;
	magnData.Z = filterResultZ;
	
#if DEBUG_AVG_DATA == 1
	printf("%d %d %d\n", magnData.X, magnData.Y, magnData.Z); 
#endif
}
