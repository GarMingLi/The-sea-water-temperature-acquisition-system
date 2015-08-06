#include  "Check_water.h"

/******************************************
�������ƣ�GPIO_DQ_Input_Mode
��    �ܣ�����DQ����Ϊ��������ģʽ
��    ������
����ֵ  ����
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

//EXTI�����ⲿ�ж�
static void EXTI_InitStructReadtempCmd()
{
	EXTI_InitTypeDef EXTI_InitStruct                                      ;
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0)          ;
	EXTI_ClearITPendingBit(EXTI_Line0)                                    ;              //���EXTI_Line8 �жϹ���λ
	EXTI_InitStruct.EXTI_Line = EXTI_Line0                                ;		         //ѡ���ж���·
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt                       ;			     //����EXTIΪ�ж�������·
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising                    ;		         //����������·x����Ϊ�ж�����
	EXTI_InitStruct.EXTI_LineCmd = ENABLE                                 ;              //�ⲿ�ж�ʹ��
	EXTI_Init(&EXTI_InitStruct)                                           ;			     //��ʼ��EXTI�Ĵ���
}


//������Ӧ�жϣ�Ƕ������������ NVIC)
static void EXIT_NVIC_InitStructReadtempCmd()
{
	NVIC_InitTypeDef NVIC_InitStruct                                       ; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup)                           ;   //����EXTI�ж���
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn                           ;   //stm32f10x.h 		      //�����ж�ͨ��8
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = Preemption_EXIT    ;   //��ռ���ȼ�����Ϊ0
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = SubPri_EXIT               ;   //���ж����ȼ�����Ϊ0
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE                            ;   //ʹ���ж�
	NVIC_Init(&NVIC_InitStruct)                                            ;   //��ʼ��Ƕ������������NVIC�Ĵ���
	//ע������EXTI9_5_IRQn��EXTI5��EXTI9����һ���ж���Ӧ����
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
      if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)                          {    // ��黽�ѱ�־�Ƿ�����              
          PWR_ClearFlag(PWR_FLAG_WU)                                       ;    // ������ѱ�־ 
          //CPU_Init()                                                     ;
          SysTickInit()                                                    ;
          USART_ReConfig()                                                 ;        
                                                                           }                                                                           
      EXTI_ClearITPendingBit(EXTI_Line0)                                   ;	//����ж������־
                                                                           }
}

void Check_Water(void)
{
    EXIT_Init()                                                            ;
    PWR_WakeUpPinCmd(PWR_WakeUpPin_1, ENABLE)                              ;
	GPIO_DQ_Init()                                                         ;
}



