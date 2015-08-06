#include "RTC.h"
#include "bsp.h"

volatile const uint32_t AsynchPrediv = 0xFF; 
volatile const uint32_t SynchPrediv = 0x7f;

volatile uint32_t Alarmk_TIME = 5;  //���Ӷ�ʱʱ��


static RTC_TimeTypeDef  RTC_TimeStructure;
static RTC_InitTypeDef  RTC_InitStructure;
static RTC_AlarmTypeDef  RTC_AlarmStructure;
static RTC_DateTypeDef  RTC_DateStructure; 

uint8_t time[]={14,4,19,11,11,41};   
  
Time Set_Clock;

/****************************************************************************
* ��    �ƣ�void NVIC_Configuration(void)
* ��    �ܣ��ж�Դ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
static void RTC_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;	
  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;					  //�����ⲿ�ж�Դ�����жϣ� 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Preemption_RTC ;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPri_RTC;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
}

static void RTC_Alarm_Exit()
{
    EXTI_InitTypeDef EXTI_InitStructure;  
    /* RTC Alarm A Interrupt Configuration */
	//�����жϽӵ���20���ⲿ�ж�  
    EXTI_ClearITPendingBit(EXTI_Line17);  
    EXTI_InitStructure.EXTI_Line = EXTI_Line17;  
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
    EXTI_Init(&EXTI_InitStructure); 	
}

/****************************************************************************
* ��    �ƣ�void RTC_Configuration(void)
* ��    �ܣ�RTC���ú���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
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
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET) { }

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  

#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock *///�ⲿ32.378K����
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
* ��    �ƣ�void clock_ini(void)
* ��    �ܣ�ʱ�ӳ�ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/  
void clock_ini(void)
{       
    if(RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2){  
        /* RTC configuration  */
        RTC_Configuration();

        /* Configure the RTC data register and RTC prescaler */
        RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
        RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
        RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
        
        /* RTC init and Check on RTC init */
        while(RTC_Init(&RTC_InitStructure) == ERROR){ }

        /*����ʱ��*/
        Set_Time(time);
         RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
    }
    else{
        /* Check if the Power On Reset flag is set */
        while(RCC_GetFlagStatus(RCC_FLAG_PORRST) == RESET){ }
        /* Check if the Pin Reset flag is set */
        while(RCC_GetFlagStatus(RCC_FLAG_PINRST) == RESET){ }

        /* Enable the PWR clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

        /* Allow access to RTC */
        PWR_RTCAccessCmd(ENABLE);

        /* Wait for RTC APB registers synchronisation */
        RTC_WaitForSynchro();

//         /* Clear the RTC Alarm Flag */
//         RTC_ClearFlag(RTC_FLAG_ALRAF);

//         /* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
//         EXTI_ClearITPendingBit(EXTI_Line17);

    }
//     
//     RTC_Alarm_Exit();
//     RTC_NVIC_Configuration();
}



void Set_Time(uint8_t * Dat_Time)
{
   	  uint32_t temp,tm;
	  Set_Clock.Year=2000+Dat_Time[0];
      Set_Clock.Month=Dat_Time[1];
	  Set_Clock.Day=Dat_Time[2];
	  Set_Clock.Hour=Dat_Time[3];
	  Set_Clock.Min=Dat_Time[4];
	  Set_Clock.Sec=Dat_Time[5];
	  tm=0;
		 for(temp=1970; temp<Set_Clock.Year; temp++){		          //����Դ�1970�굽��ǰ�������
			if(temp%4==0) tm=tm+((366*24)*3600);
			else tm=tm+((365*24)*3600);
		 }
		 for(temp=1; temp<Set_Clock.Month; temp++){		              //����Դ�1�µ���ǰ�µ�����
			if(temp%4==0&&temp==2) tm=tm+((29*24)*3600);
			else if(temp%4!=0&&temp==2) tm=tm+((28*24)*3600);
			else if(temp==4||temp==6||temp==9||temp==11) tm=tm+((30*24)*3600);
			else tm=tm+((31*24)*3600);
		 }
		 tm=tm+(((Set_Clock.Day-1)*24)*3600)+(Set_Clock.Hour*3600)+(Set_Clock.Min*60)+(Set_Clock.Sec);												  
// 		 RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
         RTC_TimeStructure.RTC_Hours = Set_Clock.Hour; 
         RTC_TimeStructure.RTC_Minutes = Set_Clock.Min; 
         RTC_TimeStructure.RTC_Seconds = Set_Clock.Sec;  
         RTC_DateStructure.RTC_Year = Set_Clock.Year; 
         RTC_DateStructure.RTC_Month = Set_Clock.Month; 
         RTC_DateStructure.RTC_Date = Set_Clock.Day; 
         
         while(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR){ } 
         while(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR){ } 
        
}

void Set_Alarm_Time(uint8_t * Dat_Time ,uint8_t Min)
{

   	 
}

void Set_Alarm(uint32_t tm)
{											  
	
}


Time Get_Time(void)
{
    Time Get_Clock; 	
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
    
	Get_Clock.Year = RTC_DateStructure.RTC_Year;               //�� 
    Get_Clock.Month = RTC_DateStructure.RTC_Month;	           //�·� 
    Get_Clock.Day = RTC_DateStructure.RTC_Date; 			   //�� 

	Get_Clock.Hour = RTC_TimeStructure.RTC_Hours; 	    	//�����Сʱ 
	Get_Clock.Min = RTC_TimeStructure.RTC_Minutes;  	    //��������� 
    Get_Clock.Sec = RTC_TimeStructure.RTC_Seconds; 		    //��������� 
    
    return Get_Clock;
}

void RTC_Alarm_IRQHandler(void)
{
    if(RTC_GetITStatus(RTC_IT_ALRA) != RESET){
        RTC_ClearITPendingBit(RTC_IT_ALRA);
        EXTI_ClearITPendingBit(EXTI_Line17);
    } 
}
