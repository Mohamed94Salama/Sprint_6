/*****************************************************************************
* Task		 : UART Driver
* File Name	 : UART.h
* Description: header file for UART driver
* Author	 : Mohamed Ehab
* Date		 : Jul 13, 2021
******************************************************************************/

#ifndef MCAL_UART_UART_H_
#define MCAL_UART_UART_H_

#define MAX_LEN	255

/*- INCLUDES -----------------------------------------------*/
#include "Register.h"
#include "STD_TYPES.h"
#include "BIT_MANIPULATE.h"
#include "UART_Cfg.h"

//#define       F_CPU    8000000UL

// define some macros
//#define BAUD 9600                                   // define baud
//#define BAUDRATE (((F_CPU)/(16UL*BAUD))-1)            // set baud rate value for UBRR

/*- FUNCTION PROTOTYPES ----------------------------------*/

enuErrorStatus_t UART_ReceiveOK(uint8_t* pu8String,uint8_t max_length);
/*************************************************************************************************
* Parameters (in) : None
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to initialize the UART protocol
*************************************************************************************************/
enuErrorStatus_t UART_Init(uint32_t Baudrate);

/*************************************************************************************************
* Parameters (in) : u8_Data
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to send data via UART
*************************************************************************************************/
enuErrorStatus_t UART_SendData(uint16_t u16Data);

/*************************************************************************************************
* Parameters (in) : None
* Parameters (out): pu8_Data
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to receive data via UART
*************************************************************************************************/
enuErrorStatus_t UART_ReceiveData(uint16_t* pu16Data);

/*************************************************************************************************
* Parameters (in) : None
* Parameters (out): pu8_Data
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to receive data via UART
*************************************************************************************************/
enuErrorStatus_t UART_RecieveDataNoBLOCK(uint16_t* pu16Data);

/*************************************************************************************************
* Parameters (in) : u8_String
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to send string via UART
*************************************************************************************************/
enuErrorStatus_t UART_SendString(uint8_t* pu8String);

/*************************************************************************************************
* Parameters (in) : None
* Parameters (out): u8_String
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to receive string via UART
*************************************************************************************************/
enuErrorStatus_t UART_ReceiveString(uint8_t* pu8String,uint8_t max_length);

/*************************************************************************************************
* Parameters (in) : u8_Data
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to send data via UART without blocking the system
*************************************************************************************************/
enuErrorStatus_t UART_SendDataNoBLOCK(uint16_t u16Data);

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to enable uart interrupt for TX complete
************************************************************************************/
enuErrorStatus_t UART_TX_Enable_Interrupt(void);

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to disable uart interrupt for TX complete
************************************************************************************/
enuErrorStatus_t UART_TX_Disable_Interrupt(void);

/************************************************************************************
* Parameters (in): void(*local_fptr)(void)
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to setup callback function for TX complete interrupt
************************************************************************************/
enuErrorStatus_t UART_TX_SetCallBack(void(*local_fptr)(void));

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to enable uart interrupt for RX complete
************************************************************************************/
enuErrorStatus_t UART_RX_Enable_Interrupt(void);

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to disable uart interrupt for RX complete
************************************************************************************/
enuErrorStatus_t UART_RX_Disable_Interrupt(void);

/************************************************************************************
* Parameters (in): void(*local_fptr)(void)
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to setup callback function for RX complete interrupt
************************************************************************************/
enuErrorStatus_t UART_RX_SetCallBack(void(*local_fptr)(void));
#endif /* MCAL_UART_UART_H_ */
