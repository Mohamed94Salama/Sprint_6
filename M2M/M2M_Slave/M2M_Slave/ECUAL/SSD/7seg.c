/*
 * 7seg.c
 *
 *  Created on: Sep 6, 2021
 *      Author: DELLL
 */

/* Includes */
#include "7seg.h"

/*Include OS header file*/
#include "FreeRTOS.h"
#include "task.h"

/* A flag to check if the 7-segment is initialized or not */
static uint8_t gu8flag=LOW;

/*************************************************************************************************
* Parameters	: uint8_t num
* Return Value	: enuErrorStatus_t
* Description  : Decoding the number to be displayed on the 7-segment 
*************************************************************************************************/
static enuErrorStatus_t SevenSegment_Decode(uint8_t num);

/* The delay required for the enable pin */
#define SSEG_DELAY	10

/* Maximum number of characters of the number to be displayed on the 7-segment */
#define ARR_SIZE	2

/*************************************************************************************************
* Parameters	: None
* Return Value	: enuErrorStatus_t
* Description  : Initialize the 7-segment
*************************************************************************************************/
enuErrorStatus_t SevenSegment_Init(void)
{
	/* Initializing the DIO pins*/
	DIO_Init(&astrDIOConfigParameters[SEG_A]);
	DIO_Init(&astrDIOConfigParameters[SEG_B]);
	DIO_Init(&astrDIOConfigParameters[SEG_C]);
	DIO_Init(&astrDIOConfigParameters[SEG_D]);
	DIO_Init(&astrDIOConfigParameters[SEG_EN1]);
	DIO_Init(&astrDIOConfigParameters[SEG_EN2]);
	
	/* Set the flag indicating that the 7-segment is initialized */
	gu8flag = HIGH;
	
	return E_OK;
}

/*************************************************************************************************
* Parameters	: uint8_t num
* Return Value	: enuErrorStatus_t
* Description  : Displaying a number on the 7-segment
*************************************************************************************************/
enuErrorStatus_t SevenSegment_WriteNum(uint8_t num)
{
   static uint8_t Seg_Flag=LOW;
	/* Returning an error if the 7-segment is not initialized */
	if(gu8flag==LOW)
	{
		return E_ERROR;
	}
	
	/* Initializing the array */
	uint8_t arr_num[ARR_SIZE]={0,0};
	/* Counter for the index of the array */
	uint8_t counter = 0;
	/* A temporary variable holding the number to be displayed */
	uint8_t temp = num;

	/* Splitting the number into 2 digits */
	while(temp != 0)
	{
		/* Getting the digit */
		arr_num[counter]=temp%10;
		/* Updating the index */
		counter++;
		/* Moving to the next digit */
		temp /= 10;
	}
	/* Writing the most significant digit */
   
   Seg_Flag^=HIGH;
   
   if (Seg_Flag)
   {
      /* EN 2 */
      /* Latching the EN pin */
      #if SSEG_MODE == COMMON_CATHODE
      DIO_Write(SEG_EN2,HIGH);
      #elif SSEG_MODE == COMMON_ANODE
      DIO_Write(SEG_EN2,LOW);
      #endif
      
      /* Displaying the digit on the 7-segment */
      SevenSegment_Decode(arr_num[1]);
      //vTaskDelay(SSEG_DELAY);
      
      #if SSEG_MODE == COMMON_CATHODE
      DIO_Write(SEG_EN2,LOW);
      #elif SSEG_MODE == COMMON_ANODE
      DIO_Write(SEG_EN2,HIGH);
      #endif
   }
   else
   {
      /* Writing the least significant digit */
      /*EN 1*/
      
      /* Latching the EN pin */
      #if SSEG_MODE == COMMON_CATHODE
      DIO_Write(SEG_EN1,HIGH);
      #elif SSEG_MODE == COMMON_ANODE
      DIO_Write(SEG_EN1,LOW);
      #endif
      
      /* Displaying the digit on the 7-segment */
      SevenSegment_Decode(arr_num[0]);
      //vTaskDelay(SSEG_DELAY);
      
      #if SSEG_MODE == COMMON_CATHODE
      DIO_Write(SEG_EN1,LOW);
      #elif SSEG_MODE == COMMON_ANODE
      DIO_Write(SEG_EN1,HIGH);
      #endif
   }
	return E_OK;
}

