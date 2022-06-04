#include "timer.h"

void TIM6_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	//ʹ�ܻ���ʧ��ָ����TIM�ж�  //ʹ��
	TIM_ITConfig( TIM6, TIM_IT_Update ,ENABLE );
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM6, ENABLE);  //ʹ��TIMx����
							 
}

/**************************************************************************
�������ܣ���ʱ��4ͨ�����벶���ʼ��
��ڲ�������ڲ�����arr���Զ���װֵ  psc��ʱ��Ԥ��Ƶ�� 
����  ֵ����
**************************************************************************/
void TIM4_Cap_Init(u16 arr,u16 psc)	
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	//ʹ��TIM5ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //ʹ��GPIOʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA  ����  //PB7��RX
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5;     //PB4��TX��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //��� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;     //2M
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//��ʼ����ʱ��4	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM4���벶�����
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //ѡ������� 
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM_ICInitStructure.TIM_ICFilter = 0x00;//���������˲��� ���˲�
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
		//��ʼ��TIM4���벶�����
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; //ѡ������� 
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM_ICInitStructure.TIM_ICFilter = 0x00;//���������˲��� ���˲�
  TIM_ICInit(TIM4, &TIM_ICInitStructure);


	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 	
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2,ENABLE);//��������ж� ,�������ж�	
  TIM_Cmd(TIM4,ENABLE ); 	//ʹ�ܶ�ʱ��
}

