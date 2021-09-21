/*****************************************************************************
* Task: I2C Driver
* File Name: I2C.c
* Description: Functions for I2C driver
* Author: Amr Mohamed
* Date: 24/7/2021
******************************************************************************/

/*- INCLUDES --------------------------------------------------*/
#include "I2C.h"


/*- MACROS --------------------------------------------------*/
/*I2C Consts */

#define I2C_MAX_ADDRESS       119

/* STATUS CODES FOR I2C BUS */
/* MASTER CODES */
#define START_TRANSMITTED                                         0x08
#define RESTART_TRANSMITTED                                       0x10
#define ARBITRATION_LOST                                          0x38

/* MASTER TRANSMITTER MODE */

#define SLA_W_TRANSMITTED_ACK                                     0x18
#define SLA_W_TRANSMITTED_NACK                                    0x20
#define MASTER_DATA_TRANSMITTED_ACK                               0x28
#define MASTER_DATA_TRANSMITTED_NACK                              0x30


/* MASTER RECEIVER MODE */
#define SLA_R_TRANSMITTED_ACK                                     0x40
#define SLA_R_TRANSMITTED_NACK                                    0x48
#define DATA_RECEIVED_ACK                                         0x50
#define DATA_RECEIVED_NACK                                        0x58


/* SLAVE MODE */
/* SLAVE RECEIVER MODE */
#define OWN_SLA_W_RECEIVED_ACK                                    0x60
#define M_ARBITRATION_LOST_OWN_SLA_W_RECEIVED_ACK                 0x68
#define GENERAL_CALL_ADDRESS_RECEIVED_ACK                         0x70
#define M_ARBITRATION_LOST_GENERAL_CALL_ADDRESS_RECEIVED_ACK      0x78
#define OWN_SLA_W_DATA_RECEIVED_ACK                               0x80
#define OWN_SLA_W_DATA_RECEIVED_NACK                              0x88
#define GENERAL_CALL_DATA_RECEIVED_ACK                            0x80
#define GENERAL_CALL_DATA_RECEIVED_NACK                           0x88
#define RECEIVED_STOP_OR_RESTART_AS_SLAVE                         0xA0

/* SLAVE TRASMITTER MODE */
#define OWN_SLA_R_RECEIVED_ACK                                    0xA8
#define M_ARBITRATION_LOST_OWN_SLA_R_RECEIVED_ACK                 0xB0
#define SLAVE_DATA_TRANSMITTED_ACK                                0xB8
#define SLAVE_DATA_TRANSMITTED_NACK                               0xC0
#define SLAVE_LAST_DATA_BYTE_TRANSMITTED_ACK                      0xC8


/* MISC CODES */
#define NO_INFO_AVAILABLE                                         0xF8
#define BUS_E_ERROR                                                 0x00


/*- GLOBAL VARIABLES --------------------------------------------------*/
void (*I2C_Callback)(void)=NULLPTR;



/*- APIs IMPLEMENTATION-----------------------------------*/

/************************************** MISC FUNCTIONS **********************************/

/************************************************************************************
* Parameters (in): void
* Parameters (out): uint8_t
* Return value: I2C status code
* Description: A function to read the I2C bus status from the TWSR register
************************************************************************************/
static uint8_t I2C_GetStatus(void)
{
   return (TWSR_R & 0xF8);
}


