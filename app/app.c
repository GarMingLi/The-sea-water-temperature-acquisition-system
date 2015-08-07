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
    OSTaskDel(      (OS_TCB     *)&taskStartTCB                                          , 
                    (OS_ERR     *)&err)                                                  ;
}

static void Task0(void *p_arg)
{
    OS_ERR  err                                                                          ;
    CPU_INT08U  flag =0x11                                                               ;

    while(1)                                                                             {		
        if(Get_Status((pfunc)Check_Water_DelayMs, 1000) == Power_Down)                   {			//水下工作								 
    		
            if((flag & 0x01) == 0x01)                                                    {
		        flag = 0x10                                                              ; 
//                 PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI)                 ;
                Cat24c_PowerOff()                                                        ;
                Power_3_3_OFF()                                                          ;
                Power_5_OFF()                                                            ;
                OSTimeDly(  (OS_TICK    )200                                                       , 
                            (OS_OPT     )OS_OPT_TIME_DLY                                         , 
                            (OS_ERR    *)&err)                                                   ;
                OSTaskSuspend(  (OS_TCB   *) &task1TCB                                   , 
                                (OS_ERR   *) &err)                                       ;				
			    OSTaskResume(   (OS_TCB   *) &task2TCB                                   , 
                                (OS_ERR   *) &err)                                       ;								 
								                                                         }	
				                                                                         }							
        else if(Get_Status((pfunc)Check_Water_DelayMs, 1000) == Power_Up)                {		    //水上工作								
            
            if((flag & 0x10) == 0x10)                                                    {
	            flag = 0x01                                                              ; 
                Power_3_3_ON()                                                           ;
                Power_5_ON()                                                             ;                                 
                IIC_Init()                                                               ;
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
    CPU_INT08U aa[] = "0123456789\t";
    CPU_INT16U WT_temp = 0;
    CPU_INT08U data[32];
    CPU_INT08U sign = 0;
    CPU_INT16U WP_temp = 0;
    OSTimeDly(  (OS_TICK    )200                                                         , 
                (OS_OPT     )OS_OPT_TIME_DLY                                             , 
                (OS_ERR    *)&err)                                                       ;
        
    while(1)                                                                             {			                                                  ;
        Read_Temperature(&sign,&WT_temp)                                                 ;	 								
		memset(data,'\0',sizeof(data))                                                   ;	
        itoa(WT_temp, &data[0],10)                                                       ;
        strcat((char *)data,"\t")                                                        ;
        USART1_SendString(data,strlen((char *)data))                                     ;
        OSTimeDly(  (OS_TICK    )20                                                     , 
	                (OS_OPT     )OS_OPT_TIME_DLY                                         , 
	                (OS_ERR    *)&err)                                                   ;
        
        WP_temp = Read_WP_Filter((pfunc)Check_Water_DelayMs, 50)                                                       ;
        memset(data,'\0',sizeof(data))                                                   ;	        
        itoa(WP_temp, &data[0],10)                                                       ;
        strcat((char *)data,"\t")                                                        ;
        USART1_SendString(data,strlen((char *)data))                                     ;
        OSTimeDly(  (OS_TICK    )20                                                     , 
	                (OS_OPT     )OS_OPT_TIME_DLY                                         , 
	                (OS_ERR    *)&err)                                                   ;
        
        CAT24C_Word_W(0x31, aa, strlen((char *)aa))                                      ;
        OSTimeDly(  (OS_TICK    )200                                                     , 
	                (OS_OPT     )OS_OPT_TIME_DLY                                         , 
	                (OS_ERR    *)&err)                                                   ;
		memset(data,'\0',sizeof(data))                                                   ;	        
        CAT24C_Selective_R(0x31, data, strlen((char *)aa))                               ;
        USART1_SendString(data, strlen((char *)data))                                    ; 
       
        OSTimeDly(  (OS_TICK    )2000                                                    , 
	                (OS_OPT     )OS_OPT_TIME_DLY                                         , 
	                (OS_ERR    *)&err)                                                   ;
                                                                                         }
}

static void Task2(void *p_arg)
{
    OS_ERR  err                                                                          ;
    
    while(1)                                                                             {		
//         Power_3_3_OFF();
        PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI)                            ;
        OSTimeDly(  (OS_TICK    )2000                                                    , 
	                (OS_OPT     )OS_OPT_TIME_DLY                                         , 
	                (OS_ERR    *)&err)                                                   ;
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

