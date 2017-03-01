/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AK8963.h
 * Author: maly_windows
 *
 * Created on 7 lutego 2017, 22:49
 */

#ifndef AK8963_H
#define AK8963_H

#ifdef __cplusplus
extern "C"
{
#endif

#define AK8963_DEV_ADDR				0x0C
	
#define AK8963_R_ASAX				0x10
#define AK8963_R_ASAY				0x11
#define AK8963_R_ASAZ				0x12

#define AK8963_R_CONTROL1			0x0A
#define AK8963_B_PWRDOWN			0x00
#define AK8963_B_ENTER_FUSE_ROM		0x0F
#define AK8963_B_CNTNS_MODE_2_100HZ	0x06
#define AK8963_B_SINGLE_MEAS		0x01
#define AK8963_B_16BIT				0x10
	
#define AK8963_R_MAGN_DATA			0x03

#define MAGN_SCALE (10.*4912./32760.0)
	
extern float magnXAdjustmentData, magnYAdjustmentData, magnZAdjustmentData;
	
void AK8963_init();
	
void getMagnData();

#ifdef __cplusplus
}
#endif

#endif /* AK8963_H */