/************************************** CORE FUNCTIONS **********************************/

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to initialize the I2C peripheral
************************************************************************************/
enuErrorStatus_t I2C_Init(void)
{
   /* set up master clock */
   TWSR_R &= 0b11111100;
   TWSR_R |= I2C_PRESCALER;
   uint16_t power=4;
   for (uint8_t i=0;i<I2C_PRESCALER;i++)
   {
      power*=power;
   }
   TWBR_R=(((F_CPU/I2C_CLK)-16)/(2*power));
   /* set up slave address */
   TWAR_R= (I2C_SLAVE_ADDRESS<<1) | I2C_SLAVE_GLOBAL_EN;
   /* Enable acknowledgment */
   if(I2C_SLAVE_ACK == ENABLE)      SET_BIT(TWCR_R,TWEA_B);
   /* write 1 to TWINT bit */
   SET_BIT(TWCR_R,TWINT_B);
   /*Enable I2C Communication*/
   SET_BIT(TWCR_R,TWEN_B);
   return E_OK;
}

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to send a start bit to the I2C bus
************************************************************************************/
enuErrorStatus_t I2C_Start(void)
{
   //Setup the TWCR register for the start condition & enable transmission
   TWCR_R=(1<<TWSTA_B) | (1<<TWEN_B) | (1<<TWINT_B);
   //wait for transmission to complete
   while(!GET_BIT(TWCR_R,TWINT_B));
   //if start bit is sent, return a E_OK
   if(I2C_GetStatus()==START_TRANSMITTED)
   {
      return E_OK;
   }
   return E_ERROR;
}

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to send a repeated start bit to the I2C bus
************************************************************************************/
enuErrorStatus_t I2C_Repeated_Start(void)
{
   //Setup the TWCR register for the repeated start condition & enable transmission
   TWCR_R=(1<<TWSTA_B) | (1<<TWEN_B) | (1<<TWINT_B);
   
   //wait for transmission to complete
   while(!GET_BIT(TWCR_R,TWINT_B));
   //if repeated start bit is sent, return a E_OK
   if(I2C_GetStatus()!=RESTART_TRANSMITTED)
   {
      return E_ERROR;
   }
   return E_OK;
}

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to send a stop bit to the I2C bus
************************************************************************************/
enuErrorStatus_t I2C_Stop(void)
{
   //Setup the TWCR register for the stop condition & enable transmission
   TWCR_R=(1<<TWSTO_B) | (1<<TWEN_B) | (1<<TWINT_B);
   //return a E_OK
   return E_OK;
}

/************************************************************************************
* Parameters (in): uint8_t u8SlaveAddress, enuI2CMode_t enuMode
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to send the address portion of the I2C frame to the I2C bus
************************************************************************************/
enuErrorStatus_t I2C_SendAddressFrame(uint8_t u8SlaveAddress, enuI2CMode_t enuMode)
{
   //if parameters are invalid
   if ((enuMode!= Read && enuMode!= Write) || enuMode > I2C_MAX_ADDRESS)
   {
      //return an E_ERROR
      return E_ERROR;
   }
   //prepare the address frame and send it to the data register
   TWDR_R = (u8SlaveAddress<<1) | enuMode;
   //enable transmission
   TWCR_R= (1<<TWINT_B)|(1<<TWEN_B);
   //wait for transmission to complete
   while(!GET_BIT(TWCR_R,TWINT_B));
   //depending of the selected transmission mode
   if (enuMode == Read)
   {
      //check if bus status is invalid
      if(I2C_GetStatus()!=SLA_R_TRANSMITTED_ACK)
      {
         //stop the communication
         I2C_Stop();
         //return an E_ERROR
         return E_ERROR;
      }
   }
   else if (enuMode == Write)
   {
      //check if bus status is invalid
      if(I2C_GetStatus()!=SLA_W_TRANSMITTED_ACK)
      {
         //stop the communication
         I2C_Stop();
         //return an E_ERROR
         return E_ERROR;
      }
   }
   //return a E_OK
   return E_OK;
}


