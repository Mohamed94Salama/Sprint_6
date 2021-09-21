#ifndef KEYPAD_H
#define KEYPAD_H

#include "Keypad_Config.h"
#include "MCAL/DIO/DIO.h"
#include "STD_TYPES.h"

typedef enum {KEYPAD_4X4, KEYPAD_4X3} enuKeypadType_t;

/*************************************************************************************************
* Parameters	: enuKeypadType
* Return Value	: enuErrorStatus_t
* Description  : Initialize the Keypad according to the configuration
*************************************************************************************************/
enuErrorStatus_t Keypad_Init(enuKeypadType_t enuKeypadType);
/*************************************************************************************************
* Parameters	: pu8Data
* Return Value	: enuErrorStatus_t
* Description  : Get the current character of the pressed key from the Keypad
*************************************************************************************************/
enuErrorStatus_t Keypad_GetChar(uint8_t* au8ButtonBuffer,uint8_t u8BufferSize);

#endif
