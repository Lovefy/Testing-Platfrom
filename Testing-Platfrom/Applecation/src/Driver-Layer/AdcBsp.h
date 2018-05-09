#ifndef __ADC_H
#define __ADC_H

#include"BSPcfg.h"

#define USE_ADCBSP_DEBUG 1

/*ACC电源ADC检测阀值
*计算公式:（实际电压/总电阻值）*电阻=最低的电压值
*客户信息定义,编译时打开SHI_MENG_PRODUCTS可编译不同低电压值的版本
*/
//#define SHI_MENG_PRODUCTS

#ifdef SHI_MENG_PRODUCTS
#define ACCVOLTAGE_VAL        	115
#define ACCVOLTAGE_MIN 			 	1297 /*11.5--1297 11.8V--1331  11.9--1342  */
#else
#define ACCVOLTAGE_VAL        	119
#define ACCVOLTAGE_MIN        	1342
#endif

#define CAPTURE_VOLTAGE_GPIO 		GPIOA
#define CAPTURE_VOLTAGE_PIN 		GPIO_Pin_4
#define CAPTURE_VOLTAGE_CHANNEL ADC_Channel_4

#define HARDWARE_DEVIDE_GPIO 		GPIOC
#define HARDWARE_DEVIDE_PIN 		GPIO_Pin_1
#define HARDWARE_DEVIDE_CHANNEL ADC_Channel_11     //_su PC1 is  ADC123_In11　(USE ADC1_IN11)


#define HARDWARE_ADC_VAL_Interval    496      //0.4*4096/3.3= 496 间隔范围

#define HARDWARE_ADC_VAL_Version_1   124      //0.1-0.5   起始值
#define HARDWARE_ADC_VAL_Version_2   744      //0.6-1     0.6*4096/3.3=744
#define HARDWARE_ADC_VAL_Version_3   1365     //1.1-1.5   1.1*4096/3.3=1365
#define HARDWARE_ADC_VAL_Version_4   1985     //1.6-2     1.6*4096/3.3=1985
#define HARDWARE_ADC_VAL_Version_5   2606     //2.1-2.5   2.1*4096/3.3=2606
#define HARDWARE_ADC_VAL_Version_6   3327     //2.6-3.3   2.6*4096/3.3=3327


#define ACCVOLTAGE_MAX 					4096	/*2031*/
#define DotResisterVal        	47000ul
#define TotalResisterVal      	517000ul

#define CompasenVal   24      	/*补偿误差值  0.1V(11)左右*/

#define ADC_DATA_SIZE  8


typedef struct
{
    u8 GatherNum;	   							/*采集次数*/
    u16 adc_value[ADC_DATA_SIZE];  /*缓冲区*/
    u16 adc_value_ave; 						/*最终值*/
    u8 ADCOverFlag;    						/*ADC溢出标志*/
    u8 CalibrationFactorVal;				/*矫正因素*/

    u8 isStandby;
} ADCPrvDataTypedef, *pADCPrvDataTypedef;


__packed typedef struct
{
    void (*ADC1_Init)(void);
    u16 (*GetADC1Average)(u8 ch);
    u8 (*ADC_Suspend)(void);
    u8 (*ADC_Resume)(void);
    pADCPrvDataTypedef pADCPrvDataInit;
}
AdcBspTypeDef;

u8 Get_Hardware_Version(void);
AdcBspTypeDef *GetAllADCBspData(void);
AdcBspTypeDef *ADC_AllBspInit(void);




#endif
