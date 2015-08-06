#include "static_init.h"


uint8_t test=0;					//�����¶ϵ���
uint8_t HardFault_Handler_X = 0;	//�Ŵ���

uint8_t Sign(uint8_t x)
{
    return ((x==1) ? '+' : '-');
}

/****************************************************************************
* ��	�ƣ�uint16_t StoB_2_BtoS(uint16_t n)				 //12ת21
* ��	�ܣ�2�ֽڵĴ�С��ת��	   
* ��ڲ�����n ת��ǰ����
* ���ڲ�����ת�������
* ˵	������ unsigned int 2�ֽڵ���
****************************************************************************/
uint16_t StoB_2_BtoS(uint16_t n)				 //12ת21
{
	char nTmp;

	nTmp = ((char*)&n)[0];
	((char*)&n)[0] = ((char*)&n)[1];
	((char*)&n)[1] = nTmp;

	return n;
}

/****************************************************************************
* ��	�ƣ�uint32_t StoB_4_BtoS(uint32_t n)				  //1234ת4321
* ��	�ܣ�4�ֽڵĴ�С��ת��(����)	   
* ��ڲ�����n ת��ǰ����
* ���ڲ�����ת�������
* ˵	������ long int 4�ֽڵ���
****************************************************************************/
uint32_t StoB_4_BtoS(uint32_t n)				  //1234ת4321
{
	char nTmp;

	nTmp = ((char*)&n)[0];
	((char*)&n)[0] = ((char*)&n)[3];
	((char*)&n)[3] = nTmp;

	nTmp = ((char*)&n)[1];
	((char*)&n)[1] = ((char*)&n)[2];
	((char*)&n)[2] = nTmp;	

	return n;
}

/****************************************************************************
* ��	�ƣ�float StoB_F4_BtoS(float f)				   //1234ת4321
* ��	�ܣ�4�ֽڵĴ�С��ת��(������)		   
* ��ڲ�����n ת��ǰ����
* ���ڲ�����ת�������
* ˵	������ float 4�ֽڵ���
****************************************************************************/
float StoB_F4_BtoS(float f)				   //1234ת4321
{
	char nTmp;

	nTmp = ((char*)&f)[0];
	((char*)&f)[0] = ((char*)&f)[3];
	((char*)&f)[3] = nTmp;

	nTmp = ((char*)&f)[1];
	((char*)&f)[1] = ((char*)&f)[2];
	((char*)&f)[2] = nTmp;	

	return f;
}

/****************************************************************************
* ��	�ƣ�uint8_t AsciiToHex(char ch)
* ��	�ܣ�����ASCII��ת16����		   
* ��ڲ�����ch	'A'-'F'��'0'-'9'
* ���ڲ�����ת�����0x0A-0x0F.0x00-0x09
* ˵	����ASCII��A-F.0-9ת16����0x0A-0x0F.0x00-0x09
****************************************************************************/
uint8_t AsciiToHex(char ch)
{
	if(ch >= '0' && ch <= '9')	//����0~9
	{
		return (ch - 0x30);
	}
	else
	{
		if(ch >= 'A' && ch <= 'F')	//��ĸA~F
		{
			return (ch - 0x37);
		}
		else
		{
			return 0xFF;
		}
	}
}

/****************************************************************************
* ��	�ƣ�char HexToAscii(uint8_t nHex)
* ��	�ܣ�����16����תASCII��		   
* ��ڲ�����nHex	0x0A-0x0F.0x00-0x09
* ���ڲ�����ת�����ASCII��
* ˵	����16����0x0A-0x0F.0x00-0x09תASCII��A-F.0-9
****************************************************************************/
char HexToAscii(uint8_t nHex)
{
	if(nHex <= 9)	//����0~9
	{
		return (nHex + 0x30);
	}
	else
	{
		if(nHex >= 0x0A && nHex <= 0x0F)	//��ĸA~F
		{
			return (nHex + 0x37);
		}
		else
		{
			return 0xFF;
		}
	}
}

