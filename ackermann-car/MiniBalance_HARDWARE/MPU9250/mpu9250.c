#include "mpu9250.h"

short Accel[3];
short Gyro[3];
short Mag[3];
short gyroX,gyroY,gyroZ;	
short accelX,accelY,accelZ;	
short magX,magY,magZ;
void MPU9250_Write_Reg(u8 Slave_add,u8 reg_add,u8 reg_dat)
{
	IIC_Start();
	IIC_Send_Byte(Slave_add);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg_add);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg_dat);
	IIC_Wait_Ack();
	IIC_Stop();
}

u8 MPU9250_Read_Reg(u8 Slave_add,u8 reg_add)
{
	u8 temp=0;

	IIC_Start();
	IIC_Send_Byte(Slave_add);
	temp=IIC_Wait_Ack();
	IIC_Send_Byte(reg_add);
	temp=IIC_Wait_Ack();
	IIC_Start();	//
	IIC_Send_Byte(Slave_add+1);
	temp=IIC_Wait_Ack();
	temp=IIC_Read_Byte(0);
	IIC_Stop();
	
	return temp;
}

void MPU6050_ReadData(u8 Slave_add,u8 reg_add,u8*Read,u8 num)
{
	u8 i;
	
	IIC_Start();
	IIC_Send_Byte(Slave_add);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg_add);
	IIC_Wait_Ack();
	
	IIC_Start();
	IIC_Send_Byte(Slave_add+1);
	IIC_Wait_Ack();
	
	for(i=0;i<(num-1);i++)
	{
		*Read=IIC_Read_Byte(1);
		Read++;
	}
	*Read=IIC_Read_Byte(0);
	IIC_Stop();
}

u8 MPU9250_Init(void)
{
	IIC_Init();
	if(MPU9250_Read_Reg(GYRO_ADDRESS,WHO_AM_I)==0x71)
	{
		//MPU9250_Write_Reg(GYRO_ADDRESS,GYRO_ADDRESS,PWR_MGMT_1,0X80);	//电源管理,复位MPU9250
		MPU9250_Write_Reg(GYRO_ADDRESS,PWR_MGMT_1,0x00);	//解除休眠状态
		MPU9250_Write_Reg(GYRO_ADDRESS,SMPLRT_DIV,0x07);	//采样频率125Hz
		MPU9250_Write_Reg(GYRO_ADDRESS,CONFIG,0X06);			//低通滤波器5Hz
		MPU9250_Write_Reg(GYRO_ADDRESS,GYRO_CONFIG,0X18);//陀螺仪量程,正负2000度
		MPU9250_Write_Reg(GYRO_ADDRESS,ACCEL_CONFIG,0X18);//加速度量程,正负16g
		return 0;
	}
	return 1;
}

//------------------------------读取MPU9250数据------------------------------//

void MPU9250_READ_ACCEL(short *accData)
{ 
	u8 BUF[6];
	BUF[0]=MPU9250_Read_Reg(ACCEL_ADDRESS,ACCEL_XOUT_L); 
	BUF[1]=MPU9250_Read_Reg(ACCEL_ADDRESS,ACCEL_XOUT_H);
	accelX=(BUF[1]<<8)|BUF[0];
	//accData[0]/=164;								//读取并计算X轴数据

	BUF[2]=MPU9250_Read_Reg(ACCEL_ADDRESS,ACCEL_YOUT_L);
	BUF[3]=MPU9250_Read_Reg(ACCEL_ADDRESS,ACCEL_YOUT_H);
	accelY=(BUF[3]<<8)|BUF[2];
	//accData[1]/=164;								//读取并计算Y轴数据

	BUF[4]=MPU9250_Read_Reg(ACCEL_ADDRESS,ACCEL_ZOUT_L);
  BUF[5]=MPU9250_Read_Reg(ACCEL_ADDRESS,ACCEL_ZOUT_H);
  accelZ=(BUF[5]<<8)|BUF[4];
	//accData[2]/=164;								//读取并计算Z轴数据

}

void MPU9250_READ_GYRO(short *gyroData)
{
	u8 BUF[8];
	BUF[0]=MPU9250_Read_Reg(GYRO_ADDRESS,GYRO_XOUT_L);
	BUF[1]=MPU9250_Read_Reg(GYRO_ADDRESS,GYRO_XOUT_H);
	gyroX=(BUF[1]<<8)|BUF[0];
	//gyroData[0]/=16.4;

	BUF[2]=MPU9250_Read_Reg(GYRO_ADDRESS,GYRO_YOUT_L);
	BUF[3]=MPU9250_Read_Reg(GYRO_ADDRESS,GYRO_YOUT_H);
	gyroY=(BUF[3]<<8)|BUF[2];
	//gyroData[1]/=16.4;

	BUF[4]=MPU9250_Read_Reg(GYRO_ADDRESS,GYRO_ZOUT_L);
	BUF[5]=MPU9250_Read_Reg(GYRO_ADDRESS,GYRO_ZOUT_H);
	gyroZ=(BUF[5]<<8)|BUF[4];
	//gyroData[2]=16.4;

  //BUF[6]=MPU9250_Read_Reg(GYRO_ADDRESS,TEMP_OUT_L);	//读取温度
  //BUF[7]=MPU9250_Read_Reg(GYRO_ADDRESS,TEMP_OUT_H); 
  //gyroData[3]=(BUF[7]<<8)|BUF[6];
  //gyroData[3] = 35+ ((double) (T_T + 13200)) / 280;
}

void MPU9250_READ_MAG(short *magData)
{ 
	u8 BUF[6];
	MPU9250_Write_Reg(GYRO_ADDRESS,INT_PIN_CFG,0x02);//turn on Bypass Mode 
	delay_ms(10);
	MPU9250_Write_Reg(MAG_ADDRESS,0x0A,0x01);//用来启动单次转换,否则磁力计输出的数据不变
	delay_ms(10);
		 
	BUF[0]=MPU9250_Read_Reg(MAG_ADDRESS,MAG_XOUT_L);
	BUF[1]=MPU9250_Read_Reg(MAG_ADDRESS,MAG_XOUT_H);
	magX=(BUF[1]<<8)|BUF[0];

	BUF[2]=MPU9250_Read_Reg(MAG_ADDRESS,MAG_YOUT_L);
	BUF[3]=MPU9250_Read_Reg(MAG_ADDRESS,MAG_YOUT_H);
	magY=(BUF[3]<<8)|BUF[2];
		 
	BUF[4]=MPU9250_Read_Reg(MAG_ADDRESS,MAG_ZOUT_L);
	BUF[5]=MPU9250_Read_Reg(MAG_ADDRESS,MAG_ZOUT_H);
	magZ=(BUF[5]<<8)|BUF[4];
}

void readImu()
{
		MPU9250_READ_ACCEL(Accel);
		MPU9250_READ_GYRO(Gyro);
		MPU9250_READ_MAG(Mag);
}

