#include "bsp.h"
#include "app.h"
#include <os_app_hooks.h>

static OS_TCB taskStartTCB                                                               ;
static CPU_STK taskStartStk[STARTUP_TASK_STK_SIZE]                                       ; 		
static OS_TCB task0TCB                                                                   ;
static CPU_STK task0_stk[TASK_STK_SIZE_0]                                                ;
static OS_TCB task1TCB                                                                   ;
static CPU_STK task1_stk[TASK_STK_SIZE_1]                                                ;
static OS_TCB task2TCB                                                                   ;
static CPU_STK task2_stk[TASK_STK_SIZE_2]                                                ;
static OS_TCB task3TCB                                                                   ;
static CPU_STK task3_stk[TASK_STK_SIZE_3]                                                ;

typedef union 
{
    uint8_t  var[2];
    uint16_t data;
}W_Data;

typedef union 
{
     struct{
         uint16_t t_data;
         uint16_t p_data;
         uint16_t year: 7;
         uint16_t month: 4;
         uint16_t day:   5;
         uint16_t hour:  5;
         uint16_t minute:6;
     }data;
     uint8_t  var[8];
}CAT_Data;

void KernelMain(void)
{
    OS_ERR err                                                                           ;
    CPU_Init()                                                                           ;
    OSInit(         (OS_ERR	    *)&err)                                                  ;
    OSTaskCreate(   (OS_TCB     *)&taskStartTCB                                          ,
                    (CPU_CHAR   *)"Task Start"                                           ,
                    (OS_TASK_PTR)TaskStart                                               ,
                    (void       *)0                                                      ,
                    (OS_PRIO    ) STARTUP_TASK_PRIO                                      ,
                    (CPU_STK    *)&taskStartStk[0]                                       ,
                    (CPU_STK_SIZE)STARTUP_TASK_STK_SIZE / 10                             ,
                    (CPU_STK_SIZE)STARTUP_TASK_STK_SIZE                                  ,
                    (OS_MSG_QTY )0                                                       ,
                    (OS_TICK    )0                                                       ,
                    (void      *)0                                                       ,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK |                                  \
                                  OS_OPT_TASK_STK_CLR)                                   , 
                    (OS_ERR     *)&err)                                                  ;       
    App_OS_SetAllHooks();
    OSStart(        (OS_ERR	    *)&err)                                                  ;
}