/****************************************************************************
* ��	�ƣ�void BufferAsciiToHex(uint8_t *pAaciiBuf, uint8_t *pHexBuf, uint16_t nAsciiLen)
* ��	�ܣ����ASCII��ת16����		   
* ��ڲ�����*pAaciiBuf	ASCII����������	   Դ
			*pHexBuf	16���ƴ������	   ���
			nAsciiLen	ת������
* ���ڲ�������
* ˵	����uint8_t x[4]="ABCD";
			uint8_t z[2]={0};		ת������{0xAB,0xCD}		 4λת2λ
****************************************************************************/
void BufferAsciiToHex(uint8_t *pAaciiBuf, uint8_t *pHexBuf, uint16_t nAsciiLen)
{
	uint16_t i;
	uint8_t temp;
	if(pAaciiBuf == 0)
		return;
	if(pHexBuf == 0)
		return;

	for(i=0; i<nAsciiLen; i+=2)
	{
		temp = AsciiToHex(pAaciiBuf[i]);
		if(temp == 0xFF)
			return;

		pHexBuf[i>>1] = temp;//��4λ
		pHexBuf[i>>1] <<= 4;

		temp = AsciiToHex(pAaciiBuf[i+1]);
		if(temp == 0xFF)
			return;

		pHexBuf[i>>1] += temp;//��4λ
	}
}


/****************************************************************************
* ��	�ƣ�void BufferHexToAscii(uint8_t *pHexBuf,uint8_t *pAaciiBuf , uint16_t nHexLen)
* ��	�ܣ����16����תASCII��	   
* ��ڲ�����*pAaciiBuf	ASCII��������	   ���
			*pHexBuf	16������������	   Դ
			nHexLen	ת������
* ���ڲ�������
* ˵	����uint8_t x[2]={0xAB,0XCD};
			uint8_t z[4]={0};		ת������{65,66,67,68}		 2λת4λ
****************************************************************************/
void BufferHexToAscii(uint8_t *pHexBuf,uint8_t *pAaciiBuf , uint16_t nHexLen)
{
	uint16_t i;
	if(pAaciiBuf == 0)
		return;
	if(pHexBuf == 0)
		return;

	for(i=0; i<nHexLen; i++)
	{
		pAaciiBuf[i<<1] = HexToAscii(pHexBuf[i]>>4);
		if(pAaciiBuf[i] == 0xFF)
			return;

		
		pAaciiBuf[(i<<1)+1] = HexToAscii(pHexBuf[i] & 0x0F);
		if(pAaciiBuf[(i<<1)+1] == 0xFF)
			return;
	}
}

/****************************************************************************
* ��	�ƣ�void GetCpuIdHex32(uint32_t *HCpuId)
* ��	�ܣ���ȡCPUоƬIDֵ32λ   
* ��ڲ�������
* ���ڲ�������
* ˵	����3��uint32_t��ֵ���CPUID
			uint32_t x[3]={0};
			0x22A6FF32
			0x32375931
			0x43097342
****************************************************************************/
void GetCpuIdHex32(uint32_t *HCpuId)
{
	HCpuId[0] = (*(uint32_t*)(0x1FFFF7E8));
	HCpuId[1] = (*(uint32_t*)(0x1FFFF7EC));
	HCpuId[2] = (*(uint32_t*)(0x1FFFF7F0));
}

/****************************************************************************
* ��	�ƣ�void GetCpuIdHex16(uint16_t *HCpuId)
* ��	�ܣ���ȡCPUоƬIDֵ16λ   
* ��ڲ�������
* ���ڲ�������
* ˵	����6��uint16_t��ֵ���CPUID
			uint16_t x[6]={0};
			0xFF32
			0x22A6
			0x5931
			0x3237
			0x7342
			0x4309
****************************************************************************/
void GetCpuIdHex16(uint16_t *HCpuId)
{
	*&HCpuId[0] = (*(uint32_t*)(0x1FFFF7E8));
	*&HCpuId[1] = (*(uint32_t*)(0x1FFFF7EA));
	*&HCpuId[2] = (*(uint32_t*)(0x1FFFF7EC));
	*&HCpuId[3] = (*(uint32_t*)(0x1FFFF7EE));
	*&HCpuId[4] = (*(uint32_t*)(0x1FFFF7F0));
	*&HCpuId[5] = (*(uint32_t*)(0x1FFFF7F2));
}

