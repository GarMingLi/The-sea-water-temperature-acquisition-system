#include  "power.h"

void power_init()
{
    //     PWR_WakeUpPinCmd(PWR_WakeUpPin_1, ENABLE)     ;
    GPIO_InitTypeDef  gpio_init                          ;  

    GPIO_StructInit((GPIO_InitTypeDef*)&gpio_init)       ;
    GPIO_Init(GPIOA, &gpio_init)                         ; 
    GPIO_Init(GPIOB, &gpio_init)                         ; 
    GPIO_Init(GPIOC, &gpio_init)                         ; 
    RCC_AHBPeriphClockCmd(
                      RCC_AHBPeriph_GPIOA                |
                      RCC_AHBPeriph_GPIOB                |
                      RCC_AHBPeriph_GPIOC                ,
                      DISABLE)                           ; 
    
    RCC_AHBPeriphClockCmd(Power_PCLOCK, ENABLE)          ; 
    
    gpio_init.GPIO_Speed = GPIO_Speed_2MHz               ;   
    gpio_init.GPIO_Pin = Power_5_Pin                     ;  
    gpio_init.GPIO_Mode = GPIO_Mode_OUT                  ;  
    gpio_init.GPIO_OType = GPIO_OType_PP                 ;        
    gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL               ;  
    GPIO_Init(Power_5_PType, &gpio_init)                 ;  
    gpio_init.GPIO_Speed = GPIO_Speed_2MHz               ;   
    gpio_init.GPIO_Pin = Power_3_3_Pin                   ;  
    gpio_init.GPIO_Mode = GPIO_Mode_OUT                  ;  
    gpio_init.GPIO_OType = GPIO_OType_PP                 ;        
    gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL               ;  
    GPIO_Init(Power_3_3_PType, &gpio_init)               ;      
}

void Cat24c_PowerOff(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure                   ;   
    RCC_AHBPeriphClockCmd(IIC_Pin_CLOCK, ENABLE)           ;     
    GPIO_InitStructure.GPIO_Pin = IIC_SDA_Pin              ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN            ;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP          ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz       ;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP            ;
    GPIO_Init(IIC_SDA_PType, &GPIO_InitStructure)          ;
    
    GPIO_InitStructure.GPIO_Pin = IIC_SCL_Pin              ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN            ;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP          ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz       ;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP            ;
    GPIO_Init(IIC_SCL_PType, &GPIO_InitStructure)          ;
    
    GPIO_InitStructure.GPIO_Pin = IIC_WP_Pin               ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN            ;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP          ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz      ;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP            ;
    GPIO_Init(IIC_WP_PType, &GPIO_InitStructure)           ;
    
}



