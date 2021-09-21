/*****************************************************************************
* Task: I2C Driver
* File Name: I2C_Cfg.h
* Description: header file for I2C driver configuration
* Author: Amr Mohamed
* Date: 24/7/2021
******************************************************************************/

#ifndef __I2C_CFG__
#define __I2C_CFG__

//prescaler values
typedef enum {I2C_PRESCALER_1=0,I2C_PRESCALER_4,I2C_PRESCALER_16,I2C_PRESCALER_64}enuI2CPrescaler_t;

//I2C SCL prescaler value
#define   I2C_PRESCALER          I2C_PRESCALER_64

//I2C SCL frequency
#define   I2C_CLK                100000UL

//I2C slave address
#define   I2C_SLAVE_ADDRESS      0x02

//I2C slave global call recognition enable
#define   I2C_SLAVE_GLOBAL_EN    ENABLE

//I2C slave acknowledgment enable
#define   I2C_SLAVE_ACK          ENABLE



#endif /* __I2C_CFG__ */