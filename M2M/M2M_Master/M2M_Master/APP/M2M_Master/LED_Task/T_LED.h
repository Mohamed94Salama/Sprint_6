/*
 * T_LED.h
 *
 * Created: 9/10/2021 4:37:04 PM
 *  Author: Amrth
 */ 


#ifndef T_LED_H_
#define T_LED_H_

#include "LED.h"

typedef enum {T_LED_OFF,T_LED_MalFunction,T_LED_DeviceSearch,T_LED_VerificationSearch,T_LED_ConnectionON,T_LED_INVALID}enuLEDPattern_t;

void LED_Task(void);
enuErrorStatus_t T_LED_SetPattern(enuLEDPattern_t Pattern);
enuErrorStatus_t T_LED_GetPattern(enuLEDPattern_t *Pattern);

#endif /* T_LED_H_ */