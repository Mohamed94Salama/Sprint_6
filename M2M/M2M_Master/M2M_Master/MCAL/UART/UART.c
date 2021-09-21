/*
 * UART.c
 *
 * Created: 7/14/2021 7:30:59 PM
 *  Author: Mohamed Ehab
 */ 

#include "UART.h"
/*Error handling*/
enuErrorStatus_t gError=E_ERROR;

void (*RXC_Callback)(void)=NULLPTR;
void (*TXC_Callback)(void)=NULLPTR;

/************************************************************
* Parameters (in) : None
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling 
* Description     : UART initialization
*************************************************************/
enuErrorStatus_t UART_Init(uint32_t Baudrate)
{
   uint8_t UCSRC_Copy=0;
	/*Set the Baud rate*/
	uint16_t UBRRVAL=((F_CPU/(16L*Baudrate))-1);
	UBRRH_R=(uint8_t)(UBRRVAL>>8);
	UBRRL_R=(uint8_t)(UBRRVAL);
	
	/*UART Transmitter and Receiver Enable*/
	UCSRB_R=(1<<TXEN_B)|(1<<RXEN_B);
	
	/* DATA BITS*/
	uint8_t data=UART_DATA_BITS;
	if(data==5)
	{
		CLEAR_BIT(UCSRC_Copy,UCSZ1_B);
		CLEAR_BIT(UCSRC_Copy,UCSZ0_B);
	}else if(data==6)
	{
		CLEAR_BIT(UCSRC_Copy,UCSZ1_B);
		SET_BIT(UCSRC_Copy,UCSZ0_B);		
	}else if(data==7)
	{
		SET_BIT(UCSRC_Copy,UCSZ1_B);
		CLEAR_BIT(UCSRC_Copy,UCSZ0_B);		
	}else if(data==8)
	{
		SET_BIT(UCSRC_Copy,UCSZ1_B);
		SET_BIT(UCSRC_Copy,UCSZ0_B);
	}else if (data==9)
	{
		SET_BIT(UCSRB_R,UCSZ2_B);
		SET_BIT(UCSRC_Copy,UCSZ1_B);
		SET_BIT(UCSRC_Copy,UCSZ0_B);		
	}
	/*PARITY*/
	uint8_t parity=UART_PARITY_BIT;
	if(parity==0)
	{
		CLEAR_BIT(UCSRC_Copy,UPM1_B);
		CLEAR_BIT(UCSRC_Copy,UPM0_B);
	}else if(parity==1)
	{
		SET_BIT(UCSRC_Copy,UPM1_B);
		SET_BIT(UCSRC_Copy,UPM0_B);	
	}else if(parity==2)
	{
		SET_BIT(UCSRC_Copy,UPM1_B);
		CLEAR_BIT(UCSRC_Copy,UPM0_B);
	}
	/*STOP BITS*/
	uint8_t stop = UART_STOP_BITS;
	if(stop==1)
	{
		CLEAR_BIT(UCSRC_Copy,USBS_B);
	}else if(stop==2)
	{
		SET_BIT(UCSRC_Copy,USBS_B);
	}
   
   //enable access to UCSRC register & store the buffer value into UCSRC buffer to achieve atomic access
   UCSRC_R = (UCSRC_Copy | (1<<URSEL_B));
   //enable access to UDDRH register
   CLEAR_BIT(UCSRC_R,URSEL_B);
   
   
	gError=E_OK;
	return gError;
}
/************************************************************
* Parameters (in) : u16Data
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling 
* Description     : Sending a data with UART
*************************************************************/
enuErrorStatus_t UART_SendData(uint16_t u16Data)
{
	/* Return an E_ERROR if UART not initialized*/
	if(gError==E_ERROR)
	{
		return E_ERROR;
	}
	gError=E_OK;
	/*Wait for the empty flag to be set*/
	while(!GET_BIT(UCSRA_R,UDRE_B));
	UDR_R=u16Data;
	return gError;
}
/************************************************************
* Parameters (in) : None
* Parameters (out): pu16Data
* Return Value    : enuErrorStatus_t - For error handling 
* Description     : Receiving a data with UART
*************************************************************/
enuErrorStatus_t UART_ReceiveData(uint16_t* pu16Data)
{
	if(gError==E_ERROR)
	{
		return E_ERROR;
	}
	gError=E_OK;
	/* Wait for the Receive complete flag to be set*/
	while(!GET_BIT(UCSRA_R,RXC_B));
	*pu16Data=UDR_R;
	return gError;
}
/************************************************************
* Parameters (in) : None
* Parameters (out): pu16Data
* Return Value    : enuErrorStatus_t - For error handling 
* Description     : Receiving a data with UART without polling
*************************************************************/
enuErrorStatus_t UART_RecieveDataNoBLOCK(uint16_t* pu16Data)
{
	if(gError==E_ERROR)
	{
		return E_ERROR;
	}
	gError=E_OK;
	if(GET_BIT(UCSRA_R,RXC_B))
	{
		*pu16Data=UDR_R;
	}
	return gError;
}
/************************************************************
* Parameters (in) : *pu8String
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling 
* Description     : Sending a string with UART
*************************************************************/
enuErrorStatus_t UART_SendString(uint8_t* pu8String)
{
	if(gError==E_ERROR)
	{
		return E_ERROR;
	}
	gError=E_OK;
	uint8_t i=0;
	for(i=0;i<MAX_LEN;i++)
	{
      if(pu8String[i]==0)
      {
         break;
      }
		UART_SendData(pu8String[i]);
	}
	return gError;
}
/************************************************************
* Parameters (in) : None
* Parameters (out): pu8String
* Return Value    : enuErrorStatus_t - For error handling 
* Description     : Sending a string with UART
*************************************************************/
enuErrorStatus_t UART_ReceiveString(uint8_t* pu8String,uint8_t max_length)
{
	if(gError==E_ERROR)
	{
		return E_ERROR;
	}
	gError=E_OK;
	uint8_t i=0;
	for(i=0;i<max_length;i++)
	{
		UART_ReceiveData((uint16_t *)&pu8String[i]);
		if(pu8String[i]=='\r')
		{
			pu8String[i]=0;
		}
      else if (pu8String[i]=='\n')
      {
         pu8String[i]=0;
         break;
      }
	}

	return gError;
}
/************************************************************
* Parameters (in) : u16Data
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling
* Description     : Sending a data with UART without polling
*************************************************************/
enuErrorStatus_t UART_SendDataNoBLOCK(uint16_t u16Data)
{
	if(gError==E_ERROR)
	{
		return E_ERROR;
	}
	gError=E_OK;
	if(GET_BIT(UCSRA_R,UDRE_B))
	{
		UDR_R=(uint8_t)u16Data;
	}
	return gError;
}