/****************************************************************************
* ��	�ƣ�void GetCpuIdHex8(uint8_t* HCpuId)
* ��	�ܣ���ȡCPUоƬIDֵ8λ   
* ��ڲ�������
* ���ڲ�������
* ˵	����12��uint8_t��ֵ���CPUID
			uint8_t x[12]={0};
			0x22 0xA6 0xFF 0x32 0x32 0x37 0x59 0x31 0x43 0x09 0x73 0x42
****************************************************************************/
void GetCpuIdHex8(uint8_t* HCpuId)
{
	uint32_t CpuId[3]={0};

	CpuId[0] = (*(uint32_t*)(0x1FFFF7E8));
	CpuId[1] = (*(uint32_t*)(0x1FFFF7EC));
	CpuId[2] = (*(uint32_t*)(0x1FFFF7F0));

   	HCpuId[0] = (uint8_t)(CpuId[0]>>24);
	HCpuId[1] = (uint8_t)(CpuId[0]>>16);
	HCpuId[2] = (uint8_t)(CpuId[0]>>8);
	HCpuId[3] = (uint8_t)(CpuId[0]);
	HCpuId[4] = (uint8_t)(CpuId[1]>>24); 
	HCpuId[5] = (uint8_t)(CpuId[1]>>16);
	HCpuId[6] = (uint8_t)(CpuId[1]>>8);
	HCpuId[7] = (uint8_t)(CpuId[1]);
	HCpuId[8] = (uint8_t)(CpuId[2]>>24); 
	HCpuId[9] = (uint8_t)(CpuId[2]>>16);
	HCpuId[10] = (uint8_t)(CpuId[2]>>8);
	HCpuId[11] = (uint8_t)(CpuId[2]);
	
	
		
}


/****************************************************************************
* ��	�ƣ�void L_From_B(float* Val,uint32_t size)   //ð��������С����
* ��	�ܣ�����������������С��������
* ��ڲ�����float* Val		��Ҫ���������
			uint32_t size 		�������
* ���ڲ�������
* �㷨˵������������С��������
****************************************************************************/
void L_From_B(uint16_t* Val,uint32_t size)   //ð��������С����
{ 
	float temp;	//��ʱ���
	uint32_t i=0;  	//�Ա���
	uint32_t j=0;   	//�Ա���

	for(;i<size-1;++i)  
	{  
		for(j=0;j<size-i-1;++j)  
		{   
			if(Val[j]>Val[j+1])  
			{
				//��������ֵ
				temp=Val[j];
				Val[j]=Val[j+1];
				Val[j+1]=temp;
			}	
		}  
	} 
}

/****************************************************************************
* �� �� ����ţ�ٲ�ֵ
* ��	�ƣ�float Ca_NewTon(void)
* ��	�ܣ�ţ�ٲ�ֵ����
* ��ڲ�������
* ���ڲ�����0��		�д����ȱ����Ч�궨���� 
			����:	ת�����
* �㷨˵����
	WeighParam_Ten	����ֵ
	ADCAverage		ģ��ֵ

	uint8_t NewTon_Num(uint16_t flag);	//���ұ궨����
	void NewTon_Num_init(CALIBRATION* CS,uint8_t num_Temp);	//����ţ�ٲ���

* ���ԣ�
	CALIBRATION CalibrationSave={15,{8.75F,47.35F,107.85F,146.1},{1.035F,1.404F,1.979F,2.342}};
	uint8_t num;
	float weight;		  //127.0
	num=NewTon_Num(CalibrationSave.ValidFlag);	//���ҵ�ǰ�궨ʹ�ø���
	NewTon_Num_init(&CalibrationSave,num);		//ţ�ٲ��̼���
	weight=Ca_NewTon(2.161F,&CalibrationSave,num);	//ţ�ٲ�ֵ����
****************************************************************************/
CALIBRATION CalibrationSave={15,			  //�궨����
		{8.75F,47.35F,107.85F,146.1F},		  //����ֵ
		{1.035F,1.404F,1.979F,2.342F}};		  //ģ��ֵ

