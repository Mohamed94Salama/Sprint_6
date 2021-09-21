#ifndef DIO_H_
#define DIO_H_

#include "DIO_Cfg.h"
#include "STD_TYPES.h"
#include "BIT_MANIPULATE.h"

typedef enum {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
				  PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7,
				  PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7,
				  PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7} enuDIOPinNo_t;
typedef enum {INPUT, OUTPUT} enuDIOPinDirection_t;
typedef enum {NO_CONNECTION, PULL_UP_ENABLE} enuDIOPinPullupResistorEnable_t;

typedef struct
{
	/* Selecting the pin number in that specific port selected */
	enuDIOPinNo_t enuDIOPinNo;
	/* Selecting the direction of data on the selected pin */
	enuDIOPinDirection_t enuDIOPinDirection;
	/* Selecting whether a pull-up resistor is connected to the selected pin or not */
	enuDIOPinPullupResistorEnable_t enuDIOPinPullupResistorEnable;
} strDIOConfigType_t;

extern strDIOConfigType_t astrDIOConfigParameters[DIO_GROUPS_NO];

/*************************************************************************************************
* Parameters	: strDIOConfigType
* Return Value	: enuErrorStatus_t
* Description  : Initialize one DIO pin according to the configuration
*************************************************************************************************/
enuErrorStatus_t DIO_Init(strDIOConfigType_t* strDIOConfigType);
/*************************************************************************************************
* Parameters	: u8GroupId, u8PinData
* Return Value	: enuErrorStatus_t
* Description  : Write a digital value into the selected DIO pin
*************************************************************************************************/
enuErrorStatus_t DIO_Write(uint8_t u8GroupId, uint8_t u8PinData);
/*************************************************************************************************
* Parameters	: u8GroupId, pu8PinData
* Return Value	: enuErrorStatus_t
* Description  : Read a digital value from the selected DIO pin
*************************************************************************************************/
enuErrorStatus_t DIO_Read(uint8_t u8GroupId, uint8_t* pu8PinData);
/*************************************************************************************************
* Parameters	: u8GroupId
* Return Value	: enuErrorStatus_t
* Description  : Toggle the digital value of the selected DIO pin
*************************************************************************************************/
enuErrorStatus_t DIO_Toggle(uint8_t u8GroupId);


#endif /* DIO_H_ */