static void TaskStart(void)
{
    OS_ERR  err                                                                          ;    
    SysTickInit()                                                                        ;
	OSTaskCreate(   (OS_TCB     *)&task0TCB                                              ,
                    (CPU_CHAR   *)"Task0"                                                ,
                    (OS_TASK_PTR)Task0                                                   ,
                    (void       *)0                                                      ,
                    (OS_PRIO    )TASK0_PRIO                                              ,
                    (CPU_STK    *)&task0_stk[0]                                          ,
                    (CPU_STK_SIZE)TASK_STK_SIZE_0 / 10                                   ,
                    (CPU_STK_SIZE)TASK_STK_SIZE_0                                        ,
                    (OS_MSG_QTY )0                                                       ,
                    (OS_TICK    )0                                                       ,
                    (void      *)0                                                       ,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK |                                  \
                                  OS_OPT_TASK_STK_CLR)                                   , 
                    (OS_ERR     *)&err)                                                  ;
	OSTaskCreate(   (OS_TCB     *)&task1TCB                                              ,
                    (CPU_CHAR   *)"Task1"                                                ,
                    (OS_TASK_PTR)Task1                                                   ,
                    (void       *)0                                                      ,
                    (OS_PRIO    )TASK1_PRIO                                              ,
                    (CPU_STK    *)&task1_stk[0]                                          ,
                    (CPU_STK_SIZE)TASK_STK_SIZE_1 / 10                                   ,
                    (CPU_STK_SIZE)TASK_STK_SIZE_1                                        ,
                    (OS_MSG_QTY )0                                                       ,
                    (OS_TICK    )0                                                       ,
                    (void      *)0                                                       ,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK |                                  \
                                  OS_OPT_TASK_STK_CLR)                                   , 
                    (OS_ERR     *)&err)                                                  ;   
    OSTaskCreate(   (OS_TCB     *)&task2TCB                                              ,
                    (CPU_CHAR   *)"Task2"                                                ,
                    (OS_TASK_PTR)Task2                                                   ,
                    (void       *)0                                                      ,
                    (OS_PRIO     ) TASK2_PRIO                                            ,
                    (CPU_STK    *)&task2_stk[0]                                          ,
                    (CPU_STK_SIZE)TASK_STK_SIZE_2 / 10                                   ,
                    (CPU_STK_SIZE)TASK_STK_SIZE_2                                        ,
                    (OS_MSG_QTY )64                                                      ,
                    (OS_TICK    )0                                                       ,
                    (void      *)0                                                       ,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK |                                  \
                                  OS_OPT_TASK_STK_CLR)                                   , 
                    (OS_ERR     *)&err)                                                  ;	
    OSTaskCreate(   (OS_TCB     *)&task3TCB                                              ,
                    (CPU_CHAR   *)"Task3"                                                ,
                    (OS_TASK_PTR)Task3                                                   ,
                    (void       *)0                                                      ,
                    (OS_PRIO     ) TASK3_PRIO                                            ,
                    (CPU_STK    *)&task3_stk[0]                                          ,
                    (CPU_STK_SIZE)TASK_STK_SIZE_3 / 10                                   ,
                    (CPU_STK_SIZE)TASK_STK_SIZE_3                                        ,
                    (OS_MSG_QTY )0                                                       ,
                    (OS_TICK    )0                                                       ,
                    (void      *)0                                                       ,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK |                                  \
                                  OS_OPT_TASK_STK_CLR)                                   , 
                    (OS_ERR     *)&err)                                                  ;
    OSTaskSuspend(  (OS_TCB   *) &task1TCB                                               , 
                    (OS_ERR   *) &err)                                                   ;				
    OSTaskSuspend(  (OS_TCB   *) &task2TCB                                               , 
                    (OS_ERR   *) &err)                                                   ;				

    OSTaskDel(      (OS_TCB     *)&taskStartTCB                                          , 
                    (OS_ERR     *)&err)                                                  ;
}

static void Task0(void *p_arg)
{
    OS_ERR  err                                                                          ;
    CPU_INT08U  flag = 0x11                                                              ;
//     App_GetTime_Init()                                                                   ;
    OSTaskResume(   (OS_TCB   *) &task1TCB                                               , 
                                (OS_ERR   *) &err)                                       ;								 
    OSTaskResume(   (OS_TCB   *) &task2TCB                                               , 
                                (OS_ERR   *) &err)                                       ;								 

    
    while(1)                                                                             {		
        if(Get_Status((pfunc)Check_Water_DelayMs, 1000) == Power_Down)                   {			//水下工作								 
    		
            if((flag & 0x01) == 0x01)                                                    {
		        flag = 0x10                                                              ; 
                Power_3_3_ON()                                                           ;
                Power_5_ON()                                                             ;                                 
                IIC_Init()                                                               ;
                W_P_Sensor_Init()                                                        ;
                OSTimeDly(  (OS_TICK    )200                                             , 
                            (OS_OPT     )OS_OPT_TIME_DLY                                 , 
                            (OS_ERR    *)&err)                                           ;
                OSTaskSuspend(  (OS_TCB   *) &task1TCB                                   , 
                                (OS_ERR   *) &err)                                       ;				
			    OSTaskResume(   (OS_TCB   *) &task2TCB                                   , 
                                (OS_ERR   *) &err)                                       ;								 
								                                                         }	
				                                                                         }							
        else if(Get_Status((pfunc)Check_Water_DelayMs, 1000) == Power_Up)                {		    //水上工作								
            
            if((flag & 0x10) == 0x10)                                                    {
	            flag = 0x01                                                              ; 
                Cat24c_PowerOff()                                                        ;
                Power_3_3_OFF()                                                          ;
                Power_5_OFF()                                                            ;                
                OSTaskSuspend(  (OS_TCB   *) &task2TCB                                   , 
                                (OS_ERR   *) &err)                                       ;				
			    OSTaskResume(   (OS_TCB   *) &task1TCB                                   , 
                                (OS_ERR   *) &err)                                       ;	
								                                                         }	
					                                                                     }											
        OSTimeDly(  (OS_TICK    )200                                                     , 
	                (OS_OPT     )OS_OPT_TIME_DLY                                         , 
	                (OS_ERR    *)&err)                                                   ;
                                                                                         }
}


