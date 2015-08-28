#include "bsp.h"



static CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;
    RCC_GetClocksFreq(&rcc_clocks);
    return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
}


void SysTickInit(void)
{
    CPU_INT32U  cpu_clk_freq                              ;
    CPU_INT32U  cnts                                      ;    
    cpu_clk_freq = BSP_CPU_ClkFreq()                      ;     /* Determine SysTick reference freq.       */
    cnts         = cpu_clk_freq                                 /* Determine nbr SysTick increments        */
                 / (CPU_INT32U)OSCfg_TickRate_Hz          ;  
    OS_CPU_SysTickInit(cnts)                              ;   
}

#ifdef Led_Debug
void Led_Init()
{
    GPIO_InitTypeDef  gpio_init                          ;   
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE)   ; 
    gpio_init.GPIO_Speed = GPIO_Speed_400KHz             ;   
    gpio_init.GPIO_Pin = Led1_Pin                        ;  
    gpio_init.GPIO_Mode = GPIO_Mode_OUT                  ;  
    gpio_init.GPIO_OType = GPIO_OType_PP                 ;       
    gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL               ;  
    GPIO_Init(Led1_PType, &gpio_init)                    ;  
}

#endif


#ifdef USART_Debug
void USART_Config()
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE)  ;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  
    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	       //USART1 TX    
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
    USART_InitStructure.USART_BaudRate = 115200;						//速率115200bps
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;		    //数据位8位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;			    //停止位1位
    USART_InitStructure.USART_Parity = USART_Parity_No;				    //无校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件流控
    USART_InitStructure.USART_Mode = USART_Mode_Tx;					//收发模式  
    
    USART_Init(USART1, &USART_InitStructure);							//配置串口参数函数
    USART_Cmd(USART1, ENABLE);	
}

void USART_ReConfig()
{
    USART_InitTypeDef USART_InitStructure;
 
    USART_InitStructure.USART_BaudRate = 115200;						//速率115200bps
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;		    //数据位8位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;			    //停止位1位
    USART_InitStructure.USART_Parity = USART_Parity_No;				    //无校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件流控
    USART_InitStructure.USART_Mode = USART_Mode_Tx;					//收发模式

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);   
    
    USART_Init(USART1, &USART_InitStructure);							//配置串口参数函数
    USART_Cmd(USART1, ENABLE);	
}

void USART1_SendByte(uint8_t Data)		   //单字符数据输出
{
	USART_SendData(USART1, Data);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/****************************************************************************
* 名	称：void USART1_SendString(u8* Data,u32 Len)
* 功	能：多字符输出
* 入口参数：Data 	输出的单字符数据
			Len		字符个数
* 出口参数：无
* 说	明：无					 
****************************************************************************/
void USART1_SendString(uint8_t* Data,uint32_t Len)		   //多字符输出
{
	uint32_t i=0;
    uint16_t j=0;
	for(i=0;i<Len;i++)
    {    
		USART_SendData(USART1, Data[i]);
        while((USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) && (++j));
	}
}

#endif



void BspInit(void)
{
	CPU_IntDis()                                         ;
    power_init()                                         ;
#ifdef Led_Debug    
    Led_Init()                                           ;
#endif
#ifdef USART_Debug    
    USART_Config()                                       ;		
#endif    
 
    Delay_init()                                         ;
    Check_Water()                                        ;
    clock_ini()                                          ;
//     WP_PowerOff()                                        ;
    Power_3_3_OFF()                                      ;
    Power_5_OFF()                                        ;
    USART1_SendString("start\n",strlen((char *)"jixuhehe\n")) ; 
}

