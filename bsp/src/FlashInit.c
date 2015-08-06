#include "FlashInit.h"
 
u8 Flash_Write(u32 Add,u8 num,u8* data)	  //地址、第几区、数据
{
	u16 cnt=0;
	u32 Data[8];
	u8 temp=0;
	volatile u32 addr;
	addr = Add + num * 32;
	for(temp=0;temp<8;temp++){
	    Data[temp]=	(u32)(data[temp*4])+(u32)(data[temp*4+1]<<8)+(u32)(data[temp*4+2]<<16)+(u32)(data[temp*4+3]<<24);
	}
	FLASH_SetLatency(FLASH_Latency_2);
	RCC_HSICmd(ENABLE);
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	if(addr==Data_Add1)FLASH_ErasePage(Data_Add1);	
	do
	{
		FLASH_ProgramWord(addr+cnt*4,Data[cnt]);
		cnt++;
		if(FLASH_GetFlagStatus(FLASH_FLAG_PGERR)==SET)      //FLASH编写错误标志
		return 0;
	}while(cnt!=8);

	FLASH_Lock();
		return 1;
}

void Flash_Read(u32 Add ,u8 num , u8* data)
{
	u8 temp=0;
	volatile u32 addr;
	addr = Add + +num*32;
	for(temp=0;temp<32;temp++)
	data[temp]=*((int*)(addr  +temp));
}