//���ҵ�ǰ�궨ʹ�ø���
//����	uint16_t flag	�궨������¼ 	������
//����	uint8_t			�궨���� 		ʮ����
uint8_t NewTon_Num(uint16_t flag)
{
	uint8_t number=0;
	for(;( (flag & 0x01) ==1);)
	{
		number++;
		flag=flag>>1;
	}
	return number;
}

//ţ�ٲ��̼���
//����	CALIBRATION* CS	�궨
//		uint8_t num_Temp		�궨���� ʮ����
void NewTon_Num_init(CALIBRATION* CS,uint8_t num_Temp)
{
	float sum;			   //�����
	float mul;			   //�����
	float temp[10]={0.0F};	   //��ʱ�����ŵ�
	uint8_t k;	//�ⲿ�Ա���
	uint8_t i;	//�궨�±���
	uint8_t j;	//�ڲ��Ա���

	for (i = 0; i < num_Temp; i++)
	{
		for (sum = 0.0F, k = 0; k <= i; k++)
		{
			for (mul = 1.0F, j = 0; j <= i; j++)
			{
				if (j != k)
				{
					mul = mul * (CS->VotageVal[k] - CS->VotageVal[j]);
				}
			}
			sum = sum + CS->WeightVal[k] / mul;
		}
		temp[i] = sum;
	}
	memcpy(CS->WeightVal,temp,4*10);
}

//ţ�ٲ�ֵ����
//����	float ADC_temp	��ǰģ��ֵ
//		CALIBRATION* CS	�궨
//		uint8_t x			�궨ʹ�ø���	ʮ����
float Ca_NewTon(float ADC_temp,CALIBRATION* CS,uint8_t x)
{
    uint8_t i,j;
	float OutYP,mul;

	for (OutYP = CS->WeightVal[0], i = 1; i < x; i++)
	{
		for (mul = CS->WeightVal[i], j = 0; j < i; j++)
		{
			mul = mul * (ADC_temp - CS->VotageVal[j]);
		}	
		OutYP = OutYP + mul;
	}
    return OutYP;
}

/****************************************************************************
* ��	�ƣ�long Sample_Wave(float* AD_New,float* AD_Old,long ADOld_Len,float Wave_Temp)
* ��	�ܣ�����˲�
* ��ڲ�����float* AD_New	����˲���ɺ����������
			float* AD_Old	�����δ�����˲�����������
			long ADOld_Len	δ�˲������ݸ���
			float Wave_Temp	��������ֵ��ƫ�Χ
* ���ڲ�����long	�˲������ݸ���
* �㷨˵����x[10]
x[1]��x[0]�Ƚϣ�����ƫ��ֵ��Ҫ��δ������ȡ��
x[2]��x[1]�Ƚϣ�����
���ԣ�
#define old_len 100		  //δ�˲������ݸ���
float Wave=1000.0F;		  //��������ֵ��ƫ�Χ
float old[100]={0.0F};	  //�����δ�����˲�����������
float new[100]={0.0F};	  //����˲���ɺ����������
long i=0;	  //�ۼ���
long x=0;	  //�˲������ݸ���
	for(i=0;i<100;i++)
		old[i]=i*2;
	old[10]=1000.0F;
	old[20]=2000.0F;
	old[30]=3000.0F;
	old[40]=4000.0F;	  //׼����������

	x=Sample_Wave(new,old,old_len,Wave);	//�˲�
****************************************************************************/
long Sample_Wave(float* AD_New,float* AD_Old,long ADOld_Len,float Wave_Temp)
{
	long ko=1; //δ�����˲��������±�
	long kn=1; //�˲����������±�

	AD_New[0]=AD_Old[0];		  //��һ��ֱ�Ӹ�ֵ

	for(ko=1;ko<ADOld_Len;ko++)	  //δ�˲������ͷ��β����
	{
		if(fabs(AD_Old[ko]-AD_New[kn-1])<=Wave_Temp)	  //�����������ľ��Բ�ֵ������ֵ���бȽ�
		{
			AD_New[kn]=AD_Old[ko];	//δ�����ͽ���ȡֵ
			kn++;	//�˲������±�����
		}
	}
	return kn;
}

