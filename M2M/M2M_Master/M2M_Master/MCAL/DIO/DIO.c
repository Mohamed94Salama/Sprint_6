#include "DIO.h"


typedef volatile uint8_t* const pu8DIORegisterAddress_t;

#define DIO_PORTS_NO            													   (4u)

#define PORTA_BASE_ADDRESS       													(0x3B)
#define PORTB_BASE_ADDRESS       													(0x38)
#define PORTC_BASE_ADDRESS       													(0x35)
#define PORTD_BASE_ADDRESS       													(0x32)

static const uint8_t au8PortsBaseAddressLUT[DIO_PORTS_NO] =
{
	PORTA_BASE_ADDRESS,
	PORTB_BASE_ADDRESS,
	PORTC_BASE_ADDRESS,
	PORTD_BASE_ADDRESS
};

#define DIO_REG_ADDRESS(CHANNEL_ID, REG_OFFSET)								(au8PortsBaseAddressLUT[CHANNEL_ID] - REG_OFFSET)

/* Output Register */
#define PORTx_R(PORT_ID)															(*((pu8DIORegisterAddress_t)(DIO_REG_ADDRESS(PORT_ID, 0x00))))
/* Direction Register */                                                                                                          
#define DDRx_R(PORT_ID)																(*((pu8DIORegisterAddress_t)(DIO_REG_ADDRESS(PORT_ID, 0x01))))
/* Input Register */                                                                                                              
#define PINx_R(PORT_ID)																(*((pu8DIORegisterAddress_t)(DIO_REG_ADDRESS(PORT_ID, 0x02))))

/* Internal Macros */
#define DIO_PIN_NO_MIN_VALUE														(-1l)
#define DIO_PIN_NO_MAX_VALUE														(32u)
#define DIO_PIN_DIRECTION_MIN_VALUE													(-1l)
#define DIO_PIN_DIRECTION_MAX_VALUE													(2u)
#define DIO_PIN_PULLUP_RESISTOR_ENABLE_MIN_VALUE									(-1l)
#define DIO_PIN_PULLUP_RESISTOR_ENABLE_MAX_VALUE									(2u)
#define DIO_PORT_NO_MIN_VALUE														(-1l)
#define DIO_PORT_NO_MAX_VALUE														(4u)
#define DIO_PORT_DIRECTION_MIN_VALUE												(0x00)
#define DIO_PORT_DIRECTION_MAX_VALUE												(0xFF)
#define DIO_PORT_PULLUP_RESISTOR_ENABLE_MIN_VALUE									(0x00)
#define DIO_PORT_PULLUP_RESISTOR_ENABLE_MAX_VALUE									(0xFF)
#define DIO_PORT_NO(PIN_NO)															(PIN_NO / 8)
#define DIO_PIN_NO(PIN_NO)															(PIN_NO % 8)

typedef enum {DIO_INIT_UNDONE, DIO_INIT_DONE} enuDIOState_t;
static enuDIOState_t aenuDIOPinState[DIO_PIN_NO_MAX_VALUE] = {DIO_INIT_UNDONE};
static enuDIOState_t aenuDIOPortState[DIO_PORT_NO_MAX_VALUE] = {DIO_INIT_UNDONE};


