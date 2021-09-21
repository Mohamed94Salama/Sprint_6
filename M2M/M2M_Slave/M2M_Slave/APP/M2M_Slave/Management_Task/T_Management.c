#include "T_Management.h"


static strBTCFG_t strBTCFGMgmt;
static uint16_t u16EEPROMAddress = 0x10;
static uint16_t u16EEPRODeviceddress = 0xF0;
static uint8_t u8Index;
static uint8_t u8DataCount = 0;
static uint8_t au8EEPROMData[20];
static uint8_t au8BTData[20];
static uint8_t au8Password[20];
static enuBTStates_t   enuBTStates;
static uint8_t u8Flag=1;
static uint8_t au8PassData[3];


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
									enuErrorStatus = T_BT_SetState(T_BT_CONNECTED);
									if(enuErrorStatus == E_OK)
									{
										enuErrorStatus = T_DISP_LCDSetMode(T_LCD_LOADING);
										if(enuErrorStatus == E_OK)
										{
											
														while(enuBTStates == T_BT_FAIL)
														{
															enuErrorStatus = T_BT_SetState(T_BT_HANDSHAKE);
															enuErrorStatus = T_BT_GetState(&enuBTStates);
															if(enuBTStates == T_BT_Pass)
															{
																enuErrorStatus = T_LED_SetPattern(T_LED_ConnectionON);
																break;
															}
															else 
															{
															}
														}
														while(enuBTStates == T_BT_FAIL)
														{
															enuErrorStatus = T_BT_SetState(T_BT_DEVICEPACKET);
															enuErrorStatus = T_BT_GetState(&enuBTStates);
															if(enuBTStates == T_BT_Pass)
															{
																enuErrorStatus = T_BT_GetDataBuffer(au8EEPROMData,20);
																if(enuErrorStatus == E_OK)
																{
																	enuErrorStatus = EEPROM_WriteString(u16EEPRODeviceddress, 2, au8EEPROMData);
																	if(enuErrorStatus == E_OK)
																	{
																		enuErrorStatus = T_DISP_LCDSetMode(T_LCD_WELCOME);
																	}
																}
																break;
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
			enuErrorStatus = T_BT_GetState(&enuBTStates);	
			if((enuErrorStatus == E_OK) && (enuBTStates == T_BT_KEYPADDATA))	
			{
				for(u8Index=0; u8Index<3; u8Index++)
				{
				u8DataCount++;
				enuErrorStatus = T_BT_GetDataBuffer(au8BTData,1);
				if(enuErrorStatus == E_OK)
				{
					if(*au8BTData>='1' && *au8BTData<='9')
					{
						au8Password[u8DataCount]= *au8BTData;
						if(u8DataCount<6)
						{
							enuErrorStatus = T_DISP_LCDSetMode(T_LCD_PASS_EN);
							if(enuErrorStatus == E_OK)
							{
								enuErrorStatus = T_DISP_SSDSetNum(au8BTData[u8DataCount]);
							}
						}
						else if(u8DataCount==6)
						{
							u8DataCount=0;
						}
					}
					else if(*au8BTData=='11')
					{
						au8Password[u8DataCount]='0';
					}
					else if(*au8BTData=='10')
					{
						if(u8DataCount>0)
						{
							u8DataCount--;
							//enuErrorStatus = T_DISP_LCDSetMode(T_LCD_PASS_EN);
							//display the element delete message
						}
					}
					else if(*au8BTData=='12')
					{
						if(u8DataCount<6)
						{
							u8DataCount=0;
						}
						else if(u8DataCount==6)
						{
							au8Password[u8DataCount]='\0'; //end of passData
							enuErrorStatus = EEPROM_ReadString(u16EEPRODeviceddress, 2, au8EEPROMData);
							if(enuErrorStatus == E_OK)
							{
								enuErrorStatus = String_compare(au8EEPROMData,au8BTData,20);
								if(enuErrorStatus == E_OK)
								{
									enuErrorStatus = T_DISP_LCDSetMode(T_LCD_PASS_OK);
									u8Flag=0;
									break;
								}
								else
								{
									u8Flag=1;
									enuErrorStatus = T_DISP_LCDSetMode(T_LCD_PASS_NOK);
								}
							}
						}
						
					}
				}
				if(u8Flag==1 && enuErrorStatus==E_OK)
				{
					enuErrorStatus = T_DISP_LCDSetMode(T_LCD_WAITING);
					vTaskDelay(10000);
					u8Index=0;
				}
				}
				else
				{
					enuErrorStatus = T_DISP_LCDSetMode(T_LCD_DATA_EN);
					
					
				}
			}
			else if((enuErrorStatus == E_OK) && (enuBTStates == T_BT_PASSDATA))
			{
				enuErrorStatus = T_DISP_LCDSetMode(T_LCD_PASS_CHANGE);
				if(enuErrorStatus == E_OK)
				{
					enuErrorStatus = T_BT_SetState(T_BT_KEYPADDATA);
				}
			}
		}
		else
		{
		}
	}
}