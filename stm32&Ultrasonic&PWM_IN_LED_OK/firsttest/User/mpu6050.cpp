/**
  ******************************************************************************
  * @file    mpu6050.cpp
  * @author  wfh
  * @version V1.0
  * @date    2015.10.22
  * @brief   This file provides the basic mpu6050 function based on the i2c.cpp
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "mpu6050.h"
#include "i2c_cpp.h"


#define i2c_read  I2C_Read_dmp
#define i2c_write I2C_Write_dmp

#define MPU6050_ADDRESS         0xD0
#define MPU6050_REG_RAW_DATA    0x3B
#define MPU6050_REG_ACC         0x3B
#define MPU6050_REG_TEM         0x41            //温度41高八位，42低八位
#define MPU6050_REG_GYR         0x43
#define MPU6050_PWR_MGMT_1	0x6B		//107	电源管理寄存器1
#define	MPU6050_SMPLRT_DIV	0x19		//19	样本频率分频寄存器  采样频率=陀螺仪输出频率/（1+SMPLRT_DIV） 
#define	MPU6050_CONFIG		0x1A		//26	配置寄存器 包含低通滤波	可设置为1，ACC与gyro都为1khz，延时2ms
#define MPU6050_GYRO_CONFIG	0x1B		//27	陀螺仪配置寄存器 包含自检和量程
#define MPU6050_ACCEL_CONFIG 	0x1C		//28	加速寂渲眉拇嫫髌 包含自检和量程
#define MPU6050_INT_PIN_CFG	0x37		//55	中断引脚/旁路使能配置寄存器	
#define MPU6050_INT_ENABLE	0x38		//56	中断使能寄存器	
#define MPU6050_USER_CTRL	0x6A		//106	用户控制寄存器

extern mpu6050_rawData_t mpu6050_rawData;//wfh define @main.cpp
extern int16_t acce_offset[3];//x,y,z   dmp后加速度计仍需校准 wfh define @main.cpp
extern int16_t gyro_offset[3];//x,y,z   dmp后加速度计仍需校准

static u8 paramtest[3]={3,2,1};

u8 mpu6050_init(void)
{
    for(int i=0;i<sizeof(mpu6050_rawData);i++)
        ((uint8_t *)&mpu6050_rawData)[i] = 0;////////结构体转数组
    //
    // 检查ID。
    uint8_t id = 0;
	u8 ic=1,ie=0;
	u8 status1=0;
	//I2C_Read_dmp(0xd0, 0x75,1,&ic);//test
	status1=i2c_read(MPU6050_ADDRESS,0x75,1,&id);
    //if(i2c_read(MPU6050_ADDRESS,0x75,1,&id) != 0)
	if(status1 != 0)
	{return MPU6050_FAILED;}
	I2C_Read_dmp(MPU6050_ADDRESS, 0x75,1,&ie);//test
    if(id != 0x68)
	{return MPU6050_FAILED;}
	ic=3;//test
    //
    // 设置MPU6050的参数。
    //const uint8_t param[][2] = 
	uint8_t param[][2] = 
    {
        // {寄存器地址,寄存器值},
        {MPU6050_PWR_MGMT_1,1     }, // 重置mpu6050，退出睡眠模式，时钟采用陀螺X轴使用的时钟。
        {MPU6050_SMPLRT_DIV,0     }, // 取样时钟不分频，1kHz。
        {MPU6050_CONFIG,2     }, // 低通滤波，截止频率98Hz左右，延时3ms
        {MPU6050_GYRO_CONFIG,3<<3  }, // 陀螺量程，2000dps。16.4lsb/deg
        {MPU6050_ACCEL_CONFIG,2<<3  }, // 加速度计量程，8g。4096lsb/g
        {MPU6050_INT_PIN_CFG,0x32},
       // {0x37,0x32  }, // 中断信号为高电平，推挽输出，直到有读取操作才消失，直通辅助I2C。
       // {0x38,1     }, // 使用“数据准备好”中断。
        {MPU6050_USER_CTRL,0x00  }, // 不使用辅助I2C。
    };
    for(int i=0;i<sizeof(param)/2;i++)
    {
        if(i2c_write(MPU6050_ADDRESS,param[i][0],1,&(param[i][1])) != 0)
            return MPU6050_FAILED;
        //
        uint8_t check = 0;
        if(i2c_read(MPU6050_ADDRESS,param[i][0],1,&check) != 0)
            return MPU6050_FAILED;
        if(check != param[i][1])
            return MPU6050_FAILED;
    }
	//i2c_write(MPU6050_ADDRESS,MPU6050_CONFIG,3,paramtest);//test
	//i2c_read(MPU6050_ADDRESS,MPU6050_CONFIG,3,paramtest);
    return MPU6050_SUCCEED;
}

u8 mpu6050_read(void)//i2c读取，读入 mpu6050_rawData 中
{
	u8 status=3;
//     if(i2c_read(MPU6050_ADDRESS,MPU6050_REG_RAW_DATA
//         ,sizeof(mpu6050_rawData),(uint8_t*)&mpu6050_rawData) != 0)
	status=i2c_read(MPU6050_ADDRESS,MPU6050_REG_RAW_DATA
        ,14,(uint8_t*)&mpu6050_rawData);
        //return MPU6050_FAILED;
    //
    // 大端转小端。先读取的是高位
    uint8_t * p = (uint8_t *)&mpu6050_rawData;
    for(int i=0;i<sizeof(mpu6050_rawData)/2;i++)
    {
        uint8_t tmp = p[2*i];
        p[2*i] = p[2*i + 1];
        p[2*i + 1] = tmp;
    }
    //
    return MPU6050_SUCCEED;
}
u8 mpu6050_getCalibratedAcc(float acc[3],short mpu6050_rawDataacc[3])
{
   // const param_t *p = param_getRamParam();
    //
    for(int i=0;i<3;i++)
      acc[i] = (acce_offset[i]+ mpu6050_rawDataacc[i]) * (9.79363 / 16384.0);//2g的范围
     //   acc[i] = (p->acc_offset[i] + mpu6050_rawData.acc[i]) * (9.7883 / 16384.0);///////////////灵敏度
     // acc[i] = (acce_offset[i]+ mpu6050_rawDataacc[i]) * (9.79363 / 4096.0);//8g的范围
    //
    return MPU6050_SUCCEED;
}

u8 mpu6050_getCalibratedGyr(float gyr[3],short mpu6050_rawDatagyr[3])
{
   // const param_t *p = &ram_param;
    //
    for(int i=0;i<3;i++)
       // gyr[i] = (p->gyr_offset[i] + mpu6050_rawData.gyr[i]) * （(3.1415926/180.0) / 16.4）;
      gyr[i] = (gyro_offset[i] + mpu6050_rawDatagyr[i]) * ((3.1415926/180.0) / 16.4);//2000度每秒的范围
    //
    return MPU6050_SUCCEED;
}
