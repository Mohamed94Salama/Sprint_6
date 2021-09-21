#ifndef LCD_H
#define LCD_H

#include "MCAL/DIO/DIO.h"
#include "LCD_Config.h"
#include "STD_TYPES.h"

typedef enum {LCD_DATA_FOUR_BIT_MODE = 4, LCD_DATA_EIGHT_BIT_MODE = 8} enuLCDDataMode_t;
typedef enum {LCD_CURSOR_OFF, LCD_CURSOR_ON} enuLCDCursorMode_t;

/*************************************************************************************************
* Parameters	: enuLCDDataMode, enuLCDCursorMode
* Return Value	: enuErrorStatus_t
* Description  : Initialize LCD module according to the configuration parameters
*************************************************************************************************/
enuErrorStatus_t LCD_Init(enuLCDDataMode_t enuLCDDataMode, enuLCDCursorMode_t enuLCDCursorMode);
/*************************************************************************************************
* Parameters	: void
* Return Value	: enuErrorStatus_t
* Description  : Clear the LCD scree
*************************************************************************************************/
enuErrorStatus_t LCD_Clear(void);
/*************************************************************************************************
* Parameters	: u8Horizontal, u8Vertical
* Return Value	: enuErrorStatus_t
* Description  :  Navigate the cursor to the desired position
*************************************************************************************************/
enuErrorStatus_t LCD_SetCursorPosition(uint8_t u8Horizontal, uint8_t u8Vertical);
/*************************************************************************************************
* Parameters	: u8Char
* Return Value	: enuErrorStatus_t
* Description  : Display a single character on the LCD screen
*************************************************************************************************/
enuErrorStatus_t LCD_DisplayChar(const uint8_t u8Char);
/*************************************************************************************************
* Parameters	: pu8String
* Return Value	: enuErrorStatus_t
* Description  : Display a string on the LCD screen
*************************************************************************************************/
enuErrorStatus_t LCD_DisplayString(const uint8_t* pu8String);

#endif
