#include "app.h"


void Check_Water_DelayMs(uint8_t ms)
{
     OS_ERR  err                                                                      ;
     OSTimeDly(  (OS_TICK    )ms                                                      , 
	             (OS_OPT     )OS_OPT_TIME_DLY                                         ,  
	             (OS_ERR    *)&err)                                                   ;
}

uint16_t Read_Temp_Filter(pfunc delay, uint16_t ms)
{
    uint16_t  Temp_Data[Temp_N]; 
     OS_ERR  err                                                                      ;
    uint8_t sign;
    uint8_t i;
    uint16_t temp = 0;
    uint32_t Sum = 0;
    while(1) {
        Read_Temperature(&sign,&temp);
        if(sign*temp<850) {
            break;
        }
        OSTimeDly(  (OS_TICK    )50                                                   , 
	             (OS_OPT     )OS_OPT_TIME_DLY                                         ,  
	             (OS_ERR    *)&err)                                                   ;
    }
    for(i=0; i<Temp_N; i++){
        Read_Temperature(&sign,&temp);
        Temp_Data[i] = sign*temp;
        (*delay)(ms);       
    }
    
    L_From_B(Temp_Data,Temp_N);
    //去掉最大最小值求平均
    for(i=2; i<Temp_N-2; i++){
        Sum += Temp_Data[i];
    }
    temp = Sum/(Temp_N-4);
    return temp;   
}

uint16_t Read_WP_Filter(pfunc delay, uint16_t ms)
{
    uint16_t  WP_Data[WP_N]; 
    uint8_t i;
    uint16_t temp;
    uint32_t Sum = 0;
    for(i=0; i<WP_N; i++){
        WP_Data[i] = W_P_Sensor_Read();
        (*delay)(ms);   
    }
    
    L_From_B(WP_Data,WP_N);
    //去掉最大最小值求平均
    for(i=1; i<WP_N-1; i++){
        Sum += WP_Data[i];
    }
    temp = Sum/(WP_N-2)* ADC_CONVERT_RATIO;
    return temp;   
}


void App_GetTime_Init()
{
    OS_ERR  err                                                                          ;
    CPU_INT08U  data[32]                                                                 ;
    CPU_INT08U  status                                                                   ;
    
    if(BKP_CheckLOCK_TimeInit()!= BKP_RTC_Flag)                                          { 
        Power_3_3_ON()                                                                   ;
        IIC_Init()                                                                       ;
        OSTimeDly(  (OS_TICK    )200                                                     , 
                    (OS_OPT     )OS_OPT_TIME_DLY                                         , 
                    (OS_ERR    *)&err)                                                   ;
        CAT24C_Mask()                                                                    ;
        OSTimeDly(  (OS_TICK    )200                                                     , 
                    (OS_OPT     )OS_OPT_TIME_DLY                                         , 
                    (OS_ERR    *)&err)                                                   ;

        nRF24L01_Initial()                                                               ;
        OSTimeDly(  (OS_TICK    )200                                                     , 
                    (OS_OPT     )OS_OPT_TIME_DLY                                         , 
                    (OS_ERR    *)&err)                                                   ;
        while (1)                                                                        {
            memset(data,'\0', sizeof(data))                                              ;
            strcat((char *)data, "$")                                                    ;
            data[1] = Get_Data_Time                                                      ;
            NRF24L01_Send(data)                                                          ;
            memset(data,'\0', sizeof(data))                                              ;
            status = NRF24L01_Receive(data)                                              ;
            if(status == 1)                                                              {
                if (data[1] == Get_Time_Over)                                            {
                    Set_Time(&data[2])                                                   ;
                    BKP_LOCK_TimeInit()                                                  ;
                    break                                                                ;
                                                                                         }
                                                                                         }
            
            OSTimeDly(  (OS_TICK    )2                                                   , 
                        (OS_OPT     )OS_OPT_TIME_DLY                                     , 
                        (OS_ERR    *)&err)                                               ;
                                                                                         }                                                                                                   } 
}
