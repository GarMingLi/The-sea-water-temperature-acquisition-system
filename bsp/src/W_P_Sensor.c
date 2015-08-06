#include "W_P_Sensor.h"

void W_P_Sensor_Init(void)   //��ˮѹ��ʼ��
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
 	/* ʹ�� HSI (16Mhz) */
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
    
    ADC_StructInit(&ADC_InitStructure);    //�ṹ��װ��ȱʡֵ
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;   //12λ�ֱ���
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;            //��·ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;       //����ģʽ
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;   //��ʹ���ⲿ�жϴ���
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;    //�Ҷ���
    ADC_InitStructure.ADC_NbrOfConversion = 1;        //ת��1�������
    ADC_Init(ADC_Source, &ADC_InitStructure);

      /* ת��ʱ�䣺192Cycles*/
      ADC_RegularChannelConfig(ADC_Source, MEASUREMENT_ADC_CHANNEL, 1, ADC_SampleTime_192Cycles);

      /* ����ģʽ��ֱ������ֵ������ת��һֱ����*/
      ADC_DelaySelectionConfig(ADC_Source, ADC_DelayLength_Freeze);
      
      /* ���߻���ʱ��ADC��Դ�ر� */
      ADC_PowerDownCmd(ADC_Source, ADC_PowerDown_Idle_Delay, ENABLE);
      
      /* Enable ADC_Source */
      ADC_Cmd(ADC_Source, ENABLE);

      /* Wait until ADC1 ON status *///�ȴ��ϴ�������Ч
      while (ADC_GetFlagStatus(ADC_Source, ADC_FLAG_ADONS) == RESET)
      {
      }

      /* ��ʼ���ת��*/
      ADC_SoftwareStartConv(ADC_Source);

      /* Wait until ADC Channel 5 or 1 end of conversion *///�ȴ��ϴ�������Ч
      while (ADC_GetFlagStatus(ADC_Source, ADC_FLAG_EOC) == RESET)
      {
      } 	 
}

uint16_t  W_P_Sensor_Read()  //��ˮѹ
{
    return  ADC_GetConversionValue(ADC_Source);
}








