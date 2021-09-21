/*
 * Misc.h
 *
 * Created: 9/11/2021 3:35:18 AM
 *  Author: Amrth
 */ 


#ifndef MISC_H_
#define MISC_H_

#include "STD_TYPES.h"
#include "BIT_MANIPULATE.h"

/************************************************************************************
* Parameters (in): uint8_t *str1,uint8_t *str2,uint16_t u16size
* Parameters (out): None
* Return value: E_ERROR  or E_OK
* Description: A function to compare two strings
************************************************************************************/
enuErrorStatus_t String_compare(uint8_t *str1,uint8_t *str2,uint16_t u16size);


/************************************************************************************
* Parameters (in): uint8_t *str, uint16_t buf_size
* Parameters (out): uint16_t * result
* Return value: E_ERROR  or E_OK
* Description: A function to get the size of a string
************************************************************************************/
enuErrorStatus_t String_length(uint8_t *str,uint16_t * result);

/************************************************************************************
* Parameters (in): uint8_t *source, uint8_t *Destination,uint16_t count
* Parameters (out): None
* Return value: E_ERROR  or E_OK
* Description: A function to copy a string into another
************************************************************************************/
enuErrorStatus_t String_copy(uint8_t *source, uint8_t *Destination,uint16_t count);



/************************************************************************************
* Parameters (in): uint8_t num
* Parameters (out): uint8_t *str
* Return value: E_ERROR  or E_OK
* Description: A function to create a string out of an integer
************************************************************************************/
enuErrorStatus_t num2str(uint8_t num,uint8_t *str);



#endif /* MISC_H_ */