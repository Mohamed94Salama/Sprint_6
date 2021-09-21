#include "../../MCAL/DIO/DIO.h"
#include "Keypad.h"

/* Internal Macros */
#define KEYPAD_TYPE_MIN_VALUE																-1l
#define KEYPAD_TYPE_MAX_VALUE																2u
#define KEYPAD_COLUMNS_MODE_MIN_VALUE													-1l
#define KEYPAD_COLUMNS_MODE_MAX_VALUE													2u
#define KEYPAD_CALCULATE_CORRESPONDING_KEY_VALUE(COL_INDEX, ROW_INDEX)		(COL_INDEX + (3 * (ROW_INDEX ))+1)
#define KEYPAD_ROWS_NUMBER																	4u
#define KEYPAD_COLS_NUMBER																	4u

typedef enum {KEYPAD_INIT_UNDONE, KEYPAD_INIT_DONE} enuKeypadState_t;
static enuKeypadState_t enuKeypadState = KEYPAD_INIT_UNDONE;

static uint8_t au8KeypadRows[KEYPAD_ROWS_NUMBER] = {KEYPAD_ROW_0_PIN_NO, KEYPAD_ROW_1_PIN_NO, KEYPAD_ROW_2_PIN_NO, KEYPAD_ROW_3_PIN_NO};
static uint8_t au8KeypadCols[KEYPAD_COLS_NUMBER] = {KEYPAD_COL_0_PIN_NO, KEYPAD_COL_1_PIN_NO, KEYPAD_COL_2_PIN_NO, KEYPAD_COL_3_PIN_NO};
	
static enuKeypadType_t enuKeypadTypeState;

/* Mapping row and column of the pressed key to its corresponding character */
static uint8_t Keypad_CharMapping(uint8_t u8ColIndex, uint8_t u8RowIndex);


/*************************************************************************************************
* Parameters	: enuKeypadType
* Return Value	: enuErrorStatus_t
* Description  : Initialize the Keypad according to the configuration
*************************************************************************************************/
enuErrorStatus_t Keypad_Init(enuKeypadType_t enuKeypadType)
{
	enuErrorStatus_t enuRetVar;
	uint8_t u8LoopIndex;
	
	if((enuKeypadType > KEYPAD_TYPE_MIN_VALUE) && (enuKeypadType < KEYPAD_TYPE_MAX_VALUE))
	{
		for(u8LoopIndex = 0; (u8LoopIndex < (KEYPAD_COLS_NUMBER - enuKeypadType)); u8LoopIndex++)
		{
			/* Initializing all col pins as output */
			DIO_PinInit(au8KeypadCols[u8LoopIndex],OUTPUT, NO_CONNECTION);
         /* Initializing all col pins to start detecting a LOW pulse from row pins */
			DIO_PinWrite(au8KeypadCols[u8LoopIndex], HIGH);
		}
		for(u8LoopIndex = 0; u8LoopIndex < KEYPAD_ROWS_NUMBER; u8LoopIndex++)
		{
			/* Initializing all row pins as input */
			DIO_PinInit(au8KeypadRows[u8LoopIndex],  INPUT, PULL_UP_ENABLE);
			
		}
		enuKeypadTypeState = enuKeypadType;
		enuKeypadState = KEYPAD_INIT_DONE;
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;	
}

/*************************************************************************************************
* Parameters	: pu8Data
* Return Value	: enuErrorStatus_t
* Description  : Get the current character of the pressed key from the Keypad
*************************************************************************************************/
enuErrorStatus_t Keypad_GetChar(uint8_t* au8ButtonBuffer,uint8_t u8BufferSize)
{
   
	enuErrorStatus_t enuRetVar=E_ERROR;
	enuDigitalStates_t enuDigitalState;
	uint8_t u8RowIndex = 0;
	uint8_t u8ColIndex = 0;
   uint8_t ButtonCount=0;

	if(au8ButtonBuffer != NULLPTR && enuKeypadState == KEYPAD_INIT_DONE)
	{
      for (ButtonCount=0;ButtonCount<u8BufferSize;ButtonCount++)
      {
         au8ButtonBuffer[ButtonCount]=0;
      }
      ButtonCount=0;
		/* Looping until a key is pressed */
      for(u8ColIndex = 0;u8ColIndex < (KEYPAD_COLS_NUMBER - enuKeypadTypeState); u8ColIndex++)
		{
			/* Setting the value of one row */
			DIO_PinWrite(au8KeypadCols[u8ColIndex], LOW);
			/* Looping on each column to read their values */
			for(u8RowIndex = 0;(u8RowIndex < KEYPAD_ROWS_NUMBER); u8RowIndex++)
			{
				/* Reading the value of the current column */
				DIO_PinRead(au8KeypadRows[u8RowIndex], &enuDigitalState);
				/* Checking if a column is attached to a row / key is pressed to raise a flag and break the loop */
				if(enuDigitalState == LOW)
				{
					ButtonCount++;
               if (ButtonCount<u8BufferSize)
               {
                  /* Mapping the pressed key to get its corresponding value */
                  au8ButtonBuffer[ButtonCount] = Keypad_CharMapping(u8ColIndex, u8RowIndex);
                  
                  enuRetVar = E_OK;
               }
               else
               {
                  enuRetVar = E_ERROR;
               }
				}
				else
				{
				}					
			}
			/* Clearing the value of the already set row */
			DIO_PinWrite(au8KeypadCols[u8ColIndex], HIGH);
		}	
	}
	else
	{
		 
	}
   if (enuRetVar == E_OK)
   {
      au8ButtonBuffer[0]=ButtonCount;
      
   }
	return enuRetVar;
}


/*************************************************************************************************
* Parameters	: u8ColIndex, u8RowIndex
* Return Value	: uint8_t
* Description  : Calculate the value the corresponds the current row and column indexes
*************************************************************************************************/
static uint8_t Keypad_CharMapping(uint8_t u8ColIndex, uint8_t u8RowIndex)
{
    uint8_t u8OutChar;
	 uint8_t u8Button=KEYPAD_CALCULATE_CORRESPONDING_KEY_VALUE(u8ColIndex, u8RowIndex);
    /* Calculating the value the corresponds the current row and column indexes         *
     * e.g: If 8 is pressed (Row = 2, Col = 1) -> Char = (1 + 1) + (3 * 2) = 2 + 6 = 8  */
    switch(u8Button)
    {
        case 1:     u8OutChar = '1';
                    break;
        case 2:     u8OutChar = '2';
                    break;
        case 3:     u8OutChar = '3';
                    break;
        case 4:     u8OutChar = '4';
                    break;
        case 5:     u8OutChar = '5';
                    break;
        case 6:     u8OutChar = '6';
                    break;
        case 7:     u8OutChar = '7';
                    break;
        case 8:     u8OutChar = '8';
                    break;
        case 9:     u8OutChar = '9';
                    break;
        case 10:    u8OutChar = 'A';
                    break;
        case 11:    u8OutChar = 'B';
                    break;
        case 12:    u8OutChar = 'C';
                    break;
        case 13:    u8OutChar = 'D';
                    break;
        default:    u8OutChar = ' ';
    }
    return u8OutChar;
}
