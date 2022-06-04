#ifndef __MPU9250_H
#define __MPU9250_H

#include "sys.h"
#include "delay.h"
#include "myiic.h"

/************** I2C Address *****************/
//AD0=0时,IIC地址1101000;AD0=1时,IIC地址1101001;
//在实际应用中,这个字节的最低位应添加0或1,表示写或者读;
//所以写地址是:11010000或11010010(D0或D2);
//读地址是:11010001或11010011(D1或D3)


#define ACCEL_ADDRESS				0xD0
#define GYRO_ADDRESS				0xD0
#define MAG_ADDRESS					0x18

typedef struct{
				int16_t X;
				int16_t Y;
				int16_t Z;}S_INT16_XYZ;

extern S_INT16_XYZ MPU9250_ACC_LAST;
extern S_INT16_XYZ MPU9250_GYRO_LAST;
extern S_INT16_XYZ MPU9250_MAG_LAST;
extern int16_t MPU9250_TEMP_LAST;

//寄存器定义

#define SELF_TEST_X_GYRO		0X00
#define SELF_TEST_Y_GYRO		0X01
#define SELF_TEST_Z_GYRO		0X02

#define MAG_CNTL						0X0A

#define SELF_TEST_X_ACCEL		0X0D
#define SELF_TEST_Y_ACCEL		0X0E
#define SELF_TEST_Z_ACCEL		0X0F

#define XG_OFFSET_H					0X13
#define XG_OFFSET_L					0X14
#define YG_OFFSET_H					0X15
#define YG_OFFSET_L					0X16
#define ZG_OFFSET_H					0X17
#define ZG_OFFSET_L					0X18

#define SMPLRT_DIV					0X19	//陀螺仪采样率 典型值为0x07 1000/(1+7)=125 Hz
#define CONFIG							0X1A	//低通滤波器 典型值为0x06 5Hz
#define GYRO_CONFIG					0X1B	//陀螺仪测量范围 0x18 正负2000度
#define ACCEL_CONFIG				0X1C	//加速度计测量范围 0x18 正负16g
#define ACCEL_CONFIG2				0X1D	//加速度计低通滤波器 0x06 5Hz

#define LP_ACCEL_ODR				0X1E
#define WOM_THR							0X1F
#define FIFO_EN							0X23

#define INT_PIN_CFG					0x37

//加速度输出数据
#define ACCEL_XOUT_H				0X3B
#define ACCEL_XOUT_L				0X3C
#define ACCEL_YOUT_H				0X3D
#define ACCEL_YOUT_L				0X3E
#define ACCEL_ZOUT_H				0X3F
#define ACCEL_ZOUT_L				0X40

//温度计输出数据
#define TEMP_OUT_H					0X41
#define TEMP_OUT_L					0X42

//陀螺仪输出数据
#define GYRO_XOUT_H					0X43
#define GYRO_XOUT_L					0X44
#define GYRO_YOUT_H					0X45
#define GYRO_YOUT_L					0X46
#define GYRO_ZOUT_H					0X47
#define GYRO_ZOUT_L					0X48

#define MAG_XOUT_L		0x03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08

//电源管理1 典型值为0x00
#define PWR_MGMT_1					0X6B

//电源管理2 典型值为0x00
#define PWR_MGMT_2					0X6C

//器件ID MPU9250默认ID为0x71
#define WHO_AM_I						0X75

//用户配置,当为0x10时使用SPI模式
#define USER_CTRL						0X6A


//指南针设备地址
#define MAG_ADDR	0x18
//指南针设备ID寄存器
#define MAG_WIA		0x00
//指南针设备ID
#define MAG_ID		0x48
//启动单次传输
#define MAG_CNTL1 0x0a


void MPU9250_Write_Reg(u8 Slave_add,u8 reg_add,u8 reg_dat);
u8 MPU9250_Read_Reg(u8 Slave_add,u8 reg_add);
void MPU6050_ReadData(u8 Slave_add,u8 reg_add,u8*Read,u8 num);

u8 MPU9250_Init(void);
void MPU9250_READ_ACCEL(short *accData);
void MPU9250_READ_GYRO(short *gyroData);
void MPU9250_READ_MAG(short *magData);

void readImu(void);
#endif





