/*
 * T_Keypad.c
 *
 * Created: 9/10/2021 9:34:29 PM
 *  Author: Amrth
 */ 

#include "T_Keypad.h"
#include "Misc.h"

/*Include OS header file*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

//extern xSemaphoreHandle bsCheck;

volatile static T_KeyPadState_t gKPStatus=T_KP_NoPress;
static uint8_t gKPDataBuffer=0;
enuErrorStatus_t T_Keypad_GetStatus(T_KeyPadState_t *KPState)
{
   if(KPState == NULLPTR)
   {
      return E_ERROR;
   }
   *KPState=gKPStatus;
   return E_OK;
}


enuErrorStatus_t T_Keypad_GetData(uint8_t *KPData)
{
   if(KPData == NULLPTR || gKPStatus != T_KP_DataPress)
   {
      return E_ERROR;
   }
   *KPData=gKPDataBuffer;
   gKPStatus=T_KP_NoPress;
   return E_OK;
}


void Keypad_Task(void)
{
   Keypad_Init(KEYPAD_4X3);
   uint8_t KeypadBuffer[KEYPAD_BUFFER_SIZE]={0};
   uint8_t KeypadHistory[KEYPAD_BUFFER_SIZE]={0};
   uint8_t PatternPressedFlag=LOW;
   uint8_t ButtonPressedFlag=LOW;
   enuErrorStatus_t CheckFlag=E_ERROR;
   enuErrorStatus_t HoldDurationFlag=0;
   while (1)
   {
      CheckFlag = Keypad_GetChar(KeypadBuffer,13);
      
      if (CheckFlag && KeypadBuffer[0]==1)
      {
         if (String_compare(KeypadBuffer,KeypadHistory,KEYPAD_BUFFER_SIZE))
         {
            ButtonPressedFlag=HIGH;            
         }
         else
         {
            ButtonPressedFlag=LOW;
            HoldDurationFlag=LOW;
            String_copy(KeypadBuffer,KeypadHistory,KEYPAD_BUFFER_SIZE);
         }
      }
 
      else if (CheckFlag && KeypadBuffer[0]==3)
      {
         if (String_compare(KeypadBuffer,KeypadHistory,KEYPAD_BUFFER_SIZE))
         {
             PatternPressedFlag=HIGH;                
         }
         else
         {
            ButtonPressedFlag=LOW;
            PatternPressedFlag=LOW;
            HoldDurationFlag=LOW;
            String_copy(KeypadBuffer,KeypadHistory,KEYPAD_BUFFER_SIZE);
         }
      }
      else
      {
         gKPStatus=T_KP_NoPress;
         ButtonPressedFlag=LOW;
         PatternPressedFlag=LOW;
         HoldDurationFlag=0;
      }
      
      if (ButtonPressedFlag==HIGH)
      {
         if (!HoldDurationFlag)
         {
            gKPStatus=T_KP_DataPress;
            gKPDataBuffer=KeypadBuffer[1];
            HoldDurationFlag=HIGH;
           // xSemaphoreGive(bsCheck);
         }
      }
      else if (PatternPressedFlag==HIGH)
      {
         if (String_compare(KeypadBuffer+1,(uint8_t *)"1A6",3))
         {
            if (HoldDurationFlag<(KEYPAD_HOLD_DURATION/KEYPAD_UPDATE_DURATION))
            {
               HoldDurationFlag++; 
            }
            else
            {
               gKPStatus=T_KP_PasswordChange;
              // xSemaphoreGive(bsCheck);
            }
         }
         else if (String_compare(KeypadBuffer+1,(uint8_t *)"16C",3))
         {
            if (HoldDurationFlag<(KEYPAD_HOLD_DURATION/KEYPAD_UPDATE_DURATION))
            {
               HoldDurationFlag++;
            }
            else
            {
               gKPStatus=T_KP_ErasePattern;
              // xSemaphoreGive(bsCheck);
            }
         }
         else
         {
            gKPStatus=T_KP_NoPress;
         }
      }
      vTaskDelay(KEYPAD_UPDATE_DURATION);
   }
}


 