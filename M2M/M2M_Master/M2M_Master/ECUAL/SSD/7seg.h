/*
 * 7seg.h
 *
 *  Created on: Sep 6, 2021
 *      Author: DELLL
 */

#ifndef SEG_H_
#define SEG_H_

/* Includes */
#include "DIO.h"

/* MACROS definitions */
/* choosing the 7-segment mode : COMMON_CATHODE or COMMON_ANODE */
#define SSEG_MODE COMMON_CATHODE
/* Enum for the DIO pins of the 7-segment from the DIO configuration parameters array */
/* A ---> PB0 , DIO group 0
   B ---> PB1 , DIO group 1
   C ---> PB2 , DIO group 2
   D ---> PB3 , DIO group 3
   SEG.EN1 ---> PD2 , DIO group 4
   SEG.EN2 ---> PD3 , DIO group 5
*/

#define SEG_A           DIO_PB0
#define SEG_B           DIO_PB1
#define SEG_C           DIO_PB2
#define SEG_D           DIO_PB3
#define SEG_EN1         DIO_PD2
#define SEG_EN2         DIO_PD3

/*Enum for the numbers displayed on the 7-segment*/
typedef enum{SEG_0,SEG_1,SEG_2,SEG_3,SEG_4,SEG_5,SEG_6,SEG_7,SEG_8,SEG_9}enuSSEGDisplay_t;

/* Functions declarations */
/*************************************************************************************************
* Parameters	: None
* Return Value	: enuErrorStatus_t
* Description  : Initialize the 7-segment
*************************************************************************************************/
enuErrorStatus_t SevenSegment_Init(void);

/*************************************************************************************************
* Parameters	: uint8_t num
* Return Value	: enuErrorStatus_t
* Description  : Displaying a number on the 7-segment
*************************************************************************************************/
enuErrorStatus_t SevenSegment_WriteNum(uint8_t num);

#endif /* 7SEG_H_ */
