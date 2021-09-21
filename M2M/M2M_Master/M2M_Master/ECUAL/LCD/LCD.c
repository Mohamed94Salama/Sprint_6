#include "LCD.h"

/*Include OS header file*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"



/* Internal Macros */
#define LCD_CUROSR_MODE_MIN_VALUE												-1l
#define LCD_CUROSR_MODE_MAX_VALUE												2u
#define LCD_CLEAR_DISPLAY_SCREEN_COMMAND										0x01
#define LCD_RETURN_HOME_COMMAND													0x02
#define LCD_DECREMENT_CURSOR_COMMAND											0x04
#define LCD_SHIFT_DISPLAY_RIGHT_COMMAND										0x05
#define LCD_INCREMENT_CURSOR_COMMAND											0x06
#define LCD_SHIFT_DISPLAY_LEFT_COMMAND											0x07
#define LCD_DISPLAY_OFF_CURSOR_OFF_COMMAND									0x08
#define LCD_DISPLAY_OFF_CURSOR_ON_COMMAND										0x0A
#define LCD_DISPLAY_ON_CURSOR_OFF_COMMAND										0x0C
#define LCD_DISPLAY_ON_CURSOR_BLINKING_COMMAND								0x0E
#define LCD_DISPLAY_ON_CURSOR_ON_COMMAND										0x0F
#define LCD_SHIFT_CURSOR_POSITION_TO_LEFT_COMMAND							0x10
#define LCD_SHIFT_CURSOR_POSITION_TO_RIGHT_COMMAND							0x14
#define LCD_SHIFT_ENTIRE_DISPLAY_TO_LEFT_COMMAND							0x18
#define LCD_SHIFT_ENTIRE_DISPLAY_TO_RIGHT_COMMAND							0x1C
#define LCD_TWO_LINES_FIVE_TIMES_SEVEN_MATRIX_FOUR_BIT_MODE_COMMAND	0x28
#define LCD_DATA_FOUR_BIT_MODE_CONFIG_TWO_COMMAND							0x32
#define LCD_DATA_FOUR_BIT_MODE_CONFIG_ONE_COMMAND							0x33
#define LCD_TWO_LINES_FIVE_TIMES_SEVEN_MATRIX_EIGHT_BIT_MODE_COMMAND	0x38
#define LCD_FORCE_CURSOR_TO_FIRST_LINE_COMMAND								0x80
#define LCD_FORCE_CURSOR_TO_SECOND_LINE_COMMAND								0xC0


/* Sending a given Command to the LCD controller */
static void LCD_SendCommand(uint8_t u8CommandId);
/* Sending a given data to the LCD controller */
static void LCD_SendData(uint8_t u8DataId);
/* Giving the LCD controller sufficient time to receive commands */
static void LCD_Delay(uint16_t u16DelayValue);

typedef enum {LCD_INIT_UNDONE, LCD_INIT_DONE} enuLCDState_t;
static enuLCDState_t enuLCDState = LCD_INIT_UNDONE;

static uint8_t au8LCDDataPins8BitMode[LCD_DATA_EIGHT_BIT_MODE] = {LCD_DB0_PIN_NO, LCD_DB1_PIN_NO, LCD_DB2_PIN_NO, LCD_DB3_PIN_NO,
																						LCD_DB4_PIN_NO, LCD_DB5_PIN_NO, LCD_DB6_PIN_NO, LCD_DB7_PIN_NO};
static uint8_t au8LCDDataPins4BitMode[LCD_DATA_FOUR_BIT_MODE] = {LCD_DB4_PIN_NO, LCD_DB5_PIN_NO, LCD_DB6_PIN_NO, LCD_DB7_PIN_NO};

static enuLCDDataMode_t enuLCDDataModeState;

