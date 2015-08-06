#ifndef _STATIC_INIT_H_	
#define _STATIC_INIT_H_

#ifdef __cplusplus		   //�����CPP����C���� //��ʼ����
extern "C" {
#endif

#include  "stm32l1xx.h"
#include "String.h"				//memcpy
#include "math.h"				//��ѧ����
#include <stdio.h>   			//printf

#define rotl(x,n) ((x>>(sizeof(x)*8-n)) | (x<<n) ) 		 //����ѭ��
#define rotr(x,n) ((x<<(sizeof(x)*8-n)) | (x>>n) )		 //����ѭ��

#define DebugPf			printf			//����printf���

typedef struct
{
	uint32_t ValidFlag;				//�궨����
	float  WeightVal[10];		//����ֵ
	float  VotageVal[10];		//ģ��ֵ
} CALIBRATION;					//ţ�ٲ�ֵ�궨

typedef struct
{
	uint32_t			*QStart;		        //��ʼλ��
	uint32_t			*QEnd;		            //����λ��
	uint32_t			*QIn;				    //����λ��
	uint32_t			*QOut;				    //���λ��
	uint32_t			Count;				    //����
} FBuff;

extern uint8_t test;						//�����¶ϵ���
extern CALIBRATION CalibrationSave;	//ţ�ٲ�ֵ�궨
extern uint8_t Sign(uint8_t x);
void L_From_B(uint16_t* Val,uint32_t size);	//ð����С����

uint16_t StoB_2_BtoS(uint16_t n);				//12ת21
uint32_t StoB_4_BtoS(uint32_t n);				//1234ת4321 ����
float StoB_F4_BtoS(float f);			//1234ת4321 ������

uint8_t AsciiToHex(char ch);				//����ASCII��A-F.0-9ת16����0x0A-0x0F.0x00-0x09
char HexToAscii(uint8_t nHex);			//����16����0x0A-0x0F.0x00-0x09תASCII��A-F.0-9
void BufferAsciiToHex(uint8_t *pAaciiBuf, uint8_t *pHexBuf, uint16_t nAsciiLen);	 	//���ASCII��ת16����	 4λת2λ
void BufferHexToAscii(uint8_t *pHexBuf,uint8_t *pAaciiBuf , uint16_t nHexLen);			//���16����תASCII��	 2λת4λ

void GetCpuIdHex8(uint8_t* HCpuId);		//��ȡCPUоƬIDֵ8λ uint8_t x[12];
void GetCpuIdHex16(uint16_t *HCpuId);		//��ȡCPUоƬIDֵ16λ uint16_t x[6];
void GetCpuIdHex32(uint32_t *HCpuId);		//��ȡCPUоƬIDֵ32λ uint32_t x[3];

uint8_t NewTon_Num(uint16_t flag);									//���ҵ�ǰ�궨ʹ�ø���
void NewTon_Num_init(CALIBRATION* CS,uint8_t num_Temp);		//ţ�ٲ��̼���
float Ca_NewTon(float ADC_temp,CALIBRATION* CS,uint8_t x);	//ţ�ٲ�ֵ����
long Sample_Wave(float* AD_New,float* AD_Old,long ADOld_Len,float Wave_Temp);	//����˲�
uint32_t ASCIItoNumber(uint8_t* data_temp,uint8_t len_temp);			//ASCIIת����
double ASCIItoStarNumber(uint8_t* data_temp,uint8_t len_temp,uint8_t RealLen);	//ASCIIתС��
uint32_t BeiNumber(uint32_t num_temp,uint8_t len_temp);					//���������λ�����з���
uint8_t JZ_chack(void);  										//8M������������ OSC_IN(5,12,23)	OSC_OUT(6,13,24)
void RCC_USEHSI(void);									//ʹ��STM32�ڲ�RCʱ�� ����ȷ ��
void FIFO(uint32_t* x,uint32_t len, uint32_t val);								//ϵͳ����
uint32_t AddKey(uint32_t Data);			//����Կ
uint32_t SubKey(uint32_t Data);			//����Կ
uint32_t Fsave(FBuff* FQ,uint32_t val,uint32_t BuffLen);				//�����ȳ���
uint8_t Fget(uint32_t* val,FBuff* FQ);				//����ȡ
uint8_t FindBo(uint32_t val,uint32_t* GetBoVal);			//�Ҳ���
char* FindString(char* Data,uint32_t DataLen,char* StringVal,uint32_t StringLen);	//�ַ�������
uint32_t FindNoZero(uint8_t* Data,uint32_t DataLen);		//�ҷ������
extern uint8_t *itoa(int32_t value, uint8_t *string, uint8_t radix);
#ifdef __cplusplus		   //�����CPP����C���� //��������
}
#endif

#endif

