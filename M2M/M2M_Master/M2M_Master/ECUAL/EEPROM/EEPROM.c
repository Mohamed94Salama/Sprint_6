/*****************************************************************************
* Task: EEPROM Task
* File Name: EEPROM.c
* Description: functions for EEPROM driver
* Author: Amr Mohamed
* Date: 25/7/2021
******************************************************************************/

/*- INCLUDES --------------------------------------------------*/
#include "EEPROM.h"

/*- MACROS --------------------------------------------------*/
#define EEPROM_DEVICE_BASE_ADDRESS        0b01010000


/*- APIs IMPLEMENTATION-----------------------------------*/

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 0=ERROR or 1=SUCCESS
* Description: A function to initialize EEPROM device
************************************************************************************/
enuErrorStatus_t EEPROM_Init      (void)
{
   //initialize I2C Bus
   I2C_Init();
   return E_OK;
}

/************************************************************************************
* Parameters (in): uint16_t u16ByteAddress , uint8_t u8Data
* Parameters (out): enuErrorStatus_t
* Return value: 0=ERROR or 1=SUCCESS
* Description: A function to write a byte in the EEPROM in a specific address
************************************************************************************/
enuErrorStatus_t EEPROM_WriteByte (uint16_t u16ByteAddress,uint8_t u8Data)
{
   //transform the byte address into a page number and a byte address within the page
   uint8_t u8ByteAddress_Page=(uint8_t)u16ByteAddress;
   uint8_t u8PageAddress=(u16ByteAddress>>8)& 0b00000111;
   //set up the EEPROM page address frame
   uint8_t u8EEPROM_Address= EEPROM_DEVICE_BASE_ADDRESS | u8PageAddress;
   
   //send a start condition
   I2C_Start();
   //send the EEPROM page address frame
   I2C_SendAddressFrame(u8EEPROM_Address,Write);
   //send the byte address frame
   I2C_SendDataFrame(u8ByteAddress_Page);
   //send the data
   I2C_SendDataFrame(u8Data);
   //send a stop bit
   I2C_Stop();
   return E_OK;
}


/************************************************************************************
* Parameters (in): uint16_t u16ByteAddress , uint8_t *pu8Data
* Parameters (out): enuErrorStatus_t
* Return value: 0=ERROR or 1=SUCCESS
* Description: A function to read a byte from the EEPROM in a specific address
************************************************************************************/
enuErrorStatus_t EEPROM_ReadByte  (uint16_t u16ByteAddress,uint8_t *pu8Data)
{
   //transform the byte address into a page number and a byte address within the page
   uint8_t u8ByteAddress_Page=(uint8_t)u16ByteAddress;
   uint8_t u8PageAddress=(u16ByteAddress>>8)& 0b00000111;
   //set up the EEPROM page address frame
   uint8_t u8EEPROM_Address= EEPROM_DEVICE_BASE_ADDRESS | u8PageAddress;
   
   //send a start condition
   I2C_Start();
   //send the EEPROM page address frame
   I2C_SendAddressFrame(u8EEPROM_Address,Write);
   //send the byte address frame
   I2C_SendDataFrame(u8ByteAddress_Page);
   //send a repeated start condition
   I2C_Repeated_Start();
   //send the EEPROM page address frame
   I2C_SendAddressFrame(u8EEPROM_Address,Read);
   //receive the data from the EEPROM and send a NACK condition
   I2C_RecieveDataFrame(pu8Data,NACK);
   //send a stop bit
   I2C_Stop();
   
   
   return E_OK;
}


/************************************************************************************
* Parameters (in): uint16_t u16ByteAddress , uint8_t u8Data
* Parameters (out): enuErrorStatus_t
* Return value: 0=ERROR or 1=SUCCESS
* Description: A function to write a string in the EEPROM in a specific address
************************************************************************************/
enuErrorStatus_t EEPROM_WriteString (uint16_t u16ByteAddress,uint16_t u16ByteCount,uint8_t *pu8Data)
{
	if (pu8Data==NULLPTR)		return E_ERROR;
	//transform the byte address into a page number and a byte address within the page
	uint8_t u8ByteAddress_Page=(uint8_t)u16ByteAddress;
	uint8_t u8PageAddress=(u16ByteAddress>>8)& 0b00000111;
	//set up the EEPROM page address frame
	uint8_t u8EEPROM_Address= EEPROM_DEVICE_BASE_ADDRESS | u8PageAddress;
	
	if (u16ByteAddress+u16ByteCount>2047)
	{
		return E_ERROR;
	}
	//send a start condition
	I2C_Start();
	//send the EEPROM page address frame
	I2C_SendAddressFrame(u8EEPROM_Address,Write);
	//send the byte address frame
	I2C_SendDataFrame(u8ByteAddress_Page);
	
	for (uint16_t i=0;i<u16ByteCount;i++)
	{
		//send the data
		I2C_SendDataFrame(pu8Data[i]);
	}
	//send a stop bit
	I2C_Stop();
	return E_OK;
}

/************************************************************************************
* Parameters (in): uint16_t u16ByteAddress , uint8_t *pu8Data
* Parameters (out): enuErrorStatus_t
* Return value: 0=ERROR or 1=SUCCESS
* Description: A function to read a string from the EEPROM in a specific address
************************************************************************************/
enuErrorStatus_t EEPROM_ReadString  (uint16_t u16ByteAddress,uint16_t u16ByteCount,uint8_t *pu8Data)
{
	if (pu8Data==NULLPTR)		return E_ERROR;
	//transform the byte address into a page number and a byte address within the page
	uint8_t u8ByteAddress_Page=(uint8_t)u16ByteAddress;
	uint8_t u8PageAddress=(u16ByteAddress>>8)& 0b00000111;
	//set up the EEPROM page address frame
	uint8_t u8EEPROM_Address= EEPROM_DEVICE_BASE_ADDRESS | u8PageAddress;
	
	if (u16ByteAddress+u16ByteCount>2047)
	{
		return E_ERROR;
	}
	
	//send a start condition
	I2C_Start();
	//send the EEPROM page address frame
	I2C_SendAddressFrame(u8EEPROM_Address,Write);
	//send the byte address frame
	I2C_SendDataFrame(u8ByteAddress_Page);
	//send a repeated start condition
	I2C_Repeated_Start();
	//send the EEPROM page address frame
	I2C_SendAddressFrame(u8EEPROM_Address,Read);
	uint8_t i;
	for (i=0;i<u16ByteCount-1;i++)
	{
		//receive the data from the EEPROM and send a ACK condition
		I2C_RecieveDataFrame(&pu8Data[i],ACK);
	}	
	//receive the data from the EEPROM and send a NACK condition
	I2C_RecieveDataFrame(&pu8Data[i],NACK);
	//send a stop bit
	I2C_Stop();
	return E_OK;

}
