/*
 * T_Display.c
 *
 * Created: 9/13/2021 2:01:10 AM
 *  Author: Amrth
 */ 


#include "T_Display.h"


/*Include OS header file*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#define LCD_MOVEMENT_TIME_STEP            50u
#define SSD_UPDATE_STEP                   10u

#define LCD_CLR_MSG            ((uint8_t *)"                ")
#define LCD_LOADING_MSG        ((uint8_t *)"System loading")
#define LCD_WELCOME_MSG        ((uint8_t *)"Welcome")
#define LCD_PASS_EN_MSG        ((uint8_t *)"Enter Pass")
#define LCD_PASS_OK_MSG        ((uint8_t *)"Pass OK")
#define LCD_PASS_NOK_MSG       ((uint8_t *)"Wrong Pass")
#define LCD_WAITING_MSG        ((uint8_t *)"Waiting")
#define LCD_DATA_EN_MSG        ((uint8_t *)"System is ON")
#define LCD_PASS_CHANGE_MSG    ((uint8_t *)"Changing Pass")

#define LCD_WIDTH                      16
#define LCD_LOADING_MSG_LENGTH         14
#define LCD_WELCOME_MSG_LENGTH         7
#define LCD_PASS_EN_MSG_LENGTH         10
#define LCD_PASS_OK_MSG_LENGTH         7
#define LCD_PASS_NOK_MSG_LENGTH        10
#define LCD_WAITING_MSG_LENGTH         7
#define LCD_DATA_EN_MSG_LENGTH         12
#define LCD_PASS_CHANGE_MSG_LENGTH     13

#define LCD_PASSWORD_MAX_DIGIT         6

#define SSD_MAX_NUM                    99


 uint8_t *MsgArr[9]={LCD_CLR_MSG, 
                          LCD_LOADING_MSG, 
                          LCD_WELCOME_MSG, 
                          LCD_PASS_EN_MSG, 
                          LCD_PASS_OK_MSG, 
                          LCD_PASS_NOK_MSG, 
                          LCD_WAITING_MSG, 
                          LCD_DATA_EN_MSG, 
                          LCD_PASS_CHANGE_MSG};
 
 const uint8_t MsgLenArr[9]={LCD_WIDTH,                                                         
                             LCD_LOADING_MSG_LENGTH,           
                             LCD_WELCOME_MSG_LENGTH,           
                             LCD_PASS_EN_MSG_LENGTH,           
                             LCD_PASS_OK_MSG_LENGTH,           
                             LCD_PASS_NOK_MSG_LENGTH,          
                             LCD_WAITING_MSG_LENGTH,           
                             LCD_DATA_EN_MSG_LENGTH,           
                             LCD_PASS_CHANGE_MSG_LENGTH};       
                                    
                                    
                                         
static enuDispMsg_t genuDisplayMessage=T_LCD_CLR;
static uint8_t gu8Password_Count=0;

static uint8_t gu8SSDNum=0;



enuErrorStatus_t T_DISP_LCDSetMode(enuDispMsg_t Message)
{
   if (Message < T_LCD_INVALID)
   {
      genuDisplayMessage=Message;
      return E_OK;
   }
   return E_ERROR;
}

enuErrorStatus_t T_DISP_LCDAddPassword(void)
{
   if ((genuDisplayMessage == T_LCD_PASS_EN) && (gu8Password_Count <=LCD_PASSWORD_MAX_DIGIT))
   {
      gu8Password_Count++;
      return E_OK;
   }
   return E_ERROR;
}
enuErrorStatus_t T_DISP_LCDDelPassword(void)
{

   if ((genuDisplayMessage == T_LCD_PASS_EN) && (gu8Password_Count > 0))
   {
      gu8Password_Count--;
      return E_OK;
   }
   return E_ERROR;

}

enuErrorStatus_t T_DISP_SSDSetNum(uint8_t num)
{
   if (num>=0 && num<=SSD_MAX_NUM)
   {
      gu8SSDNum=num;
      return E_OK;
   }
   return E_ERROR;
}



void Display_Task (void)
{
   LCD_Init(LCD_DATA_FOUR_BIT_MODE,LCD_CURSOR_OFF);
   SevenSegment_Init();
   enuDispMsg_t LocalDislaypMsg=genuDisplayMessage;
   static uint8_t localPasswordCount=0;
   uint8_t IncDecFlag=1;
   uint8_t CursorPosition=0;
   static uint8_t LCD_UpdateFlag=0;
   while(1)
   {
      if (LCD_UpdateFlag == DIPLAY_LCD_UPDATE_STEP/DIPLAY_SSD_UPDATE_STEP)
      {
         LCD_UpdateFlag=0;
         if (LocalDislaypMsg!=genuDisplayMessage)
         {
            LocalDislaypMsg=genuDisplayMessage;
            if (CursorPosition+MsgLenArr[LocalDislaypMsg]>15)
            {
               CursorPosition=LCD_WIDTH - MsgLenArr[LocalDislaypMsg];
               IncDecFlag=0;
            }
            
         }
         
         LCD_SetCursorPosition(0,0);
         LCD_DisplayString(MsgArr[T_LCD_CLR]);
         if (CursorPosition == 0)
         {
            IncDecFlag=1;
         }
         else if (CursorPosition+MsgLenArr[LocalDislaypMsg]==16)
         {
            IncDecFlag=0;
         }
         
         if (IncDecFlag)
         {
            CursorPosition++;
         }
         else
         {
            CursorPosition--;
         }
         LCD_SetCursorPosition(CursorPosition,0);
         LCD_DisplayString(MsgArr[LocalDislaypMsg]);
         
         if (LocalDislaypMsg == T_LCD_PASS_EN && localPasswordCount != gu8Password_Count)
         {
            if (localPasswordCount<gu8Password_Count)
            {
               LCD_SetCursorPosition(localPasswordCount,1);
               LCD_DisplayChar('*');
            }
            else
            {
               LCD_SetCursorPosition(gu8Password_Count,1);
               LCD_DisplayChar(' ');
            }
            localPasswordCount = gu8Password_Count;
         }
      }
      
      SevenSegment_WriteNum(gu8SSDNum);
      
      vTaskDelay(DIPLAY_SSD_UPDATE_STEP);
   }
}

