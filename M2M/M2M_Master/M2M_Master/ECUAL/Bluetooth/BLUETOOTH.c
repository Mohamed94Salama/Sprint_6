/*
 * BLUETOOTH.c
 *
 * Created: 9/5/2021 11:33:19 PM
 *  Author: Amrth
 */ 


#include "BLUETOOTH.h"

static uint8_t gau8RxBuffer[30];
static uint8_t gau8RXDataBuffer[30];
static uint16_t gu16RXDataSize=0;
static uint8_t  flag=0;
const static uint32_t gau32Baudrates[9]={9600,19200,38400,57600,115200,230400,460800,921600,1382400};
// enuErrorStatus_t  BT_SendCMD(uint8_t * AT_CMD, uint8_t response_Size);
//static enuErrorStatus_t  BT_GetStatus(void);
//static uint8_t gu8ActionFlag=LOW;
static uint32_t current_Baudrate=0;
static enuErrorStatus_t BT_Initialized= E_ERROR;




/*=============================================================================*/


enuErrorStatus_t  BT_SendCMD(uint8_t * AT_CMD,uint8_t response_size)
{
	enuErrorStatus_t enuErrorStatus = E_ERROR;

	if(AT_CMD != NULLPTR)
	{
		enuErrorStatus = UART_SendString(AT_CMD);

		if(enuErrorStatus == E_OK)
		{
			enuErrorStatus = UART_ReceiveString(gau8RxBuffer,response_size);
			if(enuErrorStatus == E_OK)
			{
				if((String_compare(gau8RxBuffer,(uint8_t *)"OK",2)==E_OK) || (gau8RxBuffer[0]=='+'))
				{
					enuErrorStatus = E_OK;
				}
				else
				{
					enuErrorStatus = E_ERROR;
				}
			}
		}
	}
	else
	{
		
	}

	return enuErrorStatus;
}



enuErrorStatus_t  BT_Test(void)
{
   enuErrorStatus_t BT_ERROR=E_ERROR;
   enuBTBaudRate_t enuBaudrate=BT_BR_9600;
	DIO_PinInit(BT_EN, OUTPUT, NO_CONNECTION);
	DIO_PinWrite(BT_EN, HIGH);
   while (BT_ERROR == E_ERROR && enuBaudrate<INVALID)
   {
      UART_Init(gau32Baudrates[enuBaudrate]);
      BT_ERROR = BT_SendCMD((uint8_t *)"AT\r\n",2);
      if(BT_ERROR==E_ERROR)
	  {
		  enuBaudrate++;
	  }
   }
   if (BT_ERROR)
   {
      current_Baudrate = gau32Baudrates[enuBaudrate];
      UART_ReceiveString(gau8RxBuffer,30);
   }
	return BT_ERROR;
}


enuErrorStatus_t  BT_Init(void)
{
	enuErrorStatus_t enuErrorStatus = E_ERROR;
	DIO_PinInit(BT_EN, OUTPUT, NO_CONNECTION);
	DIO_PinWrite(BT_EN, HIGH);
	if (current_Baudrate != gau32Baudrates[BT_BR_9600])
	{
		enuErrorStatus = BT_SendCMD((uint8_t *)"AT+UART=9600,0,0\r\n",30);
		if (enuErrorStatus==E_OK)
		{
			UART_Init(gau32Baudrates[BT_BR_38400]);
		}
	}
	if(enuErrorStatus == E_OK)
	{
//			enuErrorStatus = BT_SendCMD((uint8_t *)"AT\r\n",30);
		//if AT sent and replied with OK, and if not initialized before
//			if(enuErrorStatus == E_OK && BT_Initialized==E_ERROR)
//			{
			enuErrorStatus = BT_SendCMD((uint8_t *)"AT+INIT\r\n",30);
			BT_Initialized=enuErrorStatus;
//			}
	}

	return enuErrorStatus;
}


enuErrorStatus_t  BT_SetName(strBTCFG_t *BT_ID)
{
   enuErrorStatus_t enuErrorStatus = E_ERROR;
   uint8_t Name_Buffer[30]="AT+NAME=";
   uint16_t Name_size=0;
   String_length(BT_ID->NAME,&Name_size);
   if (Name_size<=(30-(8+2)))
   {
      String_copy(BT_ID->NAME,Name_Buffer+8,Name_size);
      String_copy((uint8_t *)"\r\n",Name_Buffer+8+Name_size,2);
      enuErrorStatus = BT_SendCMD(Name_Buffer,30);
   }
   else
   {
      enuErrorStatus = E_ERROR;
   }
   return enuErrorStatus;
}

