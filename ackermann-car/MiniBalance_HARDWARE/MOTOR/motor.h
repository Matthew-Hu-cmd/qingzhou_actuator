#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 

#define PWMA1   TIM8->CCR2  
#define PWMA2   TIM8->CCR1 

#define SERVO   TIM1->CCR1  //¶æ»úÒý½Å

#define PWMB1   TIM8->CCR4  
#define PWMB2   TIM8->CCR3
void MiniBalance_PWM_Init(u16 arr,u16 psc);
void Servo_PWM_Init(u16 arr,u16 psc);
#endif
