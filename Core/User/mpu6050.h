#ifndef __MPU6050_H
#define __MPU6050_H
 
#include "stdint.h"
 
#include "i2c.h"
#include "usart.h"
#include "stm32f1xx_hal.h"

typedef struct {
    // 加速度原始值
    int16_t acc_origin[3];
    // 角速度原始值
    int16_t gyro_origin[3];
    // 加速度处理值
    double acc_after[3];
    // 角速度处理值
    double gyro_after[3];
} Mpu6050_Typedef;
 
/* MPU6050 */

// 加速度乘此值得到真实加速度 值为 1 / 65536 * 2 * 2 * 9.8
#define MPU6050_ACC_PARA        (0.00059814453125)
// 角速度乘此值得到真实角速度 值为 1 / 65536 * 2 * 500
#define MPU6050_GYRO_PARA       (0.0152587890625)

 
#define MPU6050_DEVICE_ADDR     (0XD0)      // 8位设备地址
 
#define MPU6050_PWR_MGMT_1      (0X6B)      // 电源管理1
#define MPU6050_RA_RATE_DIV     (0X19)      // 采样频率分频器
#define MPU6050_RA_CONFIG       (0X1A)      // 低通滤波器
#define MPU6050_RA_ACCEL_CONFIG (0X1C)      // 加速度计量程设置
#define MPU6050_RA_GYRO_CONFIG  (0X1B)      // 陀螺仪量程设置
 
#define MPU6050_RA_WHO_AM_I     (0X75)      // 设备标识
 
#define MPU6050_GYRO_OUT        (0x43)      // 陀螺仪数据寄存器地址
#define MPU6050_ACC_OUT         (0x3B)      // 加速度数据寄存器地址
 
#define MPU6050_RA_TEMP_OUT_H   (0x41)      // 温度寄存器 高地址
#define MPU6050_RA_TEMP_OUT_L   (0x42)      // 温度寄存器 低地址
 
void MPU6050_Init(void);
void MPU6050_ReadAcc(int16_t *accData);
void MPU6050_ReadGyro(int16_t *gyroData);
void MPU6050_Collect(void);
void MPU6050_Data_Handler(void);
void MPU6050_WriteRegData(uint8_t reg_addr, uint8_t data);
void MPU6050_ReadRegData(uint8_t reg_addr, uint8_t *data, uint8_t num);
#endif /* __MPU6050_H */