static void Task1(void *p_arg)
{
    OS_ERR  err                                                                          ;
    CPU_INT08U data[32];
    CPU_INT08U sign = 0;
    CPU_INT08U Count = 0;
    CPU_INT08U time[]={15,8,30,23,59,11};   
    
    OSTimeDly(  (OS_TICK    )2000                                                         , 
                (OS_OPT     )OS_OPT_TIME_DLY                                             , 
                (OS_ERR    *)&err)                                                       ;
        
    while(1)                                                                             {			                           
		
        USART1_SendString("hehe\n",strlen((char *)"hehe\n")) ; 
        memset(data,'\0',sizeof(data))                                                   ;	
        strcat((char *)data,"Year:")                                                     ;
        itoa(Get_Time().Year+2000, &data[5],10)                                          ;
        strcat((char *)data,"\t")                                                        ;
		USART1_SendString(data,strlen((char *)data))                                     ;
        
        memset(data,'\0',sizeof(data))                                                   ;	
        strcat((char *)data,"Month:")                                                    ;
        itoa(Get_Time().Month, &data[6],10)                                              ;
        strcat((char *)data,"\t")                                                        ;
		USART1_SendString(data,strlen((char *)data))                                     ;
        
        memset(data,'\0',sizeof(data))                                                   ;	
        strcat((char *)data,"Day:")                                                      ;
        itoa(Get_Time().Day, &data[4],10)                                                ;
        strcat((char *)data,"\t")                                                        ;
		USART1_SendString(data,strlen((char *)data))                                     ;
        
        memset(data,'\0',sizeof(data))                                                   ;	
        strcat((char *)data,"Hour:")                                                     ;
        itoa(Get_Time().Hour, &data[5],10)                                               ;
        strcat((char *)data,"\t")                                                        ;
		USART1_SendString(data,strlen((char *)data))                                     ;
        
        memset(data,'\0',sizeof(data))                                                   ;	
        strcat((char *)data,"Min:")                                                      ;
        itoa(Get_Time().Min, &data[4],10)                                                ;
        strcat((char *)data,"\t")                                                        ;
		USART1_SendString(data,strlen((char *)data))                                     ;
        
        memset(data,'\0',sizeof(data))                                                   ;	
        strcat((char *)data,"Sec:")                                                      ;
        itoa(Get_Time().Sec, &data[4],10)                                                ;
        strcat((char *)data,"\n")                                                        ;
		USART1_SendString(data,strlen((char *)data))                                     ;        
        
        OSTimeDly(  (OS_TICK    )2000                                                    , 
	                (OS_OPT     )OS_OPT_TIME_DLY                                         , 
	                (OS_ERR    *)&err)                                                   ;
                                                                                         }
}

