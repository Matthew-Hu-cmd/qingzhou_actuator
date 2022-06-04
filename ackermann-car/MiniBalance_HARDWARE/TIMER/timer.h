#ifndef __TIMER_H
#define __TIMER_H
#include <sys.h>	 

#define TRIPA PCout(0)//A·������ PC0�Ӵ������� PA0�ӻز�����
#define TRIPB PCout(4)//B·������ PC4�Ӵ������� PA1�ӻز�����
#define TRIPC PBout(5)//C·������ PB5�Ӵ������� PB6�ӻز�����
#define TRIPD PBout(4)//D·������ PB4�Ӵ������� PB7�ӻز�����

void TIM5_Cap_Init(u16 arr,u16 psc)	; 
void TIM4_Cap_Init(u16 arr,u16 psc)	;  
void Read_Distane(void);
void TIM5_IRQHandler(void);
void TIM4_IRQHandler(void);
//void TIM3_Cap_Init(u16 arr, u16 psc);
//void TIM3_IRQHandler(void);

void TIM6_Int_Init(u16 arr,u16 psc); 
#endif
