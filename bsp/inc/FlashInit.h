#include "stm32f10x_flash.h"
#include "stm32f10x_rcc.h"
extern u8 Flash_Write(u32 Add,u8 num,u8* data);
extern void Flash_Read(u32 Add,u8 num,u8* data);


//  #define Data_Add 0x0807F800   //high_density 512K     ZE
#define Data_Add1 0x0801FC00   //mid_density  128K     C8
//#define Data_Add 0x08007C00   //low_density  32K
//extern u32 Data_Addr[];












