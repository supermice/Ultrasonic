/**
  ******************************************************************************
  * @file    DMP.c
  * @author  wfh
  * @version V1.0
  * @date    2015.10.21
  * @brief   This file will call the mpu sensor basic function and DMP setup function based on the 
  * 	inv_mpu.c(for basic settings) & inv_mpu_dmp_motion_driver(for DMP functions).
  ******************************************************************************
  * @attention
  *		mpu_init();This function will setup mpufeatures based on the variable "st" to default settings
  *		Than will call other function to change some setting
  *		Load DMP firmware and set the DMP feature
  ******************************************************************************
  */
#include "DMP.h"
#include "stm32f10x.h"
#define DEFAULT_MPU_HZ  (1000) //Sampling rate must be between 4Hz and 1kHz

static signed char gyro_orientation[9] = {-1, 0, 0,0,-1, 0,0, 0, 1};  //#原文有注释参见(motion_driver_test.c第79行)
volatile unsigned char new_gyro;  //#原为hal.new_gyro(参见motion_driver_test.c的55行)

void dmp_init(void)
{	
  struct int_param_s int_param; //#int_param_s_在inv_mpu。h定义
  unsigned short dmp_features;
	
  //init_gyro_state_s();	//wfh add for init
	
  int_param.cb = gyro_data_ready_cb;//1
  int_param.pin = 0x202;//alldefine  0x202
  int_param.lp_exit = 1;//1
  int_param.active_low = 1;
  mpu_init(&int_param);  //初始化，详见函数头说明 修改过
  mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);//#(INV_XYZ_GYRO | INV_XYZ_ACCEL)在inv_mpu.h定义//启用加速度计，陀螺仪传感器
  mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);//进入fifo的数据
  mpu_set_sample_rate(DEFAULT_MPU_HZ);//#本文件开头定义 //采样频率//mpu_init(&int_param); 中已经设置过了
  dmp_load_motion_driver_firmware(); //刷固件
	//wfh commit 15.10.25
  //dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));//#本文定义 //坐标系 wfh commit 15.10.25
  dmp_features = DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
  DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL |
  DMP_FEATURE_SEND_CAL_GYRO |DMP_FEATURE_GYRO_CAL;//#全部在(inv_mpu_dmp_motion_driver.h)中定义
  dmp_enable_feature(dmp_features);  
  dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));//#本文定义 //坐标系
  dmp_set_fifo_rate(100);//不能高于200,//wfh 15.10.26test this could no more than 200Hz,read fifo quickly,Or,the fifo will be reset and we could only read 0 from the fifo                         
 // run_self_test();//自检,z轴与重力平行
  mpu_set_dmp_state(1);//修改bypass
  int_param.cb = gyro_data_ready_cb;
}

/*******************************

*******************************/
/* These next two functions converts the orientation matrix (see
 * gyro_orientation) to a scalar representation for use by the DMP.
 * NOTE: These functions are borrowed from Invensense's MPL.
 */
static unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

/*******************************

*******************************/
static unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx)
{
    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}

/*******************************
*	Temporary not needed commited by wfh 15.10.27
*******************************/
// static void run_self_test(void)
// {
//     int result;
//     //char test_packet[4] = {0};
//     long gyro[3], accel[3];

//     result = mpu_run_self_test(gyro, accel);
//     if (result == 0x7) {
//         /* Test passed. We can trust the gyro data here, so let's push it down
//          * to the DMP.
//          */
//         float sens;
//         unsigned short accel_sens;
//         mpu_get_gyro_sens(&sens);
//         gyro[0] = (long)(gyro[0] * sens);
//         gyro[1] = (long)(gyro[1] * sens);
//         gyro[2] = (long)(gyro[2] * sens);
//         dmp_set_gyro_bias(gyro);
//         mpu_get_accel_sens(&accel_sens);
//         accel[0] *= accel_sens;
//         accel[1] *= accel_sens;
//         accel[2] *= accel_sens;
//         dmp_set_accel_bias(accel);
//     }

//     /* Report results. */  //#报告结果 改为N5110
//     //test_packet[0] = 't';
//     //test_packet[1] = result;
//   // / N5110_Set_XY(0,0);//#
//   ///  N5110_Write_Char('T');
//   ///  N5110_Write_Char(':');
//   ///  N5110_Write_Char(result+48);
//     //send_packet(PACKET_TYPE_MISC, test_packet);
// }

/*******************************

*******************************/
static void gyro_data_ready_cb(void)
{
    new_gyro = 1;
}
