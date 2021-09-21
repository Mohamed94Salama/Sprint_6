/*
 * T_Display.h
 *
 * Created: 9/13/2021 2:01:27 AM
 *  Author: Amrth
 */ 


#ifndef T_DISPLAY_H_
#define T_DISPLAY_H_

#include "LCD.h"
#include "7seg.h"

#define DIPLAY_SSD_UPDATE_STEP          10u
#define DIPLAY_LCD_UPDATE_STEP          50u

typedef enum {T_LCD_CLR,T_LCD_LOADING,
              T_LCD_WELCOME,T_LCD_PASS_EN,
              T_LCD_PASS_OK,T_LCD_PASS_NOK,
              T_LCD_WAITING,T_LCD_DATA_EN,
              T_LCD_PASS_CHANGE,T_LCD_INVALID}enuDispMsg_t;
              

enuErrorStatus_t T_DISP_LCDSetMode(enuDispMsg_t Message);
enuErrorStatus_t T_DISP_LCDAddPassword(void);
enuErrorStatus_t T_DISP_LCDDelPassword(void);

enuErrorStatus_t T_DISP_SSDSetNum(uint8_t num);

void Display_Task (void);

#endif /* T_DISPLAY_H_ */