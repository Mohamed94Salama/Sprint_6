/*****************************************************************************
* Task: I2C Driver
* File Name: I2C.h
* Description: header file for I2C driver
* Author: Amr Mohamed
* Date: 24/7/2021
******************************************************************************/


#ifndef __I2C__
#define __I2C__
/*- INCLUDES --------------------------------------------------*/
#include "STD_Types.h"
#include "BIT_MANIPULATE.h"
#include "Register.h"
#include "I2C_Cfg.h"
/*- ENUMS --------------------------------------------------*/
//acknoledgment enum
typedef enum{
   ACK,
   NACK
}enuI2CAck_t;
//data direction enum
typedef enum{Write, Read}enuI2CMode_t;

/*- APIs PROTOTYPES-----------------------------------*/

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to initialize the I2C peripheral
************************************************************************************/
enuErrorStatus_t I2C_Init(void);

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to send a start bit to the I2C bus
************************************************************************************/
enuErrorStatus_t I2C_Start(void);

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to send a repeated start bit to the I2C bus
************************************************************************************/
enuErrorStatus_t I2C_Repeated_Start(void);

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to send a stop bit to the I2C bus
************************************************************************************/
enuErrorStatus_t I2C_Stop(void);


/************************************************************************************
* Parameters (in): uint8_t u8SlaveAddress, enuI2CMode_t enuMode
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to send the address portion of the I2C frame to the I2C bus
************************************************************************************/
enuErrorStatus_t I2C_SendAddressFrame(uint8_t u8SlaveAddress, enuI2CMode_t enuMode);

/************************************************************************************
* Parameters (in): uint8_t u8Data
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to send the data portion of the I2C frame to the I2C bus
************************************************************************************/
enuErrorStatus_t I2C_SendDataFrame(uint8_t u8Data);

/************************************************************************************
* Parameters (in): uint8_t * pu8Data, enuI2CAck_t enuACK
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to receive the data portion of the I2C frame from the I2C bus
************************************************************************************/
enuErrorStatus_t I2C_RecieveDataFrame(uint8_t * pu8Data, enuI2CAck_t enuACK);


/************************************** SERVICE FUNCTIONS **********************************/

/************************************************************************************
* Parameters (in): uint8_t u8SlaveAddress,uint8_t u8Data
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to send a byte to the I2C bus as a master
************************************************************************************/
enuErrorStatus_t I2C_MASTER_SendData(uint8_t u8SlaveAddress,uint8_t u8Data);

/************************************************************************************
* Parameters (in): uint8_t u8SlaveAddress,uint8_t  *pu8Data
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to receive a byte from the I2C bus as a master
************************************************************************************/
enuErrorStatus_t I2C_MASTER_RecieveData(uint8_t u8SlaveAddress,uint8_t  *pu8Data);

/************************************************************************************
* Parameters (in): uint8_t u8Data
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to send a byte from the I2C bus as a slave
************************************************************************************/
enuErrorStatus_t I2C_SLAVE_SendData(uint8_t u8Data);

/************************************************************************************
* Parameters (in): uint8_t *pu8Data
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to receive a byte from the I2C bus as a slave
************************************************************************************/
enuErrorStatus_t I2C_SLAVE_ReceiveData(uint8_t *pu8Data);


/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to enable the I2C bus event interrupt
************************************************************************************/
enuErrorStatus_t I2C_Enable_Interrupt(void);

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to disable the I2C bus event interrupt
************************************************************************************/
enuErrorStatus_t I2C_Disable_Interrupt(void);

/************************************************************************************
* Parameters (in): void(*local_fptr)(void)
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to set the callback function of the I2C bus event interrupt
************************************************************************************/
enuErrorStatus_t I2C_SetCallBack(void(*local_fptr)(void));



#endif /* __I2C__ */
