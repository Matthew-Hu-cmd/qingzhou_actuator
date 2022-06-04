#ifndef __LED_H
#define __LED_H
#include "sys.h"

//LED 端口定义
#define LED_S PBout(14)  //LED控制引脚
#define LED_B PAout(5)  //LED控制引脚
void LED_Init(void);  //初始化
void Led_Flash(u16 time);
void Led_On(void);
#endif
