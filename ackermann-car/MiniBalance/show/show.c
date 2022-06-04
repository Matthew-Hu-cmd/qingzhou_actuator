#include "show.h"

unsigned char i;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数
/**************************************************************************
函数功能：OLED显示
入口参数：无
返回  值：无
**************************************************************************/
						int accz;
void oled_show(void)
{
					
						OLED_ShowString(00,0,"A");
						OLED_ShowNumber(15,0, Distance_A,3,12);  //PS2的数据	PS2_LX	Distance_A	
						OLED_ShowString(40,0,"B");
						OLED_ShowNumber(55,0, Distance_B,3,12);  //PS2的数据		PS2_LY    Distance_B
						OLED_ShowString(80,0,"C");
						OLED_ShowNumber(95,0, Distance_C,3,12);		//PS2_RX  Distance_C
						OLED_ShowString(0,10,"KEY");
						OLED_ShowNumber(25,10,PS2_KEY,2,12);
												
					//=============第3行显示左电机的状态=======================//	
						if( Target_Left<0)		  OLED_ShowString(00,20,"-"),
																OLED_ShowNumber(15,20,-Target_Left,5,12);
					else                 	OLED_ShowString(0,20,"+"),
																OLED_ShowNumber(15,20, Target_Left,5,12); 			
					if( Encoder_Left<0)		  OLED_ShowString(80,20,"-"),
																OLED_ShowNumber(95,20,-Encoder_Left,4,12);
					else                 	OLED_ShowString(80,20,"+"),
																OLED_ShowNumber(95,20, Encoder_Left,4,12);
					//=============第4行显示右电机的状态=======================//	
						if( Target_Right<0)		  OLED_ShowString(00,30,"-"),
																OLED_ShowNumber(15,30,-Target_Right,5,12);
					else                 	OLED_ShowString(0,30,"+"),
																OLED_ShowNumber(15,30, Target_Right,5,12); 					
					if( Encoder_Right<0)		  OLED_ShowString(80,30,"-"),
																OLED_ShowNumber(95,30,-Encoder_Right,4,12);
					else                 	OLED_ShowString(80,30,"+"),
																OLED_ShowNumber(95,30, Encoder_Right,4,12);	
					
					//=============第5行显示舵机的状态=======================//	
				 	OLED_ShowString(00,40,"Servo:"),	//舵机状态
					OLED_ShowNumber(60,40, Servo,4,12);	
					//=============第6行显示电压模式等=======================//	
											OLED_ShowString(48,50,".");
											OLED_ShowString(70,50,"V");
											OLED_ShowNumber(35,50,Voltage/100,2,12);//电压
											OLED_ShowNumber(58,50,Voltage%100,2,12);
 if(Voltage%100<10) 	OLED_ShowNumber(52,50,0,2,12);
                       //  电机使能/使能显示
											if(Flag_Stop==0)//根据Flag_Stop标志位显示电机的状态
											OLED_ShowString(103,50,"O-N");
											if(Flag_Stop==1)
											OLED_ShowString(103,50,"OFF");
											if(operationMode==0)               OLED_ShowString(0,50,"AUT");//不同的模式
											else if(operationMode==1)          OLED_ShowString(0,50,"PS2");
											OLED_Refresh_Gram();	//刷新
}
/**************************************************************************
函数功能：向APP发送数据
入口参数：无
返回  值：无
**************************************************************************/
void APP_Show(void)
{    
	  static u8 flag;
	  int app_2,app_3,app_4;
	  app_4=(Voltage-1110)*2/3;	
	  if(app_4>100)app_4=100;   //对电压数据进行处理
	  app_2=Velocity*0.7;  if(app_2<0)app_2=-app_2;	  //对编码器数据就行数据处理便于图形化
		app_3=Velocity*0.7;  if(app_3<0)app_3=-app_3;
	  flag=!flag;//错频处理，分时发送不同的参数
	 	if(PID_Send==1)//发送PID参数
	{
		printf("{C%d:%d:%d:%d:%d:%d:%d:%d:%d}$",(int)RC_Velocity,(int)Velocity_KP,(int)Velocity_KI,0,0,0,0,0,0);//打印到APP上面	
		PID_Send=0;	
	}	
   else	if(flag==0)
   printf("{A%d:%d:%d:%d}$",(u8)app_2,(u8)app_3,app_4,(int)((Servo-SERVO_INIT)*0.16)); //打印到APP上面
	 else
	 printf("{B%d:%d:%d:%d}$",(int)Pitch,(int)Roll,(int)Yaw,app_4);//打印到APP上面 显示波形
}
/**************************************************************************
函数功能：虚拟示波器往上位机发送数据 关闭显示屏
入口参数：无
返回  值：无
**************************************************************************/
void DataScope(void)
{   
		DataScope_Get_Channel_Data( Servo-SERVO_INIT, 1 );  //显示舵机信息
		DataScope_Get_Channel_Data( Encoder_Left, 2 );   // 显示左电机速度
		DataScope_Get_Channel_Data( Encoder_Right, 3 );  //显示右电机速度
		Send_Count = DataScope_Data_Generate(3);
		for( i = 0 ; i < Send_Count; i++) 
		{
		while((USART1->SR&0X40)==0);  
		USART1->DR = DataScope_OutPut_Buffer[i]; 
		}
}
//OLED画点含税
void OLED_DrawPoint_Shu(u8 x,u8 y,u8 t)
{ 
	 u8 i=0;
  OLED_DrawPoint(x,y,t);
	OLED_DrawPoint(x,y,t);
	  for(i = 0;i<8; i++)
  {
      OLED_DrawPoint(x,y+i,t);
  }
}
//OLED显示LCD函数
void OLED_Show_CCD(void)  
{ 
	 u8 i,t;
	 for(i = 0;i<128; i++)
  {
		if(ADV[i]<CCD_Yuzhi) t=1; else t=0;
		OLED_DrawPoint_Shu(i,0,t);
  }
}

//开机显示一次的内容
void oled_show_once(void)
{
   OLED_ShowString(0,00,"Turn Right Wheel");  //转动右轮
   OLED_ShowString(0,10,"TO Select Mode"); //选择模式
	 OLED_ShowString(0,20,"Current Mode Is");//当前的模式是：
	if(Flag_Way==0)         OLED_ShowString(50,30,"APP");//APP模式
	if(Flag_Way==1)         OLED_ShowString(50,30,"PS2");//PS2模式
	if(Flag_Way==2)				  OLED_ShowString(50,30,"CCD");//CCD模式
	if(Flag_Way==3)				  OLED_ShowString(50,30,"ELE");//电磁巡线模式
	OLED_ShowString(0,40,"Press User Key");// 按一下用户按键
  OLED_ShowString(0,50,"TO End Selection");//结束选择
	OLED_Refresh_Gram();	//OLED刷新
	}

