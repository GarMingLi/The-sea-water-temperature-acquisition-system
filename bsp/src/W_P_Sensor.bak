#include "W_P_Sensor.h"

void W_P_Sensor_Init(void)   //读水压初始化
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
 	/* 使能 HSI (16Mhz) */
    RCC_HSICmd(ENABLE);

    RCC_AHBPeriphClockCmd(MEASUREMENT_GPIO_CLK, ENABLE);    
    GPIO_InitStructure.GPIO_Pin =  MEASUREMENT_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(MEASUREMENT_GPIO, &GPIO_InitStructure);

    /* Check that HSI oscillator is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET){}
    /* ADC1 Configuration */      
    /* Enable ADC1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  
    
    ADC_StructInit(&ADC_InitStructure);    //结构体装入缺省值
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;   //12位分辨率
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;            //单路模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;       //连续模式
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;   //不使用外部中断触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;    //右对齐
    ADC_InitStructure.ADC_NbrOfConversion = 1;        //转换1个后完成
    ADC_Init(ADC_Source, &ADC_InitStructure);

      /* 转换时间：192Cycles*/
      ADC_RegularChannelConfig(ADC_Source, MEASUREMENT_ADC_CHANNEL, 1, ADC_SampleTime_192Cycles);

      /* 冻结模式：直到读完值，否则转换一直冻结*/
      ADC_DelaySelectionConfig(ADC_Source, ADC_DelayLength_Freeze);
      
      /* 休眠或延时，ADC电源关闭 */
      ADC_PowerDownCmd(ADC_Source, ADC_PowerDown_Idle_Delay, ENABLE);
      
      /* Enable ADC_Source */
      ADC_Cmd(ADC_Source, ENABLE);

      /* Wait until ADC1 ON status *///等待上次配置生效
      while (ADC_GetFlagStatus(ADC_Source, ADC_FLAG_ADONS) == RESET)
      {
      }

      /* 开始软件转换*/
      ADC_SoftwareStartConv(ADC_Source);

      /* Wait until ADC Channel 5 or 1 end of conversion *///等待上次配置生效
      while (ADC_GetFlagStatus(ADC_Source, ADC_FLAG_EOC) == RESET)
      {
      } 	 
}

uint16_t  W_P_Sensor_Read()  //读水压
{
    return  ADC_GetConversionValue(ADC_Source);
}








