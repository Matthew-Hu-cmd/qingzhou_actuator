#ifndef __MPU9250_H
#define __MPU9250_H

#include "sys.h"
#include "delay.h"
#include "myiic.h"

/************** I2C Address *****************/
//AD0=0ʱ,IIC��ַ1101000;AD0=1ʱ,IIC��ַ1101001;
//��ʵ��Ӧ����,����ֽڵ����λӦ���0��1,��ʾд���߶�;
//����д��ַ��:11010000��11010010(D0��D2);
//����ַ��:11010001��11010011(D1��D3)


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

//�Ĵ�������

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

#define SMPLRT_DIV					0X19	//�����ǲ����� ����ֵΪ0x07 1000/(1+7)=125 Hz
#define CONFIG							0X1A	//��ͨ�˲��� ����ֵΪ0x06 5Hz
#define GYRO_CONFIG					0X1B	//�����ǲ�����Χ 0x18 ����2000��
#define ACCEL_CONFIG				0X1C	//���ٶȼƲ�����Χ 0x18 ����16g
#define ACCEL_CONFIG2				0X1D	//���ٶȼƵ�ͨ�˲��� 0x06 5Hz

#define LP_ACCEL_ODR				0X1E
#define WOM_THR							0X1F
#define FIFO_EN							0X23

#define INT_PIN_CFG					0x37

//���ٶ��������
#define ACCEL_XOUT_H				0X3B
#define ACCEL_XOUT_L				0X3C
#define ACCEL_YOUT_H				0X3D
#define ACCEL_YOUT_L				0X3E
#define ACCEL_ZOUT_H				0X3F
#define ACCEL_ZOUT_L				0X40

//�¶ȼ��������
#define TEMP_OUT_H					0X41
#define TEMP_OUT_L					0X42

//�������������
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

//��Դ����1 ����ֵΪ0x00
#define PWR_MGMT_1					0X6B

//��Դ����2 ����ֵΪ0x00
#define PWR_MGMT_2					0X6C

//����ID MPU9250Ĭ��IDΪ0x71
#define WHO_AM_I						0X75

//�û�����,��Ϊ0x10ʱʹ��SPIģʽ
#define USER_CTRL						0X6A


//ָ�����豸��ַ
#define MAG_ADDR	0x18
//ָ�����豸ID�Ĵ���
#define MAG_WIA		0x00
//ָ�����豸ID
#define MAG_ID		0x48
//�������δ���
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





