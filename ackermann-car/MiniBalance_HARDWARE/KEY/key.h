#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEY PBin(14)
void KEY_Init(void);          //������ʼ��
u8 click_N_Double (u8 time);  //��������ɨ���˫������ɨ��
u8 click(void);               //��������ɨ��
u8 Long_Press(void);           //����ɨ��  
u8  select(int);

void user_Key_EXTI_Init(void);	//�ⲿ�жϳ�ʼ��		 
#endif  
