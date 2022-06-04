#include "control.h"	
#include "filter.h"	

u8 Flag_Target,Flag_Change;  //��ر�־λ
float Voltage_Count,Voltage_All;  //��ѹ������ر���
int j,sum;
#define T 0.245f
#define L 0.29f
#define K 14.00f
/**************************************************************************
�������ܣ�С���˶���ѧģ��
��ڲ������ٶȺ�ת��
����  ֵ����
**************************************************************************/
void Kinematic_Analysis(float velocity,float angle) //
{
	Servo=SERVO_INIT+angle*K; //���ת��   angle*
	if(Servo > 1890){
		Servo = 1890;
		angle = (double)(Servo - SERVO_INIT)/K;
	}
	else if(Servo < 1280){
		Servo = 1280;
		angle = (double)(Servo - SERVO_INIT)/K;
	}
	
  Tand = tan(angle/57.3);;//(int)tan(angle);
	Target_Left=-velocity*(1-T*Tand/2/L); 
	Target_Right=velocity*(1+T*Tand/2/L);      //���ֲ���
	Servo=SERVO_INIT+angle*K; //���ת��   
}
/**************************************************************************
�������ܣ����еĿ��ƴ��붼��������
         ��ʱ�жϴ���
         �ϸ�֤���������ݴ����ʱ��ͬ��				 
**************************************************************************/
void TIM6_IRQHandler(void)   //TIM6�ж�
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) 	
	{     	
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update);          //����жϱ�־λ  	
			Flag_Target=!Flag_Target; //��Ƶ��־λ
			if(delay_flag==1)
			{
					if(++delay_50==2)	 delay_50=0,delay_flag=0; //���������ṩ50ms�ľ�׼��ʱ
			}
		  if(Flag_Target==1)
			{
					if(Usart_Flag==0&&Usart_ON_Flag==1)  memcpy(rxbuf,Urxbuf,8*sizeof(u8));	//��������˴��ڿ��Ʊ�־λ����ȡ��ص�ָ��					 
			  	//Key();//ɨ�谴���仯	
					Voltage_All+=Get_battery_volt();     //��β����ۻ�
			    if(++Voltage_Count==100) Voltage=Voltage_All/100,Voltage_All=0,Voltage_Count=0;//��ƽ��ֵ ��ȡ��ص�ѹ	   									                                        
			}else if(Flag_Target == 0){     
					Encoder_Right=Read_Encoder(3);  //===��ȡ��������ֵ
					Encoder_Left=Read_Encoder(2);    //===��ȡ��������ֵ
					Get_RC();   //===���տ���ָ��
		//			if(Turn_Off(Voltage)==0)   //===�����ص�ѹ�������쳣
		//			{ 	
						 Kinematic_Analysis(Velocity,-Angle); 	//С���˶�ѧ����   
						 Motor_Left=Incremental_PI_Left(Encoder_Left,Target_Left);  //===�ٶȱջ����Ƽ�����������PWM   Motor_Left
						 Motor_Right=Incremental_PI_Right(Encoder_Right,Target_Right);  //===�ٶȱջ����Ƽ����ҵ������PWM  Motor_Right
						 Xianfu_Pwm(6900);                          //===PWM�޷�
						 Set_Pwm(Motor_Left,Motor_Right,Servo);     //===��ֵ��PWM�Ĵ���  
						 
		//			}
			//		else	Set_Pwm(0,0,SERVO_INIT);    //===��ֵ��PWM�Ĵ���  	
			}
 }
	// return 0;	 //����ֵ                                                                                                                                                                                                   
} 
/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ�����PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motor_a,int motor_b,int servo)
{
	  	if(motor_a>0)			PWMA1=7200,PWMA2=7200-motor_a;
			else 	            PWMA2=7200,PWMA1=7200+motor_a;
		
		  if(motor_b>0)			PWMB1=7200,PWMB2=7200-motor_b;
			else 	            PWMB2=7200,PWMB1=7200+motor_b;
	   SERVO=servo;	
}
/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������ֵ
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(int amplitude)
{	
    if(Motor_Left<-amplitude) Motor_Left=-amplitude;	//������Сֵ
		if(Motor_Left>amplitude)  Motor_Left=amplitude;	  //�������ֵ
	  if(Motor_Right<-amplitude) Motor_Right=-amplitude;//������Сֵ	
		if(Motor_Right>amplitude)  Motor_Right=amplitude;	//�������ֵ		
}
/************************************************************************
�������ܣ������޸�С������״̬ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Key(void)
{	
	u8 tmp,tmp2;
	tmp=click_N_Double(50); //˫����˫���ȴ�ʱ��500ms
	if(tmp==1)Flag_Stop=!Flag_Stop;//��������С������ͣ
	if(tmp==2)Flag_Show=!Flag_Show;//˫������С������ʾ״̬
	tmp2=Long_Press();  //����        
  if(tmp2==1)Flag_Show=!Flag_Show;//����С������ʾ״̬                 
}
/**************************************************************************
�������ܣ��쳣�رյ��
��ڲ�������ѹ
����  ֵ��1���쳣  0������
**************************************************************************/
u8 Turn_Off( int voltage)
{
	    u8 temp;
			if(voltage<9500||Flag_Stop==1)//��ص�ѹ����9.5V�رյ��
			{	                                                
      temp=1;      
     	PWMA1=0; //�������λ����                                           
			PWMB1=0; //�������λ����
			PWMA2=0; //�������λ����
			PWMB2=0; //�������λ����					
      }
			else
      temp=0;
      return temp;			
}

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
�������ܣ�����PI������
��ڲ���������������ֵ��Ŀ���ٶ�
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_Left (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int Incremental_PI_Right (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
/**************************************************************************
�������ܣ�ͨ��ָ���С������ң��
��ڲ���������ָ��
����  ֵ����
**************************************************************************/
void Get_RC(void)
{
		int Yuzhi=2;  		//PS2���Ʒ�����ֵ
		float LY,RX;  //PS2�ֱ����Ʊ���
		if(operationMode==0)//���ڵȿ��Ʒ�ʽ
		{
			if(Usart_Flag==0&&Usart_ON_Flag==1){
				 Angle=-(rxbuf[2]-60);//�ǶȻ�ȡ
				 Velocity=rxbuf[3];	//�ٶ�
			 }
		}
		else	if(operationMode==1)//PS2����
		{
				LY=PS2_LY - 128;     //����ƫ��LY
				RX=PS2_RX - 128; 
				if(LY>-Yuzhi&&LY<Yuzhi)LY=0;   //С�Ƕ���Ϊ���� ��ֹ���������쳣RX
				if(RX>-Yuzhi&&RX<Yuzhi)RX=0;
				Velocity=-(float)LY/4;	  //�ٶȺ�ҡ�˵��������
				Angle=RX/4; 	
		}	
}

