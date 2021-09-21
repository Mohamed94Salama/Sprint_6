#include "CRC.h"


#define CRC_POLYNOMIAL										0b1101110011110010

static uint16_t CRC16_Calculation(uint8_t* pu8Data, uint8_t u8DataSize);


enuErrorStatus_t CRC16_Encode(uint8_t* pu8Data, uint8_t u8DataSize, uint16_t* pu16CRCData)
{
	enuErrorStatus_t enuErrorStatus;
	uint16_t u16Remainder = 0;

	if(pu8Data != NULLPTR)
	{
		u16Remainder = CRC16_Calculation(pu8Data, u8DataSize);
		*pu16CRCData = u16Remainder;
		enuErrorStatus = E_OK;
	}
	else
	{
		enuErrorStatus = E_ERROR;
	}
	return enuErrorStatus;
}

enuErrorStatus_t CRC16_Decode(uint8_t* pu8Data, uint8_t u8DataSize, enuErrorStatus_t* penuCRCCheckStatus)
{
	enuErrorStatus_t enuErrorStatus;
	uint16_t u16Remainder = 0;

	if(pu8Data != NULLPTR)
	{
		u8DataSize += 2;
		u16Remainder = CRC16_Calculation(pu8Data, u8DataSize);		
		if(u16Remainder == 0)
		{
			*penuCRCCheckStatus = E_OK;
		}
		else
		{
			*penuCRCCheckStatus = E_ERROR;
		}
		enuErrorStatus = E_OK;
	}
	else
	{
		enuErrorStatus = E_ERROR;
	}
	return enuErrorStatus;
}


static uint16_t CRC16_Calculation(uint8_t* pu8Data, uint8_t u8DataSize)
{
   uint16_t u16Remainder = 0;
   uint8_t u8ByteNo = 2;
   uint8_t u8MaskNo = 7;
   uint8_t u8Index;

	u16Remainder = (*(pu8Data + 0) << 8);
	if(u8DataSize >= 2)
	{
		u16Remainder |= *(pu8Data + 1);
	}
	else
	{
	}
	for((u8Index = (8 * u8DataSize)); u8Index > 0; u8Index--)
	{
		if((u16Remainder & (1 << 15)))
		{
			u16Remainder ^= CRC_POLYNOMIAL;
		}
		else
		{
		}
		u16Remainder = (u16Remainder << 1);
		if(u8ByteNo < u8DataSize)
		{
			u16Remainder |= ((*(pu8Data + u8ByteNo) & (1 << u8MaskNo)) >> u8MaskNo);
			if(u8MaskNo == 0)
			{
				u8ByteNo++;
				u8MaskNo = 7;
			}
			else
			{
				u8MaskNo--;
			}
		}
		else
		{
		}
	}
	return u16Remainder;
}
