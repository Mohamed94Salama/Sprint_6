#include "DIO.h"


typedef volatile uint8_t* const pu8DIORegisterAddress_t;

#define DIO_PORTS_NO            														(4u)

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

#define DIO_REG_ADDRESS(CHANNEL_ID, REG_OFFSET)										(au8PortsBaseAddressLUT[CHANNEL_ID] - REG_OFFSET)

/* Output Register */
#define PORTx_R(PORT_ID)																	*((pu8DIORegisterAddress_t)(DIO_REG_ADDRESS(PORT_ID, 0x00)))
/* Direction Register */
#define DDRx_R(PORT_ID)																		*((pu8DIORegisterAddress_t)(DIO_REG_ADDRESS(PORT_ID, 0x01)))
/* Input Register */
#define PINx_R(PORT_ID)																		*((pu8DIORegisterAddress_t)(DIO_REG_ADDRESS(PORT_ID, 0x02)))

/* Internal Macros */
#define DIO_PIN_NO_MIN_VALUE																(0u)
#define DIO_PIN_NO_MAX_VALUE																(31u)
#define DIO_PORT_NO_MIN_VALUE																(-1l)
#define DIO_PORT_NO_MAX_VALUE																(4u)
#define DIO_PORT_DIRECTION_MIN_VALUE													(0x00)
#define DIO_PORT_DIRECTION_MAX_VALUE													(0xFF)
#define DIO_PORT_PULLUP_RESISTOR_ENABLE_MIN_VALUE									(0x00)
#define DIO_PORT_PULLUP_RESISTOR_ENABLE_MAX_VALUE									(0xFF)
#define DIO_PORT_NO(PIN_NO)																(PIN_NO / 8)
#define DIO_PIN_NO(PIN_NO)																	(PIN_NO % 8)

typedef enum {DIO_INIT_UNDONE, DIO_INIT_DONE} enuDIOState_t;
static enuDIOState_t aenuDIOState[DIO_PIN_NO_MAX_VALUE] = {DIO_INIT_UNDONE};


/*************************************************************************************************
* Parameters	: strDIOConfigType
* Return Value	: enuErrorStatus_t
* Description  : Initialize one DIO pin according to the configuration
*************************************************************************************************/
enuErrorStatus_t DIO_Init(strDIOConfigType_t* strDIOConfigType)
{
	enuErrorStatus_t enuRetVar;

	if((strDIOConfigType->enuDIOPinNo >= DIO_PIN_NO_MIN_VALUE)					&& (strDIOConfigType->enuDIOPinNo <= DIO_PIN_NO_MAX_VALUE)						&&
		((strDIOConfigType->enuDIOPinDirection == INPUT)							|| (strDIOConfigType->enuDIOPinDirection == OUTPUT))								&&
		((strDIOConfigType->enuDIOPinPullupResistorEnable == NO_CONNECTION) 	|| (strDIOConfigType->enuDIOPinPullupResistorEnable == PULL_UP_ENABLE)))
	{
		/* Setting the direction of data [Input / Output] */
		ASSIGN_BIT(DDRx_R(DIO_PORT_NO(strDIOConfigType->enuDIOPinNo)), DIO_PIN_NO(strDIOConfigType->enuDIOPinNo), strDIOConfigType->enuDIOPinDirection);
		/* Selecting whether a pull-up resistor is connected to the selected pin or not */
		ASSIGN_BIT(PORTx_R(DIO_PORT_NO(strDIOConfigType->enuDIOPinNo)), DIO_PIN_NO(strDIOConfigType->enuDIOPinNo), strDIOConfigType->enuDIOPinPullupResistorEnable);
		aenuDIOState[strDIOConfigType->enuDIOPinNo] = DIO_INIT_DONE;
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;
}

/*************************************************************************************************
* Parameters	: u8GroupId, u8PinData
* Return Value	: enuErrorStatus_t
* Description  : Write a digital value into the selected DIO pin
*************************************************************************************************/
enuErrorStatus_t DIO_Write(uint8_t u8GroupId, uint8_t u8PinData)
{
	enuErrorStatus_t enuRetVar;
	
	if((aenuDIOState[astrDIOConfigParameters[u8GroupId].enuDIOPinNo] == DIO_INIT_DONE) &&
		(astrDIOConfigParameters[u8GroupId].enuDIOPinDirection == OUTPUT))
	{
		/* Writing the value into the corresponding pin */
		ASSIGN_BIT(PORTx_R(DIO_PORT_NO(astrDIOConfigParameters[u8GroupId].enuDIOPinNo)), DIO_PIN_NO(astrDIOConfigParameters[u8GroupId].enuDIOPinNo), u8PinData);
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;	
}

/*************************************************************************************************
* Parameters	: u8GroupId, pu8PinData
* Return Value	: enuErrorStatus_t
* Description  : Read a digital value from the selected DIO pin
*************************************************************************************************/
enuErrorStatus_t DIO_Read(uint8_t u8GroupId, uint8_t* pu8PinData)
{
	enuErrorStatus_t enuRetVar;
	
	if((aenuDIOState[astrDIOConfigParameters[u8GroupId].enuDIOPinNo] == DIO_INIT_DONE) &&
		(astrDIOConfigParameters[u8GroupId].enuDIOPinDirection == INPUT))
	{
		/* Reading the value from the corresponding pin */
		*pu8PinData = GET_BIT(PINx_R(DIO_PORT_NO(astrDIOConfigParameters[u8GroupId].enuDIOPinNo)), DIO_PIN_NO(astrDIOConfigParameters[u8GroupId].enuDIOPinNo));
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;	
}

/*************************************************************************************************
* Parameters	: u8GroupId
* Return Value	: enuErrorStatus_t
* Description  : Toggle the digital value of the selected DIO pin
*************************************************************************************************/
enuErrorStatus_t DIO_Toggle(uint8_t u8GroupId)
{
	enuErrorStatus_t enuRetVar;
	uint8_t u8PinData;
	
	if((aenuDIOState[astrDIOConfigParameters[u8GroupId].enuDIOPinNo] == DIO_INIT_DONE) &&
		(astrDIOConfigParameters[u8GroupId].enuDIOPinDirection == OUTPUT))
	{
		/* Reading the value from the corresponding pin */
		u8PinData = GET_BIT(PINx_R(DIO_PORT_NO(astrDIOConfigParameters[u8GroupId].enuDIOPinNo)), DIO_PIN_NO(astrDIOConfigParameters[u8GroupId].enuDIOPinNo)); // @suppress("Field cannot be resolved")
		/* Writing the value into the corresponding pin */
		ASSIGN_BIT(PORTx_R(DIO_PORT_NO(astrDIOConfigParameters[u8GroupId].enuDIOPinNo)), DIO_PIN_NO(astrDIOConfigParameters[u8GroupId].enuDIOPinNo), ~u8PinData);
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;	
}
