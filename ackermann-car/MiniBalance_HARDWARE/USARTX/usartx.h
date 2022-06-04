#ifndef __USRATX_H
#define __USRATX_H 
#include "sys.h"	  	

void usart1_send(u8 data);
void uart5_send(u8 data);
void uart5_init(u32 bound);
void UART5_IRQHandler(void);
void UART_TX(void);
void USART_TX(void);
#endif

