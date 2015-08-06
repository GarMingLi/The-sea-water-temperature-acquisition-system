#include "static_init.h"


uint8_t test=0;					//测试下断点用
uint8_t HardFault_Handler_X = 0;	//排错用

uint8_t Sign(uint8_t x)
{
    return ((x==1) ? '+' : '-');
}

/****************************************************************************
* 名	称：uint16_t StoB_2_BtoS(uint16_t n)				 //12转21
* 功	能：2字节的大小端转换	   
* 入口参数：n 转换前的数
* 出口参数：转换后的数
* 说	明：如 unsigned int 2字节的数
****************************************************************************/
uint16_t StoB_2_BtoS(uint16_t n)				 //12转21
{
	char nTmp;

	nTmp = ((char*)&n)[0];
	((char*)&n)[0] = ((char*)&n)[1];
	((char*)&n)[1] = nTmp;

	return n;
}

/****************************************************************************
* 名	称：uint32_t StoB_4_BtoS(uint32_t n)				  //1234转4321
* 功	能：4字节的大小端转换(整型)	   
* 入口参数：n 转换前的数
* 出口参数：转换后的数
* 说	明：如 long int 4字节的数
****************************************************************************/
uint32_t StoB_4_BtoS(uint32_t n)				  //1234转4321
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
* 名	称：float StoB_F4_BtoS(float f)				   //1234转4321
* 功	能：4字节的大小端转换(浮点型)		   
* 入口参数：n 转换前的数
* 出口参数：转换后的数
* 说	明：如 float 4字节的数
****************************************************************************/
float StoB_F4_BtoS(float f)				   //1234转4321
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
* 名	称：uint8_t AsciiToHex(char ch)
* 功	能：单个ASCII码转16进制		   
* 入口参数：ch	'A'-'F'或'0'-'9'
* 出口参数：转换后的0x0A-0x0F.0x00-0x09
* 说	明：ASCII码A-F.0-9转16进制0x0A-0x0F.0x00-0x09
****************************************************************************/
uint8_t AsciiToHex(char ch)
{
	if(ch >= '0' && ch <= '9')	//数字0~9
	{
		return (ch - 0x30);
	}
	else
	{
		if(ch >= 'A' && ch <= 'F')	//字母A~F
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
* 名	称：char HexToAscii(uint8_t nHex)
* 功	能：单个16进制转ASCII码		   
* 入口参数：nHex	0x0A-0x0F.0x00-0x09
* 出口参数：转换后的ASCII码
* 说	明：16进制0x0A-0x0F.0x00-0x09转ASCII码A-F.0-9
****************************************************************************/
char HexToAscii(uint8_t nHex)
{
	if(nHex <= 9)	//数字0~9
	{
		return (nHex + 0x30);
	}
	else
	{
		if(nHex >= 0x0A && nHex <= 0x0F)	//字母A~F
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
* 名	称：void BufferAsciiToHex(uint8_t *pAaciiBuf, uint8_t *pHexBuf, uint16_t nAsciiLen)
* 功	能：多个ASCII码转16进制		   
* 入口参数：*pAaciiBuf	ASCII码数据数组	   源
			*pHexBuf	16进制存放数组	   结果
			nAsciiLen	转换个数
* 出口参数：无
* 说	明：uint8_t x[4]="ABCD";
			uint8_t z[2]={0};		转换后结果{0xAB,0xCD}		 4位转2位
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

		pHexBuf[i>>1] = temp;//高4位
		pHexBuf[i>>1] <<= 4;

		temp = AsciiToHex(pAaciiBuf[i+1]);
		if(temp == 0xFF)
			return;

		pHexBuf[i>>1] += temp;//低4位
	}
}


/****************************************************************************
* 名	称：void BufferHexToAscii(uint8_t *pHexBuf,uint8_t *pAaciiBuf , uint16_t nHexLen)
* 功	能：多个16进制转ASCII码	   
* 入口参数：*pAaciiBuf	ASCII码存放数组	   结果
			*pHexBuf	16进制数据数组	   源
			nHexLen	转换个数
* 出口参数：无
* 说	明：uint8_t x[2]={0xAB,0XCD};
			uint8_t z[4]={0};		转换后结果{65,66,67,68}		 2位转4位
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
* 名	称：void GetCpuIdHex32(uint32_t *HCpuId)
* 功	能：获取CPU芯片ID值32位   
* 入口参数：无
* 出口参数：无
* 说	明：3个uint32_t的值存放CPUID
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
* 名	称：void GetCpuIdHex16(uint16_t *HCpuId)
* 功	能：获取CPU芯片ID值16位   
* 入口参数：无
* 出口参数：无
* 说	明：6个uint16_t的值存放CPUID
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
* 名	称：void GetCpuIdHex8(uint8_t* HCpuId)
* 功	能：获取CPU芯片ID值8位   
* 入口参数：无
* 出口参数：无
* 说	明：12个uint8_t的值存放CPUID
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
* 名	称：void L_From_B(float* Val,uint32_t size)   //冒泡排序由小到大
* 功	能：对引入的数组进行由小到大排序
* 入口参数：float* Val		需要排序的数组
			uint32_t size 		数组个数
* 出口参数：无
* 算法说明：浮点数由小到大排列
****************************************************************************/
void L_From_B(uint16_t* Val,uint32_t size)   //冒泡排序由小到大
{ 
	float temp;	//零时存放
	uint32_t i=0;  	//自变量
	uint32_t j=0;   	//自变量

	for(;i<size-1;++i)  
	{  
		for(j=0;j<size-i-1;++j)  
		{   
			if(Val[j]>Val[j+1])  
			{
				//传感器数值
				temp=Val[j];
				Val[j]=Val[j+1];
				Val[j+1]=temp;
			}	
		}  
	} 
}

/****************************************************************************
* 算 法 名：牛顿插值
* 名	称：float Ca_NewTon(void)
* 功	能：牛顿插值计算
* 入口参数：无
* 出口参数：0：		有错误或缺少有效标定个数 
			其它:	转换完成
* 算法说明：
	WeighParam_Ten	物理值
	ADCAverage		模拟值

	uint8_t NewTon_Num(uint16_t flag);	//查找标定个数
	void NewTon_Num_init(CALIBRATION* CS,uint8_t num_Temp);	//计算牛顿差商

* 测试：
	CALIBRATION CalibrationSave={15,{8.75F,47.35F,107.85F,146.1},{1.035F,1.404F,1.979F,2.342}};
	uint8_t num;
	float weight;		  //127.0
	num=NewTon_Num(CalibrationSave.ValidFlag);	//查找当前标定使用个数
	NewTon_Num_init(&CalibrationSave,num);		//牛顿差商计算
	weight=Ca_NewTon(2.161F,&CalibrationSave,num);	//牛顿插值计算
****************************************************************************/
CALIBRATION CalibrationSave={15,			  //标定个数
		{8.75F,47.35F,107.85F,146.1F},		  //物理值
		{1.035F,1.404F,1.979F,2.342F}};		  //模拟值

//查找当前标定使用个数
//参数	uint16_t flag	标定个数记录 	二进制
//返回	uint8_t			标定个数 		十进制
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

//牛顿差商计算
//参数	CALIBRATION* CS	标定
//		uint8_t num_Temp		标定个数 十进制
void NewTon_Num_init(CALIBRATION* CS,uint8_t num_Temp)
{
	float sum;			   //计算和
	float mul;			   //计算乘
	float temp[10]={0.0F};	   //临时计算存放点
	uint8_t k;	//外部自变量
	uint8_t i;	//标定下标量
	uint8_t j;	//内部自变量

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

//牛顿插值计算
//参数	float ADC_temp	当前模拟值
//		CALIBRATION* CS	标定
//		uint8_t x			标定使用个数	十进制
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
* 名	称：long Sample_Wave(float* AD_New,float* AD_Old,long ADOld_Len,float Wave_Temp)
* 功	能：逐次滤波
* 入口参数：float* AD_New	存放滤波完成后的数据数组
			float* AD_Old	引入的未进行滤波的数据数组
			long ADOld_Len	未滤波的数据个数
			float Wave_Temp	相邻两个值的偏差范围
* 出口参数：long	滤波后数据个数
* 算法说明：x[10]
x[1]与x[0]比较，超过偏差值则不要，未超过则取数
x[2]与x[1]比较，……
测试：
#define old_len 100		  //未滤波的数据个数
float Wave=1000.0F;		  //相邻两个值的偏差范围
float old[100]={0.0F};	  //引入的未进行滤波的数据数组
float new[100]={0.0F};	  //存放滤波完成后的数据数组
long i=0;	  //累加数
long x=0;	  //滤波后数据个数
	for(i=0;i<100;i++)
		old[i]=i*2;
	old[10]=1000.0F;
	old[20]=2000.0F;
	old[30]=3000.0F;
	old[40]=4000.0F;	  //准备测试数据

	x=Sample_Wave(new,old,old_len,Wave);	//滤波
****************************************************************************/
long Sample_Wave(float* AD_New,float* AD_Old,long ADOld_Len,float Wave_Temp)
{
	long ko=1; //未进行滤波的数组下标
	long kn=1; //滤波的新数组下标

	AD_New[0]=AD_Old[0];		  //第一个直接赋值

	for(ko=1;ko<ADOld_Len;ko++)	  //未滤波数组从头到尾遍历
	{
		if(fabs(AD_Old[ko]-AD_New[kn-1])<=Wave_Temp)	  //相邻两个数的绝对差值与额定波动值进行比较
		{
			AD_New[kn]=AD_Old[ko];	//未超过就进行取值
			kn++;	//滤波数组下标增加
		}
	}
	return kn;
}

/****************************************************************************
* 名	称：uint32_t BeiNumber(uint32_t num_temp,uint8_t len_temp)
* 功	能：10倍数
* 入口参数：uint32_t num_temp	需要翻倍的数
			uint8_t len_temp	翻多少倍 即1后面多少个0
* 出口参数：uint32_t 翻倍后的倍数值
* 测试代码：
	uint32_t x=2;
	x=BeiNumber(x,3);
	//x=2000;
****************************************************************************/
uint32_t BeiNumber(uint32_t num_temp,uint8_t len_temp)
{
     uint32_t Bei=1;		  //默认倍数值
	 	
     while(len_temp--)//按倍数进行翻倍
          Bei*=10;	  //翻倍增加
     return Bei*num_temp;	  //返回倍数
}

/****************************************************************************
* 名	称：uint32_t ASCIItoNumber(uint8_t* data_temp,uint8_t len_temp)
* 功	能：将ASCII码数字转换成为uint32_t型数字
* 入口参数：uint8_t* data_temp	ASCII码数字
			uint8_t len_temp		ASCII码整数部分个数
* 出口参数：uint32_t unsigned int型数字
* 测试代码：
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
    uint8_t i = 0;					//长度
    uint32_t val=0; 
    
    while(1)
    {
        val = val * 10 + data_temp[i] - 0x30;     //取个数 
        
        i++;				   //处理的个数增加
        if(i >= len_temp)	   //处理的个数大于希望处理的个数
           return val;
    } 
}

/****************************************************************************
* 名	称：double ASCIItoStarNumber(uint8_t* data_temp,uint8_t len_temp,uint8_t RealLen)
* 功	能：将ASCII码数字转换小数
* 入口参数：uint8_t* data_temp	ASCII码数字
			uint8_t len_temp		小数个数
			uint8_t RealLen		实际数字总长度个数
* 出口参数：double	小数
* 测试代码：
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
    uint8_t i = RealLen - 1;		//长度
	uint8_t fi = 0;				//处理了的个数

    double val = 0.0F;

    while(1)
    {
		val = val * 0.1F + (data_temp[i] - 0x30) * 0.1F; 	  //整理出来小数
		
		i--;					  //下标位置变化
		fi++;					  //处理的个数增加

		if(fi >= len_temp)		  //处理的个数大于希望处理的个数
		   return (double)val;
    } 
}

/****************************************************************************
* 名	称：uint8_t JZ_chack(void)
* 功	能：8M主晶振起振检测 
* 入口参数：无
* 出口参数：uint8_t	1起振	0没起振
* 说    明：OSC_IN(5,12,23)	
			OSC_OUT(6,13,24)
* 测试代码			
	flag=JZ_chack();
	if(flag==1)		//起振
		flag=9;
	else			//没起振
		flag=9;
****************************************************************************/
uint8_t JZ_chack(void)
{
	uint8_t flag=0;		 //晶振起振检测延时

//	RCC_DeInit();			 		//将外设 RCC寄存器重设为缺省值
//	RCC_HSEConfig(RCC_HSE_ON);		//设置外部8M高速晶振(HSE)
									//RCC_HSE_OFF  		HSE 	晶振 OFF 
									//RCC_HSE_ON  		HSE 	晶振 ON 
									//RCC_HSE_Bypass  	HSE 	晶振被外部时钟旁路
//	flag = RCC_WaitForHSEStartUp();	//等待 HSE 起振，该函数将等待直到 HSE 就绪，或者在超时的情况下退出 
	if(flag == SUCCESS)
		return 1;
	else
		return 0;
}

/****************************************************************************
* 名	称：void RCC_USEHSI(void)
* 功	能：启用STM32内部RC晶振
* 入口参数：无
* 出口参数：无
* 说	明：
		如果使用内部RC振荡器而不使用外部晶振，请按照下面方法处理：
		1 对于100脚或144脚的产品，OSC_IN应接地，OSC_OUT应悬空。
		2 对于少于100脚的产品，有2种接法：
			2.1 OSC_IN和OSC_OUT分别通过10K电阻接地。此方法可提高EMC性能。
			2.2 分别重映射OSC_IN和OSC_OUT至PD0和PD1，再配置PD0和PD1为推挽输出并输出'0'。此方法可以减小功耗并(相对上面2.1)节省2个外部电阻。
* 测    试：
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
* 名	称：uint32_t AddCode(uint32_t Data)
* 功	能：对原始数据增加密钥 
* 入口参数：uint32_t Data	原始数据
* 出口参数：uint32_t	加密钥后的数据
* 说    明：无
****************************************************************************/
uint32_t AddKey(uint32_t Data)
{
	Data=rotr(Data,13);	  //循环右移13位
	return Data;
}

/****************************************************************************
* 名	称：uint32_t SubKey(uint32_t Data)
* 功	能：对有密钥的数据减去密钥 
* 入口参数：uint32_t Data	加密钥后的数据
* 出口参数：uint32_t	原始数据
* 说    明：无
****************************************************************************/
uint32_t SubKey(uint32_t Data)
{
	Data=rotl(Data,13);	  //循环左移13位
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
* 名	称：void FIFO(uint32_t* x,uint32_t len, uint32_t val)
* 功	能：先进先出队列	拷贝
* 入口参数：uint32_t* x			存放缓冲区
			uint32_t len			缓冲区长度
			uint32_t val			输入值
* 出口参数：无
* 说	明：无
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
* 名	称：uint32_t Fsave(FBuff* FQ,uint32_t val,uint32_t BuffLen)
* 功	能：队列存值
* 入口参数：FBuff* FQ	存放队列
			uint32_t* val	存入的值
			uint32_t BuffLen	存放区长度
* 出口参数：uint32_t			当前存放区中的个数
* 说	明：先入先出存	指针
****************************************************************************/
uint32_t Fsave(FBuff* FQ,uint32_t val,uint32_t BuffLen)
{
	*(FQ->QIn) = val;			//输入位置存数
	FQ->QIn++;					//输入位置增加
	if(FQ->QIn > FQ->QEnd)		//输入地址等于输出地址
		FQ->QIn = FQ->QStart;	//跳到开始地址
	if(FQ->Count < BuffLen)		//数据没装满的情况下
		FQ->Count++;	 			//继续装
	return FQ->Count;			//返回个数
}

/****************************************************************************
* 名	称：uint8_t Fget(uint32_t* val,FBuff* FQ)
* 功	能：队列取值
* 入口参数：uint32_t* val	取出的值
			FBuff* FQ	数据队列
* 出口参数：uint8_t	0		取值失败
				1		取值成功
* 说	明：先入后出取	指针
****************************************************************************/
uint8_t Fget(uint32_t* val,FBuff* FQ)
{
	if(FQ->Count > 0)				//缓冲区有数
	{
		FQ->QIn --;					//输入位置后退
		if(FQ->QIn < FQ->QStart)
			FQ->QIn = FQ->QEnd;	//如果处在比第一个位置还小的位置则为开始位置
		FQ->QOut = FQ->QIn;		//得到最后一个值
		*val = *(FQ->QOut);			//取值
		FQ->Count--;					//保存的个数减少
		return 1;						//取值成功
	}
	else								//缓冲区没数
		return 0;						//取值失败
}

/* 队列初始化例子
#define maxADlen 5				//数据存放区大小
static uint32_t DownData[maxADlen]={0};	//数据存放区
FBuff DownAD = 
{
	DownData,				//开始位置	Start
	DownData+(maxADlen-1),		//结束位置	End
	DownData,				//输入位置	In
	DownData,				//输出位置	Out	暂时
	0,						//个数
};
*/

/****************************************************************************
* 名	称：uint8_t FindBo(uint32_t val,uint32_t* GetBoVal)
* 功	能：找波峰
* 入口参数：uint32_t val			引入AD值
			uint32_t* GetBoVal	波峰输出值
* 出口参数：uint8_t 是否有波峰形成	1形成	0没形成
* 说	明：中间数大于两边即为波峰
****************************************************************************/
static uint32_t BoBuff[3]={0};
static uint8_t i=0;
FBuff ADB = 
{
	BoBuff,				//开始位置	Start
	BoBuff+(3-1),		//结束位置	End
	BoBuff,				//输入位置	In
	BoBuff,				//输出位置	Out	暂时
	0,						//个数
};
uint8_t FindBo(uint32_t val,uint32_t* GetBoVal)
{
	if(i<2)	//滑窗初始化	装入前2个数据
	{
		Fsave(&ADB,val,3);			//装数据
		i++;				
		return 0;			//没得到波峰
	}

	Fsave(&ADB,val,3);			//装数据

	if(BoBuff[1] > BoBuff[0] && BoBuff[1] > BoBuff[2])
	{
		*GetBoVal = BoBuff[1];	//得到波峰值
		return 1;				//得到波峰
	}
	return 0;
}

/****************************************************************************
* 名	称：uint8_t* FindString(uint8_t* Data,uint32_t DataLen,uint8_t* StringVal,uint32_t StringLen)
* 功	能：在字符串中找字符串并定位
* 入口参数：uint8_t* Data		存放字符串的数据源
			uint32_t DataLen		数据源最大个数
			uint8_t* StringVal	要查找的字符串
			uint32_t StringLen	要查找的字符串个数
* 出口参数：uint8_t*	定位字符串在数据源中的位置，首位字符串的位置	没找到返回指向0地址的指针
* 说	明：
uint8_t a[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};	//26
uint8_t b[1] = 'a';								//1
uint8_t* f = 0;
f = FindString(a,sizeof(a),b,sizeof(b));
****************************************************************************/
char* FindString(char* Data,uint32_t DataLen,char* StringVal,uint32_t StringLen)
{
	char* s = 0;		 //源地址指针
	char*	p = 0;		 //比较数据地址指针
	uint32_t i = 0;		 //比较数据个数

	if(DataLen == 0 || StringLen == 0 || StringVal == 0 || Data == 0)	 //查找的数据和数据源为0数据为空
		return p;

	s = Data;		 //指向数据源
	p = StringVal;	 //指向查找数据

	while(DataLen > 0)		//源数据没比较完
	{
		if(*s != *p)	  	//没有相似数据
		{
			DataLen--;		//比较过的数据
			p = StringVal;	//指向查找数据
			if(i != 0)		//前面有几次比较成功了的
			{
				s -= (StringLen - 1);		//对成功过的第二个开始从新比较		abcabcd	比较abcd
				i = 0;				//比较成功个数归零
			} 
		}
		else			  	//找到相似数据
		{
			i++;			//比较成功个数增加

			if(i >= StringLen)				//找到
				return s - (StringLen - 1);	//移到比较的首字符位置

			p++;			//比较数据后移一个
		}
		s++;				//源数据位移
	}

	p = 0;			  //没找到
	return p;
}

/****************************************************************************
* 名	称：uint32_t FindNoZero(uint8_t* Data,uint32_t DataLen)
* 功	能：在数据中查找非零个数
* 入口参数：uint8_t* Data		数据
			uint32_t DataLen		数据最大个数
* 出口参数：uint32_t				非零个数
* 说	明：遇到零退出，如果中途为0则中途退出
****************************************************************************/
uint32_t FindNoZero(uint8_t* Data,uint32_t DataLen)
{
	uint32_t i = 0;	//累加器

	if(Data == 0)
		return 0;

	while(Data[i] != 0x00)	//数据不为0则继续循环
	{	
		i++;
		if(i >= DataLen)	//超过最大数据退出循环
			break;
	}	
	return i;
}

/******************************************************
		整形数据转字符串函数
        char *itoa(int value, char *string, int radix)
		radix=10 标示是10进制	非十进制，转换结果为0;  

	    例：d=-379;
		执行	itoa(d, buf, 10); 后
		
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
