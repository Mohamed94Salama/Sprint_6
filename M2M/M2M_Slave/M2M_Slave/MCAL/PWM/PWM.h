#ifndef PWM_H_
#define PWM_H_

#include "PWM_Cfg.h"
#include "STD_TYPES.h"
#include "BIT_MANIPULATE.h"

typedef enum {PWM0, PWM1A, PWM1B, PWM2} enuPWMModule_t;
typedef enum {PC_PWM_8_BITS = 1, PC_PWM_9_BITS, PC_PWM_10_BITS, 
			  	  FAST_PWM_8_BITS = 5, FAST_PWM_9_BITS, FAST_PWM_10_BITS,
				  PFC_PWM_ICR1, PFC_PWM_OCR1A, PC_PWM_ICR1, PC_PWM_OCR1A,
				  FAST_PWM_ICR1 = 14, FAST_PWM_OCR1A = 15} enuPWMMode_t;
typedef enum {NORMAL_OPERATION, NON_INVERTED_PWM = 2, INVERTED_PWM} enuPWMCompareOutputMode_t;
typedef enum {PWM_NO_CLOCK, PWM_NO_PRESCALER, PWM_PRESCALER_8, 
				  PWM_PRESCALER_64, PWM_PRESCALER_256, PWM_PRESCALER_1024} enuPWMClock_t;

typedef struct
{
	/* Selecting the PWM module used */
	enuPWMModule_t enuPWMModule;
	/* Selecting the PWM module mode used */
	enuPWMMode_t enuPWMMode;
	/* Selecting the PWM module compare output behavior */
	enuPWMCompareOutputMode_t enuPWMCompareOutputMode;
	/* Selecting the PWM module prescaler used */
	enuPWMClock_t enuPWMClock;
} strPWMConfigType_t;

extern strPWMConfigType_t astrPWMConfigParameters[PWM_GROUPS_NO];

/*************************************************************************************************
* Parameters	: strPWMConfigType
* Return Value	: enuErrorStatus_t
* Description  : Initialize the PWM module according to the configuration
*************************************************************************************************/
enuErrorStatus_t PWM_Init(strPWMConfigType_t* strPWMConfigType);
/*************************************************************************************************
* Parameters	: u8GroupId, u8DutyCycle
* Return Value	: enuErrorStatus_t
* Description  : Start the PWM signal with the specified duty cycle
*************************************************************************************************/
enuErrorStatus_t PWM_Start(uint8_t u8GroupId, uint8_t u8DutyCycle);
/*************************************************************************************************
* Parameters	: u8GroupId
* Return Value	: enuErrorStatus_t
* Description  : Stop the PWM signal immediately
*************************************************************************************************/
enuErrorStatus_t PWM_Stop(uint8_t u8GroupId);

#endif /* PWM_H_ */
