#include "delay.h"

void delay_us(uint32_t count)
{
   uint16_t num = count&0xffff;
   TIM4->CNT =num ; //���ü���ֵ 
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
   TIM4->CNT =num ; //���ü���ֵ 
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		    //��ʱ��3 ʱ��ʹ��
    timInitStruct.TIM_ClockDivision = 0;   // ��ʱ����׼Ƶ��72MHz
    timInitStruct.TIM_Prescaler = 72;                    // ����Ƶ��Ϊ1MHz
    timInitStruct.TIM_CounterMode = TIM_CounterMode_Down; // ���¼���
    timInitStruct.TIM_Period = 1; // ���ֵʵ���Ͼ���TIMX->ARR����ʱ��ʼʱ�����趨����
    TIM_TimeBaseInit(TIM4, &timInitStruct);
    TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE); // �������ʱ�����ж�
    TIM_Cmd(TIM4, DISABLE);
 }
 