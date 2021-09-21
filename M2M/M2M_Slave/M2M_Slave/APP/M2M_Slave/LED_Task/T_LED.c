/*
 * T_LED.c
 *
 * Created: 9/10/2021 4:36:53 PM
 *  Author: Amrth
 */ 
#include "T_LED.h"

/*Include OS header file*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

static const uint16_t LEDPatternTPeriod[5]={0,3000,1500,1200,300};

static enuLEDPattern_t genuCurrentPattern=LED_OFF;

enuErrorStatus_t T_LED_SetPattern(enuLEDPattern_t Pattern)
{
   if (Pattern>=T_LED_OFF && Pattern<T_LED_INVALID)
   {
      genuCurrentPattern=Pattern;
      return E_OK;
   }
   return E_ERROR;
}

enuErrorStatus_t T_LED_GetPattern(enuLEDPattern_t *Pattern)
{
   if (Pattern != NULLPTR)
   {
      *Pattern = genuCurrentPattern;
      return E_OK;
   }
   return E_ERROR;
}

void LED_Task(void)
{
   strLEDCfg_t x={astrPWMConfigParameters,0};
   enuLEDPattern_t LEDState= T_LED_OFF;
   LED_Init(&x);
   LED_SwitchOFF(&x,500);
   while (1)
   {
      LEDState=genuCurrentPattern;
      switch(genuCurrentPattern)
      {
         case T_LED_OFF:
            LED_SwitchOFF(&x,1);
            break;
         case T_LED_MalFunction:
         case T_LED_DeviceSearch:
         case T_LED_VerificationSearch:
         case T_LED_ConnectionON:
            vTaskDelay(LEDPatternTPeriod[LEDState]/2);
            LED_SwitchON(&x,LEDPatternTPeriod[LEDState]/6);
            vTaskDelay(LEDPatternTPeriod[LEDState]/6);
            LED_SwitchOFF(&x,LEDPatternTPeriod[LEDState]/6);
            break;
         default:
            break;
      }
   }
}