/*************************************************************************************************
* Parameters	: enuLCDDataMode, enuLCDCursorMode
* Return Value	: enuErrorStatus_t
* Description  : Initialize LCD module according to the configuration parameters
*************************************************************************************************/
enuErrorStatus_t LCD_Init(enuLCDDataMode_t enuLCDDataMode, enuLCDCursorMode_t enuLCDCursorMode)
{
	enuErrorStatus_t enuRetVar;
	uint8_t u8LoopIndex;
	
	if(((enuLCDDataMode == LCD_DATA_FOUR_BIT_MODE)		|| (enuLCDDataMode == LCD_DATA_EIGHT_BIT_MODE))	&&
		 (enuLCDCursorMode > LCD_CUROSR_MODE_MIN_VALUE)	&& (enuLCDCursorMode < LCD_CUROSR_MODE_MAX_VALUE))
	{
		enuLCDDataModeState = enuLCDDataMode;
		/* Initializing RS pin as output */
		DIO_PinInit(LCD_RS_PIN_NO, OUTPUT, NO_CONNECTION);
		/* Initializing RW pin as output */
		DIO_PinInit(LCD_RW_PIN_NO, OUTPUT, NO_CONNECTION);
		/* Initializing EN pin as output */
		DIO_PinInit(LCD_EN_PIN_NO, OUTPUT, NO_CONNECTION);
		/* Selecting whether to interface LCD in 4-bit mode or 8-bit mode */
		if(enuLCDDataMode == LCD_DATA_FOUR_BIT_MODE)
		{
			/* Initializing all LCD data pins as outputs */
			for(u8LoopIndex = 0; u8LoopIndex < enuLCDDataMode; u8LoopIndex++)
			{
				DIO_PinInit(au8LCDDataPins4BitMode[u8LoopIndex], OUTPUT, NO_CONNECTION);
			}
			/* Initializing the LCD module to work as 4-bit data mode */
			LCD_SendCommand(LCD_DATA_FOUR_BIT_MODE_CONFIG_ONE_COMMAND);
			LCD_SendCommand(LCD_DATA_FOUR_BIT_MODE_CONFIG_TWO_COMMAND);
			LCD_SendCommand(LCD_TWO_LINES_FIVE_TIMES_SEVEN_MATRIX_FOUR_BIT_MODE_COMMAND);
		}
		else
		{
			/* Initializing all LCD data pins as outputs */
			for(u8LoopIndex = 0; u8LoopIndex < enuLCDDataMode; u8LoopIndex++)
			{
				DIO_PinInit(au8LCDDataPins8BitMode[u8LoopIndex], OUTPUT, NO_CONNECTION);
			}		
			/* Initializing the LCD module to work as 8-bit data mode */
			LCD_SendCommand(LCD_TWO_LINES_FIVE_TIMES_SEVEN_MATRIX_EIGHT_BIT_MODE_COMMAND);
		}
		/* Selecting whether to turn on/off the blinking cursor */
		if(enuLCDCursorMode == LCD_CURSOR_OFF)
		{
			LCD_SendCommand(LCD_DISPLAY_ON_CURSOR_OFF_COMMAND);
		}
		else
		{
			LCD_SendCommand(LCD_DISPLAY_ON_CURSOR_BLINKING_COMMAND);
		}
		/* Clearing the LCD screen before use */
		LCD_SendCommand(LCD_CLEAR_DISPLAY_SCREEN_COMMAND);
		enuLCDState = LCD_INIT_DONE;
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;
}

/*************************************************************************************************
* Parameters	: void
* Return Value	: enuErrorStatus_t
* Description  : Clear the LCD scree
*************************************************************************************************/
enuErrorStatus_t LCD_Clear(void)
{
	enuErrorStatus_t enuRetVar;
	
	if(enuLCDState == LCD_INIT_DONE)
	{
		/* Clearing the screen */
		LCD_SendCommand(LCD_CLEAR_DISPLAY_SCREEN_COMMAND);
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;
}

/*************************************************************************************************
* Parameters	: u8Horizontal, u8Vertical
* Return Value	: enuErrorStatus_t
* Description  :  Navigate the cursor to the desired position
*************************************************************************************************/
enuErrorStatus_t LCD_SetCursorPosition(uint8_t u8Horizontal, uint8_t u8Vertical)
{
	enuErrorStatus_t enuRetVar;
	
	if((u8Vertical == 0) && (enuLCDState == LCD_INIT_DONE))
	{
		/* Moving the cursor in the selected place in the first row */
		LCD_SendCommand(LCD_FORCE_CURSOR_TO_FIRST_LINE_COMMAND + u8Horizontal);
		enuRetVar = E_OK;
	}
	else if((u8Vertical == 1) && (enuLCDState == LCD_INIT_DONE))
	{
		/* Moving the cursor in the selected place in the second row */
		LCD_SendCommand(LCD_FORCE_CURSOR_TO_SECOND_LINE_COMMAND + u8Horizontal);
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;
}

/*************************************************************************************************
* Parameters	: u8Char
* Return Value	: enuErrorStatus_t
* Description  : Display a single character on the LCD screen
*************************************************************************************************/
enuErrorStatus_t LCD_DisplayChar(const uint8_t u8Char)
{
	enuErrorStatus_t enuRetVar;
	
	if(enuLCDState == LCD_INIT_DONE)
	{
		/* Displaying the character data on the LCD */
      LCD_SendData(u8Char);
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;
}

/*************************************************************************************************
* Parameters	: pu8String
* Return Value	: enuErrorStatus_t
* Description  : Display a string on the LCD screen
*************************************************************************************************/
enuErrorStatus_t LCD_DisplayString(const uint8_t* pu8String)
{
	enuErrorStatus_t enuRetVar;
	
	if(enuLCDState == LCD_INIT_DONE)
	{
		/* Displaying the string data on the LCD */
		while((*pu8String) != '\0')
		{
			LCD_SendData(*pu8String);
			pu8String ++;
		}
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;
}


/*************************************************************************************************
* Parameters	: u8CommandId
* Return Value	: void
* Description  : Send a given Command to the LCD controller
*************************************************************************************************/
static void LCD_SendCommand(uint8_t u8CommandId)
{
	uint8_t u8LoopIndex;
	uint8_t u8DIOData;
	uint8_t u8Command;

    /* Sending a LOW value on RS to select the u8Command mode on LCD */
	DIO_PinWrite(LCD_RS_PIN_NO, LOW);
	/* Sending a LOW value on RW to specify a write to LCD */
   DIO_PinWrite(LCD_RW_PIN_NO, LOW);
   /* Sending the 8-bit u8Command on the 8 data pins once */
	if(enuLCDDataModeState == LCD_DATA_EIGHT_BIT_MODE)
	{
		for(u8LoopIndex = 0; u8LoopIndex < enuLCDDataModeState; u8LoopIndex++)
		{
			/* Getting the LSB of u8CommandId */
         u8DIOData = u8CommandId & 0x01;
         /* Converting from binary value into 8-bit value to be used in DIO_PinWrite() API *
          * e.g: u8DIOData = 0x01 -> u8DIOData = 0xFE + 1 = 0xFF (HIGH)                    *
          *      u8DIOData = 0x00 -> u8DIOData = 0xFF + 1 = 0x00 (LOW)                     */
			u8DIOData = (~u8DIOData)+ 1;
         /* Shifting the u8CommandId variable by one for the next loop iteration */
			u8CommandId = u8CommandId >> 1;
         /* Writing the selected bit on its corresponding LCD pin */
			DIO_PinWrite(au8LCDDataPins8BitMode[u8LoopIndex], u8DIOData);
		}
      /* Triggering a falling edge on EN pin to send the u8Command */
		DIO_PinWrite(LCD_EN_PIN_NO, HIGH);
		DIO_PinWrite(LCD_EN_PIN_NO, LOW);
	}
   /* Sending the 8-bit u8Command on the 4 data pins twice */
	else if(enuLCDDataModeState == LCD_DATA_FOUR_BIT_MODE)
	{
		/* Specifying the 4 MSB of the u8CommandId */
      u8Command = (u8CommandId & 0xF0) >> 4;
      /* Sending the MSB on the 4-bit data pins */
		for(u8LoopIndex = 0; u8LoopIndex < enuLCDDataModeState; u8LoopIndex++)
		{
			u8DIOData = u8Command & 0x01;
			u8DIOData = (~u8DIOData) + 1;
			u8Command = u8Command >> 1;
			DIO_PinWrite(au8LCDDataPins4BitMode[u8LoopIndex], u8DIOData);
		}
      /* Triggering a falling edge on EN pin to send the MSB of the u8Command */
		DIO_PinWrite(LCD_EN_PIN_NO, HIGH);
		DIO_PinWrite(LCD_EN_PIN_NO, LOW);
      /* Specifying the 4 LSB of the u8CommandId*/
		u8Command = (u8CommandId & 0x0F);
      /* Sending the LSB on the 4-bit data pins */
		for(u8LoopIndex = 0; u8LoopIndex < enuLCDDataModeState; u8LoopIndex++)
		{
			u8DIOData = u8Command & 0x01;
			u8DIOData = (~u8DIOData) + 1;
			u8Command = u8Command >> 1;
			DIO_PinWrite(au8LCDDataPins4BitMode[u8LoopIndex], u8DIOData);
		}
        /* Triggering a falling edge on EN pin to send the LSB of the u8Command */
		DIO_PinWrite(LCD_EN_PIN_NO, HIGH);
		DIO_PinWrite(LCD_EN_PIN_NO, LOW);
		LCD_Delay(2);
	}
	else
	{
	}
}

/*************************************************************************************************
* Parameters	: u8DataId
* Return Value	: void
* Description  : Send a given Data to the LCD controller
*************************************************************************************************/
static void LCD_SendData(uint8_t u8DataId)
{
	uint16_t u8LoopIndex;
	uint8_t u8DIOData;
	uint8_t u8Command;

   /* Sending a HIGH value on RS to select the data mode on LCD */
	DIO_PinWrite(LCD_RS_PIN_NO, HIGH);
	DIO_PinWrite(LCD_RW_PIN_NO, LOW);
	if(enuLCDDataModeState == LCD_DATA_EIGHT_BIT_MODE)
	{
		for(u8LoopIndex = 0; u8LoopIndex < enuLCDDataModeState; u8LoopIndex++)
		{
			u8DIOData = u8DataId & 0x01;
			u8DIOData = (~u8DIOData) + 1;
			u8DataId = u8DataId >> 1;
			DIO_PinWrite(au8LCDDataPins8BitMode[u8LoopIndex], u8DIOData);
		}
		DIO_PinWrite(LCD_EN_PIN_NO, HIGH);
		DIO_PinWrite(LCD_EN_PIN_NO, LOW);
	}
	else if(enuLCDDataModeState == LCD_DATA_FOUR_BIT_MODE)
	{
		u8Command = (u8DataId & 0xF0) >> 4;
		for(u8LoopIndex = 0; u8LoopIndex < enuLCDDataModeState; u8LoopIndex++)
		{
			u8DIOData = u8Command & 0x01;
			u8DIOData = (~u8DIOData) + 1;
			u8Command = u8Command >> 1;
			DIO_PinWrite(au8LCDDataPins4BitMode[u8LoopIndex], u8DIOData);
		}
		DIO_PinWrite(LCD_EN_PIN_NO, HIGH);
		DIO_PinWrite(LCD_EN_PIN_NO, LOW);
		u8Command = (u8DataId & 0x0F);
		for(u8LoopIndex = 0; u8LoopIndex < enuLCDDataModeState; u8LoopIndex++)
		{
			u8DIOData = u8Command & 0x01;
			u8DIOData = (~u8DIOData) + 1;
			u8Command = u8Command >> 1;
			DIO_PinWrite(au8LCDDataPins4BitMode[u8LoopIndex], u8DIOData);
		}
		DIO_PinWrite(LCD_EN_PIN_NO, HIGH);
		DIO_PinWrite(LCD_EN_PIN_NO, LOW);
	}
	else
	{
	}
}

/*************************************************************************************************
* Parameters	: u8DelayValue
* Return Value	: void
* Description  : Give the LCD controller sufficient time to receive commands
*************************************************************************************************/
static void LCD_Delay(uint16_t u16DelayValue)
{
	vTaskDelay(u16DelayValue);
}
