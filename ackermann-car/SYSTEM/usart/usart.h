#ifndef __USRAT3_H
#define __USRAT3_H 
#include "sys.h"	  	

extern u8 Usart3_Receive;
void usart1_init(u32 bound);
void usart3_init(u32 bound);
int USART1_IRQHandler(void);
void USART3_IRQHandler(void);
#endif












