#ifndef __CAT24C516_H
#define __CAT24C516_H

#include "bsp.h"

//A1 A2 A3 Ϊ 0
#define    Address_Write   0xa0
#define    Address_Read    0xa1
#define   IIC_Pin_CLOCK  IIC_PCLOCK

#define   SDA_Out()      {                                                  \
                          GPIO_InitTypeDef GPIO_InitStructure ;             \
                          GPIO_InitStructure.GPIO_Pin = IIC_SDA_Pin;        \
                          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     \
                          GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;    \
                          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; \
                          GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      \
                          GPIO_Init(IIC_SDA_PType ,&GPIO_InitStructure);    \
                         }
                         
#define   SDA_In()       {                                                  \
                          GPIO_InitTypeDef GPIO_InitStructure ;             \
                          GPIO_InitStructure.GPIO_Pin = IIC_SDA_Pin;        \
                          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;      \
                          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; \
                          GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      \
                          GPIO_Init(IIC_SDA_PType ,&GPIO_InitStructure);    \
                         }
                         
#define   SDA_H()        GPIO_SetBits(IIC_SDA_PType, IIC_SDA_Pin)        
#define   SDA_L()        GPIO_ResetBits(IIC_SDA_PType, IIC_SDA_Pin)  
#define   SCL_H()        GPIO_SetBits(IIC_SCL_PType, IIC_SCL_Pin)  
#define   SCL_L()        GPIO_ResetBits(IIC_SCL_PType, IIC_SCL_Pin) 
#define   WP_H()         GPIO_SetBits(IIC_WP_PType, IIC_WP_Pin)  
#define   WP_L()         GPIO_ResetBits(IIC_WP_PType, IIC_WP_Pin)   

extern void IIC_Init(void);
extern void CAT24C_Word_W(uint16_t word_adds, uint8_t * words, uint16_t count);
extern void CAT24C_Page_W(uint16_t word_adds, uint8_t * words, uint8_t count);
extern void CAT24C_Selective_R(uint16_t word_adds, uint8_t * words, uint8_t count);
extern void CAT24C_Sequential_R(uint16_t word_adds, uint8_t * words, uint8_t count);
extern void CAT24C_Mask(void);

#endif

