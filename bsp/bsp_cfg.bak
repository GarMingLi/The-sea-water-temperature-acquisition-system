#define Led_Debug                         //使用Led
#define USART_Debug                       //使用串口

#ifdef Led_Debug
    #define Led1_PType      GPIOA
    #define Led1_Pin        GPIO_Pin_10
    #define LED1_ON()       GPIO_SetBits(Led1_PType, Led1_Pin) 
    #define LED1_OFF()      GPIO_ResetBits(Led1_PType, Led1_Pin)
#endif

#define  Power_PCLOCK       RCC_AHBPeriph_GPIOB
#define  Power_5_PType      GPIOB
#define  Power_3_3_PType    GPIOB
#define  Power_5_Pin        GPIO_Pin_11
#define  Power_3_3_Pin      GPIO_Pin_10

#define  IIC_PCLOCK         RCC_AHBPeriph_GPIOB
#define  IIC_SDA_PType      GPIOB
#define  IIC_SCL_PType      GPIOB
#define  IIC_WP_PType       GPIOB
#define  IIC_WP_Pin         GPIO_Pin_5
#define  IIC_SCL_Pin        GPIO_Pin_6
#define  IIC_SDA_Pin        GPIO_Pin_7

#define  DS18B20_PCLOCK     RCC_AHBPeriph_GPIOB
#define  DS18B20_PType      GPIOB
#define  DS18B20_Pin        GPIO_Pin_9

#define  NRF20L01_SPI       SPI1
#define  NRF20L01_PCLOCK    RCC_AHBPeriph_GPIOB
#define  NRF_IRQ_PType      GPIOB
#define  NRF_CE_PType       GPIOB
#define  NRF_CSN_PType      GPIOB
#define  NRF_IRQ_Pin        GPIO_Pin_13
#define  NRF_CE_Pin         GPIO_Pin_14
#define  NRF_CSN_Pin        GPIO_Pin_15

#define  EXIT_PCLOCK        RCC_AHBPeriph_GPIOA
#define  EXIT_PType         GPIOA
#define  EXIT_Pin           GPIO_Pin_0
#define  EXIT_Pin_          GPIO_Pin_2



#define   Temp_N       10     //??????
#define   WP_N         20     //??????

#define  NVIC_PriorityGroup     NVIC_PriorityGroup_2          //??????   0-3   0-3

#define  Preemption_RTC         2           //RTC?????
#define  SubPri_RTC             0           //RTC?????
#define  Preemption_NRF         1           //NRF?????
#define  SubPri_NRF             0           //NRF?????
#define  Preemption_EXIT        0           //?????????
#define  SubPri_EXIT            0           //?????????


#define  BKP_Read_RTC()      RTC_ReadBackupRegister(RTC_BKP_DR0)
#define  BKP_RTC_Flag        0x32F2
#define  BKP_Write_RTC()     RTC_WriteBackupRegister(RTC_BKP_DR0, BKP_RTC_Flag)