/****************************************************************************
* ��	�ƣ�uint32_t BeiNumber(uint32_t num_temp,uint8_t len_temp)
* ��	�ܣ�10����
* ��ڲ�����uint32_t num_temp	��Ҫ��������
			uint8_t len_temp	�����ٱ� ��1������ٸ�0
* ���ڲ�����uint32_t ������ı���ֵ
* ���Դ��룺
	uint32_t x=2;
	x=BeiNumber(x,3);
	//x=2000;
****************************************************************************/
uint32_t BeiNumber(uint32_t num_temp,uint8_t len_temp)
{
     uint32_t Bei=1;		  //Ĭ�ϱ���ֵ
	 	
     while(len_temp--)//���������з���
          Bei*=10;	  //��������
     return Bei*num_temp;	  //���ر���
}

/****************************************************************************
* ��	�ƣ�uint32_t ASCIItoNumber(uint8_t* data_temp,uint8_t len_temp)
* ��	�ܣ���ASCII������ת����Ϊuint32_t������
* ��ڲ�����uint8_t* data_temp	ASCII������
			uint8_t len_temp		ASCII���������ָ���
* ���ڲ�����uint32_t unsigned int������
* ���Դ��룺
	uint32_t num=0;
	num=ASCIItoNumber("1234",4);
	//num=1234;

	num = ASCIItoNumber("123.45678",3);
	num = 123;
	num = ASCIItoNumber("35.124",2);
	num = 35;
	num = ASCIItoNumber("123.458",3);
	num = 123;
	num = ASCIItoNumber("3.14159",1);
	num = 3;
****************************************************************************/
uint32_t ASCIItoNumber(uint8_t* data_temp,uint8_t len_temp)
{
    uint8_t i = 0;					//����
    uint32_t val=0; 
    
    while(1)
    {
        val = val * 10 + data_temp[i] - 0x30;     //ȡ���� 
        
        i++;				   //����ĸ�������
        if(i >= len_temp)	   //����ĸ�������ϣ������ĸ���
           return val;
    } 
}

/****************************************************************************
* ��	�ƣ�double ASCIItoStarNumber(uint8_t* data_temp,uint8_t len_temp,uint8_t RealLen)
* ��	�ܣ���ASCII������ת��С��
* ��ڲ�����uint8_t* data_temp	ASCII������
			uint8_t len_temp		С������
			uint8_t RealLen		ʵ�������ܳ��ȸ���
* ���ڲ�����double	С��
* ���Դ��룺
	double num = 0.0F;

	num = ASCIItoStarNumber("123.45678",5,9);
	//num = 0.45678;
	num = ASCIItoStarNumber("35.124",3,6);
	//num = 0.124;
	num = ASCIItoStarNumber("123.458",3,7);
	//num = 0.458;
	num = ASCIItoStarNumber("3.14159",5,7);
	//num = 0.14159
****************************************************************************/
double ASCIItoStarNumber(uint8_t* data_temp,uint8_t len_temp,uint8_t RealLen)
{
    uint8_t i = RealLen - 1;		//����
	uint8_t fi = 0;				//�����˵ĸ���

    double val = 0.0F;

    while(1)
    {
		val = val * 0.1F + (data_temp[i] - 0x30) * 0.1F; 	  //�������С��
		
		i--;					  //�±�λ�ñ仯
		fi++;					  //����ĸ�������

		if(fi >= len_temp)		  //����ĸ�������ϣ������ĸ���
		   return (double)val;
    } 
}

