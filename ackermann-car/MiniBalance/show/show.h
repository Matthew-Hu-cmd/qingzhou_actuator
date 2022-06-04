#ifndef __SHOW_H
#define __SHOW_H
#include "sys.h"

void oled_show(void);
void APP_Show(void);
void OLED_DrawPoint_Shu(u8 x,u8 y,u8 t);
void OLED_Show_CCD(void);
void oled_show_once(void);
void DataScope(void);
#endif
