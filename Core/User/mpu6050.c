#include "mpu6050.h"
#include "u_console.h"

Mpu6050_Typedef mpu6050;

/**
  * @brief MPU6050初始化
  */
void MPU6050_Init(void)
{
  int8_t i;
 
	/* 解除休眠 */
	MPU6050_WriteRegData(MPU6050_PWR_MGMT_1, 0X00);
 
	/* 陀螺仪采样率 = 1kHz / (1 + 采样率分频) = 100Hz */
	MPU6050_WriteRegData(MPU6050_RA_RATE_DIV, 0x09);
 
	/* 采样频率fs = 100Hz，带宽 = fs/2 = 50Hz */
	MPU6050_WriteRegData(MPU6050_RA_CONFIG, 0X03);
 
	/* 配置加速度计量程范围（最小计量范围）：±2g */
	MPU6050_WriteRegData(MPU6050_RA_ACCEL_CONFIG, 0x00);
 
	/* 配置陀螺仪量程范围（最小计量范围）：±250°/s */
	MPU6050_WriteRegData(MPU6050_RA_GYRO_CONFIG, 0X00);

  for(i=0; i<3; i++){
    mpu6050.acc_origin[i] = 0;
    mpu6050.acc_after[i] = 0;
    mpu6050.gyro_origin[i] = 0;
    mpu6050.gyro_after[i] = 0;
  }
}
 
/**
  * @brief 写数据到MPU6050寄存器 
  * @param reg_addr     待写入的寄存器地址
  * @param data         待写入的数据
  */
void MPU6050_WriteRegData(uint8_t reg_addr, uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_DEVICE_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xFFFF);
}
 
/**
  * @brief 从指定寄存器读取数据
  * @param reg_addr     待读取的寄存器地址
  * @param data         数据指针
  * @param num          待读取的字节数
  */
void MPU6050_ReadRegData(uint8_t reg_addr, uint8_t *data, uint8_t num)
{
	HAL_I2C_Mem_Read(&hi2c1, MPU6050_DEVICE_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, data, num, 0XFFFF);
}
 
 
/**
  * @brief 读取加速度值
  * @param accData  加速度数组指针
  */
void MPU6050_ReadAcc(int16_t *accData)
{
	uint8_t buf[6] = {0};
 
	/* 连续读取6个寄存器值，高位在前 */
	MPU6050_ReadRegData(MPU6050_ACC_OUT, buf, 6);
 
	/* 数据保存到传入的数组指针 */
	accData[0] = (buf[0] << 8) | buf[1];
	accData[1] = (buf[2] << 8) | buf[3];
	accData[2] = (buf[4] << 8) | buf[5];
}
 
/**
  * @brief 读取陀螺仪值
  * @param accData  加速度数组指针
  */
void MPU6050_ReadGyro(int16_t *gyroData)
{
	uint8_t buf[6] = {0};
 
	/* 连续读取6个寄存器值，高位在前 */
	MPU6050_ReadRegData(MPU6050_GYRO_OUT, buf, 6);
 
	/* 数据保存到传入的数组指针 */
	gyroData[0] = (buf[0] << 8) | buf[1];
	gyroData[1] = (buf[2] << 8) | buf[3];
	gyroData[2] = (buf[4] << 8) | buf[5];
}

void MPU6050_Data_Handler(void) {
    for(uint8_t i=0; i<3; i++){
      mpu6050.acc_after[i] = (double) mpu6050.acc_origin[i] * MPU6050_ACC_PARA;
      mpu6050.gyro_after[i] = (double) mpu6050.gyro_origin[i] * MPU6050_GYRO_PARA;
    }
}

void MPU6050_Collect(void) {
    MPU6050_ReadAcc(mpu6050.acc_origin);
    MPU6050_ReadGyro(mpu6050.gyro_origin);
}