enuErrorStatus_t  BT_GetName(strBTCFG_t *BT_ID)
{
   enuErrorStatus_t enuErrorStatus = E_ERROR;
   uint16_t Name_size=0;
   enuErrorStatus = BT_SendCMD((uint8_t *)"AT+NAME?\r\n",30);
   if (enuErrorStatus == E_OK)
   {
      enuErrorStatus = String_compare(gau8RxBuffer,(uint8_t *)"+NAME:",6);
      if(enuErrorStatus == E_OK)
      {
         String_length(gau8RxBuffer+6,&Name_size);
         String_copy(gau8RxBuffer+6,BT_ID->NAME,Name_size);

         enuErrorStatus= UART_ReceiveString(gau8RxBuffer,30);
      }
      enuErrorStatus = E_OK;
   }
   else
   {
      enuErrorStatus = E_ERROR;
   }
   return enuErrorStatus;
}


enuErrorStatus_t  BT_SetPassword(strBTCFG_t *BT_ID)
{
   enuErrorStatus_t enuErrorStatus = E_ERROR;
   uint8_t Pass_Buffer[20]="AT+PSWD=";
   uint16_t Pass_size=0;
   String_length(BT_ID->PASSWORD,&Pass_size);
   if (Pass_size<=(20-(8+2)))
   {
      String_copy(BT_ID->PASSWORD,Pass_Buffer+8,Pass_size);
      String_copy((uint8_t *)"\r\n",Pass_Buffer+8+Pass_size,2);
      enuErrorStatus = BT_SendCMD(Pass_Buffer,30);
   }
   else
   {
      enuErrorStatus = E_ERROR;
   }
   return enuErrorStatus;
}

enuErrorStatus_t  BT_GetPassword(strBTCFG_t *BT_ID)
{
   enuErrorStatus_t enuErrorStatus = E_ERROR;
   uint16_t Pass_size=0;
   enuErrorStatus = BT_SendCMD((uint8_t *)"AT+PSWD?\r\n",30);
   if (enuErrorStatus == E_OK)
   {
      enuErrorStatus = String_compare(gau8RxBuffer,(uint8_t *)"+PSWD:",6);
      if(enuErrorStatus == E_OK)
      {
         String_length(gau8RxBuffer+6,&Pass_size);
         String_copy(gau8RxBuffer+6,BT_ID->PASSWORD,Pass_size);

         enuErrorStatus= UART_ReceiveString(gau8RxBuffer,30); //TO RECEIVE OK

      }
      enuErrorStatus = E_OK;
   }
   else
   {
      enuErrorStatus = E_ERROR;
   }
   return enuErrorStatus;
}


enuErrorStatus_t  BT_SetMode(strBTCFG_t *BT_ID)
{
   enuErrorStatus_t enuErrorStatus = E_ERROR;
   uint8_t Mode_Buffer[20]="AT+ROLE= \r\n";
   Mode_Buffer[8]=BT_ID->MODE + '0';
   enuErrorStatus = BT_SendCMD(Mode_Buffer,30);
   return enuErrorStatus;
}

enuErrorStatus_t  BT_GetMode(strBTCFG_t *BT_ID)
{
   enuErrorStatus_t enuErrorStatus = E_ERROR;
   enuErrorStatus = BT_SendCMD((uint8_t *)"AT+ROLE?\r\n",30);
   if (enuErrorStatus == E_OK)
   {
      enuErrorStatus = String_compare(gau8RxBuffer,(uint8_t *)"+ROLE:",6);
      if(enuErrorStatus == E_OK)
      {
         BT_ID->MODE= gau8RxBuffer[6] - '0';
      }
      enuErrorStatus= UART_ReceiveString(gau8RxBuffer,30);
   }
   else
   {
      enuErrorStatus = E_ERROR;
   }
   return enuErrorStatus;
}

enuErrorStatus_t  BT_SetCredentials(strBTCFG_t *BT_ID)
{
   enuErrorStatus_t E_ErrorState=E_ERROR;
   
   E_ErrorState=BT_SetName(BT_ID);
   if (E_ErrorState==E_OK)
   {
      E_ErrorState=BT_SetPassword(BT_ID);
      if (E_ErrorState ==E_OK)
      {
         E_ErrorState=BT_SetMode(BT_ID);
      }
   }
   return E_ErrorState;
}

enuErrorStatus_t  BT_GetCredentials(strBTCFG_t *BT_ID)
{
   enuErrorStatus_t E_ErrorState=E_ERROR;
   
   E_ErrorState=BT_GetName(BT_ID);
   if (E_ErrorState==E_OK)
   {
      E_ErrorState=BT_GetPassword(BT_ID);
      if (E_ErrorState ==E_OK)
      {
         E_ErrorState=BT_GetMode(BT_ID);
      }
   }
   return E_ErrorState;
}




