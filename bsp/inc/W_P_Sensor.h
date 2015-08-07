#ifndef __W_P_SENSOR_H_
#define __W_P_SENSOR_H_

#include "bsp.h"

#define  MEASUREMENT_PIN             WP_Pin
#define  MEASUREMENT_GPIO_CLK        WP_PCLOCK
#define  MEASUREMENT_GPIO            WP_PType
#define  MEASUREMENT_ADC_CHANNEL     WP_ADC_CHANNEL
#define  ADC_Source                  WP_ADC_Source
#define  ADC_CONVERT_RATIO           3300 /4095   


extern  void  W_P_Sensor_Init(void);
extern  uint16_t  W_P_Sensor_Read(void);  //¶ÁË®Ñ¹
#endif
