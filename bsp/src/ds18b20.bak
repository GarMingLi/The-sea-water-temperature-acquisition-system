#include "ds18b20.h"
/*GPIO_TypeDef * DS18B20_PType;
uint16_t  * GPIO_Pin;
void DO_IO_init()
{
   DS18B20_PType=GPIOD;
   *GPIO_Pin= GPIO_Pin_5;
} */
 /******************************************
函数名称：GPIO_DQ_Out_Mode
功    能：设置DQ引脚为开漏输出模式
参    数：无
返回值  ：无
*******************************************/
static void GPIO_DQ_Out_Mode(void)
{
    GPIO_InitTypeDef GPIO_InitStructure ;
    GPIO_InitStructure.GPIO_Pin = DS18B20_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(DS18B20_PType ,&GPIO_InitStructure) ;
}
/******************************************
函数名称：GPIO_DQ_Input_Mode
功    能：设置DQ引脚为浮空输入模式
参    数：无
返回值  ：无
*******************************************/
static void GPIO_DQ_Input_Mode(void)
{
    GPIO_InitTypeDef GPIO_InitStructure ;
    GPIO_InitStructure.GPIO_Pin = DS18B20_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN  ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;  
    GPIO_Init(DS18B20_PType ,&GPIO_InitStructure) ;
}
/******************************************
函数名称：Tx_ResetPulse
功    能：发送复位脉冲
参    数：无
返回值  ：无
*******************************************/
void Tx_ResetPulse(void)
{
    GPIO_DQ_Out_Mode() ;
    DQ_Write_1() ;  //复位脉冲
    delay_us(20) ; //至少保持480us
    DQ_Write_0() ;  //加速上升沿速度
    delay_us(500) ;
}
/******************************************
函数名称：Rx_PresencePulse
功    能：接受应答信号
参    数：无
返回值  ：无
*******************************************/
void Rx_PresencePulse(void)
{
    GPIO_DQ_Input_Mode() ;
    while( DQ_ReadBit()) ;  //等待DS18b20应答
    while( DQ_ReadBit() == 0) ; //DS18b20将总线拉低60~240us ,然后总线由上拉电阻拉高
    delay_us(50) ;
    GPIO_DQ_Out_Mode() ;    //接受完成，主机重新控制总线
}
/******************************************
函数名称：Write_OneByte_ToDS18b20
功    能：写一个字节到DS18b20
参    数：无
返回值  ：无
*******************************************/
void Write_OneByte_ToDS18b20(unsigned char data)
{
    unsigned char i ;
    GPIO_DQ_Out_Mode() ;
    for(i=0 ;i<8 ;i++)
    {
        if(data&0x01)    //低位在前
        {
            //写1
            DQ_Write_0() ; //写时间空隙总是从总线的低电平开始
            delay_us(1) ;  //15us内拉高
            DQ_Write_1() ;
            delay_us(60) ; //整个写1时隙不低于60us
        }
        else
        {
            //写0
            DQ_Write_0() ;
            delay_us(60) ; //保持在60us到120us之间
            DQ_Write_1() ;
            delay_us(5) ;
        }
        data >>= 1 ;
    }
}
/******************************************
函数名称：Read_OneByte_FromDS18b20
功    能：从DS18b20读一个字节
参    数：无
返回值  ：读出的数据
*******************************************/
unsigned char Read_OneByte_FromDS18b20(void)
{
    unsigned char i ,data = 0 ;
    
    for(i=0 ;i<8 ;i++)
    {
        GPIO_DQ_Out_Mode() ;
        data >>= 1 ;
        DQ_Write_0() ;
        delay_us(2) ;
        GPIO_DQ_Input_Mode() ;
        delay_us(1) ;
        if(DQ_ReadBit())
        {
            data |= 0x80 ;
        }
        delay_us(10) ;   //等待这一位数据完成传输
    }
    GPIO_DQ_Out_Mode() ;
    return data ;
}
/******************************************
函数名称：Read_Temperature
功    能：读取温度信息
参    数：*sign - 保存符号（零上或零下）
          *integer - 保存整数部分
          *decimal - 保存小数部分
返回值  ：无
*******************************************/
void Read_Temperature(uint8_t *sign , uint16_t *temp )
{
    uint8_t a=0;
    uint8_t b=0;
    //volatile unsigned char c=0;
    //volatile unsigned char d=0;
    //volatile unsigned char e=0;
    
    uint16_t tmp ;
    
    DS18B20_Init();
    Write_OneByte_ToDS18b20(ROM_Read_Cmd);
  
    DS18B20_Init();
	
    Write_OneByte_ToDS18b20(ROM_Skip_Cmd);//跳过读序列号操作
    Write_OneByte_ToDS18b20(Convert_T); //启动温度转换
	delay_ms(100);
    
    DS18B20_Init();
    Write_OneByte_ToDS18b20(ROM_Skip_Cmd);
	delay_us(10);
    Write_OneByte_ToDS18b20(Read_Scratchpad); //读取寄存器内容（可以从寄存器0读到寄存器8）
    
    a= Read_OneByte_FromDS18b20();     //温度低8位
    b= Read_OneByte_FromDS18b20();     //温度高8位
    //c= Read_OneByte_FromDS18B20();   //TH
    //d= Read_OneByte_FromDS18B20();   //TL
    //e= Read_OneByte_FromDS18B20();   //Configuration Register
      
    Tx_ResetPulse();  //中断数据读取
	
    tmp = (b<<8) | a ;
	 
   if(b & 0xF0)
    {
    *sign = 0 ;              //符号部分
    tmp = ~tmp+1 ;
    }
    else 
    {
     *sign = 1 ;
    }
	  				   
    *temp = tmp*0.625;  //整数部分

}
/******************************************
函数名称：Write_EEPROM
功    能：写配置参数
参    数：Th - 报警温度上限
          Tl - 报警温度下限
          Register_Con - 控制寄存器的值
返回值  ：读出的数据
*******************************************/
void Write_EEPROM(unsigned char Th,unsigned char Tl,unsigned char Register_Con )
{
  
    DS18B20_Init();
    Write_OneByte_ToDS18b20(ROM_Skip_Cmd);
    Write_OneByte_ToDS18b20(Write_Scratchpad);
    
    Write_OneByte_ToDS18b20(Th);//Th=7F
    Write_OneByte_ToDS18b20(Tl);//Tl=FF 最高位符号位
    Write_OneByte_ToDS18b20(Register_Con);//12位模式
    
    delay_ms(100);
    DS18B20_Init();
    Write_OneByte_ToDS18b20(ROM_Skip_Cmd);
    Write_OneByte_ToDS18b20(Copy_Scratchpad);//将寄存器的配置值写入EEPROM
    
    delay_ms(100);
 
}
/******************************************
函数名称：DS18B20_Init
功    能：初始化DS18b20
参    数：无
返回值  ：无
*******************************************/
void DS18B20_Init(void)
{
    static uint8_t  Flag = 0;
    if(Flag == 0){ //锁
        RCC_AHBPeriphClockCmd(DS18B20_PCLOCK, ENABLE); 
        Flag = 1;     
    }
    Tx_ResetPulse();
    Rx_PresencePulse(); 
}
