#include "RTC.h"


volatile const uint32_t AsynchPrediv = 0x7F; 
volatile const uint32_t SynchPrediv = 0xFF; //(32768/128-1)=0xff

volatile uint32_t Alarmk_TIME = 5;  //闹钟定时时间


static RTC_TimeTypeDef  RTC_TimeStructure;
static RTC_InitTypeDef  RTC_InitStructure;
static RTC_AlarmTypeDef  RTC_AlarmStructure;
static RTC_DateTypeDef  RTC_DateStructure; 

static uint8_t time[]={15,8,8,11,11,11};   
  
Time Set_Clock;

/****************************************************************************
* 名    称：void NVIC_Configuration(void)
* 功    能：中断源配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
static void RTC_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;	
  NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;					  //配置外部中断源（秒中断） 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Preemption_RTC ;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPri_RTC;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
}

static void RTC_Alarm_Exit()
{
    EXTI_InitTypeDef EXTI_InitStructure;  
	//闹钟中断接到第17线外部中断  
    EXTI_ClearITPendingBit(EXTI_Line17);  
    EXTI_InitStructure.EXTI_Line = EXTI_Line17;  
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
    EXTI_Init(&EXTI_InitStructure); 	
}

/****************************************************************************
* 名    称：void RTC_Configuration(void)
* 功    能：RTC配置函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/
static void RTC_Configuration(void)
{ 
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    /* Allow access to RTC */
    PWR_RTCAccessCmd(ENABLE);
     /*!< Reset RTC Domain */
    RCC_RTCResetCmd(ENABLE);
    RCC_RTCResetCmd(DISABLE);

#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
    /* The RTC Clock may varies due to LSI frequency dispersion. */   
    /* Enable the LSI OSC */ 
    RCC_LSICmd(ENABLE);
    /* Wait till LSI is ready */  
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET) {}
    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  
#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock *///外部32.378K晶振
    /* Enable the LSE OSC */
    RCC_LSEConfig(RCC_LSE_ON);
    /* Wait till LSE is ready */  
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) { }
    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
#else
    #error Please select the RTC Clock source inside the main.c file
#endif /* RTC_CLOCK_SOURCE_LSI */
  
    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);
    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();
}


/****************************************************************************
* 名    称：void clock_ini(void)
* 功    能：时钟初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/  
void clock_ini(void)
{       
    if(BKP_CheckLOCK_RTC() != BKP_RTC_Flag){  
        RTC_Configuration();
        RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
        RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
        RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;       
        while(RTC_Init(&RTC_InitStructure) == ERROR) {}
        Set_Time(time);
        BKP_LOCK_RTC();
    }else{
        while(RCC_GetFlagStatus(RCC_FLAG_PORRST) == RESET) {}
        while(RCC_GetFlagStatus(RCC_FLAG_PINRST) == RESET) {}
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
        PWR_RTCAccessCmd(ENABLE);
        RTC_WaitForSynchro();
    }   
    
    RTC_Alarm_Exit();
    RTC_NVIC_Configuration();
    
    RTC_ClearFlag(RTC_FLAG_ALRAF);
    PWR_ClearFlag(PWR_FLAG_WU);
    if(PWR_GetFlagStatus(PWR_FLAG_SB) != RESET) {
        PWR_ClearFlag(PWR_FLAG_SB);
        RTC_WaitForSynchro();
    }
}



void Set_Time(uint8_t * Dat_Time)
{
    Set_Clock.Year=Dat_Time[0];
    Set_Clock.Month=Dat_Time[1];
    Set_Clock.Day=Dat_Time[2];
    Set_Clock.Hour=Dat_Time[3];
    Set_Clock.Min=Dat_Time[4];
    Set_Clock.Sec=Dat_Time[5];
	  
    RTC_TimeStructure.RTC_Hours = Set_Clock.Hour; 
    RTC_TimeStructure.RTC_Minutes = Set_Clock.Min; 
    RTC_TimeStructure.RTC_Seconds = Set_Clock.Sec;  
    RTC_DateStructure.RTC_Year = Set_Clock.Year; 
    RTC_DateStructure.RTC_Month = Set_Clock.Month; 
    RTC_DateStructure.RTC_Date = Set_Clock.Day; 

    while(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR){ } 
    while(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR){ } 
        
}

void Set_Alarm_Time(uint8_t Sec)
{
    uint32_t sec_time;
    RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
    sec_time = Get_Time().Hour*3600 + Get_Time().Min*60 + Get_Time().Sec + Sec;         
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = (sec_time / 3600) % 24;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = (sec_time % 3600) / 60;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = sec_time % 60;
    RTC_AlarmStructure.RTC_AlarmDateWeekDay = Get_Time().Day + (sec_time > 86400 ? 1 : 0);
    RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
    RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_None;
    RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
    RTC_ITConfig(RTC_IT_ALRA, ENABLE);
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE); 
}


Time Get_Time(void)
{
    Time Get_Clock; 	
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
    
	Get_Clock.Year = RTC_DateStructure.RTC_Year;               //年 
    Get_Clock.Month = RTC_DateStructure.RTC_Month;	           //月份 
    Get_Clock.Day = RTC_DateStructure.RTC_Date; 			   //日 

	Get_Clock.Hour = RTC_TimeStructure.RTC_Hours; 	    	//计算出小时 
	Get_Clock.Min = RTC_TimeStructure.RTC_Minutes;  	    //计算出分钟 
    Get_Clock.Sec = RTC_TimeStructure.RTC_Seconds; 		    //计算出秒钟 
    
    return Get_Clock;
}

void RTC_Alarm_IRQHandler(void)
{
    if(RTC_GetITStatus(RTC_IT_ALRA) != RESET){
        RTC_ClearITPendingBit(RTC_IT_ALRA);
        EXTI_ClearITPendingBit(EXTI_Line17);
    } 
}

