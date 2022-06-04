#ifndef __TIMER_H
#define __TIMER_H
#include <sys.h>	 

#define TRIPA PCout(0)//A路超声波 PC0接触发引脚 PA0接回波引脚
#define TRIPB PCout(4)//B路超声波 PC4接触发引脚 PA1接回波引脚
#define TRIPC PBout(5)//C路超声波 PB5接触发引脚 PB6接回波引脚
#define TRIPD PBout(4)//D路超声波 PB4接触发引脚 PB7接回波引脚

void TIM5_Cap_Init(u16 arr,u16 psc)	; 
void TIM4_Cap_Init(u16 arr,u16 psc)	;  
void Read_Distane(void);
void TIM5_IRQHandler(void);
void TIM4_IRQHandler(void);
//void TIM3_Cap_Init(u16 arr, u16 psc);
//void TIM3_IRQHandler(void);

void TIM6_Int_Init(u16 arr,u16 psc); 
#endif
