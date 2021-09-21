/*
 * T_Bluetooth.c
 *
 * Created: 9/14/2021 12:23:57 AM
 *  Author: Amrth
 */ 

#include "T_Bluetooth.h"


static strBTCFG_t gstrBTConfig;
static uint8_t garrBTDeviceMACAdd[20];
static enuBTStates_t genuBTMState=T_BT_INIT;
static uint8_t gu8BTDataBuffer[BT_BUFFER_SIZE];
static uint16_t gu16CRC;
static enuErrorStatus_t genuCRCCheckStatus;


enuErrorStatus_t T_BT_SetState(enuBTStates_t State)
{
   if (State>=T_BT_INVALID)
   {
      return E_ERROR;
   }
   genuBTMState = State;
   return E_OK;
}


enuErrorStatus_t T_BT_GetState(enuBTStates_t *State)
{
   if (State == NULLPTR)
   {
      return E_ERROR;
   }
   *State = genuBTMState;
   return E_OK;
}

enuErrorStatus_t T_BT_SetConfig(const strBTCFG_t *strBTCFG)
{
	if(strBTCFG == NULLPTR)
	{
		return E_ERROR;
	}
	gstrBTConfig = *strBTCFG;
	return E_OK;	
}

enuErrorStatus_t T_BT_GetConfig(strBTCFG_t *strBTCFG)
{
	if(strBTCFG == NULLPTR)
	{
		return E_ERROR;
	}
	*strBTCFG = gstrBTConfig;
	return E_OK;
}

enuErrorStatus_t T_BT_GetMACAddress(uint8_t* pu8MACAddress)
{
	if(pu8MACAddress != NULLPTR)
	{
		return E_ERROR;
	}
	pu8MACAddress = garrBTDeviceMACAdd;
	return E_OK;	
}

enuErrorStatus_t T_BT_UpdateDataBuffer(uint8_t *au8Data,uint8_t Size)
{
   if (au8Data == NULLPTR || Size == 0)
   {
      return E_ERROR;
   }
   else if (Size>BT_BUFFER_SIZE)
   {
      Size=BT_BUFFER_SIZE;
   }
   String_copy(au8Data,gu8BTDataBuffer,Size);
   return E_OK;
}

enuErrorStatus_t T_BT_GetDataBuffer(uint8_t *au8Data,uint8_t MaxSize)
{
   if (au8Data == NULLPTR || MaxSize == 0)
   {
      return E_ERROR;
   }
   else if (MaxSize > BT_BUFFER_SIZE)
   {
      MaxSize=BT_BUFFER_SIZE;
   }
   String_copy(gu8BTDataBuffer,au8Data,MaxSize);
   return E_OK;
}