/*************************************************************************************************
* Parameters	: uint8_t num
* Return Value	: enuErrorStatus_t
* Description  : Decoding the number to be displayed on the 7-segment
*************************************************************************************************/
static enuErrorStatus_t SevenSegment_Decode(uint8_t num)
{
	/* A B C D = X X X X */
	switch(num)
	{
		/* Displaying number 0 */
		case SEG_0:
			
			#if SSEG_MODE == COMMON_CATHODE
			DIO_Write(SEG_A, LOW);
			DIO_Write(SEG_B, LOW);
			DIO_Write(SEG_C, LOW);
			DIO_Write(SEG_D, LOW);
			#elif SSEG_MODE == COMMON_ANODE
			DIO_Write(SEG_A, HIGH);
			DIO_Write(SEG_B, HIGH);
			DIO_Write(SEG_C, HIGH);
			DIO_Write(SEG_D, HIGH);
			#endif			
			break;
		
		/* Displaying number 1 */
		case SEG_1:
			
			#if SSEG_MODE == COMMON_CATHODE
			DIO_Write(SEG_A, HIGH);
			DIO_Write(SEG_B, LOW);
			DIO_Write(SEG_C, LOW);
			DIO_Write(SEG_D, LOW);
			#elif SSEG_MODE == COMMON_ANODE
			DIO_Write(SEG_A, LOW);
			DIO_Write(SEG_B, HIGH);
			DIO_Write(SEG_C, HIGH);
			DIO_Write(SEG_D, HIGH);
			#endif			
			break;
		
		/* Displaying number 2 */	
		case SEG_2:
			
			#if SSEG_MODE == COMMON_CATHODE
			DIO_Write(SEG_A, LOW);
			DIO_Write(SEG_B, HIGH);
			DIO_Write(SEG_C, LOW);
			DIO_Write(SEG_D, LOW);
			#elif SSEG_MODE == COMMON_ANODE
			DIO_Write(SEG_A, HIGH);
			DIO_Write(SEG_B, LOW);
			DIO_Write(SEG_C, HIGH);
			DIO_Write(SEG_D, HIGH);
			#endif			
			break;
			
		/* Displaying number 3 */
		case SEG_3:
			
			#if SSEG_MODE == COMMON_CATHODE
			DIO_Write(SEG_A, HIGH);
			DIO_Write(SEG_B, HIGH);
			DIO_Write(SEG_C, LOW);
			DIO_Write(SEG_D, LOW);
			#elif SSEG_MODE == COMMON_ANODE
			DIO_Write(SEG_A, LOW);
			DIO_Write(SEG_B, LOW);
			DIO_Write(SEG_C, HIGH);
			DIO_Write(SEG_D, HIGH);
			#endif			
			break;
		
		/* Displaying number 4 */
		case SEG_4:
		
			#if SSEG_MODE == COMMON_CATHODE
			DIO_Write(SEG_A, LOW);
			DIO_Write(SEG_B, LOW);
			DIO_Write(SEG_C, HIGH);
			DIO_Write(SEG_D, LOW);
			#elif SSEG_MODE == COMMON_ANODE
			DIO_Write(SEG_A, HIGH);
			DIO_Write(SEG_B, HIGH);
			DIO_Write(SEG_C, LOW);
			DIO_Write(SEG_D, HIGH);
			#endif			
			break;
			
		/* Displaying number 5 */	
		case SEG_5:
			
			#if SSEG_MODE == COMMON_CATHODE
			DIO_Write(SEG_A, HIGH);
			DIO_Write(SEG_B, LOW);
			DIO_Write(SEG_C, HIGH);
			DIO_Write(SEG_D, LOW);
			#elif SSEG_MODE == COMMON_ANODE
			DIO_Write(SEG_A, LOW);
			DIO_Write(SEG_B, HIGH);
			DIO_Write(SEG_C, LOW);
			DIO_Write(SEG_D, HIGH);
			#endif			
			break;
			
		/* Displaying number 6 */	
		case SEG_6:
			
			#if SSEG_MODE == COMMON_CATHODE
			DIO_Write(SEG_A, LOW);
			DIO_Write(SEG_B, HIGH);
			DIO_Write(SEG_C, HIGH);
			DIO_Write(SEG_D, LOW);
			#elif SSEG_MODE == COMMON_ANODE
			DIO_Write(SEG_A, HIGH);
			DIO_Write(SEG_B, LOW);
			DIO_Write(SEG_C, LOW);
			DIO_Write(SEG_D, HIGH);
			#endif			
			break;
			
		/* Displaying number 7 */	
		case SEG_7:
			
			#if SSEG_MODE == COMMON_CATHODE
			DIO_Write(SEG_A, HIGH);
			DIO_Write(SEG_B, HIGH);
			DIO_Write(SEG_C, HIGH);
			DIO_Write(SEG_D, LOW);
			#elif SSEG_MODE == COMMON_ANODE
			DIO_Write(SEG_A, LOW);
			DIO_Write(SEG_B, LOW);
			DIO_Write(SEG_C, LOW);
			DIO_Write(SEG_D, HIGH);
			#endif			
			break;
			
		/* Displaying number 8 */	
		case SEG_8:
			
			#if SSEG_MODE == COMMON_CATHODE
			DIO_Write(SEG_A, LOW);
			DIO_Write(SEG_B, LOW);
			DIO_Write(SEG_C, LOW);
			DIO_Write(SEG_D, HIGH);
			#elif SSEG_MODE == COMMON_ANODE
			DIO_Write(SEG_A, HIGH);
			DIO_Write(SEG_B, HIGH);
			DIO_Write(SEG_C, HIGH);
			DIO_Write(SEG_D, LOW);
			#endif			
			break;
			
		/* Displaying number 9 */	
		case SEG_9:

			#if SSEG_MODE == COMMON_CATHODE
			DIO_Write(SEG_A, HIGH);
			DIO_Write(SEG_B, LOW);
			DIO_Write(SEG_C, LOW);
			DIO_Write(SEG_D, HIGH);
			#elif SSEG_MODE == COMMON_ANODE
			DIO_Write(SEG_A, LOW);
			DIO_Write(SEG_B, HIGH);
			DIO_Write(SEG_C, HIGH);
			DIO_Write(SEG_D, LOW);
			#endif			
			break;
	}
	return E_OK;
}

