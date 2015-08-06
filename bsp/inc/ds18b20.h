#ifndef __DS18B20_H
#define __DS18B20_H

#include "bsp.h"

typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
} TestStatus ;


//#define DS18B20_Pin *GPIO_Pin

//REGISTER COMMANDS
#define REGISTER_9_BITS  0x1F
#define REGISTER_10_BITS 0x3F
#define REGISTER_11_BITS 0x5F
#define REGISTER_12_BIT2 0x7F
//ROM COMMANDS
#define ROM_Search_Cmd   0xF0
#define ROM_Read_Cmd     0x33
#define ROM_Match_Cmd    0x55
#define ROM_Skip_Cmd     0xCC
#define ROM_AlarmSearch_Cmd 0xEC
//DS18b20 FUNCTION COMMANDS
#define Convert_T          0x44
#define Write_Scratchpad  0x4E
#define Read_Scratchpad   0xBE
#define Copy_Scratchpad   0x48
#define Recall_EEPROM     0x88
#define Read_PowerSupply  0x84
#define DQ_Write_1()     GPIO_SetBits(DS18B20_PType ,DS18B20_Pin)  //д1
#define DQ_Write_0()     GPIO_ResetBits(DS18B20_PType ,DS18B20_Pin)//д0
#define DQ_ReadBit()     GPIO_ReadInputDataBit(DS18B20_PType ,DS18B20_Pin) //��DQ�ϵ�ֵ

static void GPIO_DQ_Out_Mode(void) ;                                 //DQ���ģʽ
static void GPIO_DQ_Input_Mode(void) ;                               //DQ����ģʽ
static void Tx_ResetPulse(void) ;    //���͸�λ����
static void Rx_PresencePulse(void) ; //����Ӧ���ź�
static void Write_OneByte_ToDS18b20(uint8_t data) ; //дһ���ֽڵ�18b20
static unsigned char Read_OneByte_FromDS18b20(void) ;     //��18b20��һ���ֽ�
extern void Read_Temperature(uint8_t *sign ,uint16_t *temp) ; //���¶�
//д���ò���TH��TL�ͳ�ʼ�����üĴ���
extern void Write_EEPROM(uint8_t Th,uint8_t Tl,uint8_t Register_Con );
static void DS18B20_Init(void) ; //��ʼ��
#endif /*DS18B20*/