/************************************************************************************
* Parameters (in): uint8_t u8Data
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to send the data portion of the I2C frame to the I2C bus
************************************************************************************/
enuErrorStatus_t I2C_SendDataFrame(uint8_t u8Data)
{
   //wait for current transmission to complete
   while(!GET_BIT(TWCR_R,TWINT_B));
   //set the sent value to the data register
   TWDR_R=u8Data;
   //enable transmission
   TWCR_R= (1<<TWINT_B)|(1<<TWEN_B);
   //wait for transmission to complete
   while(!GET_BIT(TWCR_R,TWINT_B));
   //check if bus status is invalid
   uint8_t status=I2C_GetStatus();
   if(status !=  MASTER_DATA_TRANSMITTED_ACK &&
   status !=  SLAVE_DATA_TRANSMITTED_ACK  &&
   status !=  SLAVE_LAST_DATA_BYTE_TRANSMITTED_ACK)
   {
      return E_ERROR;
   }
   //return a E_OK
   return E_OK;
}

/************************************************************************************
* Parameters (in): uint8_t * pu8Data, enuI2CAck_t enuACK
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to receive the data portion of the I2C frame from the I2C bus
************************************************************************************/
enuErrorStatus_t I2C_RecieveDataFrame(uint8_t * pu8Data, enuI2CAck_t enuACK)
{
   //check if pointer points to an invalid location
   if (pu8Data == NULLPTR)
   {
      //if so return an E_ERROR
      return E_ERROR;
   }
   //wait for current transmission to complete
   while(!GET_BIT(TWCR_R,TWINT_B));
   //depending on the acknowledgment
   switch(enuACK)
   {
      //set the acknowledgments bit in control register & enable transmission
      case ACK:      TWCR_R= (1<<TWINT_B)|(1<<TWEN_B)|(1<<TWEA_B);   break;
      case NACK:     TWCR_R= (1<<TWINT_B)|(1<<TWEN_B);               break;
      default:       return E_ERROR;                                   break;
   }
   
   //wait for current transmission to complete
   while(!GET_BIT(TWCR_R,TWINT_B));
   
   //check if bus status is invalid
   uint8_t status=I2C_GetStatus();
   switch(enuACK)
   {
      case ACK:
      if(status != DATA_RECEIVED_ACK &&
      status != OWN_SLA_W_DATA_RECEIVED_ACK &&
      status != GENERAL_CALL_DATA_RECEIVED_ACK)
      {
         //return an E_ERROR
         return E_ERROR;
      }
      break;
      
      case NACK:
      if(status != DATA_RECEIVED_NACK &&
      status != OWN_SLA_W_DATA_RECEIVED_NACK &&
      status != GENERAL_CALL_DATA_RECEIVED_NACK)
      {
         //return an E_ERROR
         return E_ERROR;
      }
      break;
   }
   
   //store data in sent pointer
   *pu8Data = TWDR_R;
   //return E_OK
   return E_OK;
}


/************************************** SERVICE FUNCTIONS **********************************/

/************************************************************************************
* Parameters (in): uint8_t u8SlaveAddress,uint8_t u8Data
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to send a byte to the I2C bus as a master
************************************************************************************/
enuErrorStatus_t I2C_MASTER_SendData(uint8_t u8SlaveAddress,uint8_t u8Data)
{
   //start transmission
   I2C_Start();
   //Send the SLA+W frame
   I2C_SendAddressFrame(u8SlaveAddress,Write);
   //send the data frame
   I2C_SendDataFrame(u8Data);
   //check if bus status is invalid
   if(I2C_GetStatus()!=MASTER_DATA_TRANSMITTED_ACK)
   {
      //stop transmission
      I2C_Stop();
      //return an E_ERROR
      return E_ERROR;
   }
   //stop transmission
   I2C_Stop();
   //return a E_OK
   return E_OK;
}