/****************************************************************************
* ��	�ƣ�uint8_t JZ_chack(void)
* ��	�ܣ�8M������������ 
* ��ڲ�������
* ���ڲ�����uint8_t	1����	0û����
* ˵    ����OSC_IN(5,12,23)	
			OSC_OUT(6,13,24)
* ���Դ���			
	flag=JZ_chack();
	if(flag==1)		//����
		flag=9;
	else			//û����
		flag=9;
****************************************************************************/
uint8_t JZ_chack(void)
{
	uint8_t flag=0;		 //������������ʱ

//	RCC_DeInit();			 		//������ RCC�Ĵ�������Ϊȱʡֵ
//	RCC_HSEConfig(RCC_HSE_ON);		//�����ⲿ8M���پ���(HSE)
									//RCC_HSE_OFF  		HSE 	���� OFF 
									//RCC_HSE_ON  		HSE 	���� ON 
									//RCC_HSE_Bypass  	HSE 	�����ⲿʱ����·
//	flag = RCC_WaitForHSEStartUp();	//�ȴ� HSE ���񣬸ú������ȴ�ֱ�� HSE �����������ڳ�ʱ��������˳� 
	if(flag == SUCCESS)
		return 1;
	else
		return 0;
}

/****************************************************************************
* ��	�ƣ�void RCC_USEHSI(void)
* ��	�ܣ�����STM32�ڲ�RC����
* ��ڲ�������
* ���ڲ�������
* ˵	����
		���ʹ���ڲ�RC��������ʹ���ⲿ�����밴�����淽������
		1 ����100�Ż�144�ŵĲ�Ʒ��OSC_INӦ�ӵأ�OSC_OUTӦ���ա�
		2 ��������100�ŵĲ�Ʒ����2�ֽӷ���
			2.1 OSC_IN��OSC_OUT�ֱ�ͨ��10K����ӵء��˷��������EMC���ܡ�
			2.2 �ֱ���ӳ��OSC_IN��OSC_OUT��PD0��PD1��������PD0��PD1Ϊ������������'0'���˷������Լ�С���Ĳ�(�������2.1)��ʡ2���ⲿ���衣
* ��    �ԣ�
	uint32_t x=0;
	while(1)
	{
		DebugPf("--- %d ---\r\n",x);
		x++;
		Delay_S(1);
	}
****************************************************************************/
void RCC_USEHSI(void)
{

}

/****************************************************************************
* ��	�ƣ�uint32_t AddCode(uint32_t Data)
* ��	�ܣ���ԭʼ����������Կ 
* ��ڲ�����uint32_t Data	ԭʼ����
* ���ڲ�����uint32_t	����Կ�������
* ˵    ������
****************************************************************************/
uint32_t AddKey(uint32_t Data)
{
	Data=rotr(Data,13);	  //ѭ������13λ
	return Data;
}

/****************************************************************************
* ��	�ƣ�uint32_t SubKey(uint32_t Data)
* ��	�ܣ�������Կ�����ݼ�ȥ��Կ 
* ��ڲ�����uint32_t Data	����Կ�������
* ���ڲ�����uint32_t	ԭʼ����
* ˵    ������
****************************************************************************/
uint32_t SubKey(uint32_t Data)
{
	Data=rotl(Data,13);	  //ѭ������13λ
	return Data;
}

/*
	uint32_t srcval=0xABCDEF12;
	uint32_t srcval_T=0;

	srcval_T = AddKey(srcval);
	DebugPf("Src = %X\t A = %X\r\n",srcval,srcval_T);
	srcval_T = SubKey(srcval);	
	DebugPf("Src = %X\t S = %X\r\n",srcval,srcval_T);
	DebugPf("Src = %X\t S = %X\r\n",srcval,AddKey(srcval_T));
*/

