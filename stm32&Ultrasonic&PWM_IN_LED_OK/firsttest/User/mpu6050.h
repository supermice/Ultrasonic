/**
  ******************************************************************************
  * @file    mpu6050.h
  * @author  wfh
  * @version V1.0
  * @date    2015.10.22
  * @brief   This file is for mpu6050.cpp 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef _MPU6050_H__
#define _MPU6050_H__

#include "stm32f10x.h"

typedef enum _mpu6050_status
{
    MPU6050_SUCCEED = 0,
    MPU6050_FAILED = 1,
}mpu6050_status;

// typedef struct _mpu6050_rawData
// {
//     int16_t acc[3];
//     int16_t tem;
//     int16_t gyr[3];
// }mpu6050_rawData_t;
// static struct 
// {
//     int16_t acc[3];
//     int16_t tem;
//     int16_t gyr[3];
// }mpu6050_rawData;

u8 mpu6050_init(void);
u8 mpu6050_read(void);
u8 mpu6050_getRawAcc(int16_t acc[3]);
u8 mpu6050_getRawGyr(int16_t gyr[3]);
u8 mpu6050_getCalibratedAcc(float acc[3],short mpu6050_rawDataacc[3]);
u8 mpu6050_getCalibratedGyr(float gyr[3],short mpu6050_rawDatagyr[3]);

#endif 
