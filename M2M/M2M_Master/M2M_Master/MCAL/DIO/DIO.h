#ifndef DIO_H_
#define DIO_H_

#include "STD_TYPES.h"
#include "BIT_MANIPULATE.h"

typedef enum {PORTA, PORTB, PORTC, PORTD} enuDIOPortNo_t;
typedef enum {DIO_PA0, DIO_PA1, DIO_PA2, DIO_PA3, DIO_PA4, DIO_PA5, DIO_PA6, DIO_PA7,
              DIO_PB0, DIO_PB1, DIO_PB2, DIO_PB3, DIO_PB4, DIO_PB5, DIO_PB6, DIO_PB7,
              DIO_PC0, DIO_PC1, DIO_PC2, DIO_PC3, DIO_PC4, DIO_PC5, DIO_PC6, DIO_PC7,
              DIO_PD0, DIO_PD1, DIO_PD2, DIO_PD3, DIO_PD4, DIO_PD5, DIO_PD6, DIO_PD7} enuDIOPinNo_t;
typedef enum {INPUT, OUTPUT} enuDIOPinDirection_t;
typedef enum {NO_CONNECTION, PULL_UP_ENABLE} enuDIOPinPullupResistorEnable_t;

/*************************************************************************************************
* Parameters	: enuDIOPinNo, enuDIOPinDirection, enuDIOPinPullupResistorEnable
* Return Value	: enuErrorStatus_t
* Description  : Initialize one DIO pin according to the configuration
*************************************************************************************************/
enuErrorStatus_t DIO_PinInit(enuDIOPinNo_t enuDIOPinNo, enuDIOPinDirection_t enuDIOPinDirection, enuDIOPinPullupResistorEnable_t enuDIOPinPullupResistorEnable);
/*************************************************************************************************
* Parameters	: enuDIOPortNo, u8DIOPortDirection, u8DIOPortPullupResistorEnable
* Return Value	: enuErrorStatus_t
* Description  : Initialize one DIO port according to the configuration
*************************************************************************************************/
enuErrorStatus_t DIO_PortInit(enuDIOPortNo_t enuDIOPortNo, uint8_t u8DIOPortDirection, uint8_t u8DIOPortPullupResistorEnable);
/*************************************************************************************************
* Parameters	: enuDIOPinNo, u8PinData
* Return Value	: enuErrorStatus_t
* Description  : Write a digital value into the selected DIO pin
*************************************************************************************************/
enuErrorStatus_t DIO_PinWrite(enuDIOPinNo_t enuDIOPinNo, uint8_t u8PinData);
/*************************************************************************************************
* Parameters	: enuDIOPortNo, u8PortData
* Return Value	: enuErrorStatus_t
* Description  : Write a digital value into the selected DIO port
*************************************************************************************************/
enuErrorStatus_t DIO_PortWrite(enuDIOPortNo_t enuDIOPortNo, uint8_t u8PortData);
/*************************************************************************************************
* Parameters	: enuDIOPinNo, pu8PinData
* Return Value	: enuErrorStatus_t
* Description  : Read a digital value from the selected DIO pin
*************************************************************************************************/
enuErrorStatus_t DIO_PinRead(enuDIOPinNo_t enuDIOPinNo, uint8_t* pu8PinData);
/*************************************************************************************************
* Parameters	: enuDIOPortNo, pu8PortData
* Return Value	: enuErrorStatus_t
* Description  : Read a digital value from the selected DIO port
*************************************************************************************************/
enuErrorStatus_t DIO_PortRead(enuDIOPortNo_t enuDIOPortNo, uint8_t* pu8PortData);
/*************************************************************************************************
* Parameters	: enuDIOPinNo
* Return Value	: enuErrorStatus_t
* Description  : Toggle the digital value of the selected DIO pin
*************************************************************************************************/
enuErrorStatus_t DIO_PinToggle(enuDIOPinNo_t enuDIOPinNo);
/*************************************************************************************************
* Parameters	: enuDIOPortNo
* Return Value	: enuErrorStatus_t
* Description  : Toggle the digital value of the selected DIO port
*************************************************************************************************/
enuErrorStatus_t DIO_PortToggle(enuDIOPortNo_t enuDIOPortNo);

#endif /* DIO_H_ */