static void Task2(void *p_arg)
{
    OS_ERR  err                                                                          ;
    CPU_INT08U  data_string[32]                                                          ;
    CPU_INT08U  data_buff[16]                                                            ;    
    CPU_INT16U  WT_temp = 0                                                              ;
    CPU_INT16U  WP_temp = 0                                                              ;
    W_Data  WT_Data                                                                      ;
    W_Data  WP_Data                                                                      ;
    W_Data  W_Buff                                                                       ;
    CAT_Data C_Data;
    CAT_Data C_Data_Buff;
    CPU_INT16U count = 0;

    OSTimeDly(  (OS_TICK    )2000                                                        , 
                (OS_OPT     )OS_OPT_TIME_DLY                                             , 
                (OS_ERR    *)&err)                                                       ;
      
    while(1)                                                                             {		
		RTC_WakeUpCmd(DISABLE)                                                           ;
        Power_3_3_ON()                                                                   ;
        Power_5_ON()                                                                     ;                                 
        IIC_Init()                                                                       ;
        W_P_Sensor_Init()                                                                ;
        OSTimeDly(  (OS_TICK    )2                                                       , 
	                (OS_OPT     )OS_OPT_TIME_DLY                                         , 
	                (OS_ERR    *)&err)                                                   ;    
        USART1_SendString("\n",strlen((char *)"\n"))                                     ;
        
        C_Data.data.year = 16                                                            ;
        C_Data.data.month = 2                                                            ;
        C_Data.data.day = 3                                                              ;
        C_Data.data.hour = 12                                                            ;
        C_Data.data.minute = 12                                                          ;
        C_Data.data.p_data = 200                                                         ;
        C_Data.data.t_data = 270                                                         ;
        
        CAT24C_Word_W(0, &C_Data.var[0], 6)                                              ;
        OSTimeDly(  (OS_TICK    )100                                                     , 
	                (OS_OPT     )OS_OPT_TIME_DLY                                         , 
	                (OS_ERR    *)&err)                                                   ;
		memset(data_buff,'\0',sizeof(data_buff))                                         ;	        
        CAT24C_Selective_R(0, C_Data_Buff.var,6)                                         ;
        itoa(C_Data_Buff.data.year, &data_buff[0],10)                                    ;
        strcat((char *)data_buff,"\t")                                                   ;
        itoa(C_Data_Buff.data.month, &data_buff[3],10)                                   ;
        strcat((char *)data_buff,"\t")                                                   ;
        itoa(C_Data_Buff.data.day, &data_buff[5],10)                                     ;
        strcat((char *)data_buff,"\t")                                                   ;
        USART1_SendString(data_buff, strlen((char *)data_buff))                          ; 
        
        WT_temp = Read_Temp_Filter((pfunc)Check_Water_DelayMs, 20)                       ; 
        WT_Data.data = WT_temp                                                           ;
        memset(data_buff,'\0',sizeof(data_buff))                                         ;	
        itoa(WT_temp, &data_buff[0],10)                                                  ;
        strcat((char *)data_buff,"\t")                                                   ;
        USART1_SendString(data_buff,strlen((char *)data_buff))                           ;
        OSTimeDly(  (OS_TICK    )20                                                      , 
	                (OS_OPT     )OS_OPT_TIME_DLY                                         , 
	                (OS_ERR    *)&err)                                                   ;

        WP_temp = Read_WP_Filter((pfunc)Check_Water_DelayMs, 50)                         ;
        WP_Data.data = WP_temp                                                           ;
        memset(data_buff,'\0',sizeof(data_buff))                                         ;	        
        itoa(WP_temp, &data_buff[0],10)                                                  ;
        strcat((char *)data_buff,"\t")                                                   ;
        USART1_SendString(data_buff,strlen((char *)data_buff))                           ;
        OSTimeDly(  (OS_TICK    )20                                                      , 
	                (OS_OPT     )OS_OPT_TIME_DLY                                         , 
	                (OS_ERR    *)&err)                                                   ;
    
//         CAT24C_Word_W(count, &WT_Data.var[0], 2)                                      ;
//         OSTimeDly(  (OS_TICK    )1                                                    , 
// 	                (OS_OPT     )OS_OPT_TIME_DLY                                         , 
// 	                (OS_ERR    *)&err)                                                   ;
//         CAT24C_Word_W(count+2, &WP_Data.var[0], 2)                                    ;
//         OSTimeDly(  (OS_TICK    )1                                                    , 
// 	                (OS_OPT     )OS_OPT_TIME_DLY                                         , 
// 	                (OS_ERR    *)&err)                                                   ;

		memset(data_buff,'\0',sizeof(data_buff))                                         ;	        
        CAT24C_Selective_R(count, W_Buff.var,2)                                          ;
        itoa(W_Buff.data, &data_buff[0],10)                                              ;
        strcat((char *)data_buff,"\t")                                                   ;
        USART1_SendString(data_buff, strlen((char *)data_buff))                          ; 
        memset(data_buff,'\0',sizeof(data_buff))                                         ;	        
        CAT24C_Selective_R(count+2, W_Buff.var,2)                                        ;
        itoa(W_Buff.data, &data_buff[0],10)                                              ;
        strcat((char *)data_buff,"\t")                                                   ;
        USART1_SendString(data_buff, strlen((char *)data_buff))                          ; 
        
        memset(data_buff,'\0',sizeof(data_buff))                                         ;	
        itoa(count, &data_buff[0],10)                                                    ;
        strcat((char *)data_buff,"\t")                                                   ;
        USART1_SendString(data_buff,strlen((char *)data_buff))                           ;
        count += 4                                                                       ;
        
        RTC_WakeUpCmd(ENABLE)                                                            ;
        Cat24c_PowerOff()                                                                ;
        WP_PowerOff()                                                                    ;
        WP_DS18B20Off()                                                                  ;
        Power_3_3_OFF()                                                                  ;
        Power_5_OFF()                                                                    ;                
        Set_Alarm_Time(4)                                                                ;
       
#if defined  (STOP_Mode)  
        USART1_SendString("STOPMode\n",strlen((char *)"STOPMode\n"))                     ; 
        PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI)                            ;
        Clock_Resume()                                                                   ;           
#elif defined  (TANDBY_Mode) 
        USART1_SendString("STANDBYMode\n",strlen((char *)"STANDBYMode\n"))               ; 
        PWR_EnterSTANDBYMode()                                                           ;
#else
        PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI)                            ;
        Clock_Resume()                                                                   ;           