/*************************************************************************************************
* Parameters	: enuDIOPinNo, enuDIOPinDirection, enuDIOPinPullupResistorEnable
* Return Value	: enuErrorStatus_t
* Description  : Initialize one DIO pin according to the configuration
*************************************************************************************************/
enuErrorStatus_t DIO_PinInit(enuDIOPinNo_t enuDIOPinNo, enuDIOPinDirection_t enuDIOPinDirection, enuDIOPinPullupResistorEnable_t enuDIOPinPullupResistorEnable)
{
	enuErrorStatus_t enuRetVar;

	if((enuDIOPinNo > DIO_PIN_NO_MIN_VALUE)											&& (enuDIOPinNo < DIO_PIN_NO_MAX_VALUE)												&&
		(enuDIOPinDirection > DIO_PIN_DIRECTION_MIN_VALUE)							&& (enuDIOPinDirection < DIO_PIN_DIRECTION_MAX_VALUE)								&&
		(enuDIOPinPullupResistorEnable > DIO_PIN_PULLUP_RESISTOR_ENABLE_MIN_VALUE) 	&& (enuDIOPinPullupResistorEnable < DIO_PIN_PULLUP_RESISTOR_ENABLE_MAX_VALUE))
	{
		/* Setting the direction of data [Input / Output] */
		ASSIGN_BIT(DDRx_R(DIO_PORT_NO(enuDIOPinNo)), DIO_PIN_NO(enuDIOPinNo), enuDIOPinDirection);
		/* Selecting whether a pull-up resistor is connected to the selected pin or not */
		ASSIGN_BIT(PORTx_R(DIO_PORT_NO(enuDIOPinNo)), DIO_PIN_NO(enuDIOPinNo), enuDIOPinPullupResistorEnable);
		aenuDIOPinState[enuDIOPinNo] = DIO_INIT_DONE;
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;
}

/*************************************************************************************************
* Parameters	: enuDIOPortNo, u8DIOPortDirection, u8DIOPortPullupResistorEnable
* Return Value	: enuErrorStatus_t
* Description  : Initialize one DIO port according to the configuration
*************************************************************************************************/
enuErrorStatus_t DIO_PortInit(enuDIOPortNo_t enuDIOPortNo, uint8_t u8DIOPortDirection, uint8_t u8DIOPortPullupResistorEnable)
{
	enuErrorStatus_t enuRetVar;

	if((enuDIOPortNo > DIO_PORT_NO_MIN_VALUE)											&& (enuDIOPortNo < DIO_PORT_NO_MAX_VALUE)													&&
		(u8DIOPortDirection >= DIO_PORT_DIRECTION_MIN_VALUE)							&& (u8DIOPortDirection <= DIO_PORT_DIRECTION_MAX_VALUE)								&&
		(u8DIOPortPullupResistorEnable >= DIO_PORT_PULLUP_RESISTOR_ENABLE_MIN_VALUE)	&& (u8DIOPortPullupResistorEnable <= DIO_PORT_PULLUP_RESISTOR_ENABLE_MAX_VALUE))
	{
		/* Setting the direction of data [Input / Output] */
		DDRx_R(enuDIOPortNo) = u8DIOPortDirection;
		/* Selecting whether a pull-up resistor is connected to the selected port or not */
		PORTx_R(enuDIOPortNo) = u8DIOPortPullupResistorEnable;
		aenuDIOPortState[enuDIOPortNo] = DIO_INIT_DONE;
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;
}

/*************************************************************************************************
* Parameters	: enuDIOPinNo, u8PinData
* Return Value	: enuErrorStatus_t
* Description  : Write a digital value into the selected DIO pin
*************************************************************************************************/
enuErrorStatus_t DIO_PinWrite(enuDIOPinNo_t enuDIOPinNo, uint8_t u8PinData)
{
	enuErrorStatus_t enuRetVar;
	
	if(aenuDIOPinState[enuDIOPinNo] == DIO_INIT_DONE)
	{
		/* Writing the value into the corresponding pin */
		ASSIGN_BIT(PORTx_R(DIO_PORT_NO(enuDIOPinNo)), DIO_PIN_NO(enuDIOPinNo), u8PinData);
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;	
}

/*************************************************************************************************
* Parameters	: enuDIOPortNo, u8PortData
* Return Value	: enuErrorStatus_t
* Description  : Write a digital value into the selected DIO port
*************************************************************************************************/
enuErrorStatus_t DIO_PortWrite(enuDIOPortNo_t enuDIOPortNo, uint8_t u8PortData)
{
	enuErrorStatus_t enuRetVar;
	
	if(aenuDIOPortState[enuDIOPortNo] == DIO_INIT_DONE)
	{
		/* Writing the value into the corresponding port */
		PORTx_R(enuDIOPortNo) = u8PortData;
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;	
}

/*************************************************************************************************
* Parameters	: enuDIOPinNo, pu8PinData
* Return Value	: enuErrorStatus_t
* Description  : Read a digital value from the selected DIO pin
*************************************************************************************************/
enuErrorStatus_t DIO_PinRead(enuDIOPinNo_t enuDIOPinNo, uint8_t* pu8PinData)
{
	enuErrorStatus_t enuRetVar;
	
	if(aenuDIOPinState[enuDIOPinNo] == DIO_INIT_DONE)
	{
		/* Reading the value from the corresponding pin */
		*pu8PinData = GET_BIT(PINx_R(DIO_PORT_NO(enuDIOPinNo)), DIO_PIN_NO(enuDIOPinNo));
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;	
}

/*************************************************************************************************
* Parameters	: enuDIOPortNo, pu8PortData
* Return Value	: enuErrorStatus_t
* Description  : Read a digital value from the selected DIO port
*************************************************************************************************/
enuErrorStatus_t DIO_PortRead(enuDIOPortNo_t enuDIOPortNo, uint8_t* pu8PortData)
{
	enuErrorStatus_t enuRetVar;
	
	if(aenuDIOPortState[enuDIOPortNo] == DIO_INIT_DONE)
	{
		/* Reading the value from the corresponding port */
		*pu8PortData = PINx_R(enuDIOPortNo);
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;	
}

/*************************************************************************************************
* Parameters	: enuDIOPinNo
* Return Value	: enuErrorStatus_t
* Description  : Toggle the digital value of the selected DIO pin
*************************************************************************************************/
enuErrorStatus_t DIO_PinToggle(enuDIOPinNo_t enuDIOPinNo)
{
	enuErrorStatus_t enuRetVar;
	uint8_t u8PinData;
	
	if(aenuDIOPinState[enuDIOPinNo] == DIO_INIT_DONE)
	{
      
		/* Reading the value from the corresponding pin */
		u8PinData = GET_BIT(PINx_R(DIO_PORT_NO(enuDIOPinNo)), DIO_PIN_NO(enuDIOPinNo));
		/* Writing the value into the corresponding pin */
		ASSIGN_BIT(PORTx_R(DIO_PORT_NO(enuDIOPinNo)), DIO_PIN_NO(enuDIOPinNo), ~u8PinData);
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;	
}

/*************************************************************************************************
* Parameters	: enuDIOPortNo
* Return Value	: enuErrorStatus_t
* Description  : Toggle the digital value of the selected DIO port
*************************************************************************************************/
enuErrorStatus_t DIO_PortToggle(enuDIOPortNo_t enuDIOPortNo)
{
	enuErrorStatus_t enuRetVar;
	uint8_t u8PortData;
	
	if(aenuDIOPortState[enuDIOPortNo] == DIO_INIT_DONE)
	{
		/* Reading the value from the corresponding port */
		u8PortData = PINx_R(enuDIOPortNo);
		/* Writing the value into the corresponding port */
		PORTx_R(enuDIOPortNo) = ~u8PortData;
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;	
}
