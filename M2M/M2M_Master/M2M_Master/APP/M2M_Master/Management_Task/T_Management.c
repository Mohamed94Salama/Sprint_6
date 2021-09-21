#include "T_Management.h"


static strBTCFG_t strBTCFGMgmt;
static uint16_t u16EEPROMAddress = 0x10;
static uint16_t u16EEPRODeviceddress = 0xF0;
static uint8_t u8Index;
static uint8_t u8MACAddressCount = 0;
static uint8_t au8EEPROMData[20];
static uint8_t au8BTMAC[20];
static T_KeyPadState_t T_KeyPadState;
static enuBTStates_t   enuBTStates;
static uint8_t u8Flag=1;
static uint8_t au8PassData[3];
static uint8_t au8Data[30];

void Management_Task(void)
{
	enuErrorStatus_t enuErrorStatus;
	
	enuErrorStatus = T_BT_SetState(T_BT_INIT);
	if(enuErrorStatus == E_ERROR)
	{
		enuErrorStatus = T_LED_SetPattern(T_LED_MalFunction);
	}
	else
	{
		enuErrorStatus = T_BT_SetState(T_BT_CONFIGREAD);
		if(enuErrorStatus == E_OK)
		{
			enuErrorStatus = T_BT_GetConfig(&strBTCFGMgmt);
			if(enuErrorStatus == E_OK)
			{
				if(!((strBTCFGMgmt.MODE == strBTCFG.MODE) && (strBTCFGMgmt.NAME == strBTCFG.NAME) && (strBTCFGMgmt.PASSWORD == strBTCFG.PASSWORD)))
				{
					enuErrorStatus = T_BT_SetConfig(&strBTCFG);
					if(enuErrorStatus == E_OK)
					{
						enuErrorStatus = T_LED_SetPattern(T_LED_DeviceSearch);
						if(enuErrorStatus == E_OK)
						{
							for(u8Index = 0; u8Index < u8MACAddressCount; u8Index++)
							{
								enuErrorStatus = EEPROM_ReadString((u16EEPROMAddress + (u8Index * 0x10)), 2, au8EEPROMData);		
								if(enuErrorStatus == E_OK)
								{
									enuErrorStatus = T_BT_SetState(T_BT_ADVERTISING);
									if(enuErrorStatus == E_OK)
									{
										enuErrorStatus = T_BT_GetMACAddress(au8BTMAC);
										if(enuErrorStatus == E_OK)
										{
											enuErrorStatus = String_compare(au8EEPROMData, au8BTMAC, 2);
											if(enuErrorStatus == E_OK)
											{
												enuErrorStatus = T_BT_SetState(T_BT_CONNECT);
												if(enuErrorStatus == E_OK)
												{
													enuErrorStatus = T_LED_SetPattern(T_LED_VerificationSearch);
													if(enuErrorStatus == E_OK)
													{
														for(uint8_t i=0; i<5; i++)
														{
															enuErrorStatus = T_BT_SetState(T_BT_HANDSHAKE);
															vTaskDelay(200);
															enuErrorStatus = T_BT_GetState(&enuBTStates);
															if(enuBTStates == T_BT_Pass)
															{
																u8Flag = 0;
																break;
															}
															else if(enuBTStates == T_BT_FAIL)
															{
																u8Flag = 1;
															}
														}
														if(u8Flag == 1)
														{
															enuErrorStatus = T_BT_SetState(T_BT_DISCONNECT);
															if(enuErrorStatus == E_OK)
															{
																enuErrorStatus = T_LED_SetPattern(T_LED_DeviceSearch);
															}
															else
															{
															}
														}
														else
														{
															enuErrorStatus = T_LED_SetPattern(T_LED_ConnectionON);
															if(enuErrorStatus == E_OK)
															{
																for(uint8_t i=0; i<5; i++)
																{
																	enuErrorStatus = T_BT_SetState(T_BT_DEVICEPACKET);
																	vTaskDelay(200);
																	enuErrorStatus = T_BT_GetState(&enuBTStates);
																	if(enuBTStates == T_BT_Pass)
																	{
																		u8Flag = 0;
																		break;
																	}
																	else if(enuBTStates == T_BT_FAIL)
																	{
																		u8Flag = 1;
																	}
																}
																if(u8Flag == 1)
																{
																	enuErrorStatus = T_BT_SetState(T_BT_DISCONNECT);
																	if(enuErrorStatus == E_OK)
																	{
																		enuErrorStatus = T_LED_SetPattern(T_LED_DeviceSearch);
																	}
																	else
																	{
																	}
																}
																else
																{
																	enuErrorStatus = T_BT_GetDataBuffer(au8Data,22);
																	if(enuErrorStatus == E_OK)
																	{
																		enuErrorStatus = EEPROM_WriteString(u16EEPRODeviceddress, 22, au8Data);
																	}
																	else
																	{
																	}
																}
															}
														}
													}
													else
													{
													}
												}
												else
												{
												}
											}
											else
											{
											}
										}
										else
										{
										}
									}
									else
									{
									}
									break;
								}			
								else
								{
								}		
							}
						}
						else
						{
						}
					}
					else
					{
					}
				}
				else
				{
				}
			}
			else
			{
			}
		}
		else
		{
		}		
	}

	while(1)
	{
		if(enuErrorStatus == E_OK)
		{
			enuErrorStatus = T_Keypad_GetStatus(&T_KeyPadState);	
			if((enuErrorStatus == E_OK) && (T_KeyPadState == T_KP_ErasePattern))	
			{
				enuErrorStatus = EEPROM_WriteString(u16EEPROMAddress, 2, au8BTMAC);
				if(enuErrorStatus == E_OK)
				{
					u8MACAddressCount++;
				}
				else
				{
				}
			}
			else if((enuErrorStatus == E_OK) && (T_KeyPadState == T_KP_PasswordChange))
			{
				enuErrorStatus = T_Keypad_GetData(au8PassData);
				if(enuErrorStatus == E_OK)
				{
					enuErrorStatus = T_BT_UpdateDataBuffer(au8PassData, 20);
					if(enuErrorStatus == E_OK)
					{
						enuErrorStatus = T_BT_SetState(T_BT_PASSDATA);
					}
					else
					{
					}
				}
				else
				{
				}
			}
			else if((enuErrorStatus == E_OK) && (T_KeyPadState == T_KP_DataPress))
			{
				enuErrorStatus = T_Keypad_GetData(au8PassData);
				if(enuErrorStatus == E_OK)
				{
					enuErrorStatus = T_BT_UpdateDataBuffer(au8PassData, 20);
					if(enuErrorStatus == E_OK)
					{
						enuErrorStatus = T_BT_SetState(T_BT_KEYPADDATA);
					}
					else
					{
					}
				}
				else
				{
				}
			}
		}
		else
		{
		}
	}
}