#include "PWM.h"


typedef volatile uint8_t* const pu8PWMRegisterAddress_t;
typedef volatile uint16_t* const pu16PWMRegisterAddress_t;

#define OCR2_R       																			*((pu8PWMRegisterAddress_t)	0x43)
#define TCCR2_R      																			*((pu8PWMRegisterAddress_t)	0x45)
#define ICR1_R																						*((pu16PWMRegisterAddress_t) 	0x46)
#define ICR1L_R																					*((pu8PWMRegisterAddress_t) 	0x46)
#define ICR1H_R																					*((pu8PWMRegisterAddress_t) 	0x47)
#define OCR1B_R																					*((pu16PWMRegisterAddress_t) 	0x48)
#define OCR1BL_R																					*((pu8PWMRegisterAddress_t) 	0x48)
#define OCR1BH_R																					*((pu8PWMRegisterAddress_t) 	0x49)
#define OCR1A_R																					*((pu16PWMRegisterAddress_t) 	0x4A)
#define OCR1AL_R																					*((pu8PWMRegisterAddress_t) 	0x4A)
#define OCR1AH_R																					*((pu8PWMRegisterAddress_t) 	0x4B)
#define TCCR1B_R																					*((pu8PWMRegisterAddress_t) 	0x4E)
#define TCCR1A_R																					*((pu8PWMRegisterAddress_t) 	0x4F)
#define TCCR0_R   																				*((pu8PWMRegisterAddress_t)	0x53)
#define OCR0_R    																				*((pu8PWMRegisterAddress_t)	0x5C)

/* Internal Macros */
#define PWM_MODULES_NO																			(4u)
#define PWM_CLOCK_MIN_VALUE																	(0u)
#define PWM_CLOCK_MAX_VALUE																	(6u)
#define PWM_MODE_MIN_VALUE																		(0u)
#define PWM_MODE_MAX_VALUE																		(15u)
#define PWM_TC0_WGM00																			(6u)
#define PWM_TC0_WGM01																			(3u)
#define PWM_TC0_COMPARE_OUTPUT_MODE_START_BIT											(4u)
#define PWM_TC0_COMPARE_OUTPUT_MODE_MASK													(0x03)
#define PWM_TC0_COMPARE_OUTPUT_REGISTER_START_BIT										(0u)
#define PWM_TC0_COMPARE_OUTPUT_REGISTER_MASK												(0xFF)
#define PWM_TC1_WGM10_WGM11_START_BIT														(0u)
#define PWM_TC1_WGM10_WGM11_MASK																(0x03)
#define PWM_TC1_WGM12_WGM13_START_BIT														(3u)
#define PWM_TC1_WGM12_WGM13_MASK																(0x0C)
#define PWM_TC1A_COMPARE_OUTPUT_MODE_START_BIT											(6u)
#define PWM_TC1A_COMPARE_OUTPUT_MODE_MASK													(0x03)
#define PWM_TC1B_COMPARE_OUTPUT_MODE_START_BIT											(4u)
#define PWM_TC1B_COMPARE_OUTPUT_MODE_MASK													(0x03)
#define PWM_TC1_COMPARE_OUTPUT_REGISTER_START_BIT										(0u)
#define PWM_TC1_COMPARE_OUTPUT_REGISTER_MASK												(0xFFFF)
#define PWM_TC2_WGM20																			(6u)
#define PWM_TC2_WGM21																			(3u)
#define PWM_TC2_COMPARE_OUTPUT_MODE_START_BIT											(4u)
#define PWM_TC2_COMPARE_OUTPUT_MODE_MASK													(0x03)
#define PWM_TC2_COMPARE_OUTPUT_REGISTER_START_BIT										(0u)
#define PWM_TC2_COMPARE_OUTPUT_REGISTER_MASK												(0xFF)
#define PWM_CLOCK_SELECT_START_BIT															(0u)
#define PWM_CLOCK_SELECT_MASK																	(0x07)
#define PWM_WGM_VALUE_LOW																		(0u)
#define PWM_WGM_VALUE_HIGH																		(1u)
#define PWM_FREQUENCY_10K																		(10000ul)
#define PWM_CALCULATE_REGISTER_VALUE(FREQUENCY, DUTY_CYCLE)							((((uint32_t)FREQUENCY) * ((uint32_t)DUTY_CYCLE)) / 100)
#define PWM_DUTY_CYCLE_MIN_VALUE																(0u)
#define PWM_DUTY_CYCLE_MAX_VALUE																(100u)

