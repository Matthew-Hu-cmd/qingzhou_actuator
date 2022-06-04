#include "stm32f10x.h"
#include "sys.h"

u8 Flag_Left,Flag_Right,Flag_Direction=0,Flag_Way,Flag_Next; //����ң����صı���
u8 operationMode;
u8 Flag_Stop=1,Flag_Show; //ֹͣ��־λ�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��
int Encoder_Left,Encoder_Right;  //���������������                 
long int Motor_Left,Motor_Right; //���PWM����
long int Target_Left,Target_Right; //���Ŀ��ֵ
float Velocity,Angle,Servo; //�ٶȺͽǶȱ���
int Voltage;//��ص�ѹ������صı���                       
u8 delay_50,delay_flag; //��ʱ��ر���
u8 rxbuf[8],Urxbuf[8],CAN_ON_Flag=0,Usart_ON_Flag=0,Usart_Flag,PID_Send;  //CAN�ʹ��ڿ�����ر���
u8 txbuf[8],txbuf2[8];  //CAN������ر���
float Pitch,Roll,Yaw,Gryo_Z;   //����Ƕ� Z�������Ǻ�XYZ��Ŀ���ٶ�
float Velocity_KP=62,Velocity_KI=62;	          //�ٶȿ���PID����
int RC_Velocity=30;         //����ң�ص��ٶȺ�λ��ֵ
int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY,lastPS3Key,Accel_Key; //PS2��ر��� 
u16 CCD_Zhongzhi,CCD_Yuzhi,ADV[128]={0};//CCD��ر���
int Sensor_Left,Sensor_Middle,Sensor_Right,Sensor;//���Ѳ�����
int Remoter_Ch1,Remoter_Ch2,Remoter_Ch3,Remoter_Ch4;//��ģң�زɼ���ر���
int Distance_A,Distance_B,Distance_C,Distance_D;//��������ر��� 
float Tand;
int main(void)
  { 
		delay_init();	    	            //=====��ʱ������ʼ��	
		JTAG_Set(SWD_ENABLE);           //=====��SWD�ӿ� �������������SWD�ӿڵ���
		LED_Init();                     //=====��ʼ���� LED ���ӵ�Ӳ���ӿ�
	  //KEY_Init();                     //=====������ʼ��
		MY_NVIC_PriorityGroupConfig(2);	//=====�����жϷ���
    MiniBalance_PWM_Init(7199,0);   //=====��ʼ��PWM 10KHZ������������� �����ʼ������ӿ�
		Servo_PWM_Init(9999,71);   		  //=====��ʼ��PWM50HZ���� ���	
		//uart5_init(115200);             //=====����5��ʼ��
		//usart3_init(9600);              //=====����3��ʼ�� 
		//usart1_init(115200);						//=====����1��ʼ�� 
		OLED_Init();                    //=====OLED��ʼ��	    
    Encoder_Init_TIM2();            //=====�������ӿ�
		Encoder_Init_TIM3();            //=====�������ӿ�

		Flag_Way=1;
		Flag_Show = 0;
		Flag_Stop=1;	
		delay_ms(500);                  //=====��ʱ�ȴ��ȶ�
		while(MPU9250_Init());
		CAN1_Mode_Init(1,2,3,6,0);			//=====CAN��ʼ��,������1Mbps
		Adc_Init();                     //=====adc��ʼ��		
	 
		PS2_Init();											//=====PS2�ֱ���ʼ��
		PS2_SetInit();									//=====ps2���ó�ʼ��,���á����̵�ģʽ������ѡ���Ƿ�����޸�
	  
		Target_Left = 0;
		Target_Right = 0;
		//TIM5_Cap_Init(0XFFFF,72-1);	  //=====������ʼ�� Ĭ��ע�� ���������� �ο�timer.h�ļ�
		//TIM4_Cap_Init(0XFFFF,72-1);	  //=====������ʼ�� Ĭ��ע�� ���������� �ο�timer.h�ļ�
		TIM6_Int_Init(99,7199);      		//=====10ms,99��ʱ�жϳ�ʼ��
		Accel_Key = 4;
		Led_On();
    while(1)
	  {	
				PS2_KEY=PS2_DataKey();
				if(PS2_KEY == 0 && lastPS3Key == 4){
						operationMode = !operationMode;
				}
				if(PS2_KEY == 0 && lastPS3Key == 13){
					if(Accel_Key > 1)
						Accel_Key--;
					else if(Accel_Key == 1)
						Accel_Key = 1;
				}
				if(PS2_KEY == 0 && lastPS3Key == 15){
					if(Accel_Key < 5)
						Accel_Key++;
					else if(Accel_Key == 4)
						Accel_Key = 4;
				}
				if(operationMode == 1){		//PS2ģʽ
						PS2_LX=PS2_AnologData(PSS_LX);    //PS2���ݲɼ�    
						PS2_LY=PS2_AnologData(PSS_LY);
						PS2_RX=PS2_AnologData(PSS_RX);
						PS2_RY=PS2_AnologData(PSS_RY);
				}else if(operationMode == 0){		//�Զ���ʻģʽ
						//CAN1_SEND();             //CAN����	
				}
				readImu();   //===������̬	
			  oled_show();          		//��ʾ����
				//USART_TX();               //���ڷ���
				//Read_Distane();
				delay_flag=1;	
				lastPS3Key=PS2_KEY;
				delay_50=0;
				while(delay_flag);	     //ͨ����ʱ�ж�ʵ�ֵ�50ms��׼��ʱ	
	  } 
}

