#include "encoder.h"
#include "stm32f10x_gpio.h"

/**************************************************************************
�������ܣ���TIM2��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);// ��Ҫʹ��AFIOʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//ʹ�ܶ�ʱ��2��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PA�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PB�˿�ʱ��
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);//������ӳ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOA
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB 
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //�趨�������Զ���װֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 10;
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM_SetCounter(TIM2,0);
  TIM_Cmd(TIM2, ENABLE); 
}
/*
void Encoder_Init_TIM3(void)
{
	RCC->APB1ENR|=1<<1;     //TIM3ʱ��ʹ��
	RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��
	GPIOA->CRL&=0X00FFFFFF; //PA6 PA7
	GPIOA->CRL|=0X44000000; //��������
	TIM3->DIER|=1<<0;   //��������ж�				
	TIM3->DIER|=1<<6;   //�������ж�
	MY_NVIC_Init(1,3,TIM3_IRQn,1);
	TIM3->PSC = 0x0;//Ԥ��Ƶ��
	TIM3->ARR = ENCODER_TIM_PERIOD;//�趨�������Զ���װֵ 
	TIM3->CR1 &=~(3<<8);// ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM3->CR1 &=~(3<<5);// ѡ�����ģʽ:���ض���ģʽ
		
	TIM3->CCMR1 |= 1<<0; //CC1S='01' IC1FP1ӳ�䵽TI1
	TIM3->CCMR1 |= 1<<8; //CC2S='01' IC2FP2ӳ�䵽TI2
	TIM3->CCER &= ~(1<<1);	 //CC1P='0'	 IC1FP1�����࣬IC1FP1=TI1
	TIM3->CCER &= ~(1<<5);	 //CC2P='0'	 IC2FP2�����࣬IC2FP2=TI2
	TIM3->CCMR1 |= 3<<4; //	IC1F='1000' ���벶��1�˲���
	TIM3->SMCR |= 3<<0;	 //SMS='011' ���е�������������غ��½�����Ч
	TIM3->CR1 |= 0x01;    //CEN=1��ʹ�ܶ�ʱ��
}

void Encoder_Init_TIM3(void)
{
	RCC->APB1ENR|=1<<1;     //TIM2ʱ��ʹ��
	RCC->APB2ENR|=1<<3;    //ʹ��PORTAʱ��
	GPIOA->CRL&=0X00FFFFFF;//PA6 PA7
	GPIOA->CRL|=0X44000000;//��������
	TIM3->DIER|=1<<0;   //��������ж�				
	TIM3->DIER|=1<<6;   //�������ж�
	MY_NVIC_Init(1,3,TIM3_IRQn,1);
	TIM3->PSC = 0x0;//Ԥ��Ƶ��
	TIM3->ARR = ENCODER_TIM_PERIOD;//�趨�������Զ���װֵ 
	TIM3->CR1 &=~(3<<8);// ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM3->CR1 &=~(3<<5);// ѡ�����ģʽ:���ض���ģʽ
		
	TIM3->CCMR1 |= 1<<0; //CC1S='01' IC1FP1ӳ�䵽TI1
	TIM3->CCMR1 |= 1<<8; //CC2S='01' IC2FP2ӳ�䵽TI2
	TIM3->CCER &= ~(1<<1);	 //CC1P='0'	 IC1FP1�����࣬IC1FP1=TI1
	TIM3->CCER &= ~(1<<5);	 //CC2P='0'	 IC2FP2�����࣬IC2FP2=TI2
//	TIM2->CCMR1 |= 3<<4; //	IC1F='1000' ���벶��1�˲���
	TIM3->SMCR |= 3<<0;	 //SMS='011' ���е�������������غ��½�����Ч
	TIM3->CR1 |= 0x01;    //CEN=1��ʹ�ܶ�ʱ��
}
*/
/**************************************************************************
�������ܣ���TIM3��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/

void Encoder_Init_TIM3(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);// ��Ҫʹ��AFIOʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//ʹ�ܶ�ʱ��3��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PB�˿�ʱ��
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);//������ӳ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;	//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //�趨�������Զ���װֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
  TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 10;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM_SetCounter(TIM3,0);
  TIM_Cmd(TIM3, ENABLE); 
}

/**************************************************************************
�������ܣ���λʱ���ȡ����������
��ڲ�������ʱ��
����  ֵ���ٶ�ֵ
**************************************************************************/
int Read_Encoder(u8 TIMX)
{
    int Encoder_TIM;    
   switch(TIMX)
	 {
	   case 2:  
			 Encoder_TIM= (short)TIM2 -> CNT; 
			TIM2 -> CNT=0;break;
		 case 3:  
			 Encoder_TIM= (short)TIM3 -> CNT;  
			TIM3 -> CNT=0;break;	
		 default:  Encoder_TIM=0;
	 }
		Encoder_TIM=(Encoder_TIM*11/17);
	 
		return Encoder_TIM;
}
/**************************************************************************
�������ܣ�TIM3�жϷ�����
��ڲ�������
����  ֵ����
**************************************************************************/
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//����ж�
	{    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}
/**************************************************************************
�������ܣ�TIM2�жϷ�����
��ڲ�������
����  ֵ����
**************************************************************************/
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//����ж�
	{    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//����жϱ�־λ 	    
}

