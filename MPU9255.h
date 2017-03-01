#ifndef MPU9255_H
#define MPU9255_H

#ifdef __cplusplus
extern "C"
{
#endif

// <Device name>_[(R)egister addres]_[(B)it name]_<Details>
	
#define MPU9255_DEV_ADDR		0x68

#define MPU9255_R_SMPLRT_DIV	0x19
#define MPU9255_SAMPLE_RATE_DIV	8
#define MPU9255_SAMPLE_RATE_DIV_IN_REG	(MPU9255_SAMPLE_RATE_DIV - 1)

#define MPU9255_R_CONFIG		0x1A
#define MPU9255_B_BAND50HZ		0x06
	
#define MPU9255_R_GYRO_CONFIG	0x1B
#define MPU9255_B_FULL_SC_1000	0x10

#define MPU9255_R_ACCEL_CONFIG	0x1C
#define MPU9255_B_FULL_SC_2G	0x00
	
#define MPU9255_R_ACC_OFFSET	0x77
	
#define MPU9255_R_ACCEL_DATA	0x3B
#define	MPU9255_R_TEMP_OUT_H	0x41
#define	MPU9255_R_TEMP_OUT_L	0x42
#define MPU9255_R_GYRO_DATA		0x43

#define MPU9255_R_PWR_MGMT1		0x6B
#define MPU9255_B_RUN_ALL		0x00	// Run all and select internal 20 MHz running.
	
#define MPU9255_R_BYPASS_ENABLE 0x37
#define MPU9255_B_BYPASS_ENABLE 0x02

	
void MPU9255_init();

void getAccOffset();

void getGyroOffset();

void getGyroData();

void getAccelData();

#ifdef __cplusplus
}
#endif

#endif /* MPU9255_H */

