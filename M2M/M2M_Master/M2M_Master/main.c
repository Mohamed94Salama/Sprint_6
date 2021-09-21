/*
 * M2M_Master.c
 *
 * Created: 9/5/2021 8:12:59 PM
 * Author : Amrth
 */ 

#include "BLUETOOTH.h"
#include "LED.h"
#include "DIO.h"
#include "LCD.h"
#include "PWM.h"
#include "Keypad.h"
#include "T_LED.h"
#include "T_Keypad.h"

/*Include OS header file*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

void test(void);

TaskHandle_t th_LED_Task;
TaskHandle_t th_Keypad_Task;
TaskHandle_t th_Display_Task;
TaskHandle_t th_test;

int main(void)
{
    DIO_PinInit(DIO_PC3,OUTPUT,NO_CONNECTION);
    DIO_PinInit(DIO_PC4,OUTPUT,NO_CONNECTION);
    
    xTaskCreate(test       ,"Test"       ,100,NULL,  1, &th_test);
    //xTaskCreate(Keypad_Task,"Keypad_Task",100,NULL,  3, &th_Keypad_Task);
    //xTaskCreate(LED_Task   ,"T_LED_Task" ,100,NULL,  1, &th_LED_Task);
    
    
    vTaskStartScheduler();

     while (1) 
     {

     }
}

void test(void)
{
   uint8_t Pass_count=0;
   uint8_t incdec=1;
   //enuLEDPattern_t x= T_LED_OFF;

   while(1)
   {
      DIO_PinWrite(DIO_PC3,HIGH);
//       x++;
//       if (x==T_LCD_INVALID)
//       {
//          x=T_LCD_CLR;
//          
//       }
      
      if (Pass_count==6)
      {
         incdec=0;
      }
      else if (Pass_count==0)
      {
         incdec=1;
      }
      
      if (incdec==1)
      {
         Pass_count++;
      }
      else if (incdec==0)
      {
         Pass_count--;
      }
       
      
      DIO_PinWrite(DIO_PC3,LOW);
      
      vTaskDelay(1000);
      
      //vTaskDelay(100);
      /*
      
      vTaskDelay(5000);
      T_LED_SetPattern(T_LED_MalFunction);
      vTaskDelay(5000);
      T_LED_SetPattern(T_LED_DeviceSearch);
      vTaskDelay(5000);
      T_LED_SetPattern(T_LED_VerificationSearch);
      vTaskDelay(5000);
      T_LED_SetPattern(T_LED_ConnectionON);
      vTaskDelay(5000);*/
      
   }
}