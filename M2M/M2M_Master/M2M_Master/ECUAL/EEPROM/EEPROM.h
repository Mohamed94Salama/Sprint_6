/*****************************************************************************
* Task: EEPROM Task
* File Name: EEPROM.h
* Description: header file for EEPROM driver
* Author: Amr Mohamed
* Date: 25/7/2021
******************************************************************************/


#ifndef EEPROM_H_
#define EEPROM_H_

/*- INCLUDES --------------------------------------------------*/
#include "MCAL/I2C/I2C.h"
#include "STD_Types.h"
#include "BIT_MANIPULATE.h"

/*- APIs PROTOTYPES-----------------------------------*/

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 0=ERROR or 1=SUCCESS
* Description: A function to initialize EEPROM device
************************************************************************************/
enuErrorStatus_t EEPROM_Init      (void);

/************************************************************************************
* Parameters (in): uint16_t u16ByteAddress , uint8_t u8Data
* Parameters (out): enuErrorStatus_t
* Return value: 0=ERROR or 1=SUCCESS
* Description: A function to write a byte in the EEPROM in a specific address
************************************************************************************/
enuErrorStatus_t EEPROM_WriteByte (uint16_t u16ByteAddress,uint8_t u8Data);

/************************************************************************************
* Parameters (in): uint16_t u16ByteAddress , uint8_t *pu8Data
* Parameters (out): enuErrorStatus_t
* Return value: 0=ERROR or 1=SUCCESS
* Description: A function to read a byte from the EEPROM in a specific address
************************************************************************************/
enuErrorStatus_t EEPROM_ReadByte  (uint16_t u16ByteAddress,uint8_t *pu8Data);

/************************************************************************************
* Parameters (in): uint16_t u16ByteAddress , uint8_t u8Data
* Parameters (out): enuErrorStatus_t
* Return value: 0=ERROR or 1=SUCCESS
* Description: A function to write a string in the EEPROM in a specific address
************************************************************************************/
enuErrorStatus_t EEPROM_WriteString (uint16_t u16ByteAddress,uint16_t u16ByteCount,uint8_t *pu8Data);

/************************************************************************************
* Parameters (in): uint16_t u16ByteAddress , uint8_t *pu8Data
* Parameters (out): enuErrorStatus_t
* Return value: 0=ERROR or 1=SUCCESS
* Description: A function to read a string from the EEPROM in a specific address
************************************************************************************/
enuErrorStatus_t EEPROM_ReadString  (uint16_t u16ByteAddress,uint16_t u16ByteCount,uint8_t *pu8Data);

#endif /* EEPROM_H_ */
