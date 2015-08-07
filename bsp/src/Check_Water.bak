#include  "Check_water.h"

/******************************************
函数名称：GPIO_DQ_Input_Mode
功    能：设置DQ引脚为上拉输入模式
参    数：无
返回值  ：无
*******************************************/
static void GPIO_DQ_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure                                   ;
	RCC_AHBPeriphClockCmd(EXIT_PCLOCK, ENABLE)                            ;
    GPIO_InitStructure.GPIO_Pin = EXIT_Pin | EXIT_Pin_                    ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN                           ;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL                       ;
    GPIO_Init(EXIT_PType, &GPIO_InitStructure)                            ;  
}

uint8_t Get_Status(pfunc delay, uint16_t ms)
{
    if(GPIO_ReadInputDataBit(EXIT_PType, EXIT_Pin_) == Power_Down)	      {
	    (*delay)(ms)                                                      ;
		if(GPIO_ReadInputDataBit(EXIT_PType, EXIT_Pin_) == Power_Down)    {
		    return  Power_Down                                            ;
		                                                                  }
	                                                                      }
	return  Power_Up                                                      ;
}



static void EXIT_RCC_ABP2PeriphReadtempyCmd()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE)                 ;
}

//EXTI配置外部中断
static void EXTI_InitStructReadtempCmd()
{
	EXTI_InitTypeDef EXTI_InitStruct                                      ;
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0)          ;
	EXTI_ClearITPendingBit(EXTI_Line0)                                    ;              //清除EXTI_Line8 中断挂起位
	EXTI_InitStruct.EXTI_Line = EXTI_Line0                                ;		         //选择中断线路
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt                       ;			     //配置EXTI为中断请求线路
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising                    ;		         //配置输入线路x升沿为中断请求
	EXTI_InitStruct.EXTI_LineCmd = ENABLE                                 ;              //外部中断使能
	EXTI_Init(&EXTI_InitStruct)                                           ;			     //初始化EXTI寄存器
}


//配置相应中断（嵌套向量控制器 NVIC)
static void EXIT_NVIC_InitStructReadtempCmd()
{
	NVIC_InitTypeDef NVIC_InitStruct                                       ; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup)                           ;   //配置EXTI中断组
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn                           ;   //stm32f10x.h 		      //配置中断通道8
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = Preemption_EXIT    ;   //先占优先级设置为0
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = SubPri_EXIT               ;   //从中断优先级设置为0
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE                            ;   //使能中断
	NVIC_Init(&NVIC_InitStruct)                                            ;   //初始化嵌套向量控制器NVIC寄存器
	//注意这里EXTI9_5_IRQn是EXTI5到EXTI9共用一个中断响应函数
}									   
									   

static void EXIT_Init()
{
	EXIT_RCC_ABP2PeriphReadtempyCmd()                                      ;
	EXTI_InitStructReadtempCmd()                                           ;
	EXIT_NVIC_InitStructReadtempCmd()                                      ;
}

void EXTI0_IRQHandler(void)
{ 	
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)				                   {		
      if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)                          {    // 检查唤醒标志是否设置              
          PWR_ClearFlag(PWR_FLAG_WU)                                       ;    // 清除唤醒标志 
          //CPU_Init()                                                     ;
          SysTickInit()                                                    ;
          USART_ReConfig()                                                 ;        
                                                                           }                                                                           
      EXTI_ClearITPendingBit(EXTI_Line0)                                   ;	//清除中断请求标志
                                                                           }
}

void Check_Water(void)
{
    EXIT_Init()                                                            ;
    PWR_WakeUpPinCmd(PWR_WakeUpPin_1, ENABLE)                              ;
	GPIO_DQ_Init()                                                         ;
}