#endif            
                                                                                         }
}


static void Task3(void *p_arg)
{
    OS_ERR  err                                                                          ;
    
    while(1)                                                                             {		
        GPIO_ToggleBits(Led1_PType, Led1_Pin)                                            ;	         
//         LED1_ON();
        OSTimeDly(  (OS_TICK    )200                                                     , 
	                (OS_OPT     )OS_OPT_TIME_DLY                                         , 
	                (OS_ERR    *)&err)                                                   ;
                                                                                         }
}

















// static void Task1(void *p_arg)
// {
//     OS_ERR  err                                                                          ;
//     CPU_INT08U aa[] = "0123456789\t";
//     CPU_INT16U WT_temp = 0;
//     CPU_INT08U data[32];
//     CPU_INT08U sign = 0;
//     CPU_INT16U WP_temp = 0;
//     CPU_INT08U Count = 0;
//     CPU_INT08U time[]={15,8,30,23,59,11};   
//     CPU_INT08U NRF_Data[32];
//     
//     OSTimeDly(  (OS_TICK    )200                                                         , 
//                 (OS_OPT     )OS_OPT_TIME_DLY                                             , 
//                 (OS_ERR    *)&err)                                                       ;
//         
//     while(1)                                                                             {			                           
    
//         CAT24C_Word_W(0x31, aa, strlen((char *)aa))                                      ;
//         OSTimeDly(  (OS_TICK    )200                                                     , 
// 	                (OS_OPT     )OS_OPT_TIME_DLY                                         , 
// 	                (OS_ERR    *)&err)                                                   ;
// 		memset(data,'\0',sizeof(data))                                                   ;	        
//         CAT24C_Selective_R(0x31, data, strlen((char *)aa))                               ;
//         USART1_SendString(data, strlen((char *)data))                                    ; 
//        

//         
//         memset(NRF_Data,'\0',sizeof(NRF_Data));
// 		strcat((char *)NRF_Data,"$");
//         NRF24L01_Send(NRF_Data); 
//         
//         if (10 == Count++%5)                                                             {
//             Set_Time(time)                                                               ;
//             USART1_SendString("睡眠model\n",strlen((char *)"睡眠model\n"))               ;  
//             Set_Alarm_Time(5)                                                            ;
  
//         OSTimeDly(  (OS_TICK    )2000                                                    , 
// 	                (OS_OPT     )OS_OPT_TIME_DLY                                         , 
// 	                (OS_ERR    *)&err)                                                   ;
//                                                                                          }
// }

// static void Task2(void *p_arg)
// {
//     OS_ERR  err                                                                          ;
//     
//     while(1)                                                                             {		
// //         Power_3_3_OFF();
//         PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI)                            ;
//         OSTimeDly(  (OS_TICK    )2000                                                    , 
// 	                (OS_OPT     )OS_OPT_TIME_DLY                                         , 
// 	                (OS_ERR    *)&err)                                                   ;
//                                                                                          }
// }


// static void Task3(void *p_arg)
// {
//     OS_ERR  err                                                                          ;
//     
//     while(1)                                                                             {		
//         GPIO_ToggleBits(Led1_PType, Led1_Pin)                                            ;	         
// //         LED1_ON();
//         OSTimeDly(  (OS_TICK    )200                                                     , 
// 	                (OS_OPT     )OS_OPT_TIME_DLY                                         , 
// 	                (OS_ERR    *)&err)                                                   ;
//                                                                                          }
// }

