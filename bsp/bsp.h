#ifndef _bsp_h_
#define _bsp_h_

#include  <stdio.h>
#include  <stdarg.h>

#include  <os.h>

#include  <cpu.h>
#include  <cpu_core.h>

#include  <lib_def.h>
#include  <lib_ascii.h>

#include  <stm32l1xx_gpio.h>
#include  <stm32l1xx_rcc.h>
#include  <stm32l1xx_exti.h>
#include  <misc.h>

#include  "stm32l1xx_syscfg.h"

#include  "bsp_cfg.h"
#include  "delay.h"
#include  "ds18b20.h"
#include  "W_P_Sensor.h"
#include  "RTC.h"
#include  "cat24c516.h"
#include  "NRF24L01.h"
#include  "Check_water.h"
#include  "power.h"

 #define OS_TICKS_PER_SEC 500		//滴答频率(ucos系统频率)，100分之1秒，10ms
/********************************/
/*       系统配置函数           */
/********************************/
#ifdef USART_Debug
extern void USART1_SendString(uint8_t* Data,uint32_t Len);		   //多字符输出
#endif
void USART_ReConfig(void);
void SysTickInit(void);
void BspInit(void);



#endif

