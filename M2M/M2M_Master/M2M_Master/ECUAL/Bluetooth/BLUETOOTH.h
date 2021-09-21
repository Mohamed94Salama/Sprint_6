/*
 * BLUETOOTH.h
 *
 * Created: 9/5/2021 11:33:00 PM
 *  Author: Amrth
 */ 


#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "UART.h"
#include "DIO.h"
#include "Misc.h"

#define BT_EN												0

typedef enum{BT_BR_9600,BT_BR_19200,BT_BR_38400,BT_BR_57600,BT_BR_115200,BT_BR_230400,BT_BR_460800,BT_BR_921600,BT_BR_1382400,INVALID}enuBTBaudRate_t;
typedef enum{BT_SLAVE,BT_MASTER,BT_SLAVE_LOOP}enuBTMode_t;
    
   
typedef struct {
	enuBTBaudRate_t enuBTBaudRate;
   enuBTMode_t MODE;
   uint8_t * NAME;
   uint8_t * PASSWORD;
} strBTCFG_t;

extern const strBTCFG_t strBTCFG;
	
enuErrorStatus_t  BT_SendCMD(uint8_t * AT_CMD, uint8_t response_Size);
enuErrorStatus_t  BT_Test(void);   
enuErrorStatus_t  BT_Init(void);
enuErrorStatus_t  BT_SetName(strBTCFG_t *BT_ID);
enuErrorStatus_t  BT_GetName(strBTCFG_t *BT_ID);
enuErrorStatus_t  BT_SetPassword(strBTCFG_t *BT_ID);
enuErrorStatus_t  BT_GetPassword(strBTCFG_t *BT_ID);
enuErrorStatus_t  BT_SetMode(strBTCFG_t *BT_ID);
enuErrorStatus_t  BT_GetMode(strBTCFG_t *BT_ID);
enuErrorStatus_t  BT_SetCredentials(strBTCFG_t *BT_ID);
enuErrorStatus_t  BT_GetCredentials(strBTCFG_t *BT_ID);
enuErrorStatus_t  BT_Search(strBTCFG_t *BT_ID,uint8_t * MAC_ADD);
enuErrorStatus_t  BT_Connect(uint8_t * MAC_ADD,uint8_t u8Delay_s);
enuErrorStatus_t  BT_Disconnect(void);
enuErrorStatus_t  BT_Reset(void);
enuErrorStatus_t  BT_SendString(uint8_t *str);
enuErrorStatus_t  BT_ReceiveString(uint8_t *str,uint8_t Max_Size);



#endif /* BLUETOOTH_H_ */
