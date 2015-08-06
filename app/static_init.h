#ifndef _STATIC_INIT_H_	
#define _STATIC_INIT_H_

#ifdef __cplusplus		   //定义对CPP进行C处理 //开始部分
extern "C" {
#endif

#include  "stm32l1xx.h"
#include "String.h"				//memcpy
#include "math.h"				//数学计算
#include <stdio.h>   			//printf

#define rotl(x,n) ((x>>(sizeof(x)*8-n)) | (x<<n) ) 		 //左移循环
#define rotr(x,n) ((x<<(sizeof(x)*8-n)) | (x>>n) )		 //左移循环

#define DebugPf			printf			//调试printf输出

typedef struct
{
	uint32_t ValidFlag;				//标定个数
	float  WeightVal[10];		//物理值
	float  VotageVal[10];		//模拟值
} CALIBRATION;					//牛顿插值标定

typedef struct
{
	uint32_t			*QStart;		        //开始位置
	uint32_t			*QEnd;		            //结束位置
	uint32_t			*QIn;				    //输入位置
	uint32_t			*QOut;				    //输出位置
	uint32_t			Count;				    //个数
} FBuff;

extern uint8_t test;						//测试下断点用
extern CALIBRATION CalibrationSave;	//牛顿插值标定
extern uint8_t Sign(uint8_t x);
void L_From_B(uint16_t* Val,uint32_t size);	//冒泡由小到大

uint16_t StoB_2_BtoS(uint16_t n);				//12转21
uint32_t StoB_4_BtoS(uint32_t n);				//1234转4321 整型
float StoB_F4_BtoS(float f);			//1234转4321 浮点型

uint8_t AsciiToHex(char ch);				//单个ASCII码A-F.0-9转16进制0x0A-0x0F.0x00-0x09
char HexToAscii(uint8_t nHex);			//单个16进制0x0A-0x0F.0x00-0x09转ASCII码A-F.0-9
void BufferAsciiToHex(uint8_t *pAaciiBuf, uint8_t *pHexBuf, uint16_t nAsciiLen);	 	//多个ASCII码转16进制	 4位转2位
void BufferHexToAscii(uint8_t *pHexBuf,uint8_t *pAaciiBuf , uint16_t nHexLen);			//多个16进制转ASCII码	 2位转4位

void GetCpuIdHex8(uint8_t* HCpuId);		//获取CPU芯片ID值8位 uint8_t x[12];
void GetCpuIdHex16(uint16_t *HCpuId);		//获取CPU芯片ID值16位 uint16_t x[6];
void GetCpuIdHex32(uint32_t *HCpuId);		//获取CPU芯片ID值32位 uint32_t x[3];

uint8_t NewTon_Num(uint16_t flag);									//查找当前标定使用个数
void NewTon_Num_init(CALIBRATION* CS,uint8_t num_Temp);		//牛顿差商计算
float Ca_NewTon(float ADC_temp,CALIBRATION* CS,uint8_t x);	//牛顿插值计算
long Sample_Wave(float* AD_New,float* AD_Old,long ADOld_Len,float Wave_Temp);	//逐次滤波
uint32_t ASCIItoNumber(uint8_t* data_temp,uint8_t len_temp);			//ASCII转数字
double ASCIItoStarNumber(uint8_t* data_temp,uint8_t len_temp,uint8_t RealLen);	//ASCII转小数
uint32_t BeiNumber(uint32_t num_temp,uint8_t len_temp);					//根据输入的位数进行翻倍
uint8_t JZ_chack(void);  										//8M主晶振起振检测 OSC_IN(5,12,23)	OSC_OUT(6,13,24)
void RCC_USEHSI(void);									//使用STM32内部RC时钟 不精确 慢
void FIFO(uint32_t* x,uint32_t len, uint32_t val);								//系统重启
uint32_t AddKey(uint32_t Data);			//加密钥
uint32_t SubKey(uint32_t Data);			//减密钥
uint32_t Fsave(FBuff* FQ,uint32_t val,uint32_t BuffLen);				//先入先出存
uint8_t Fget(uint32_t* val,FBuff* FQ);				//队列取
uint8_t FindBo(uint32_t val,uint32_t* GetBoVal);			//找波峰
char* FindString(char* Data,uint32_t DataLen,char* StringVal,uint32_t StringLen);	//字符串查找
uint32_t FindNoZero(uint8_t* Data,uint32_t DataLen);		//找非零个数
extern uint8_t *itoa(int32_t value, uint8_t *string, uint8_t radix);
#ifdef __cplusplus		   //定义对CPP进行C处理 //结束部分
}
#endif

#endif