/****************************************************************************
* ��	�ƣ�void FIFO(uint32_t* x,uint32_t len, uint32_t val)
* ��	�ܣ��Ƚ��ȳ�����	����
* ��ڲ�����uint32_t* x			��Ż�����
			uint32_t len			����������
			uint32_t val			����ֵ
* ���ڲ�������
* ˵	������
****************************************************************************/
void FIFO(uint32_t* x,uint32_t len, uint32_t val)
{
	uint32_t i;
	for(i = 0;i < (len - 1);i++) 
	{
		x[i] = x[i + 1];
	}
	x[len - 1] = val;   
}

/****************************************************************************
* ��	�ƣ�uint32_t Fsave(FBuff* FQ,uint32_t val,uint32_t BuffLen)
* ��	�ܣ����д�ֵ
* ��ڲ�����FBuff* FQ	��Ŷ���
			uint32_t* val	�����ֵ
			uint32_t BuffLen	���������
* ���ڲ�����uint32_t			��ǰ������еĸ���
* ˵	���������ȳ���	ָ��
****************************************************************************/
uint32_t Fsave(FBuff* FQ,uint32_t val,uint32_t BuffLen)
{
	*(FQ->QIn) = val;			//����λ�ô���
	FQ->QIn++;					//����λ������
	if(FQ->QIn > FQ->QEnd)		//�����ַ���������ַ
		FQ->QIn = FQ->QStart;	//������ʼ��ַ
	if(FQ->Count < BuffLen)		//����ûװ���������
		FQ->Count++;	 			//����װ
	return FQ->Count;			//���ظ���
}

/****************************************************************************
* ��	�ƣ�uint8_t Fget(uint32_t* val,FBuff* FQ)
* ��	�ܣ�����ȡֵ
* ��ڲ�����uint32_t* val	ȡ����ֵ
			FBuff* FQ	���ݶ���
* ���ڲ�����uint8_t	0		ȡֵʧ��
				1		ȡֵ�ɹ�
* ˵	����������ȡ	ָ��
****************************************************************************/
uint8_t Fget(uint32_t* val,FBuff* FQ)
{
	if(FQ->Count > 0)				//����������
	{
		FQ->QIn --;					//����λ�ú���
		if(FQ->QIn < FQ->QStart)
			FQ->QIn = FQ->QEnd;	//������ڱȵ�һ��λ�û�С��λ����Ϊ��ʼλ��
		FQ->QOut = FQ->QIn;		//�õ����һ��ֵ
		*val = *(FQ->QOut);			//ȡֵ
		FQ->Count--;					//����ĸ�������
		return 1;						//ȡֵ�ɹ�
	}
	else								//������û��
		return 0;						//ȡֵʧ��
}

/* ���г�ʼ������
#define maxADlen 5				//���ݴ������С
static uint32_t DownData[maxADlen]={0};	//���ݴ����
FBuff DownAD = 
{
	DownData,				//��ʼλ��	Start
	DownData+(maxADlen-1),		//����λ��	End
	DownData,				//����λ��	In
	DownData,				//���λ��	Out	��ʱ
	0,						//����
};
*/

/****************************************************************************
* ��	�ƣ�uint8_t FindBo(uint32_t val,uint32_t* GetBoVal)
* ��	�ܣ��Ҳ���
* ��ڲ�����uint32_t val			����ADֵ
			uint32_t* GetBoVal	�������ֵ
* ���ڲ�����uint8_t �Ƿ��в����γ�	1�γ�	0û�γ�
* ˵	�����м����������߼�Ϊ����
****************************************************************************/
static uint32_t BoBuff[3]={0};
static uint8_t i=0;
FBuff ADB = 
{
	BoBuff,				//��ʼλ��	Start
	BoBuff+(3-1),		//����λ��	End
	BoBuff,				//����λ��	In
	BoBuff,				//���λ��	Out	��ʱ
	0,						//����
};
uint8_t FindBo(uint32_t val,uint32_t* GetBoVal)
{
	if(i<2)	//������ʼ��	װ��ǰ2������
	{
		Fsave(&ADB,val,3);			//װ����
		i++;				
		return 0;			//û�õ�����
	}

	Fsave(&ADB,val,3);			//װ����

	if(BoBuff[1] > BoBuff[0] && BoBuff[1] > BoBuff[2])
	{
		*GetBoVal = BoBuff[1];	//�õ�����ֵ
		return 1;				//�õ�����
	}
	return 0;
}

