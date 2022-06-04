#include "show.h"

unsigned char i;          //��������
unsigned char Send_Count; //������Ҫ���͵����ݸ���
/**************************************************************************
�������ܣ�OLED��ʾ
��ڲ�������
����  ֵ����
**************************************************************************/
						int accz;
void oled_show(void)
{
					
						OLED_ShowString(00,0,"A");
						OLED_ShowNumber(15,0, Distance_A,3,12);  //PS2������	PS2_LX	Distance_A	
						OLED_ShowString(40,0,"B");
						OLED_ShowNumber(55,0, Distance_B,3,12);  //PS2������		PS2_LY    Distance_B
						OLED_ShowString(80,0,"C");
						OLED_ShowNumber(95,0, Distance_C,3,12);		//PS2_RX  Distance_C
						OLED_ShowString(0,10,"KEY");
						OLED_ShowNumber(25,10,PS2_KEY,2,12);
												
					//=============��3����ʾ������״̬=======================//	
						if( Target_Left<0)		  OLED_ShowString(00,20,"-"),
																OLED_ShowNumber(15,20,-Target_Left,5,12);
					else                 	OLED_ShowString(0,20,"+"),
																OLED_ShowNumber(15,20, Target_Left,5,12); 			
					if( Encoder_Left<0)		  OLED_ShowString(80,20,"-"),
																OLED_ShowNumber(95,20,-Encoder_Left,4,12);
					else                 	OLED_ShowString(80,20,"+"),
																OLED_ShowNumber(95,20, Encoder_Left,4,12);
					//=============��4����ʾ�ҵ����״̬=======================//	
						if( Target_Right<0)		  OLED_ShowString(00,30,"-"),
																OLED_ShowNumber(15,30,-Target_Right,5,12);
					else                 	OLED_ShowString(0,30,"+"),
																OLED_ShowNumber(15,30, Target_Right,5,12); 					
					if( Encoder_Right<0)		  OLED_ShowString(80,30,"-"),
																OLED_ShowNumber(95,30,-Encoder_Right,4,12);
					else                 	OLED_ShowString(80,30,"+"),
																OLED_ShowNumber(95,30, Encoder_Right,4,12);	
					
					//=============��5����ʾ�����״̬=======================//	
				 	OLED_ShowString(00,40,"Servo:"),	//���״̬
					OLED_ShowNumber(60,40, Servo,4,12);	
					//=============��6����ʾ��ѹģʽ��=======================//	
											OLED_ShowString(48,50,".");
											OLED_ShowString(70,50,"V");
											OLED_ShowNumber(35,50,Voltage/100,2,12);//��ѹ
											OLED_ShowNumber(58,50,Voltage%100,2,12);
 if(Voltage%100<10) 	OLED_ShowNumber(52,50,0,2,12);
                       //  ���ʹ��/ʹ����ʾ
											if(Flag_Stop==0)//����Flag_Stop��־λ��ʾ�����״̬
											OLED_ShowString(103,50,"O-N");
											if(Flag_Stop==1)
											OLED_ShowString(103,50,"OFF");
											if(operationMode==0)               OLED_ShowString(0,50,"AUT");//��ͬ��ģʽ
											else if(operationMode==1)          OLED_ShowString(0,50,"PS2");
											OLED_Refresh_Gram();	//ˢ��
}
/**************************************************************************
�������ܣ���APP��������
��ڲ�������
����  ֵ����
**************************************************************************/
void APP_Show(void)
{    
	  static u8 flag;
	  int app_2,app_3,app_4;
	  app_4=(Voltage-1110)*2/3;	
	  if(app_4>100)app_4=100;   //�Ե�ѹ���ݽ��д���
	  app_2=Velocity*0.7;  if(app_2<0)app_2=-app_2;	  //�Ա��������ݾ������ݴ������ͼ�λ�
		app_3=Velocity*0.7;  if(app_3<0)app_3=-app_3;
	  flag=!flag;//��Ƶ������ʱ���Ͳ�ͬ�Ĳ���
	 	if(PID_Send==1)//����PID����
	{
		printf("{C%d:%d:%d:%d:%d:%d:%d:%d:%d}$",(int)RC_Velocity,(int)Velocity_KP,(int)Velocity_KI,0,0,0,0,0,0);//��ӡ��APP����	
		PID_Send=0;	
	}	
   else	if(flag==0)
   printf("{A%d:%d:%d:%d}$",(u8)app_2,(u8)app_3,app_4,(int)((Servo-SERVO_INIT)*0.16)); //��ӡ��APP����
	 else
	 printf("{B%d:%d:%d:%d}$",(int)Pitch,(int)Roll,(int)Yaw,app_4);//��ӡ��APP���� ��ʾ����
}
/**************************************************************************
�������ܣ�����ʾ��������λ���������� �ر���ʾ��
��ڲ�������
����  ֵ����
**************************************************************************/
void DataScope(void)
{   
		DataScope_Get_Channel_Data( Servo-SERVO_INIT, 1 );  //��ʾ�����Ϣ
		DataScope_Get_Channel_Data( Encoder_Left, 2 );   // ��ʾ�����ٶ�
		DataScope_Get_Channel_Data( Encoder_Right, 3 );  //��ʾ�ҵ���ٶ�
		Send_Count = DataScope_Data_Generate(3);
		for( i = 0 ; i < Send_Count; i++) 
		{
		while((USART1->SR&0X40)==0);  
		USART1->DR = DataScope_OutPut_Buffer[i]; 
		}
}
//OLED���㺬˰
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
//OLED��ʾLCD����
void OLED_Show_CCD(void)  
{ 
	 u8 i,t;
	 for(i = 0;i<128; i++)
  {
		if(ADV[i]<CCD_Yuzhi) t=1; else t=0;
		OLED_DrawPoint_Shu(i,0,t);
  }
}

//������ʾһ�ε�����
void oled_show_once(void)
{
   OLED_ShowString(0,00,"Turn Right Wheel");  //ת������
   OLED_ShowString(0,10,"TO Select Mode"); //ѡ��ģʽ
	 OLED_ShowString(0,20,"Current Mode Is");//��ǰ��ģʽ�ǣ�
	if(Flag_Way==0)         OLED_ShowString(50,30,"APP");//APPģʽ
	if(Flag_Way==1)         OLED_ShowString(50,30,"PS2");//PS2ģʽ
	if(Flag_Way==2)				  OLED_ShowString(50,30,"CCD");//CCDģʽ
	if(Flag_Way==3)				  OLED_ShowString(50,30,"ELE");//���Ѳ��ģʽ
	OLED_ShowString(0,40,"Press User Key");// ��һ���û�����
  OLED_ShowString(0,50,"TO End Selection");//����ѡ��
	OLED_Refresh_Gram();	//OLEDˢ��
	}

