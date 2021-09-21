/*
 * Misc.c
 *
 * Created: 9/11/2021 3:35:04 AM
 *  Author: Amrth
 */ 

#include "Misc.h"

/************************************************************************************
* Parameters (in): uint8_t *str1,uint8_t *str2,uint16_t u16size
* Parameters (out): None
* Return value: E_ERROR  or E_OK
* Description: A function to compare two strings
************************************************************************************/
enuErrorStatus_t String_compare(uint8_t *str1,uint8_t *str2,uint16_t u16size)
{
   enuErrorStatus_t enuErrorStatus=E_OK;
   uint16_t u16i;
   //Traverse both strings
   for(u16i=0 ; u16i < u16size ; u16i++)
   {
      // look for any difference
      if(str1[u16i] < str2[u16i] || str1[u16i] > str2[u16i])
      {
         enuErrorStatus=E_ERROR;
         break;
      }
   }
   return enuErrorStatus;
}


/************************************************************************************
* Parameters (in): uint8_t *str, uint16_t buf_size
* Parameters (out): uint16_t * result
* Return value: E_ERROR  or E_OK
* Description: A function to get the size of a string
************************************************************************************/
enuErrorStatus_t String_length(uint8_t *str,uint16_t * result)
{
   enuErrorStatus_t ErrorStatus=E_OK;
   if (str == NULLPTR || result == NULLPTR)
   {
      ErrorStatus=E_ERROR;
   }
   else
   {
      uint16_t u16i;
      for(u16i=0 ;(str[u16i]); u16i++);
      *result = u16i;
      ErrorStatus=E_OK;
   }
   return ErrorStatus;
}

/************************************************************************************
* Parameters (in): uint8_t *source, uint8_t *Destination,uint16_t count
* Parameters (out): None
* Return value: E_ERROR  or E_OK
* Description: A function to copy a string into another
************************************************************************************/
enuErrorStatus_t String_copy(uint8_t *source, uint8_t *Destination,uint16_t count)
{
   enuErrorStatus_t ErrorStatus=E_OK;
   if (source == NULLPTR || Destination == NULLPTR || count ==0)
   {
      ErrorStatus=E_ERROR;
   }
   else
   {
      uint16_t u16i;
      for(u16i=0 ;(source[u16i] && (u16i < count)); u16i++)
      {
         Destination[u16i]=source[u16i];
      }
      Destination[u16i]=0;
   }
   return ErrorStatus;
}



/************************************************************************************
* Parameters (in): uint8_t num
* Parameters (out): uint8_t *str
* Return value: E_ERROR  or E_OK
* Description: A function to create a string out of an integer
************************************************************************************/
enuErrorStatus_t num2str(uint8_t num,uint8_t *str)
{
   enuErrorStatus_t Error=E_OK;
   if (str!=NULLPTR)
   {
      uint8_t temp=num,count=0;
      while(temp>0)
      {
         count++;
         temp/=10;
      }
      uint8_t i;

      for(i=1; i<=count; i++)
      {
         str[count-i] = (uint8_t) ((num % 10UL) + '0');
         num/=10;
      }
      str[i-1] = '\0';
   }
   else
   {
      Error=E_ERROR;
   }
   return Error;
}
