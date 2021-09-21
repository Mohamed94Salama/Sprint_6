/*
 * LED.h
 *
 * Created: 9/9/2021 5:22:27 PM
 *  Author: Amrth
 */ 


#ifndef LED_H_
#define LED_H_

#include "STD_TYPES.h"
#include "BIT_MANIPULATE.h"
#include "DIO.h"
#include "PWM.h"


typedef enum{LED_OFF,LED_ON,LED_TRANS}enuLEDState_t;

typedef struct{
   strPWMConfigType_t*  LED_PWM_CH;  
   uint16_t             LED_Volt;
   }strLEDCfg_t;


enuErrorStatus_t LED_Init(strLEDCfg_t *LED_ID);
enuErrorStatus_t LED_SwitchON(strLEDCfg_t *LED_ID,uint16_t u16TransDuration_ms);
enuErrorStatus_t LED_SwitchOFF(strLEDCfg_t *LED_ID,uint16_t u16TransDuration_ms);


#endif /* LED_H_ */