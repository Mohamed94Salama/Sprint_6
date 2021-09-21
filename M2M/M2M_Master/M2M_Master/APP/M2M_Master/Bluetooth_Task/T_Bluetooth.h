/*
 * T_Bluetooth.h
 *
 * Created: 9/14/2021 12:23:42 AM
 *  Author: Amrth
 */ 


#ifndef T_BLUETOOTH_H_
#define T_BLUETOOTH_H_

#include "BLUETOOTH.h"
#include "T_Keypad.h"
#include "Misc.h"
#include "CRC.h"

/*Include OS header file*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#define BT_BUFFER_SIZE             100UL

typedef enum {T_BT_INIT,T_BT_CONFIGREAD,T_BT_CONFIGSET,T_BT_ADVERTISING,
              T_BT_CONNECT,T_BT_HANDSHAKE,T_BT_DISCONNECT,T_BT_DEVICEPACKET,T_BT_PASSDATA,
			  T_BT_KEYPADDATA,T_BT_IDLE,T_BT_Pass,T_BT_FAIL,T_BT_INVALID}enuBTStates_t;


enuErrorStatus_t T_BT_SetState(enuBTStates_t State);
enuErrorStatus_t T_BT_GetState(enuBTStates_t *State);
enuErrorStatus_t T_BT_SetConfig(const strBTCFG_t *strBTCFG);
enuErrorStatus_t T_BT_GetConfig(strBTCFG_t *strBTCFG);
enuErrorStatus_t T_BT_GetMACAddress(uint8_t* pu8MACAddress);
enuErrorStatus_t T_BT_UpdateDataBuffer(uint8_t *au8Data,uint8_t Size);
enuErrorStatus_t T_BT_GetDataBuffer(uint8_t *au8Data,uint8_t MaxSize);

void Bluetooth_Master_Task (void);




#endif /* T_BLUETOOTH_H_ */