/**************************************************************************
�������ܣ���ʱ��5ͨ�����벶���ʼ��
��ڲ�������ڲ�����arr���Զ���װֵ  psc��ʱ��Ԥ��Ƶ�� 
����  ֵ����
**************************************************************************/
void TIM5_Cap_Init(u16 arr,u16 psc)	
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);	//ʹ��TIM5ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);  //ʹ��GPIOʱ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA  ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_4;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //��� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;     //2M
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//��ʼ����ʱ��5	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM5���벶�����
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //ѡ������� 
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM_ICInitStructure.TIM_ICFilter = 0x00;//���������˲��� ���˲�
  TIM_ICInit(TIM5, &TIM_ICInitStructure);
	
		//��ʼ��TIM5���벶�����
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; //ѡ������� 
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM_ICInitStructure.TIM_ICFilter = 0x00;//���������˲��� ���˲�
  TIM_ICInit(TIM5, &TIM_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2,ENABLE);//��������ж� ,�������ж�	
  TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��
}
u16 TIM5CH1_CAPTURE_STA,TIM5CH1_CAPTURE_VAL,TIM5CH2_CAPTURE_STA,TIM5CH2_CAPTURE_VAL;
u16 TIM4CH1_CAPTURE_STA,TIM4CH1_CAPTURE_VAL,TIM4CH2_CAPTURE_STA,TIM4CH2_CAPTURE_VAL;
/**************************************************************************
�������ܣ����������ջز�����
��ڲ�������
����  ֵ����
**************************************************************************/
void Read_Distane(void)
{   
	 TRIPA=1;//�ߵ�ƽ����
	 delay_us(15);  //��ʱ
	 TRIPA=0;	//�͵�ƽ
		 if(TIM5CH1_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			Distance_A=TIM5CH1_CAPTURE_STA&0X3F;
			Distance_A*=65536;	 //���ʱ���ܺ�
			Distance_A+=TIM5CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
			Distance_A=Distance_A*170/1000;//ת����mmΪ��λ�����������ڿ����д����ٶ�340m/s
			TIM5CH1_CAPTURE_STA=0;			//������һ�β���
		}	
		
	  TRIPB=1;//�ߵ�ƽ����
	  delay_us(15);  //��ʱ
	  TRIPB=0;	//�͵�ƽ
		 if(TIM5CH2_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			Distance_B=TIM5CH2_CAPTURE_STA&0X3F;
			Distance_B*=65536;	 //���ʱ���ܺ�
			Distance_B+=TIM5CH2_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
			Distance_B=Distance_B*170/1000;//ת����mmΪ��λ�����������ڿ����д����ٶ�340m/s
			TIM5CH2_CAPTURE_STA=0;			//������һ�β���
		}		

	  TRIPC=1;//�ߵ�ƽ����
	  delay_us(15);  //��ʱ
	  TRIPC=0;	//�͵�ƽ
		 if(TIM4CH1_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			Distance_C=TIM4CH1_CAPTURE_STA&0X3F;
			Distance_C*=65536;	 //���ʱ���ܺ�
			Distance_C+=TIM4CH1_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
			Distance_C=Distance_C*170/1000;//ת����mmΪ��λ�����������ڿ����д����ٶ�340m/s
			TIM4CH1_CAPTURE_STA=0;			//������һ�β���
		}		

	  TRIPD=1;//�ߵ�ƽ����
	  delay_us(15);  //��ʱ
	  TRIPD=0;	//�͵�ƽ
		 if(TIM4CH2_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			Distance_D=TIM4CH2_CAPTURE_STA&0X3F;
			Distance_D*=65536;	 //���ʱ���ܺ�
			Distance_D+=TIM4CH2_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
			Distance_D=Distance_D*170/1000;//ת����mmΪ��λ�����������ڿ����д����ٶ�340m/s
			TIM4CH2_CAPTURE_STA=0;			//������һ�β���
		}				
}
/**************************************************************************
�������ܣ��������ز������ȡ�ж�
��ڲ�������
����  ֵ����
**************************************************************************/
void TIM5_IRQHandler(void)
{ 		    		  			    
	u16 tsr;
	tsr=TIM5->SR;
				/////////////////////ͨ��һ///////////////////////////////
	if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
				{
					if(tsr&0X01)//���
						{	     
								if(TIM5CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
								{
									if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
									{
										TIM5CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
										TIM5CH1_CAPTURE_VAL=0XFFFF;
									}else TIM5CH1_CAPTURE_STA++;
								}	 
						}
						if(tsr&0x02)//����1���������¼�
					{	 	
									if(TIM5CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
									{	  	
									TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
									TIM5CH1_CAPTURE_VAL=TIM5->CCR1;	//��ȡ��ǰ�Ĳ���ֵ.
									TIM5->CCER&=~(1<<1);			//CC1P=0 ����Ϊ�����ز���
									}else  								//��δ��ʼ,��һ�β���������
									{	
									TIM5CH1_CAPTURE_STA=0;			//���
									TIM5CH1_CAPTURE_VAL=0;
									TIM5CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
									TIM5->CNT=0;					//���������
									TIM5->CCER|=1<<1; 				//CC1P=1 ����Ϊ�½��ز���
									}		    
					 }			     	    					   
		   }
		 /////////////////////ͨ����///////////////////////////////
			if((TIM5CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
				{
					if(tsr&0X01)//���
						{	     
								if(TIM5CH2_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
								{
									if((TIM5CH2_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
									{
										TIM5CH2_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
										TIM5CH2_CAPTURE_VAL=0XFFFF;
									}else TIM5CH2_CAPTURE_STA++;
								}	 
						}
						if(tsr&0x04)//����2���������¼�
					{	 	
									if(TIM5CH2_CAPTURE_STA&0X40)		//����һ���½��� 		
									{	  	
									TIM5CH2_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
									TIM5CH2_CAPTURE_VAL=TIM5->CCR2;	//��ȡ��ǰ�Ĳ���ֵ.
									TIM5->CCER&=~(1<<5);   //CC1P=0 ����Ϊ�����ز���
									}else  								//��δ��ʼ,��һ�β���������
									{	
									TIM5CH2_CAPTURE_STA=0;//���
									TIM5CH2_CAPTURE_VAL=0;
									TIM5CH2_CAPTURE_STA|=0X40;//��ǲ�����������
									TIM5->CNT=0;					//���������
									TIM5->CCER|=1<<5; 				//CC1P=1 ����Ϊ�½��ز���
										}		    
					 }			     	    					   
		   } 
			 TIM5->SR=0;//����жϱ�־λ 	     
}


/**************************************************************************
�������ܣ��������ز������ȡ�ж�
��ڲ�������
����  ֵ����
**************************************************************************/
void TIM4_IRQHandler(void)
{ 		    		  			    
	u16 tsr;
	tsr=TIM4->SR;
				/////////////////////ͨ��һ///////////////////////////////
	if((TIM4CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
				{
					if(tsr&0X01)//���
						{	     
								if(TIM4CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
								{
									if((TIM4CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
									{
											TIM4CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
											TIM4CH1_CAPTURE_VAL=0XFFFF;
									}else TIM4CH1_CAPTURE_STA++;
								}	 
						}
						if(tsr&0x02)//����1���������¼�
					{	 	
									if(TIM4CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
									{	  	
											TIM4CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
											TIM4CH1_CAPTURE_VAL=TIM4->CCR1;	//��ȡ��ǰ�Ĳ���ֵ.
											TIM4->CCER&=~(1<<1);			//CC1P=0 ����Ϊ�����ز���
									}else  								//��δ��ʼ,��һ�β���������
									{	
											TIM4CH1_CAPTURE_STA=0;			//���
											TIM4CH1_CAPTURE_VAL=0;
											TIM4CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
											TIM4->CNT=0;					//���������
											TIM4->CCER|=1<<1; 				//CC1P=1 ����Ϊ�½��ز���
									}		    
					 }			     	    					   
		   }
		 /////////////////////ͨ����///////////////////////////////
			if((TIM4CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
				{
					if(tsr&0X01)//���
						{	     
								if(TIM4CH2_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
								{
									if((TIM4CH2_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
									{
										TIM4CH2_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
										TIM4CH2_CAPTURE_VAL=0XFFFF;
									}else TIM4CH2_CAPTURE_STA++;
								}	 
						}
						if(tsr&0x04)//����2���������¼�
						{	 	
								if(TIM4CH2_CAPTURE_STA&0X40)		//����һ���½��� 		
								{	  	
										TIM4CH2_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
										TIM4CH2_CAPTURE_VAL=TIM4->CCR2;	//��ȡ��ǰ�Ĳ���ֵ.
										TIM4->CCER&=~(1<<5);   //CC1P=0 ����Ϊ�����ز���
								}else  								//��δ��ʼ,��һ�β���������
								{	
										TIM4CH2_CAPTURE_STA=0;//���
										TIM4CH2_CAPTURE_VAL=0;
										TIM4CH2_CAPTURE_STA|=0X40;//��ǲ�����������
										TIM4->CNT=0;					//���������
										TIM4->CCER|=1<<5; 				//CC1P=1 ����Ϊ�½��ز���
								}		    
						}			     	    					   
		   } 
	     
			 TIM4->SR=0;//����жϱ�־λ 	     
}




