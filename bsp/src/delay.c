#include "delay.h"

void delay_us(uint32_t count)
{
   uint16_t num = count&0xffff;
   TIM4->CNT =num ; //设置计数值 
   TIM_Cmd(TIM4, ENABLE);   
   while(num>1)
   {
       num =TIM4->CNT;   //  TIM_GetCounter(TIM4);
   }
   TIM4->CR1 |= TIM_CR1_CEN;//   TIM_Cmd(TIM4,DISABLE);
}

void delay_ms(uint16_t count)
{
   uint16_t num = (count*1000)&0xffff;
   TIM4->CNT =num ; //设置计数值 
   TIM_Cmd(TIM4, ENABLE);   
   while(num>1)
   {
       num =TIM4->CNT;   //  TIM_GetCounter(TIM4);
   }
   TIM4->CR1 |= TIM_CR1_CEN;//   TIM_Cmd(TIM4,DISABLE);
}

 void Delay_init()
 {
    TIM_TimeBaseInitTypeDef timInitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		    //定时器3 时钟使能
    timInitStruct.TIM_ClockDivision = 0;   // 定时器基准频率72MHz
    timInitStruct.TIM_Prescaler = 72;                    // 计数频率为1MHz
    timInitStruct.TIM_CounterMode = TIM_CounterMode_Down; // 向下计数
    timInitStruct.TIM_Period = 1; // 这个值实际上就是TIMX->ARR，延时开始时重新设定即可
    TIM_TimeBaseInit(TIM4, &timInitStruct);
    TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE); // 计数溢出时触发中断
    TIM_Cmd(TIM4, DISABLE);
 }
 