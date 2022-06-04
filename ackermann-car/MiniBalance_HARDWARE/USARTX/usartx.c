#include "usartx.h"

u8 Uart5_Receive;
/**************************ʵ�ֺ���**********************************************
*��    ��:		usart����һ���ֽ�
*********************************************************************************/
void uart5_send(u8 data)
{
	UART5->DR = data;
	while((UART5->SR&0x40)==0);	
}

/**************************ʵ�ֺ���**********************************************
*��    ��:		usart1����һ���ֽ�
*********************************************************************************/
void usart1_send(u8 data)
{
	USART1->DR = data;
	while((USART1->SR&0x40)==0);	
}

/**************************************************************************
�������ܣ�����5��ʼ��
��ڲ����� bound:������
����  ֵ����
**************************************************************************/
void uart5_init(u32 bound)
{  	 
	  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);// ��Ҫʹ��AFIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//ʹ��GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	//ʹ��GPIOʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//ʹ��USARTʱ��

	//USART_TX  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //Pc12
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);
   
  //USART_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//Pd2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  //UsartNVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(UART5, &USART_InitStructure);     //��ʼ������
  USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(UART5, ENABLE);                    //ʹ�ܴ���
}

/**************************************************************************
�������ܣ�����5�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
void UART5_IRQHandler(void)
{	
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) //���յ�����
	{	  
				u8 temp;
				static u8 count,last_data,last_last_data,Usart_ON_Count;
		    if(Usart_ON_Flag==0)
				{	
		    if(++Usart_ON_Count>10)Usart_ON_Flag=1;
				}
				temp=UART5->DR;
				 if(Usart_Flag==0)
					{	
					if(last_data==0xfe&&last_last_data==0xff) 
					Usart_Flag=1,count=0;	
					}
				 if(Usart_Flag==1)
					{	
						Urxbuf[count]=temp;     
						count++;                
						if(count==8)Usart_Flag=0;
					}
					last_last_data=last_data;
					last_data=temp;
	}  											 
} 


/**************************************************************************
�������ܣ�ͨ�����ڰ�����Ĵ��������ͳ�ȥ
**************************************************************************/
void UART_TX(void)
{
        u8 Direction_Left,Direction_Right;//���ʹ������ݵ��ⲿ
	      u16 Temp_GZ,Temp_Roll;
	           if(Encoder_Left>0) Direction_Left=0;
        else if(Encoder_Left<0) Direction_Left=2;
	      else                 		Direction_Left=1;
		         if(Encoder_Right>0) Direction_Right=0;
        else if(Encoder_Right<0) Direction_Right=2;
	      else                 		 Direction_Right=1;     

      	Temp_GZ=Gryo_Z+32768;//���ٶ����ݴ���
				Temp_Roll=Roll*100+15000;//ǰ������Ƕ�
				uart5_send(0xff); //֡ͷ
				uart5_send(0xfe); //֡ͷ
				uart5_send(abs(Encoder_Left));//�����ٶ�
				uart5_send(Direction_Left);	//��������
				uart5_send(abs(Encoder_Right));	//�ҵ���ٶ�	
				uart5_send(Direction_Right);	//�ҵ������
				uart5_send(Temp_Roll>>8);	//ǰ������Ƕ����ݸ�8λ	
				uart5_send(Temp_Roll&0x00ff);	//ǰ������Ƕ����ݵ�8λ
				uart5_send(Temp_GZ>>8);	//���������ݸ�8λ	
				uart5_send(Temp_GZ&0x00ff);	//���������ݵ�8λ
}