void Bluetooth_Master_Task (void)
{
   enuBTStates_t Local_State=T_BT_INIT;
   enuErrorStatus_t ErrorState;
   while(1)
   {
      Local_State = genuBTMState;
      switch(Local_State)
      {
         case T_BT_INIT:
            ErrorState = BT_Test();
            if (ErrorState == E_OK)
            {
               ErrorState = BT_Init();
               if (ErrorState == E_OK)
               {
                  genuBTMState=T_BT_Pass;
               }
            }
            break;

         case T_BT_CONFIGREAD:
            ErrorState = BT_GetCredentials(&gstrBTConfig);
            if (ErrorState == E_OK)
            {
               genuBTMState=T_BT_Pass;
            }
            else
            {
               genuBTMState=T_BT_FAIL;
            }
            break;
         case T_BT_CONFIGSET:
            ErrorState = BT_SetCredentials(&gstrBTConfig);
            if (ErrorState == E_OK)     
            {
               genuBTMState=T_BT_Pass;
            }
            else
            {
               genuBTMState=T_BT_FAIL;
            }
            break;
         case T_BT_ADVERTISING:
            ErrorState = BT_Search(&gstrBTConfig,garrBTDeviceMACAdd);
            if (ErrorState == E_OK)
            {
               genuBTMState=T_BT_CONNECT;
            }
            else
            {
               genuBTMState=T_BT_FAIL;
            }
            break;
            
         case T_BT_CONNECT:
            ErrorState = BT_Connect(garrBTDeviceMACAdd, 20);
            if (ErrorState == E_OK)
            {
               genuBTMState=T_BT_CONNECT;
            }
            else
            {
               genuBTMState=T_BT_FAIL;
            }
            break;
            
         case T_BT_HANDSHAKE:
            gu8BTDataBuffer[0] =0xAA;
            gu8BTDataBuffer[1] =0x01;
            gu8BTDataBuffer[2] =0x03;
            gu8BTDataBuffer[3] =0x41;
            gu8BTDataBuffer[4] =0x45;
            gu8BTDataBuffer[5] =0x48;
            
				/*CRC */
				CRC16_Encode(gu8BTDataBuffer, 6, &gu16CRC);
            gu8BTDataBuffer[6] = (uint8_t)(gu16CRC >> 8);
				gu8BTDataBuffer[7] = (uint8_t)(gu16CRC);
				
            ErrorState =BT_SendString(gu8BTDataBuffer);
            if (ErrorState == E_OK)
            {
               ErrorState= E_ERROR;
               while(ErrorState == E_ERROR)
               {
                  ErrorState =BT_ReceiveString(gu8BTDataBuffer,BT_BUFFER_SIZE);
                  if (ErrorState == E_ERROR)
                  {
                     vTaskDelay(50);
                  }
                  else
                  {
							CRC16_Decode(gu8BTDataBuffer, 6, &genuCRCCheckStatus);
                     if (gu8BTDataBuffer[0] == 0xAA && gu8BTDataBuffer[1] == 0x01 && gu8BTDataBuffer[2] == 0x03 &&
                         gu8BTDataBuffer[3] == 0x41 && gu8BTDataBuffer[4] == 0x45 && gu8BTDataBuffer[5] == 0x48 &&
                         genuCRCCheckStatus == E_OK)
                     {
                        genuBTMState=T_BT_Pass;
                     }
                     else
                     {
                        genuBTMState=T_BT_FAIL;
                     }
                  }
               }
               
            }
            break;
			
         case T_BT_DISCONNECT:
		    ErrorState = BT_Disconnect();
            if (ErrorState == E_OK)
            {
               genuBTMState=T_BT_Pass;
            }
            else
            {
               genuBTMState=T_BT_FAIL;
            }
            break;
			
         case T_BT_DEVICEPACKET:
            gu8BTDataBuffer[0] =0xAA;
            gu8BTDataBuffer[1] =0x02;
            gu8BTDataBuffer[2] =0x14;
            String_copy(gstrBTConfig.NAME,gu8BTDataBuffer+3,7);
            String_copy(garrBTDeviceMACAdd,gu8BTDataBuffer+10,12);
				
            /*CRC */
				CRC16_Encode(gu8BTDataBuffer, 22, &gu16CRC);
				gu8BTDataBuffer[22] = (uint8_t)(gu16CRC >> 8);
				gu8BTDataBuffer[23] = (uint8_t)(gu16CRC);
           
            ErrorState =BT_SendString(gu8BTDataBuffer);
            if (ErrorState == E_OK)
            {
               ErrorState= E_ERROR;
               while(ErrorState == E_ERROR)
               {
                  ErrorState =BT_ReceiveString(gu8BTDataBuffer,BT_BUFFER_SIZE);
                  if (ErrorState == E_ERROR)
                  {
                     vTaskDelay(50);
                  }
                  else
                  {
							CRC16_Decode(gu8BTDataBuffer, 6, &genuCRCCheckStatus);                     
							if (gu8BTDataBuffer[0] == 0xAA && gu8BTDataBuffer[1] == 0x01 && gu8BTDataBuffer[2] == 0x03 &&
                     gu8BTDataBuffer[3] == 0x41 && gu8BTDataBuffer[4] == 0x45 && gu8BTDataBuffer[5] == 0x48 &&
                     genuCRCCheckStatus == E_OK)
                     {
                        genuBTMState=T_BT_Pass;
                     }
                     else
                     {
                        genuBTMState=T_BT_FAIL;
                     }
                  }
               }
               
            }
            break;
            
		case T_BT_PASSDATA:
		    gu8BTDataBuffer[0] =0xAA;
            gu8BTDataBuffer[1] =0x04;	//request type
            gu8BTDataBuffer[2] =0x00;	//request length
			
            /*CRC */
				CRC16_Encode(gu8BTDataBuffer, 3, &gu16CRC);
				gu8BTDataBuffer[3] = (uint8_t)(gu16CRC >> 8);
				gu8BTDataBuffer[4] = (uint8_t)(gu16CRC);
				            
            ErrorState =BT_SendString(gu8BTDataBuffer);
            if (ErrorState == E_OK)
            {
               ErrorState= E_ERROR;
               while(ErrorState == E_ERROR)
               {
                  ErrorState =BT_ReceiveString(gu8BTDataBuffer,BT_BUFFER_SIZE);
                  if (ErrorState == E_ERROR)
                  {
                     vTaskDelay(50);
                  }
                  else
                  {
							CRC16_Decode(gu8BTDataBuffer, 3, &genuCRCCheckStatus);
							if (gu8BTDataBuffer[0] == 0xAA && gu8BTDataBuffer[1] == 0x04 && gu8BTDataBuffer[2] == 0x00 && genuCRCCheckStatus == E_OK)
                     {
                        genuBTMState=T_BT_Pass;
                     }
                     else
                     {
                        genuBTMState=T_BT_FAIL;
                     }
                  }
               }
               
            }
            break;
         
		 case T_BT_KEYPADDATA:
		    gu8BTDataBuffer[0] =0xAA;
            gu8BTDataBuffer[1] =0x03;
            gu8BTDataBuffer[2] =0x01;
			T_Keypad_GetData(gu8BTDataBuffer+3);
			
            /*CRC */
				CRC16_Encode(gu8BTDataBuffer, 4, &gu16CRC);
				gu8BTDataBuffer[4] = (uint8_t)(gu16CRC >> 8);
				gu8BTDataBuffer[5] = (uint8_t)(gu16CRC);
				           
            ErrorState =BT_SendString(gu8BTDataBuffer);
            if (ErrorState == E_OK)
            {
               ErrorState= E_ERROR;
               while(ErrorState == E_ERROR)
               {
                  ErrorState =BT_ReceiveString(gu8BTDataBuffer,BT_BUFFER_SIZE);
                  if (ErrorState == E_ERROR)
                  {
                     vTaskDelay(50);
                  }
                  else
                  {
                     CRC16_Decode(gu8BTDataBuffer, 3, &genuCRCCheckStatus);
							if (gu8BTDataBuffer[0] == 0xAA && gu8BTDataBuffer[1] == 0x03 && gu8BTDataBuffer[2] == 0x01 &&
                     genuCRCCheckStatus == E_OK)
                     {
                        genuBTMState=T_BT_Pass;
                     }
                     else
                     {
                        genuBTMState=T_BT_FAIL;
                     }
                  }
               }
            }
            break;
			
		case T_BT_IDLE:
            vTaskDelay(100);
            break;
			
         case T_BT_Pass:
            vTaskDelay(100);
            break;
            
         case T_BT_FAIL:
            vTaskDelay(100);
            break;

         default:
            break;
      }
      
   }
}