/************************************************************************************
* Parameters (in): uint8_t u8SlaveAddress,uint8_t  *pu8Data
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to receive a byte from the I2C bus as a master
************************************************************************************/
enuErrorStatus_t I2C_MASTER_RecieveData(uint8_t u8SlaveAddress,uint8_t  *pu8Data)
{
   //check if pointer points to an invalid location
   if (pu8Data == NULLPTR)
   {
      //if so return an E_ERROR
      return E_ERROR;
   }
   //start transmission
   I2C_Start();
   //Send the SLA+R frame
   I2C_SendAddressFrame(u8SlaveAddress,Read);
   //send the data frame
   I2C_RecieveDataFrame(pu8Data,NACK);
   //check if bus status is invalid
   if(I2C_GetStatus()!=DATA_RECEIVED_NACK)
   {
      //stop transmission
      I2C_Stop();
      //return an E_ERROR
      return E_ERROR;
   }
   //stop transmission
   I2C_Stop();
   //return a E_OK
   return E_OK;
}

/************************************************************************************
* Parameters (in): uint8_t u8Data
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to send a byte from the I2C bus as a slave
************************************************************************************/
enuErrorStatus_t I2C_SLAVE_SendData(uint8_t u8Data)
{
   //prepare peripheral to send data
   SET_BIT(TWCR_R,TWINT_B);
   //wait for current transmission to finish
   while(!GET_BIT(TWCR_R,TWINT_B));
   //if status is valid
   if(I2C_GetStatus()==OWN_SLA_R_RECEIVED_ACK || I2C_GetStatus()==M_ARBITRATION_LOST_OWN_SLA_R_RECEIVED_ACK)
   {
      //send the data
      I2C_SendDataFrame(u8Data);
      //return a E_OK state
      return E_OK;
   }
   //else return a fail state
   return E_ERROR;
}

/************************************************************************************
* Parameters (in): uint8_t *pu8Data
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to receive a byte from the I2C bus as a slave
************************************************************************************/
enuErrorStatus_t I2C_SLAVE_ReceiveData(uint8_t *pu8Data)
{
   //prepare peripheral to receive data
   SET_BIT(TWCR_R,TWINT_B);
   //wait for current transmission to finish
   while(!GET_BIT(TWCR_R,TWINT_B));
   //if status is valid
   if(I2C_GetStatus()==OWN_SLA_W_RECEIVED_ACK || I2C_GetStatus()== GENERAL_CALL_ADDRESS_RECEIVED_ACK)
   {
      //receive the data
      I2C_RecieveDataFrame(pu8Data,ACK);
      //complete transmission
      SET_BIT(TWCR_R,TWINT_B);
      //wait for current transmission to finish
      while(!GET_BIT(TWCR_R,TWINT_B));
      //if stop bit is received
      if(I2C_GetStatus()==RECEIVED_STOP_OR_RESTART_AS_SLAVE)
      {
         //return a E_OK
         return E_OK;
      }
   }
   //else return a fail
   return E_ERROR;
}


/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to enable the I2C bus event interrupt
************************************************************************************/
enuErrorStatus_t I2C_Enable_Interrupt(void)
{
   //set the interrupt enable bit in the control register
   SET_BIT(TWCR_R,TWIE_B);
   return E_OK;
}

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to disable the I2C bus event interrupt
************************************************************************************/
enuErrorStatus_t I2C_Disable_Interrupt(void)
{
   //clear the interrupt enable bit in the control register
   CLEAR_BIT(TWCR_R,TWIE_B);
   return E_OK;
}

/************************************************************************************
* Parameters (in): void(*local_fptr)(void)
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to set the callback function of the I2C bus event interrupt
************************************************************************************/
enuErrorStatus_t I2C_SetCallBack(void(*local_fptr)(void))
{
   //check if callback function pointer points to a valid function
   if(local_fptr == NULLPTR)
   {
      return E_ERROR;
   }
   //set the callback function to the sent function pointer
   I2C_Callback=local_fptr;
   return E_OK;
}

/********************************** ISR FUNCTION *************************/


/************************************************************************************
* Parameters (in): void
* Parameters (out): void
* Return value: void
* Description: I2C event interrupt service routine
************************************************************************************/
ISR(TWI_vect)
{
   //check if callback function pointer points to a valid function
   if(I2C_Callback != NULLPTR)
   {
      //call the callback function
      I2C_Callback();
   }
}
