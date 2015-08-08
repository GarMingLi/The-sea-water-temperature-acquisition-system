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
        if(sign*temp<600) {
            break;
        }
        OSTimeDly(  (OS_TICK    )50                                                      , 
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
    for(i=1; i<Temp_N-1; i++){
        Sum += Temp_Data[i];
    }
    temp = Sum/(Temp_N-2);
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