/****************************************************************************
* ��	�ƣ�uint8_t* FindString(uint8_t* Data,uint32_t DataLen,uint8_t* StringVal,uint32_t StringLen)
* ��	�ܣ����ַ��������ַ�������λ
* ��ڲ�����uint8_t* Data		����ַ���������Դ
			uint32_t DataLen		����Դ������
			uint8_t* StringVal	Ҫ���ҵ��ַ���
			uint32_t StringLen	Ҫ���ҵ��ַ�������
* ���ڲ�����uint8_t*	��λ�ַ���������Դ�е�λ�ã���λ�ַ�����λ��	û�ҵ�����ָ��0��ַ��ָ��
* ˵	����
uint8_t a[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};	//26
uint8_t b[1] = 'a';								//1
uint8_t* f = 0;
f = FindString(a,sizeof(a),b,sizeof(b));
****************************************************************************/
char* FindString(char* Data,uint32_t DataLen,char* StringVal,uint32_t StringLen)
{
	char* s = 0;		 //Դ��ַָ��
	char*	p = 0;		 //�Ƚ����ݵ�ַָ��
	uint32_t i = 0;		 //�Ƚ����ݸ���

	if(DataLen == 0 || StringLen == 0 || StringVal == 0 || Data == 0)	 //���ҵ����ݺ�����ԴΪ0����Ϊ��
		return p;

	s = Data;		 //ָ������Դ
	p = StringVal;	 //ָ���������

	while(DataLen > 0)		//Դ����û�Ƚ���
	{
		if(*s != *p)	  	//û����������
		{
			DataLen--;		//�ȽϹ�������
			p = StringVal;	//ָ���������
			if(i != 0)		//ǰ���м��αȽϳɹ��˵�
			{
				s -= (StringLen - 1);		//�Գɹ����ĵڶ�����ʼ���±Ƚ�		abcabcd	�Ƚ�abcd
				i = 0;				//�Ƚϳɹ���������
			} 
		}
		else			  	//�ҵ���������
		{
			i++;			//�Ƚϳɹ���������

			if(i >= StringLen)				//�ҵ�
				return s - (StringLen - 1);	//�Ƶ��Ƚϵ����ַ�λ��

			p++;			//�Ƚ����ݺ���һ��
		}
		s++;				//Դ����λ��
	}

	p = 0;			  //û�ҵ�
	return p;
}

/****************************************************************************
* ��	�ƣ�uint32_t FindNoZero(uint8_t* Data,uint32_t DataLen)
* ��	�ܣ��������в��ҷ������
* ��ڲ�����uint8_t* Data		����
			uint32_t DataLen		����������
* ���ڲ�����uint32_t				�������
* ˵	�����������˳��������;Ϊ0����;�˳�
****************************************************************************/
uint32_t FindNoZero(uint8_t* Data,uint32_t DataLen)
{
	uint32_t i = 0;	//�ۼ���

	if(Data == 0)
		return 0;

	while(Data[i] != 0x00)	//���ݲ�Ϊ0�����ѭ��
	{	
		i++;
		if(i >= DataLen)	//������������˳�ѭ��
			break;
	}	
	return i;
}

/******************************************************
		��������ת�ַ�������
        char *itoa(int value, char *string, int radix)
		radix=10 ��ʾ��10����	��ʮ���ƣ�ת�����Ϊ0;  

	    ����d=-379;
		ִ��	itoa(d, buf, 10); ��
		
		buf="-379"							   			  
**********************************************************/
uint8_t *itoa(int32_t value, uint8_t *string, uint8_t radix)
{
    uint32_t     i, d;
    uint32_t     flag = 0;
    uint8_t  *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} 