enuErrorStatus_t  BT_Search(strBTCFG_t *BT_ID,uint8_t * MAC_ADD)
{
   enuErrorStatus_t enuErrorStatus=E_ERROR;
   if (BT_ID!= NULLPTR && MAC_ADD!=NULLPTR)
   {
      if (BT_Initialized == E_OK)
      {
         enuErrorStatus=BT_SendCMD((uint8_t *)"AT+INQ\r\n",30);
         if (enuErrorStatus==E_OK)
         {
            enuErrorStatus=String_compare(gau8RxBuffer,(uint8_t *)"+INQ:",5);
            if (enuErrorStatus==E_OK)
            {
               String_copy(gau8RxBuffer+5,MAC_ADD,14);
               MAC_ADD[4]=',';
               MAC_ADD[7]=',';
               
               enuErrorStatus= UART_ReceiveString(gau8RxBuffer,30);
       
            }
         }
      }
   }
   return enuErrorStatus; 
}


enuErrorStatus_t  BT_Connect(uint8_t * MAC_ADD,uint8_t u8Delay_s)
{
   enuErrorStatus_t enuErrorStatus=E_ERROR;
   if (BT_Initialized==E_OK )
   {
      uint8_t Link_Buffer[30]="AT+LINK=98D3,11,FC5F64\r\n";
      enuErrorStatus = BT_SendCMD(Link_Buffer,30);
//      uint8_t Delay_Buffer[5]={0};
      uint16_t MAC_size=14;
//      String_length(MAC_ADD,&MAC_size);
//      num2str(u8Delay_s,Delay_Buffer);
//      String_length(Delay_Buffer,&Delay_size);
      if (MAC_size<=(30-(8+2)))
      {
//         String_copy(MAC_ADD,Link_Buffer+8,MAC_size);
//         String_copy((uint8_t *)"\r\n",Link_Buffer+8+MAC_size,2);
//         Link_Buffer[10+MAC_size]='\0';
//         LCD_displayStringRowColumn(0, 0, Link_Buffer);
//         LCD_displayStringRowColumn(1, 0, Link_Buffer+11);
//         BT_SendCMD((uint8_t*)"AT+CLASS=1\r\n", 30);
//         enuErrorStatus = BT_SendCMD(Link_Buffer,30);
      }
      else
      {
         enuErrorStatus = E_ERROR;
      }
   }
   return enuErrorStatus;
}

enuErrorStatus_t  BT_Disconnect(void)
{
   /*configure EN pin*/
   enuErrorStatus_t enuErrorStatus=BT_SendCMD((uint8_t *)"AT+DISC\r\n",2);
   if(enuErrorStatus==E_OK)
   {
      enuErrorStatus=BT_SendCMD((uint8_t *)"AT+RMAAD\r\n",30);
   }
   /*configure EN pin*/
   return enuErrorStatus;
}

enuErrorStatus_t  BT_Reset(void)
{
   if (/*link state*/1)
   {
      /*configure EN pin*/
   }
   enuErrorStatus_t enuErrorStatus=BT_SendCMD((uint8_t *)"AT+RESET\r\n",30);
   return enuErrorStatus;
}


enuErrorStatus_t  BT_SendString(uint8_t *str)
{
   enuErrorStatus_t enuErrorStatus=E_OK;
	DIO_PinWrite(BT_EN, LOW);
   if (str!= NULL && BT_Initialized==E_OK)
   {
      /*check EN pin*/
      enuErrorStatus=UART_SendString(str);
   }
	DIO_PinWrite(BT_EN, HIGH);
   return enuErrorStatus;
}

void BT_RXCallBack(void)
{
	static volatile uint16_t i=0;
	
	UART_RecieveDataNoBLOCK((uint16_t*)gau8RXDataBuffer+i);
	if(gau8RXDataBuffer[i]!='\r' || gau8RXDataBuffer[i]!='\n')
	{
		i++;
		gu16RXDataSize++;
	}
	else
	{
		flag=1;
		i=0;
	}
}

enuErrorStatus_t  BT_ReceiveString(uint8_t *str,uint8_t Max_Size)
{
   enuErrorStatus_t enuErrorStatus=E_OK;
	DIO_PinWrite(BT_EN, LOW);
   if (str!= NULL && BT_Initialized==E_OK)
   {
      /*check EN pin*/
      //enuErrorStatus=UART_ReceiveString(str,Max_Size);
	  enuErrorStatus=UART_RX_Enable_Interrupt();
	  if(enuErrorStatus==E_OK)
	  {
		enuErrorStatus=UART_RX_SetCallBack(BT_RXCallBack);
	  }
	  if(flag==1)
	  {
		  String_copy(gau8RXDataBuffer,str,gu16RXDataSize);
	  }
   }
   return enuErrorStatus;
}