/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to enable uart interrupt for TX complete
************************************************************************************/
enuErrorStatus_t UART_TX_Enable_Interrupt(void)
{
	//set appropriate bit to enable interrupt
	SET_BIT(UCSRB_R,TXCIE_B);
	return E_OK;
}

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to disable uart interrupt for TX complete
************************************************************************************/
enuErrorStatus_t UART_TX_Disable_Interrupt(void)
{
	//clear appropriate bit to disable interrupt
	CLEAR_BIT(UCSRB_R,TXCIE_B);
	return E_OK;
}

/************************************************************************************
* Parameters (in): void(*local_fptr)(void)
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to enable setup callback funtion for TX complete interrupt
************************************************************************************/
enuErrorStatus_t UART_TX_SetCallBack(void(*local_fptr)(void))
{
	//check if the sent pointer points to a valid position
	if (local_fptr== NULLPTR)     return E_ERROR;
	//if so, store the pointer address in the global pointer to function
	TXC_Callback=local_fptr;
	return E_OK;
}

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to enable uart interrupt for RX complete
************************************************************************************/
enuErrorStatus_t UART_RX_Enable_Interrupt(void)
{
	//set appropriate bit to enable interrupt
	SET_BIT(UCSRB_R,RXCIE_B);
	return E_OK;
}

/************************************************************************************
* Parameters (in): void
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to disable uart interrupt for RX complete
************************************************************************************/
enuErrorStatus_t UART_RX_Disable_Interrupt(void)
{
	//clear appropriate bit to disable interrupt
	CLEAR_BIT(UCSRB_R,RXCIE_B);
	return E_OK;
}

/************************************************************************************
* Parameters (in): void(*local_fptr)(void)
* Parameters (out): enuErrorStatus_t
* Return value: 1=E_OK or 0=FAIL
* Description: A function to enable setup callback funtion for RX complete interrupt
************************************************************************************/
enuErrorStatus_t UART_RX_SetCallBack(void(*local_fptr)(void))
{
	//check if the sent pointer points to a valid position
	if (local_fptr== NULLPTR)     return E_ERROR;
	//if so, store the pointer address in the global pointer to function
	RXC_Callback=local_fptr;
	return E_OK;
}




/* ISRs----------------------------------------------------------*/

ISR(UART_RX_vect)
{
	//if the global pointer to function points to a valid position
	if (RXC_Callback!=NULLPTR)
	{
		//call the function
		RXC_Callback();
	}
}
ISR(UART_TX_vect)
{
	//if the global pointer to function points to a valid position
	if (TXC_Callback!=NULLPTR)
	{
		//call the function
		TXC_Callback();
	}
}
