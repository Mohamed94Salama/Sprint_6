/*
 * LED.c
 *
 * Created: 9/9/2021 5:22:16 PM
 *  Author: Amrth
 */ 


#include "LED_Cfg.h"
#include "LED.h"
#include "FreeRTOS.h"
#include "task.h"



#define VOLT2DC(VOLT)               ((((uint32_t)VOLT)*(100U))/(LED_MAX_VOLTAGE))

enuErrorStatus_t LED_Init(strLEDCfg_t *LED_ID)
{
   enuErrorStatus_t ErrorState=E_ERROR;
   if (LED_ID != NULLPTR)
   {
		ErrorState= DIO_Init(&astrDIOConfigParameters[LED_PIN_ID]);
      if (ErrorState==E_OK)
      {
         ErrorState= PWM_Init(LED_ID->LED_PWM_CH);
         if (ErrorState==E_OK)
         {
            ErrorState = PWM_Stop(LED_PWM_ID);
         }
      }         
   }
   return ErrorState;
}


enuErrorStatus_t LED_SwitchON(strLEDCfg_t *LED_ID,uint16_t u16TransDuration_ms)
{
   enuErrorStatus_t ErrorState=E_ERROR;
   uint16_t VoltageChangeRate_mv=  LED_MAX_VOLTAGE /(u16TransDuration_ms/LED_STEP_TIME);
   if ((LED_ID != NULLPTR) && (LED_ID->LED_Volt == LED_MIN_VOLTAGE))
   {
      ErrorState = PWM_Stop(LED_PWM_ID);
      do
      {
         LED_ID->LED_Volt+=VoltageChangeRate_mv;
         ErrorState = PWM_Start(LED_PWM_ID,VOLT2DC(LED_ID->LED_Volt));
         vTaskDelay(LED_STEP_TIME);
      }while (LED_ID->LED_Volt<LED_MAX_VOLTAGE);
   }   
   return ErrorState;
}


enuErrorStatus_t LED_SwitchOFF(strLEDCfg_t *LED_ID,uint16_t u16TransDuration_ms)
{
   enuErrorStatus_t ErrorState=E_ERROR;
   uint16_t VoltageChangeRate_mv=  LED_MAX_VOLTAGE /(u16TransDuration_ms/LED_STEP_TIME);
   if ((LED_ID != NULLPTR) && (LED_ID->LED_Volt == LED_MAX_VOLTAGE))
   {
      ErrorState = PWM_Start(LED_PWM_ID,100);
      do
      {
         LED_ID->LED_Volt-=VoltageChangeRate_mv;
         PWM_Start(LED_PWM_ID,VOLT2DC(LED_ID->LED_Volt));
         vTaskDelay(LED_STEP_TIME);
      }while (LED_ID->LED_Volt>LED_MIN_VOLTAGE);
      //PWM_Stop(LED_PWM_ID);
      ErrorState=E_OK;
   }
   return ErrorState;
}