#include "ds18b20.h"
/*GPIO_TypeDef * DS18B20_PType;
uint16_t  * GPIO_Pin;
void DO_IO_init()
{
   DS18B20_PType=GPIOD;
   *GPIO_Pin= GPIO_Pin_5;
} */
 /******************************************
�������ƣ�GPIO_DQ_Out_Mode
��    �ܣ�����DQ����Ϊ��©���ģʽ
��    ������
����ֵ  ����
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
�������ƣ�GPIO_DQ_Input_Mode
��    �ܣ�����DQ����Ϊ��������ģʽ
��    ������
����ֵ  ����
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
�������ƣ�Tx_ResetPulse
��    �ܣ����͸�λ����
��    ������
����ֵ  ����
*******************************************/
void Tx_ResetPulse(void)
{
    GPIO_DQ_Out_Mode() ;
    DQ_Write_1() ;  //��λ����
    delay_us(20) ; //���ٱ���480us
    DQ_Write_0() ;  //�����������ٶ�
    delay_us(500) ;
}
/******************************************
�������ƣ�Rx_PresencePulse
��    �ܣ�����Ӧ���ź�
��    ������
����ֵ  ����
*******************************************/
void Rx_PresencePulse(void)
{
    GPIO_DQ_Input_Mode() ;
    while( DQ_ReadBit()) ;  //�ȴ�DS18b20Ӧ��
    while( DQ_ReadBit() == 0) ; //DS18b20����������60~240us ,Ȼ��������������������
    delay_us(50) ;
    GPIO_DQ_Out_Mode() ;    //������ɣ��������¿�������
}
/******************************************
�������ƣ�Write_OneByte_ToDS18b20
��    �ܣ�дһ���ֽڵ�DS18b20
��    ������
����ֵ  ����
*******************************************/
void Write_OneByte_ToDS18b20(unsigned char data)
{
    unsigned char i ;
    GPIO_DQ_Out_Mode() ;
    for(i=0 ;i<8 ;i++)
    {
        if(data&0x01)    //��λ��ǰ
        {
            //д1
            DQ_Write_0() ; //дʱ���϶���Ǵ����ߵĵ͵�ƽ��ʼ
            delay_us(1) ;  //15us������
            DQ_Write_1() ;
            delay_us(60) ; //����д1ʱ϶������60us
        }
        else
        {
            //д0
            DQ_Write_0() ;
            delay_us(60) ; //������60us��120us֮��
            DQ_Write_1() ;
            delay_us(5) ;
        }
        data >>= 1 ;
    }
}
/******************************************
�������ƣ�Read_OneByte_FromDS18b20
��    �ܣ���DS18b20��һ���ֽ�
��    ������
����ֵ  ������������
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
        delay_us(10) ;   //�ȴ���һλ������ɴ���
    }
    GPIO_DQ_Out_Mode() ;
    return data ;
}
/******************************************
�������ƣ�Read_Temperature
��    �ܣ���ȡ�¶���Ϣ
��    ����*sign - ������ţ����ϻ����£�
          *integer - ������������
          *decimal - ����С������
����ֵ  ����
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
	
    Write_OneByte_ToDS18b20(ROM_Skip_Cmd);//���������кŲ���
    Write_OneByte_ToDS18b20(Convert_T); //�����¶�ת��
	delay_ms(100);
    
    DS18B20_Init();
    Write_OneByte_ToDS18b20(ROM_Skip_Cmd);
	delay_us(10);
    Write_OneByte_ToDS18b20(Read_Scratchpad); //��ȡ�Ĵ������ݣ����ԴӼĴ���0�����Ĵ���8��
    
    a= Read_OneByte_FromDS18b20();     //�¶ȵ�8λ
    b= Read_OneByte_FromDS18b20();     //�¶ȸ�8λ
    //c= Read_OneByte_FromDS18B20();   //TH
    //d= Read_OneByte_FromDS18B20();   //TL
    //e= Read_OneByte_FromDS18B20();   //Configuration Register
      
    Tx_ResetPulse();  //�ж����ݶ�ȡ
	
    tmp = (b<<8) | a ;
	 
   if(b & 0xF0)
    {
    *sign = 0 ;              //���Ų���
    tmp = ~tmp+1 ;
    }
    else 
    {
     *sign = 1 ;
    }
	  				   
    *temp = tmp*0.625;  //��������

}
/******************************************
�������ƣ�Write_EEPROM
��    �ܣ�д���ò���
��    ����Th - �����¶�����
          Tl - �����¶�����
          Register_Con - ���ƼĴ�����ֵ
����ֵ  ������������
*******************************************/
void Write_EEPROM(unsigned char Th,unsigned char Tl,unsigned char Register_Con )
{
  
    DS18B20_Init();
    Write_OneByte_ToDS18b20(ROM_Skip_Cmd);
    Write_OneByte_ToDS18b20(Write_Scratchpad);
    
    Write_OneByte_ToDS18b20(Th);//Th=7F
    Write_OneByte_ToDS18b20(Tl);//Tl=FF ���λ����λ
    Write_OneByte_ToDS18b20(Register_Con);//12λģʽ
    
    delay_ms(100);
    DS18B20_Init();
    Write_OneByte_ToDS18b20(ROM_Skip_Cmd);
    Write_OneByte_ToDS18b20(Copy_Scratchpad);//���Ĵ���������ֵд��EEPROM
    
    delay_ms(100);
 
}
/******************************************
�������ƣ�DS18B20_Init
��    �ܣ���ʼ��DS18b20
��    ������
����ֵ  ����
*******************************************/
void DS18B20_Init(void)
{
    static uint8_t  Flag = 0;
    if(Flag == 0){ //��
        RCC_AHBPeriphClockCmd(DS18B20_PCLOCK, ENABLE); 
        Flag = 1;     
    }
    Tx_ResetPulse();
    Rx_PresencePulse(); 
}