/**************************************************************************
�������ܣ�ͨ�����ڰ�����Ĵ��������ͳ�ȥ
**************************************************************************/
int send_cnt = 0;
static u8 Send_rasberry[60];
int re_Encoder_Left,re_Encoder_Right;
void USART_TX(void)
{
				 Send_rasberry[0] = 0xA5;
				 Send_rasberry[1] = 0x5A;
				 Send_rasberry[2] = 0x2E;
				 re_Encoder_Left = -Encoder_Left;
				 re_Encoder_Right = -Encoder_Right;
					for(send_cnt=0; send_cnt<4; send_cnt++)  
					{  
							Send_rasberry[3+send_cnt] = ((unsigned char *)&re_Encoder_Left)[send_cnt];  //Target_Left  Encoder_Left Motor_Left  Distance_A,Distance_B
					}  
					 
					for(send_cnt=0; send_cnt<4; send_cnt++)  
					{  
							Send_rasberry[7+send_cnt] = ((unsigned char *)&re_Encoder_Right)[send_cnt];  //Encoder_Right  Target_Right Motor_Right
					}  
					for(send_cnt=0; send_cnt<4; send_cnt++)  
					{  
							Send_rasberry[11+send_cnt] = ((unsigned char *)&Voltage)[send_cnt];  
					} 
					for(send_cnt=0; send_cnt<2; send_cnt++)  
					{  
							Send_rasberry[15+send_cnt] = ((unsigned char *)&accelX)[send_cnt];  
					}
					for(send_cnt=0; send_cnt<2; send_cnt++)  
					{  
							Send_rasberry[17+send_cnt] = ((unsigned char *)&accelY)[send_cnt];  
					}
					for(send_cnt=0; send_cnt<2; send_cnt++)  
					{  
							Send_rasberry[19+send_cnt] = ((unsigned char *)&accelZ)[send_cnt];  
					}
					//send gyro X Y Z
					for(send_cnt=0; send_cnt<2; send_cnt++)  
					{  
							Send_rasberry[21+send_cnt] = ((unsigned char *)&gyroX)[send_cnt];  
					}
					for(send_cnt=0; send_cnt<2; send_cnt++)  
					{  
							Send_rasberry[23+send_cnt] = ((unsigned char *)&gyroY)[send_cnt];  
					}
					for(send_cnt=0; send_cnt<2; send_cnt++)  
					{  
							Send_rasberry[25+send_cnt] = ((unsigned char *)&gyroZ)[send_cnt];  
					}
					//send MAG X Y Z
					for(send_cnt=0; send_cnt<2; send_cnt++)  
					{  
							Send_rasberry[27+send_cnt] = ((unsigned char *)&magX)[send_cnt];  
					}
					for(send_cnt=0; send_cnt<2; send_cnt++)  
					{  
							Send_rasberry[29+send_cnt] = ((unsigned char *)&magY)[send_cnt];  
					}
					for(send_cnt=0; send_cnt<2; send_cnt++)  
					{  
							Send_rasberry[31+send_cnt] = ((unsigned char *)&magZ)[send_cnt];  
					}
					
					//send ultrasonic A B C D
					for(send_cnt=0; send_cnt<4; send_cnt++)  
					{  
							Send_rasberry[33+send_cnt] = ((unsigned char *)&Distance_A)[send_cnt];  
					}
					for(send_cnt=0; send_cnt<4; send_cnt++)  
					{  
							Send_rasberry[37+send_cnt] = ((unsigned char *)&Distance_B)[send_cnt];  
					}
					for(send_cnt=0; send_cnt<4; send_cnt++)  
					{  
							Send_rasberry[41+send_cnt] = ((unsigned char *)&Distance_C)[send_cnt];  
					}
					for(send_cnt=0; send_cnt<4; send_cnt++)  
					{  
							Send_rasberry[45+send_cnt] = ((unsigned char *)&Distance_D)[send_cnt];  
					}
					//send Send_rasberry
				 for(send_cnt = 0;send_cnt < 49;send_cnt++){
						usart1_send(Send_rasberry[send_cnt]);
				 }
					memset(Send_rasberry, 0, sizeof(u8)*50);//��������
}

/*
void USART_TX(void)
{
				 Send_rasberry[0] = 0xA5;
				 Send_rasberry[1] = 0x5A;
				 Send_rasberry[2] = 0x06;
				 
					//send MAG X Y Z
					for(send_cnt=0; send_cnt<2; send_cnt++)  
					{  
							Send_rasberry[3+send_cnt] = ((unsigned char *)&magX)[send_cnt];  //accelZ magX
					}
					for(send_cnt=0; send_cnt<2; send_cnt++)  
					{  
							Send_rasberry[5+send_cnt] = ((unsigned char *)&magY)[send_cnt];  
					}
					for(send_cnt=0; send_cnt<2; send_cnt++)  
					{  
							Send_rasberry[7+send_cnt] = ((unsigned char *)&magZ)[send_cnt];  
					}
					
					//send Send_rasberry
				 for(send_cnt = 0;send_cnt < 9;send_cnt++){
						usart1_send(Send_rasberry[send_cnt]);
				 }
					memset(Send_rasberry, 0, sizeof(u8)*10);//��������
}
*/


