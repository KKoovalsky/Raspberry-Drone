#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
#endif
	
#define DEBUG_RAW_DATA 0
#define DEBUG_RAW_ACC_DATA 0
#define DEBUG_RAW_MAGN_DATA 1
	
#define DEBUG_AVG_DATA 1
#define DEBUG_QUATERNIONS 0
#define WEIRDO_PROTHESIS 0
	
#define NORM_ACC(X) ((X)/16384)
#define TO_RAD(X) ((X)/180*3.14)
	
struct XYZData 
{
	int16_t X;
	int16_t Y;
	int16_t Z;
};

typedef union XYZData_u
{
	uint8_t XYZByArray[6];
	struct XYZData XYZByStruct; 
} XYZData_us;
	
typedef struct
{
	uint8_t index;
	int16_t avgBuffer[8];
} avgDigFilter;

typedef struct 
{
	int32_t X, Y, Z;
} dataXYZComputed;

extern XYZData_us gyroDataRaw, accDataRaw, magnDataRaw;
extern dataXYZComputed gyroData, accData, magnData;

extern int fdMPU9255, fdAK8963;

// Digital filter implementation. Copypasted from Waveshare examples.
void calAvgValue(uint8_t *pIndex, int16_t *pAvgBuffer, int16_t InVal, int32_t *pOutVal);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */

