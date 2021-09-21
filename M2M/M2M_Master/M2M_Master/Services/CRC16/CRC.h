#ifndef CRC_H_
#define CRC_H_

#include "STD_TYPES.h"

enuErrorStatus_t CRC16_Encode(uint8_t* pu8Data, uint8_t u8DataSize, uint16_t* pu16CRCData);
enuErrorStatus_t CRC16_Decode(uint8_t* pu8Data, uint8_t u8DataSize, enuErrorStatus_t* penuCRCCheckStatus);

#endif /* CRC_H_ */