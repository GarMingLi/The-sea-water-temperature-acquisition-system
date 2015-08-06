#ifndef _APP_H_
#define _APP_H_

#include "static_init.h"
#include "bsp.h"
#include "app_cfg.h"
#include  <os.h>



void KernelMain(void);
static void TaskStart(void);
static void Task0(void *p_arg);
static void Task1(void *p_arg);
static void Task2(void *p_arg);
static void Task3(void *p_arg);

void Check_Water_DelayMs(uint8_t);
uint16_t Read_Temp_Filter(void);
uint16_t Read_WP_Filter(void);
void TimingDelay_Decrement(void);


#endif
