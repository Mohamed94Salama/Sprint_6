/*
 * T_Keypad.h
 *
 * Created: 9/10/2021 9:34:46 PM
 *  Author: Amrth
 */ 


#ifndef T_KEYPAD_H_
#define T_KEYPAD_H_

#include "Keypad.h"

#define KEYPAD_UPDATE_DURATION      50UL
#define KEYPAD_HOLD_DURATION        3000UL
#define KEYPAD_BUFFER_SIZE          13

typedef enum {T_KP_NoPress,T_KP_DataPress,T_KP_ErasePattern,T_KP_PasswordChange,T_KP_INVALID}T_KeyPadState_t;

enuErrorStatus_t T_Keypad_GetStatus(T_KeyPadState_t *KPState);
enuErrorStatus_t T_Keypad_GetData(uint8_t *KPData);
void Keypad_Task(void);


#endif /* T_KEYPAD_H_ */