typedef enum {PWM_INIT_UNDONE, PWM_INIT_DONE} enuPWMState_t;
static enuPWMState_t aenuPWMState[PWM_MODULES_NO] = {PWM_INIT_UNDONE};


/*************************************************************************************************
* Parameters	: strPWMConfigType
* Return Value	: enuErrorStatus_t
* Description  : Initialize the PWM module according to the configuration
*************************************************************************************************/
enuErrorStatus_t PWM_Init(strPWMConfigType_t* strPWMConfigType)
{
	enuErrorStatus_t enuRetVar;
	
	if(((((strPWMConfigType->enuPWMModule == PWM0) 			|| (strPWMConfigType->enuPWMModule == PWM2)) 											&&
		  ((strPWMConfigType->enuPWMMode == PC_PWM_8_BITS) || (strPWMConfigType->enuPWMMode == FAST_PWM_8_BITS))) 								||
		 (((strPWMConfigType->enuPWMModule == PWM1A) 		|| (strPWMConfigType->enuPWMModule == PWM1B)) 											&&
		  ((strPWMConfigType->enuPWMMode >= PWM_MODE_MIN_VALUE) && (strPWMConfigType->enuPWMMode < PWM_MODE_MAX_VALUE)))) 					&&
	   ((strPWMConfigType->enuPWMCompareOutputMode == NORMAL_OPERATION)	|| (strPWMConfigType->enuPWMCompareOutputMode == INVERTED_PWM) ||
		 (strPWMConfigType->enuPWMCompareOutputMode == NON_INVERTED_PWM))	&&
		(strPWMConfigType->enuPWMClock >= PWM_CLOCK_MIN_VALUE)				&& (strPWMConfigType->enuPWMClock < PWM_CLOCK_MAX_VALUE))
	{
		switch(strPWMConfigType->enuPWMModule)
		{
			case PWM0:
				/* Selecting the PWM module mode */
				if(strPWMConfigType->enuPWMMode == PC_PWM_8_BITS)
				{
					/* Selecting PC PWM mode */
					ASSIGN_BIT(TCCR0_R, PWM_TC0_WGM00, PWM_WGM_VALUE_HIGH);
					ASSIGN_BIT(TCCR0_R, PWM_TC0_WGM01, PWM_WGM_VALUE_LOW);
				}
				else if(strPWMConfigType->enuPWMMode == FAST_PWM_8_BITS)
				{
					/* Selecting Fast PWM mode */
					ASSIGN_BIT(TCCR0_R, PWM_TC0_WGM00, PWM_WGM_VALUE_HIGH);
					ASSIGN_BIT(TCCR0_R, PWM_TC0_WGM01, PWM_WGM_VALUE_HIGH);
				}
				else
				{
				}
				/* Selecting the behavior of the compare match mode output (Inverted or Non-inverted PWM) */
				ASSIGN_BITS(TCCR0_R, PWM_TC0_COMPARE_OUTPUT_MODE_START_BIT, strPWMConfigType->enuPWMCompareOutputMode, PWM_TC0_COMPARE_OUTPUT_MODE_MASK);
				/* Selecting the pre-configured clock prescaler value */
				ASSIGN_BITS(TCCR0_R, PWM_CLOCK_SELECT_START_BIT, strPWMConfigType->enuPWMClock, PWM_CLOCK_SELECT_MASK);
				/* Stopping the PWM signal at the beginning of initialization phase */
				if(strPWMConfigType->enuPWMCompareOutputMode == INVERTED_PWM)
				{
					CLEAR_BITS(OCR0_R, PWM_TC0_COMPARE_OUTPUT_REGISTER_START_BIT, PWM_TC0_COMPARE_OUTPUT_REGISTER_MASK);
				}
				else if(strPWMConfigType->enuPWMCompareOutputMode == NON_INVERTED_PWM)
				{
					SET_BITS(OCR0_R, PWM_TC0_COMPARE_OUTPUT_REGISTER_START_BIT, PWM_TC0_COMPARE_OUTPUT_REGISTER_MASK);
				}
				else
				{
				}
				break;
			case PWM1A:
				/* Selecting the PWM module mode */
				ASSIGN_BITS(TCCR1A_R, PWM_TC1_WGM10_WGM11_START_BIT, strPWMConfigType->enuPWMMode, PWM_TC1_WGM10_WGM11_MASK);
				ASSIGN_BITS(TCCR1B_R, PWM_TC1_WGM12_WGM13_START_BIT, strPWMConfigType->enuPWMMode, PWM_TC1_WGM12_WGM13_MASK);
				/* Selecting the behavior of the compare match mode output for channel A (Inverted or Non-inverted PWM) */
				ASSIGN_BITS(TCCR1A_R, PWM_TC1A_COMPARE_OUTPUT_MODE_START_BIT, strPWMConfigType->enuPWMCompareOutputMode, PWM_TC1A_COMPARE_OUTPUT_MODE_MASK);
				/* Selecting the pre-configured clock prescaler value */
				ASSIGN_BITS(TCCR1B_R, PWM_CLOCK_SELECT_START_BIT, strPWMConfigType->enuPWMClock, PWM_CLOCK_SELECT_MASK);
				/* Stopping the PWM signal at the beginning of initialization phase */
				if(strPWMConfigType->enuPWMCompareOutputMode == INVERTED_PWM)
				{
					CLEAR_BITS(OCR1A_R, PWM_TC1_COMPARE_OUTPUT_REGISTER_START_BIT, PWM_TC1_COMPARE_OUTPUT_REGISTER_MASK);
				}
				else if(strPWMConfigType->enuPWMCompareOutputMode == NON_INVERTED_PWM)
				{
					SET_BITS(OCR1A_R, PWM_TC1_COMPARE_OUTPUT_REGISTER_START_BIT, PWM_TC1_COMPARE_OUTPUT_REGISTER_MASK);
				}
				else
				{
				}
				break;
			case PWM1B:
				/* Selecting the PWM module mode */
				ASSIGN_BITS(TCCR1A_R, PWM_TC1_WGM10_WGM11_START_BIT, strPWMConfigType->enuPWMMode, PWM_TC1_WGM10_WGM11_MASK);
				ASSIGN_BITS(TCCR1B_R, PWM_TC1_WGM12_WGM13_START_BIT, strPWMConfigType->enuPWMMode, PWM_TC1_WGM12_WGM13_MASK);
				/* Selecting the behavior of the compare match mode output for channel B (Inverted or Non-inverted PWM) */
				ASSIGN_BITS(TCCR1A_R, PWM_TC1B_COMPARE_OUTPUT_MODE_START_BIT, strPWMConfigType->enuPWMCompareOutputMode, PWM_TC1B_COMPARE_OUTPUT_MODE_MASK);
				/* Selecting the pre-configured clock prescaler value */
				ASSIGN_BITS(TCCR1B_R, PWM_CLOCK_SELECT_START_BIT, strPWMConfigType->enuPWMClock, PWM_CLOCK_SELECT_MASK);
				/* Stopping the PWM signal at the beginning of initialization phase */
				if(strPWMConfigType->enuPWMCompareOutputMode == INVERTED_PWM)
				{
					CLEAR_BITS(OCR1B_R, PWM_TC1_COMPARE_OUTPUT_REGISTER_START_BIT, PWM_TC1_COMPARE_OUTPUT_REGISTER_MASK);
				}
				else if(strPWMConfigType->enuPWMCompareOutputMode == NON_INVERTED_PWM)
				{
					SET_BITS(OCR1B_R, PWM_TC1_COMPARE_OUTPUT_REGISTER_START_BIT, PWM_TC1_COMPARE_OUTPUT_REGISTER_MASK);
				}
				else
				{
				}
				break;
			case PWM2:
				/* Selecting the PWM module mode */
				if(strPWMConfigType->enuPWMMode == PC_PWM_8_BITS)
				{
					/* Selecting PC PWM mode */
					ASSIGN_BIT(TCCR2_R, PWM_TC2_WGM20, PWM_WGM_VALUE_HIGH);
					ASSIGN_BIT(TCCR2_R, PWM_TC2_WGM21, PWM_WGM_VALUE_LOW);
				}
				else if(strPWMConfigType->enuPWMMode == FAST_PWM_8_BITS)
				{
					/* Selecting Fast PWM mode */
					ASSIGN_BIT(TCCR2_R, PWM_TC2_WGM20, PWM_WGM_VALUE_HIGH);
					ASSIGN_BIT(TCCR2_R, PWM_TC2_WGM21, PWM_WGM_VALUE_HIGH);
				}
				else
				{
				}
				/* Selecting the behavior of the compare match mode output (Inverted or Non-inverted PWM) */
				ASSIGN_BITS(TCCR2_R, PWM_TC2_COMPARE_OUTPUT_MODE_START_BIT, strPWMConfigType->enuPWMCompareOutputMode, PWM_TC2_COMPARE_OUTPUT_MODE_MASK);
				/* Selecting the pre-configured clock prescaler value */
				ASSIGN_BITS(TCCR2_R, PWM_CLOCK_SELECT_START_BIT, strPWMConfigType->enuPWMClock, PWM_CLOCK_SELECT_MASK);
				/* Stopping the PWM signal at the beginning of initialization phase */
				if(strPWMConfigType->enuPWMCompareOutputMode == INVERTED_PWM)
				{
					CLEAR_BITS(OCR2_R, PWM_TC0_COMPARE_OUTPUT_REGISTER_START_BIT, PWM_TC0_COMPARE_OUTPUT_REGISTER_MASK);
				}
				else if(strPWMConfigType->enuPWMCompareOutputMode == NON_INVERTED_PWM)
				{
					SET_BITS(OCR2_R, PWM_TC0_COMPARE_OUTPUT_REGISTER_START_BIT, PWM_TC0_COMPARE_OUTPUT_REGISTER_MASK);
				}
				else
				{
				}
				break;
			default:
				break;
		}
		aenuPWMState[strPWMConfigType->enuPWMModule] = PWM_INIT_DONE;
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;	
}

/*************************************************************************************************
* Parameters	: u8GroupId, u8DutyCycle
* Return Value	: enuErrorStatus_t
* Description  : Start the PWM signal with the specified duty cycle
*************************************************************************************************/
enuErrorStatus_t PWM_Start(uint8_t u8GroupId, uint8_t u8DutyCycle)
{
	enuErrorStatus_t enuRetVar;
	uint16_t u16PWMRegisterValue;
	
	if((aenuPWMState[astrPWMConfigParameters[u8GroupId].enuPWMModule] == PWM_INIT_DONE) &&
		(u8DutyCycle >= PWM_DUTY_CYCLE_MIN_VALUE) && (u8DutyCycle <= PWM_DUTY_CYCLE_MAX_VALUE))
	{
		/* Calculating the PWM register value required according to the equation */
		u16PWMRegisterValue = PWM_CALCULATE_REGISTER_VALUE(PWM_FREQUENCY_10K, u8DutyCycle);
		switch(astrPWMConfigParameters[u8GroupId].enuPWMModule)
		{
			case PWM0:
				/* Loading the PWM register value into OCR0 register */
				OCR0_R = (uint8_t)u16PWMRegisterValue;
				break;
			case PWM1A:
				ICR1_R = PWM_FREQUENCY_10K;
				/* Loading the PWM register value into OCR1A register */
				OCR1A_R = u16PWMRegisterValue;
				break;
			case PWM1B:
				ICR1_R = PWM_FREQUENCY_10K;
				/* Loading the PWM register value into OCR1B register */
				OCR1B_R = u16PWMRegisterValue;
				break;
			case PWM2:
				/* Loading the PWM register value into OCR2 register */
				OCR2_R = (uint8_t)u16PWMRegisterValue;
				break;
			default:
				break;
		}
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
* Description  : Stop the PWM signal immediately
*************************************************************************************************/
enuErrorStatus_t PWM_Stop(uint8_t u8GroupId)
{
	enuErrorStatus_t enuRetVar = E_OK;
	
	if(aenuPWMState[astrPWMConfigParameters[u8GroupId].enuPWMModule] == PWM_INIT_DONE)
	{
		switch(astrPWMConfigParameters[u8GroupId].enuPWMModule)
		{
			case PWM0:
				/* Stopping the PWM signal */
				if(astrPWMConfigParameters[u8GroupId].enuPWMCompareOutputMode == INVERTED_PWM)
				{
					CLEAR_BITS(OCR0_R, PWM_TC0_COMPARE_OUTPUT_REGISTER_START_BIT, PWM_TC0_COMPARE_OUTPUT_REGISTER_MASK);
				}
				else if(astrPWMConfigParameters[u8GroupId].enuPWMCompareOutputMode == NON_INVERTED_PWM)
				{
					SET_BITS(OCR0_R, PWM_TC0_COMPARE_OUTPUT_REGISTER_START_BIT, PWM_TC0_COMPARE_OUTPUT_REGISTER_MASK);
				}
				else
				{
				}
				break;
			case PWM1A:
				/* Stopping the PWM signal */
				if(astrPWMConfigParameters[u8GroupId].enuPWMCompareOutputMode == INVERTED_PWM)
				{
					CLEAR_BITS(OCR1A_R, PWM_TC1_COMPARE_OUTPUT_REGISTER_START_BIT, PWM_TC1_COMPARE_OUTPUT_REGISTER_MASK);
				}
				else if(astrPWMConfigParameters[u8GroupId].enuPWMCompareOutputMode == NON_INVERTED_PWM)
				{
					SET_BITS(OCR1A_R, PWM_TC1_COMPARE_OUTPUT_REGISTER_START_BIT, PWM_TC1_COMPARE_OUTPUT_REGISTER_MASK);
				}
				else
				{
				}
				break;
			case PWM1B:
				/* Stopping the PWM signal */
				if(astrPWMConfigParameters[u8GroupId].enuPWMCompareOutputMode == INVERTED_PWM)
				{
					CLEAR_BITS(OCR1B_R, PWM_TC1_COMPARE_OUTPUT_REGISTER_START_BIT, PWM_TC1_COMPARE_OUTPUT_REGISTER_MASK);
				}
				else if(astrPWMConfigParameters[u8GroupId].enuPWMCompareOutputMode == NON_INVERTED_PWM)
				{
					SET_BITS(OCR1B_R, PWM_TC1_COMPARE_OUTPUT_REGISTER_START_BIT, PWM_TC1_COMPARE_OUTPUT_REGISTER_MASK);
				}
				else
				{
				}
				break;
			case PWM2:
				/* Stopping the PWM signal */
				if(astrPWMConfigParameters[u8GroupId].enuPWMCompareOutputMode == INVERTED_PWM)
				{
					CLEAR_BITS(OCR2_R, PWM_TC2_COMPARE_OUTPUT_REGISTER_START_BIT, PWM_TC2_COMPARE_OUTPUT_REGISTER_MASK);
				}
				else if(astrPWMConfigParameters[u8GroupId].enuPWMCompareOutputMode == NON_INVERTED_PWM)
				{
					SET_BITS(OCR2_R, PWM_TC2_COMPARE_OUTPUT_REGISTER_START_BIT, PWM_TC2_COMPARE_OUTPUT_REGISTER_MASK);
				}
				else
				{
				}
				break;
			default:
				break;
		}
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;	
}
